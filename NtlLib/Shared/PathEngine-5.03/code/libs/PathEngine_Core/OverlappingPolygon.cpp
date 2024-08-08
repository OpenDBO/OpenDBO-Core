//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/OverlappingPolygon.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/ArrangeIntoCircuits.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/CollisionShape.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "platform_common/CoordinateRanges.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/algorithm.h"

static bool
PolygonIsEntirelyWithinStartFace(
        const tFace& startFace,
        const tPoint* pointVector,
        long points
        )
{
    long i;
    for(i = 0; i < points; i++)
    {
        if(!PointIsInFace<tMesh>(startFace, pointVector[i]))
        {
            return false;
        }
    }
    return true;
}

cCircuitElement*
CircuitForPolygonEntirelyWithinStartFace(
        tFace startFace,
        const tPoint* pointVector,
        long points
        )
{
    cCircuitElement* head = 0;
    cCircuitElement* prev = 0;
    long i;
    for(i = 0; i < points; i++)
    {
        long nextI = i + 1;
        if(nextI == points)
        {
            nextI = 0;
        }
        tLine line(pointVector[i], pointVector[nextI]);
        cCircuitElement* e = new cCircuitElement;
        e->init_InSingleFace(line, startFace);
        if(prev)
        {
            prev->connectTo(*e);
        }
        else
            head = e;
        prev = e;
    }
    prev->connectTo(*head);
    return head;
}


class cEntryOrExitInfo
{
public:

    tEdge e;
    long i;

    bool operator==(const cEntryOrExitInfo &rhs) const
    {
        return e == rhs.e &&
               i == rhs.i;
    }
};

long
GetEntriesAndExits(tFace f,
        const tPoint* pointArray,
        long points,
        std::vector<cEntryOrExitInfo>& info
        )
{
    PROFILER_SCOPE("GetEntriesAndExits");
    long i;

  // loop through lines in polygon to find any that enter this face

    long numberOfEntries = 0;
    long prevI = points - 1;
    for(i = 0; i < points; i++)
    {
        tLine reverseLine(pointArray[i], pointArray[prevI]);
        bool enters;
        tEdge e;
        enters = LineCrossesAndExitsFace_SI<tMesh>(f, reverseLine, e);
        if(enters)
        {
            info[numberOfEntries * 2].i = prevI;
            info[numberOfEntries * 2].e = e;
            numberOfEntries++;
        }
        prevI = i;
    }

    PROFILER_CHANGES("GetEntriesAndExits exits");

  // get exit info for each entry

    for(i = 0; i < numberOfEntries; i++)
    {
        long j = info[i * 2].i;
        tEdge edgeCrossed;
        bool exited;

        while(1)
        {
            long nextJ = j + 1;
            if(nextJ == points)
            {
                nextJ = 0;
            }
            tLine line(pointArray[j], pointArray[nextJ]);
            cPoint_PointLike target(line.end());

            tFace traverseF = f;
            bool iterate;
            iterate = TraverseTowardsTarget_SI<tMesh>(traverseF, line, target, edgeCrossed);
            exited = traverseF->isExternal();
            if(iterate || exited)
            {
                break;
            }

            j = nextJ;
        }

        info[i * 2 + 1].i = j;
        info[i * 2 + 1].e = edgeCrossed;
    }
    return numberOfEntries;
}

void
AddAllAdjoining(
        tFace f,
        std::list<tFace> &open,
        const std::list<tFace> &closed,
        std::list<tFace> &external
        )
{
    std::list<tFace> added;
    tEdge e=tMesh::beginEdge(f);
    do
    {
        tFace f2=e.twin().face();
        if(f2->isExternal())
        {
            if(std::find(external.begin(),external.end(),f2)!=external.end())
                continue;
            external.push_back(f2);
        }
        else
        {
            if(std::find(open.begin(),open.end(),f2)!=open.end())
                continue;
            if(std::find(closed.begin(),closed.end(),f2)!=closed.end())
                continue;
            added.push_back(f2);
        }
    }
    while(++e!=tMesh::beginEdge(f));
    open.splice(open.begin(),added);
}

void
AddAdjoiningBetween(
        const cEntryOrExitInfo& exit,
        const cEntryOrExitInfo& entry,
        std::list<tFace>& open,
        const std::list<tFace>& closed,
        std::list<tFace>& external
        )
{
    std::list<tFace> added;
    tEdge e = exit.e;
    tEdge after = entry.e;
    ++after;
    do
    {
        tFace f2 = e.twin().face();
        if(f2->isExternal())
        {
            if(std::find(external.begin(),external.end(),f2)!=external.end())
            {
                continue;
            }
            external.push_back(f2);
        }
        else
        {
            if(std::find(open.begin(),open.end(),f2)!=open.end())
            {
                continue;
            }
            if(std::find(closed.begin(),closed.end(),f2)!=closed.end())
            {
                continue;
            }
            added.push_back(f2);
        }
    }
    while(++e != after);
    open.splice(open.begin(),added);
}


void
AddAdjoiningFaces(
        tFace f,
        const std::vector<cEntryOrExitInfo>& info,
        long numberOfEntries,
        std::list<tFace>& open,
        const std::list<tFace>& closed,
        std::list<tFace>& external
        )
{
    if(numberOfEntries == 0)
    {
        AddAllAdjoining(f, open, closed, external);
        return;
    }
    long i;
    for(i = 0; i < numberOfEntries; i++)
    {
        size_t nextI = i + 1;
        if(nextI == numberOfEntries)
            nextI = 0;
        AddAdjoiningBetween(info[i * 2 + 1], info[nextI * 2 + 0], open, closed, external);
    }
}

static bool
CircuitElementsConnect(const cCircuitElement& e1, const cCircuitElement& e2)
{
    if(e1.exits() || e2.enters())
    {
        return false;
    }
    if(e1.getEdgesCrossed() == 0 || e2.getEdgesCrossed() == 0)
    {
        return false;
    }
    if(e1.refLine() != e2.refLine())
    {
        return false;
    }
    return e1.getLastEdgeCrossed() == e2.getEdgeCrossed(0);
}

void
AddCircuitPart(
        const cEntryOrExitInfo &entry,
        const cEntryOrExitInfo &exit,
        const tPoint* pointVector,
        long points,
        std::list<cCircuitElement*> &circuits,
        std::list<cCircuitElement*> &circuitEnds
        )
{
    assertD(!(exit == entry));

// construct circuit part

    cCircuitElement* part = 0;
    cCircuitElement* partEnd;
    size_t i = entry.i;
    while(1)
    {
        size_t nextI = i + 1;
        if(nextI == points)
        {
            nextI = 0;
        }
        tLine line(pointVector[i], pointVector[nextI]);

        cEdgesTraversal traversal;
        if(i == entry.i)
        {
            if(entry.e.twin().face()->isExternal())
            {
                tEdge entryEE(entry.e);
                traversal.init_Entry(entryEE);
            }
            else
            {
                traversal.init_InFace(entry.e.twin().face());
                traversal.pushEdgeCrossing(entry.e.twin());
            }
        }
        else
        {
            traversal.init_InFace(entry.e.face());
        }
        if(i == exit.i)
        {
            if(exit.e.twin().face()->isExternal())
            {
                tEdge exitEE(exit.e);
                traversal.pushExit(exitEE);
            }
            else
            {
                traversal.pushEdgeCrossing(exit.e);
            }
        }

        cCircuitElement* e = new cCircuitElement;
        e->init(line, traversal);

        if(part)
        {
            partEnd->connectTo(*e);
        }
        else
        {
            part = e;
        }
        partEnd = e;

        if(i == exit.i)
        {
            break;
        }

        i = nextI;
    }


// check if this part must be connected to any other part

    std::list<cCircuitElement*>::iterator it, endIt;

    // check for end of a previously added circuit connecting to start of part
    for(it = circuits.begin(), endIt = circuitEnds.begin();
        it != circuits.end();
        ++it, ++endIt)
    {
        if(CircuitElementsConnect(**endIt, *part))
        {
            (*endIt)->popEdgeCrossing();
            (*endIt)->addTraversal(*part);
            if(part == partEnd)
            {
                partEnd = *endIt;
                delete part;
            }
            else
            {
                cCircuitElement* afterFirst = part->getNext();
                part->removeAndDelete();
                (*endIt)->connectTo(afterFirst);
            }

            part=*it;
            circuits.erase(it);
            circuitEnds.erase(endIt);

            // check for case where resulting circuit must be closed
            if(CircuitElementsConnect(*partEnd, *part))
            {
                assertD(part != partEnd);
                partEnd->popEdgeCrossing();
                partEnd->addTraversal(part);
                cCircuitElement* start = part->getNext();
                part->removeAndDelete();
                partEnd->connectTo(start);
                circuits.push_back(start);
                circuitEnds.push_back(partEnd);

                return;
            }
            break;
        }
    }

    // check for end of part connecting to start of a previously added circuit
    for(it=circuits.begin(),endIt=circuitEnds.begin();
        it!=circuits.end();
        ++it,++endIt)
    {
        if(CircuitElementsConnect(*partEnd, **it))
        {
            partEnd->popEdgeCrossing();
            partEnd->addTraversal(*it);
            if(*it == *endIt)
            {
                *endIt = partEnd;
                delete *it;
            }
            else
            {
                cCircuitElement* afterFirst = (*it)->getNext();
                (*it)->removeAndDelete();
                partEnd->connectTo(afterFirst);
            }

            *it=part;
            return;
        }
    }

    circuits.push_back(part);
    circuitEnds.push_back(partEnd);
}

void
AddCircuitPartsInFace(
        const std::vector<cEntryOrExitInfo> &info,
        long numberOfEntries,
        const tPoint* pointVector,
        long points,
        std::list<cCircuitElement*> &circuits,
        std::list<cCircuitElement*> &circuitEnds
        )
{
    long i;
    for(i = 0; i < numberOfEntries; i++)
    {
        AddCircuitPart(info[i * 2], info[i * 2 + 1], pointVector, points, circuits, circuitEnds);
    }
}

static void
CircuitPartsForOverlappingPolygon(
        tFace centreF,
        const tPoint* pointVector,
        long points,
        std::list<cCircuitElement*>& circuits,
        std::list<cCircuitElement*>& circuitEnds,
        std::list<tFace>& containedExternalFaces
        )
{
    std::vector<cEntryOrExitInfo> info(points * 2);

    std::list<tFace> open,closed;
    open.push_back(centreF);
    while(!open.empty())
    {
        tFace f=open.front();
        open.pop_front();
        long numberOfEntries = GetEntriesAndExits(f, pointVector, points, info);
        AddCircuitPartsInFace(info, numberOfEntries, pointVector, points, circuits, circuitEnds);
        AddAdjoiningFaces(f, info, numberOfEntries, open, closed, containedExternalFaces);
        closed.push_back(f);
    }

    // at this point we can remove external faces that any circuits enter from

    std::list<cCircuitElement*>::iterator it;
    for(it = circuits.begin(); it != circuits.end(); ++it)
    {
        if((*it)->getPrev() == 0) // checks that the circuit part is not already closed
        {
            tFace entryFace = (*it)->getExternalFaceForEntry();
            std::list<tFace>::iterator i;
            i = std::find(containedExternalFaces.begin(), containedExternalFaces.end(), entryFace);
            if(i != containedExternalFaces.end())
            {
                containedExternalFaces.erase(i);
            }
        }
    }
}

static void
CircuitsFromCircuitParts(std::list<cCircuitElement*>& circuits,
                         std::list<cCircuitElement*>& circuitEnds,
                         tMeshCircuits& appendTo)
{
// connect entries and exits

    // first add any circuits that are already closed

    std::list<cCircuitElement*>::iterator it, endIt;

    for(it = circuits.begin(), endIt = circuitEnds.begin();
        it != circuits.end();
        )
    {
        if((*it)->getPrev()!=0)
        {
            assertD((*it)->getPrev() == (*endIt));
            assertD((*endIt)->getNext() == (*it));
            
            appendTo.push_back(new tMeshCutSequence);
            appendTo.back()->push_back(new cMeshCut(*it));

            it = circuits.erase(it);
            endIt = circuitEnds.erase(endIt);
        }
        else
        {
            ++it;
            ++endIt;
        }
    }

    cHandle<tMeshCutSequence> toArrange = new tMeshCutSequence;
    while(!circuits.empty())
    {
        circuitEnds.front()->connectTo(*circuits.front());
        toArrange->push_back(new cMeshCut(circuits.front()));
        circuits.pop_front();
        circuitEnds.pop_front();
    }

    ArrangeIntoCircuits_Convex(*toArrange, appendTo);
}


void
CircuitsForOverlappingPolygon(
        tFace startF,
        const tPoint* pointVector,
        long points,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        )
{
    if(PolygonIsEntirelyWithinStartFace(startF, pointVector, points))
    {
        cCircuitElement* e = CircuitForPolygonEntirelyWithinStartFace(startF, pointVector, points);
        appendTo.push_back(new tMeshCutSequence);
        appendTo.back()->push_back(new cMeshCut(e));
        return;
    }

    std::list<cCircuitElement*> circuits;
    std::list<cCircuitElement*> circuitEnds;
    CircuitPartsForOverlappingPolygon(startF, pointVector, points, circuits, circuitEnds, containedExternalFaces);

    CircuitsFromCircuitParts(circuits, circuitEnds, appendTo);
    return;
}

void
CircuitsForOverlappingPolygon(
        const cInternalPosition& centre,
        const cCollisionShape& shape,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        )
{
    std::vector<tPoint> placedPolygon(shape.size());
    long i;
    for(i = 0; i < shape.size(); i++)
    {
        placedPolygon[i] = centre.p + shape.getVertex(i);
    }
    CircuitsForOverlappingPolygon(centre.f, &placedPolygon.front(), SizeL(placedPolygon), containedExternalFaces, appendTo);
}


template <class tPolygon> bool
VertexIsInPolygon(const tPolygon& polygon, const tPoint& p)
{
    long i;
    for(i = 0; i < polygon.edges(); i++)
    {
        if(polygon.sideOfBoundaryForPoint(i, p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}

template <class tPolygon> bool
PolygonCrossesEdge(const tLine& l, const tPolygon& polygon)
{
    long i;
    for(i = 0; i != polygon.edges(); ++i)
    {
        if(polygon.sideOfLineForVertex(l, i) == SIDE_LEFT)
        {
            long prevI = i - 1;
            if(prevI == -1)
            {
                prevI = polygon.edges() - 1;
            }
            if(polygon.sideOfLineForVertex(l, prevI) != SIDE_LEFT)
            {
                if(polygon.sideOfBoundaryForPoint(prevI, l.start()) != SIDE_RIGHT
                 && polygon.sideOfBoundaryForPoint(prevI, l.end()) == SIDE_RIGHT
                 )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

template <class tPolygon> void
AddAdjoiningFaces(
        tFace f,
        const tPolygon& polygon,
        tFace* openBegin, tFace*& openEnd,
        cFlagVector& closed,
        bool& exitsMesh
        )
{
    tEdge e = tMesh::beginEdge(f);
    bool vertexInside = VertexIsInPolygon(polygon, GetVertexPoint(e));
    do
    {
        bool prevVertexInside = vertexInside;
        vertexInside = VertexIsInPolygon(polygon, GetVertexPoint(e.next()));

        tFace f2 = e.twin().face();

        if(prevVertexInside && vertexInside)
        {
            if(f2->isExternal())
            {
                exitsMesh = true;
                continue;
            }
            if(!closed[f2.index()])
            {
                closed.set(f2.index());
                *openEnd++ = f2;
            }
            continue;
        }

        // in the following cases we don't care if the polygon crosses out of this face
        //.. optimise - look at avoiding point in polygon tests for vertices when these conditions apply to edges in sequence
        if(exitsMesh && f2->isExternal())
        {
            continue;
        }
        if(!f2->isExternal() && closed[f2.index()])
        {
            continue;
        }

        if(!prevVertexInside && !vertexInside)
        {
          // in this case it is necessary to test for whether the polygon crosses out of the middle of the edge
            if(!PolygonCrossesEdge(GetEdgeLine(e), polygon))
            {
                continue;
            }
        }

      // the polygon crosses over this edge

        if(f2->isExternal())
        {
            exitsMesh = true;
            continue;
        }

        if(!closed[f2.index()])
        {
            closed.set(f2.index());
            *openEnd++ = f2;
        }
    }
    while(e.toNextInFace());
}

template <class tPolygon> void
FacesOverlappedByOverlappingPolygon(
        tFace centreF,
        const tPolygon& polygon,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    exitsMesh = false;
    resultBuffer.clear();
    resultBuffer.set(centreF.index());
    tFace* openBegin = openQueueBuffer;
    tFace* openEnd = openQueueBuffer;
    AddAdjoiningFaces(centreF, polygon, openBegin, openEnd, resultBuffer, exitsMesh);
    while(openBegin != openEnd)
    {
        tFace f = *openBegin++;
        AddAdjoiningFaces(f, polygon, openBegin, openEnd, resultBuffer, exitsMesh);
    }
}


class cSIModelPlacedPolygon
{
    const tPoint* _pointArray;
    long _points;

public:

    cSIModelPlacedPolygon(const tPoint* pointArray, long points) :
     _pointArray(pointArray),
     _points(points)
    {
    }
    long edges() const
    {
        return _points;
    }
    eSide sideOfBoundaryForPoint(long edge, const tPoint& p) const
    {
        long nextEdge = edge + 1;
        if(nextEdge == edges())
        {
            nextEdge = 0;
        }
        tPoint axis, offset;
        axis = _pointArray[nextEdge] - _pointArray[edge];
        offset = p - _pointArray[edge];
        eSide result = axis.side(offset);
        if(result == SIDE_CENTRE)
        {
            result = SideOfAxis_SI(axis);
        }
        return result;
    }
    eSide sideOfLineForVertex(const tLine& l, long vertex) const
    {
        eSide result = l.side(_pointArray[vertex]);
        if(result == SIDE_CENTRE)
        {
            return OtherSide(SideOfLine_SI(l));
        }
        return result;
    }
};
void
FacesOverlappedByOverlappingPolygon_SI(
        tFace centreF,
        const tPoint* pointArray,
        long points,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    cSIModelPlacedPolygon polygon(pointArray, points);
    FacesOverlappedByOverlappingPolygon(centreF, polygon, openQueueBuffer, resultBuffer, exitsMesh);
}

class cOFModelPointAndShape
{
    const tPoint& _centre;
    const cCollisionShape& _shape;

public:

    cOFModelPointAndShape(const tPoint& centre, const cCollisionShape& shape) :
     _centre(centre),
     _shape(shape)
    {
    }
    long edges() const
    {
        return _shape.size();
    }
    eSide sideOfBoundaryForPoint(long edge, const tPoint& p) const
    {
        tLine l;
        _shape.getEdgeLine(edge, l);
        l += _centre;
        return l.side(p);
    }
    eSide sideOfLineForVertex(const tLine& l, long vertex) const
    {
        return l.side(_centre + _shape.getVertex(vertex));
    }
};
void
FacesOverlappedByOverlappingPolygon_OF(
        const cInternalPosition& centre,
        const cCollisionShape& shape,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    cOFModelPointAndShape polygon(centre.p, shape);
    FacesOverlappedByOverlappingPolygon(centre.f, polygon, openQueueBuffer, resultBuffer, exitsMesh);
}

class cOFModelIntersectionAndShape
{
    const tIntersection& _centre;
    const cCollisionShape& _shape;

public:

    cOFModelIntersectionAndShape(const tIntersection& centre, const cCollisionShape& shape) :
     _centre(centre),
     _shape(shape)
    {
    }
    long edges() const
    {
        return _shape.size();
    }
    eSide sideOfBoundaryForPoint(long edge, const tPoint& p) const
    {
        tLine l(p + _shape.expandOffset(edge), p + _shape.expandOffset(_shape.nextVertex(edge)));
        return l.side(_centre);
    }
    eSide sideOfLineForVertex(const tLine& l, long vertex) const
    {
        return l.side(_centre + _shape.getVertex(vertex));
    }
};
void
FacesOverlappedByOverlappingPolygon_OF(
        tFace centreF, const tIntersection& centreP,
        const cCollisionShape& shape,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        )
{
    cOFModelIntersectionAndShape polygon(centreP, shape);
    FacesOverlappedByOverlappingPolygon(centreF, polygon, openQueueBuffer, resultBuffer, exitsMesh);
}

bool
ShapeCollidesWithMeshEdge(
        cQueryContext& qc,
        const cInternalPosition& centre,
        const cCollisionShape& shape
        )
{
    tFace* openQueueBuffer = qc.lockFaceBuffer();
    cFlagVector& resultBuffer = qc.lockFaceFlagVector();
    bool exitsMesh;
    FacesOverlappedByOverlappingPolygon_OF(centre, shape, openQueueBuffer, resultBuffer, exitsMesh);
    qc.releaseFlagVector();
    qc.releaseGeneralPurposeBuffer();
    return exitsMesh;
}

static bool
FaceIsCompletelyContained(const tPoint& centre, const cCollisionShape& polygon, tFace f)
{
    tEdge e = tMesh::beginEdge(f);
    do
    {
        if(!polygon.pointIsInside(GetVertexPoint(e) - centre))
        {
            return false;
        }
    }
    while(++e != tMesh::beginEdge(f));
    return true;
}

cHandle<tMeshCircuits> CircuitsForInfiniteCylinder(
        tMesh& mesh,
        const tPoint& centre,
        const cCollisionShape& shape,
        std::list<tFace>& containedExternalFaces
        )
{
    cHandle<tMeshCircuits> result = new tMeshCircuits;

    std::vector<tPoint> polygon(shape.size());
    long i;
    for(i = 0; i < shape.size(); i++)
    {
        polygon[i] = centre + shape.getVertex(i);
    }

// get circuit parts and completely contained external faces

    std::list<cCircuitElement*> circuits;
    std::list<cCircuitElement*> circuitEnds;

    std::vector<cEntryOrExitInfo> info(polygon.size() * 2);

    tFace f = mesh.beginFace();
    do
    {
        if(f->isExternal())
        {
            if(FaceIsCompletelyContained(centre, shape, f))
            {
                containedExternalFaces.push_back(f);
            }
            continue;
        }
        if(PointIsInFace<tMesh>(f, centre))
        {
            if(PolygonIsEntirelyWithinStartFace(f, &polygon.front(), SizeL(polygon)))
            {
                cCircuitElement* e;
                e = CircuitForPolygonEntirelyWithinStartFace(
                        f, &polygon.front(), SizeL(polygon)
                        );
                result->push_back(new tMeshCutSequence);
                result->back()->push_back(new cMeshCut(e));
                continue;
            }
        }
        long numberOfEntries = GetEntriesAndExits(f, &polygon.front(), SizeL(polygon), info);
        AddCircuitPartsInFace(info, numberOfEntries, &polygon.front(), SizeL(polygon), circuits, circuitEnds);
    }
    while((++f).valid());

    CircuitsFromCircuitParts(circuits, circuitEnds, *result);
    return result;
}


void
GetSquareBounds(
    const cInternalPosition& centre,
    long radius,
    std::list<tFace>& containedExternalFaces,
    tMeshCircuits& appendTo
    )
{
    tPoint pointarray[4];
    pointarray[0]=tPoint(-radius,radius,SHAPE_RANGE);
    pointarray[1]=tPoint(radius,radius,SHAPE_RANGE);
    pointarray[2]=tPoint(radius,-radius,SHAPE_RANGE);
    pointarray[3]=tPoint(-radius,-radius,SHAPE_RANGE);
    cCollisionShape shape(4,pointarray);
    CircuitsForOverlappingPolygon(centre, shape, containedExternalFaces, appendTo);
}

cHandle<tMeshCircuits> GetSquareBounds_InfiniteCylinder(
    tMesh& mesh,
    const tPoint& centre,
    long radius,
    std::list<tFace>& containedExternalFaces
    )
{
    tPoint pointarray[4];
    pointarray[0] = tPoint(-radius,radius,SHAPE_RANGE);
    pointarray[1] = tPoint(radius,radius,SHAPE_RANGE);
    pointarray[2] = tPoint(radius,-radius,SHAPE_RANGE);
    pointarray[3] = tPoint(-radius,-radius,SHAPE_RANGE);
    cCollisionShape shape(4,pointarray);
    return CircuitsForInfiniteCylinder(mesh, centre, shape, containedExternalFaces);
}
