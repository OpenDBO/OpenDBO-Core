//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "PathAwayTargets.h"
#include "ElementInFaceLookup.h"
#include "interface/OverlappingPolygon.h"
#include "interface/PointIsInsideCircuits.h"
#include "interface/QueryContext.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/CollisionShape.h"
#include "platform_common/CoordinateRanges.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/algorithm.h"

static void
GetEntryAndExitPoints(
        const cCircuitElement& el,
        const cLocalisedConvexShapeSet& expansions,
        std::vector<tPoint>& points
        )
{
    tPoint::tDistance startN, startD;
    tPoint::tDistance endN, endD;
    el.getStartOrEntryFraction(startN, startD);
    el.getEndOrExitFraction(endN, endD);
    cAxisFraction startFraction(startN, startD);
    cAxisFraction endFraction(endN, endD);
    cAxisFraction startFraction_Reversed(endD - endN, endD);
    cAxisFraction endFraction_Reversed(startD - startN, startD);
    tLine line_Reversed = el.refLine();
    line_Reversed.reverse();

    cBitVector expansionChecked(expansions.size(), false);
    long i;
    for(i = 0; i != el.facesCrossed(); ++i)
    {
        tFace f = el.faceCrossed(i);
        long j;
        for(j = 0; j != expansions.shapesInFace(f.index()); ++j)
        {
            long expansionIndex = expansions.indexForShapeInFace(f.index(), j);
            if(expansionChecked[expansionIndex])
            {
                continue;
            }
            expansionChecked.setBit(expansionIndex, true);
            const cLocalisedConvexShape& shape = expansions[expansionIndex];
            long collidingLineIndex;
            if(shape.lineCollides2D(el.refLine(), startFraction, endFraction, collidingLineIndex))
            {
                tLine intersectingLine = shape.getBoundaryLine(collidingLineIndex);
                points.push_back(el.refLine().approximateIntersection(intersectingLine.reversed()));
            }
            if(shape.lineCollides2D(line_Reversed, startFraction_Reversed, endFraction_Reversed, collidingLineIndex))
            {
                tLine intersectingLine = shape.getBoundaryLine(collidingLineIndex);
                points.push_back(el.refLine().approximateIntersection(intersectingLine.reversed()));
            }
        }
    }
}

static void
GetEntryAndExitPoints(
        const cCircuitElement* baseStartElement,
        const cLocalisedConvexShapeSet& expansions,
        std::vector<tPoint>& points
        )
{
    const cCircuitElement* e1 = baseStartElement;
    do
    {
        GetEntryAndExitPoints(*e1, expansions, points);
        e1 = e1->getNext();
    }
    while(e1 != baseStartElement);
}

static void
GetEntryAndExitPoints(
        const cCircuitElement* baseStartElement,
        const cCircuitElement& test,
        std::vector<tPoint>& points
        )
{
    const cCircuitElement* e1 = baseStartElement;
    do
    {
        long faceIndex1, faceIndex2;
        bool atStart1, atEnd1, atStart2, atEnd2;
        if(e1->elementEntersThroughThis(
                test,
                atStart1, atEnd1, atStart2,
                faceIndex1, faceIndex2))
        {
            if(atStart1)
            {
                const tLine& intersect = e1->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine()));
            }
            else
            if(atEnd1)
            {
                const tLine& intersect = e1->getNext()->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine()));
            }
            else
            {
                points.push_back(e1->refLine().approximateIntersection(test.refLine()));
            }
        }
        if(e1->elementExitsThroughThis(
                test,
                atStart1, atEnd1, atEnd2,
                faceIndex1, faceIndex2))
        {
            if(atStart1)
            {
                const tLine& intersect = e1->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine().reversed()));
            }
            else
            if(atEnd1)
            {
                const tLine& intersect = e1->getNext()->getIntersectAtCorner(test.refLine());
                points.push_back(intersect.approximateIntersection(test.refLine().reversed()));
            }
            else
            {
                points.push_back(e1->refLine().approximateIntersection(test.refLine().reversed()));
            }
        }
        e1 = e1->getNext();
    }
    while(e1 != baseStartElement);
}

cPathAwayTargets::cPathAwayTargets(
        cQueryContext& qc,
        const cInternalPosition& centre, long radius,
        const std::vector<std::vector<cElementInFaceEntry> >& edgeExpansionLookup,
        const cLocalisedConvexShapeSet& expansions
        )
{
    GetSquareBounds(centre, radius, _containedExternalFaces, _boundCircuits);

    _bottomLeft = centre.p + tPoint(-radius, -radius, SHAPE_RANGE);
    _topRight = centre.p + tPoint(radius, radius, SHAPE_RANGE);

    cFlagVector& elementFlags = qc.lockElementFlagVector();
    tMeshCircuits::const_iterator i;
    for(i = _boundCircuits.begin(); i != _boundCircuits.end() ; ++i)
    {
        tMeshCutSequence::const_iterator j;
        for(j = (*i)->begin(); j != (*i)->end() ; ++j)
        {
            const cCircuitElement* e1 = (*j)->getStartElement();

            elementFlags.clear();
            long k;
            for(k = 0; k < e1->facesCrossed(); ++k)
            {
                long fIndex = e1->getFaceCrossed(k).index();
                long l;
                for(l = 0; l < SizeL(edgeExpansionLookup[fIndex]); ++l)
                {
                    const cCircuitElement* e2 = edgeExpansionLookup[fIndex][l].element;
                    if(elementFlags[e2->getIndex()])
                    {
                        continue;
                    }
                    elementFlags.set(e2->getIndex());
                    GetEntryAndExitPoints(e1, *e2, _pointTargets);
                }
            }

            GetEntryAndExitPoints(e1, expansions, _pointTargets);
        }
    }
    qc.releaseFlagVector();
}

bool
cPathAwayTargets::positionIsInside(const cInternalPosition& p) const
{
    return PointIsInsideCircuits(_boundCircuits, _containedExternalFaces, p);
}
