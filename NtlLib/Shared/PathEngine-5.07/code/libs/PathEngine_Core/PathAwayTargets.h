//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "common/interface/Assert.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "platform_common/CoordinateRanges.h"
#include "common/STL/vector.h"

class cInternalPosition;
class cQueryContext;
class cElementInFaceEntry;
class cLocalisedConvexShapeSet;

class cPathAwayTargets
{
    tPoint _bottomLeft, _topRight;
    std::vector<tPoint> _pointTargets;
    tMeshCircuits _boundCircuits;
    std::list<tFace> _containedExternalFaces;

public:

//    cPathAwayTargets(
//            cQueryContext& qc,
//            const cInternalPosition& centre, long radius,
//            const std::vector<std::vector<cElementInFaceEntry> >& elementLookup
//            );

    cPathAwayTargets(
            cQueryContext& qc,
            const cInternalPosition& centre, long radius,
            const std::vector<std::vector<cElementInFaceEntry> >& edgeExpansionLookup,
            const cLocalisedConvexShapeSet& expansions
            );

    bool pointIsInside(const tPoint &p) const
    {
        if(p.getX()<=_bottomLeft.getX())
            return false;
        if(p.getX()>=_topRight.getX())
            return false;
        if(p.getY()<=_bottomLeft.getY())
            return false;
        if(p.getY()>=_topRight.getY())
            return false;
        return true;
    }
    bool positionIsInside(const cInternalPosition& p) const;

    long getNumberOfTargets() const
    {
        return SizeL(_pointTargets) + 4;
    }
    tPoint getTargetPoint(long index, const tPoint &parent) const
    {
        assertD(pointIsInside(parent));
        if(index < SizeL(_pointTargets))
        {
            return _pointTargets[index];
        }
        index -= SizeL(_pointTargets);
        switch(index)
        {
        default:
            invalid();
        case 0: 
            return tPoint(parent.getX(),_topRight.getY(),WORLD_RANGE+SHAPE_RANGE);
        case 1:
            return tPoint(_topRight.getX(),parent.getY(),WORLD_RANGE+SHAPE_RANGE);
        case 2:
            return tPoint(parent.getX(),_bottomLeft.getY(),WORLD_RANGE+SHAPE_RANGE);
        case 3:
            return tPoint(_bottomLeft.getX(),parent.getY(),WORLD_RANGE+SHAPE_RANGE);
        }
    }
    unsigned long minimumDistanceToTarget(const tPoint &p) const
    {
        unsigned long result=p.getX()-_bottomLeft.getX();
        unsigned long candidate;
        candidate=_topRight.getX()-p.getX();
        if(candidate<result)
            result=candidate;
        candidate=p.getY()-_bottomLeft.getY();
        if(candidate<result)
            result=candidate;
        candidate=_topRight.getY()-p.getY();
        if(candidate<result)
            result=candidate;
        return result;
    }
};

