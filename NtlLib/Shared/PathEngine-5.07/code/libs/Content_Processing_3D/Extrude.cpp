//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "Extrude.h"
#include "tMesh_Simple3D.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/TemplateMesh/Helper.h"
#include "common/STL/vector.h"

// note that faces may have more than 3 edges
// and some edges may be inline
// SIDE_RIGHT = facing up
// SIDE_LEFT = facing down
// SIDE_CENTRE = vertical
static eSide 
FacingDirection(tFace_Simple3D f)
{
    eSide result = SIDE_CENTRE;
    tEdge_Simple3D e = tMesh_Simple3D::beginEdge(f);
    tLine l = GetEdgeLine(e);
    bool not_completed;
    not_completed = e.toNextInFace();
    assertD(not_completed);
    not_completed = e.toNextInFace();
    assertD(not_completed);
    do
    {
        eSide side = l.side(GetVertexPoint(e));
        if(side != result)
        {
            assertD(result == SIDE_CENTRE);
            result = side;
        }
    }
    while(e.toNextInFace());
    return result;
}

static void
DoSplit(tMesh_Simple3D &hull, tEdge_Simple3D& e1, tEdge_Simple3D& e2)
{
    hull.splitVertex(e1, e2);
    assertD(ValidateMeshConnections(hull));
    *e1.prev() = *e1;
    *e2.prev() = *e2;
    --e1;
    --e2;
    hull.breakEdge(e1);
}

void
SplitHullForExtrusion(tMesh_Simple3D& hull, std::vector<tVertex_Simple3D>& verticesBelow)
{
    assertD(!hull.empty());
    assertD(verticesBelow.empty());
    long i;
    
// determine facing direction of each face

    std::vector<eSide> sides(hull.faceSize());
    tFace_Simple3D f=hull.beginFace();
    do
    {
        sides[f.index()] = FacingDirection(f);
    }
    while((++f).valid());

// get vertices connected to downwards faces

    verticesBelow.resize(hull.vertices());
    verticesBelow.clear();
    {
        tVertex_Simple3D v = hull.beginVertex();
        do
        {
            tEdge_Simple3D e = tMesh_Simple3D::firstEdge(v);
            do
            {
                if(sides[e.face().index()] == SIDE_LEFT)
                {
                    verticesBelow.push_back(v);
                    break;
                }
            }
            while(e.toNextAroundVertex());
        }
        while((++v).valid());
    }

// get a sequence of edges for the boundary between facing up and other faces

    std::vector<tEdge_Simple3D> boundary;
    {
    // get an edge to start the boundary
        tEdge_Simple3D e = hull.beginEdge();
        while(sides[e.face().index()] == SIDE_RIGHT || sides[e.twin().face().index()] != SIDE_RIGHT)
        {
            e.toNextInMesh();
        }

    // for SIDE_CENTRE case, traverse backwards along the boundary if necessary to ensure edge is first in face
    // for the SIDE_LEFT case, we dont care about this
    // also note that for the SIDE_LEFT case this face could be the only face on the boundary
    // in which case the while loop would never terminate
        if(sides[e.face().index()] == SIDE_CENTRE)
        {
            while(sides[e.prev().twin().face().index()] == SIDE_RIGHT)
            {
                --e;
            }
        }

    // follow the boundary around the hull
        do
        {
            boundary.push_back(e);
            ++e;

            if(sides[e.face().index()] == SIDE_CENTRE)
            {
            // skip any further boundary edges in this face
            // the whole face will be dealt with in one go and have only one entry in left[] and right[]
                while(sides[e.twin().face().index()] == SIDE_RIGHT)
                {
                    ++e;
                }
            }

            while(sides[e.twin().face().index()] != SIDE_RIGHT)
            {
                e = e.twin().next();
            }
        }
        while(e != boundary.front());
    }

// add faces or modify existing faces around the boundary

    std::vector<tEdge_Simple3D> left(boundary.size()), right(boundary.size());
    std::vector<char> at_vertex(boundary.size(), false);

    //.. optimise - construct a set of free vertices first and then add faces around these
    tMesh_Simple3D::tEdgeData edges[4];
    tMesh_Simple3D::tVertexData vertices[4];
    for(i = 0; i < SizeL(boundary); i++)
    {
        tEdge_Simple3D e = boundary[i];
        tFace_Simple3D f = e.face();
        if(sides[f.index()] == SIDE_LEFT)
        {
        // new face is added at this edge
            tEdge_Simple3D e_twin = e.twin();
            hull.breakEdge(e);
            edges[0] = *(e.next());
            edges[1] = *e;
            vertices[0] = *(e.next().vertex());
            vertices[1] = *e.vertex();
            tFace_Simple3D added = hull.addFace(tMesh_Simple3D::tFaceData(), 2, edges,vertices);
            hull.connectEdges(e, tMesh_Simple3D::beginEdge(added));
            hull.connectEdges(e_twin, tMesh_Simple3D::beginEdge(added).next());
            left[i] = tMesh_Simple3D::beginEdge(added).next();
            right[i] = tMesh_Simple3D::beginEdge(added);
            at_vertex[i] = true;
        }
        else
        {
        // face is vertical
        // add two new edges to existing face
        // (except in special cases for vertical edges)

            tEdge_Simple3D start_e = e;

            // skip past edges on the boundary
            do
            {
                ++e;
            }
            while(e.twin().valid() && sides[e.twin().face().index()] == SIDE_RIGHT);

            tPoint3 axis = GetEdgeAxis3(e);
            right[i] = e;

            do
            {
                ++e;
            }
            while(e != start_e.prev());

            axis = GetEdgeAxis3(e);
            if(axis.getX() != 0 || axis.getY() != 0)
            {
                at_vertex[i] = true;
                left[i] = e.next();
            }
            else
            {
            // case for when two vertical faces connect at a vertical edge
            // at left hand side of a face
                left[i] = e;
            }

        }
    }

// split the vertex for any at_vertex cases

    if(at_vertex.front())
    {
        DoSplit(hull, right.back(), left.front());
    }
    for(i = 0; i + 1 < SizeL(left); i++)
    {
        if(at_vertex[i + 1])
        {
            DoSplit(hull, right[i], left[i + 1]);
        }
    }

// sew new faces together

    assertD(GetVertexPoint3(left.front()) == GetVertexPoint3(right.back().next()));
    assertD(GetVertexPoint3(left.front().next()) == GetVertexPoint3(right.back()));
    if(!left.front().twin().valid())
    {
        hull.connectEdges(left.front(), right.back());
    }
    for(i = 0; i + 1 < SizeL(left); i++)
    {
        assertD(GetVertexPoint3(left[i+1]) == GetVertexPoint3(right[i].next()));
        assertD(GetVertexPoint3(left[i+1].next()) == GetVertexPoint3(right[i]));
        if(!right[i].twin().valid())
        {
            hull.connectEdges(right[i], left[i+1]);
        }
    }

//// remove any inline edges
//    for(i=0;i<left.size();i++)
//    {
//        tPoint3 axis=GetVertexPoint3(left[i])-GetVertexPoint3(left[i].prev());
//        if(axis.getX()==0 && axis.getY()==0)
//            left[i].prev().unSplit();
//    }

    assertD(ValidateMeshConnections(hull));
}

static void
FlipFace(tFace_Simple3D face)
{
    tEdge_Simple3D e = tMesh_Simple3D::beginEdge(face);
    assertD(CountEdgesAround(e) == 3);
    tMesh_Simple3D::tVertexData v = *e.next().vertex();
    *e.next().vertex() = *e.vertex();
    *e.vertex() = v;
}

static void
AdjustAndInsertIfNoExistingVerticalEdge(tMesh_Simple3D& mesh, tEdge_Simple3D& e)
{
    if(GetVertexPoint(e) == GetVertexPoint(e.prev()))
    {
    // already just after a vertical edge
        return;
    }
    if(GetVertexPoint(e) == GetVertexPoint(e.next()))
    {
        ++e;
        return;
    }
    // insert
    mesh.splitEdge(e.prev(), *e.vertex(), *e, *e);
}

static bool
IsAboveTest(tEdge_Simple3D e1, tEdge_Simple3D e2, tEdge_Simple3D eTest)
{
    tPoint p1 = GetVertexPoint(e1);
    tPoint p2 = GetVertexPoint(e2);
    tPoint pTest = GetVertexPoint(eTest);
    assertD((p2 - p1).side(p2 - p1) == SIDE_CENTRE);
    long sExtents, sTest;
    if(p1.getX() == p2.getX())
    {
        sExtents = p2.getY() - p1.getY();
        assertD(sExtents != 0);
        sTest = pTest.getY() - p1.getY();
    }
    else
    {
        sExtents = p2.getX() - p1.getX();
        sTest = pTest.getX() - p1.getX();
    }
    if(sExtents < 0)
    {
        sExtents = -sExtents;
        sTest = -sTest;
    }
    long zExtents = e2.vertex()->_p.getZ() - e1.vertex()->_p.getZ();
    long zTest = eTest.vertex()->_p.getZ() - e1.vertex()->_p.getZ();
    tPoint::tDistance zExtents_MultipliedOut, zTest_MultipliedOut;
    tMultiplier::multiply(zTest, sExtents, zTest_MultipliedOut);
    tMultiplier::multiply(zExtents, sTest, zExtents_MultipliedOut);
    int comparison = tMultiplier::compare(zExtents_MultipliedOut, zTest_MultipliedOut);
    assertD(comparison != 0); // not expecting inline vertices during vertical face extrusion
    return comparison == 1;
}

static void
ExtrudeVerticalFace(tMesh_Simple3D& toExtrude, long amount)
{
// find the furthest apart pair of vertices in x and y
// (this is the pair of vertices at which to split the face)

    tEdge_Simple3D e1, e2; // candidates
    tEdge_Simple3D bestE1, bestE2; // best so far

    bestE1 = toExtrude.beginEdge();
    bestE2 = bestE1.next();
    tPoint::tDistance bestDistance = (GetVertexPoint(bestE2) - GetVertexPoint(bestE1)).lengthSquared();

    e1 = toExtrude.beginEdge();
    do
    {
        e2 = e1;
        while(e2.toNextInMesh())
        {
            tPoint::tDistance distance = (GetVertexPoint(e2) - GetVertexPoint(e1)).lengthSquared();
            if(distance > bestDistance)
            {
                bestE1 = e1;
                bestE2 = e2;
                bestDistance = distance;
            }
        }
    }
    while(e1.toNextInMesh());

    e1 = bestE1;
    e2 = bestE2;

// add extra verts where there are not already vertical edges
// and adjust e1 and e2 to be exactly at start of upper and lower sequences respectively

    if(e2 != e1.next())
    {
    // test e1.next() to determine which sequence is upper and which is lower
        if(!IsAboveTest(e1, e2, e1.next()))
        {
            std::swap(e1, e2);
        }
    }
    else
    {
    // test e2.next() to determine which sequence is upper and which is lower
        if(IsAboveTest(e1, e2, e2.next()))
        {
            std::swap(e1, e2);
        }
    }

    AdjustAndInsertIfNoExistingVerticalEdge(toExtrude, e1);
    AdjustAndInsertIfNoExistingVerticalEdge(toExtrude, e2);

// go ahead and do the extrude

    tPoint3 offset(0, 0, -amount, amount);
    tEdge_Simple3D e = e2;    
    do
    {
        e.vertex()->_p += offset;
        ++e;
    }
    while(e != e1);
}

static void
ExtrudeUpwardFacingFace(tMesh_Simple3D& toExtrude, long amount)
{
    long edges = CountEdgesAround(toExtrude.beginEdge());

  // add face underneath

    tPoint3 extrudeOffset(0, 0, -amount, amount);

    std::vector<tMesh_Simple3D::tVertexData> vertexData(edges);
    std::vector<tMesh_Simple3D::tEdgeData> edgeData(edges);
    long i = edges;
    tEdge_Simple3D e = toExtrude.beginEdge();
    do
    {
        --i;
        vertexData[i] = *e.vertex();
        vertexData[i]._p += extrudeOffset;
    }
    while(e.toNextInFace());
    tFace_Simple3D underneathF = toExtrude.addFace(tMesh_Simple3D::tFaceData(), edges, &edgeData.front(), &vertexData.front());

  // add faces around and connect to top and bottom

    vertexData.resize(4);
    edgeData.resize(4);

    tEdge_Simple3D eTop = toExtrude.beginEdge();
    tEdge_Simple3D eBottom = tMesh_Simple3D::beginEdge(underneathF).prev().prev();
    do
    {
        assertD(GetVertexPoint(eTop) == GetVertexPoint(eBottom.next()));
        assertD(GetVertexPoint(eTop.next()) == GetVertexPoint(eBottom));
        vertexData[0] = *eTop.next().vertex();
        vertexData[1] = *eTop.vertex();
        vertexData[2] = *eBottom.next().vertex();
        vertexData[3] = *eBottom.vertex();
        tFace_Simple3D f = toExtrude.addFace(tMesh_Simple3D::tFaceData(), 4, &edgeData.front(), &vertexData.front());
        tEdge_Simple3D e = tMesh_Simple3D::beginEdge(f);
        toExtrude.connectEdges(e, eTop);
        ++++e;
        toExtrude.connectEdges(e, eBottom);
        --eBottom;
    }
    while(eTop.toNextInFace());

  // connect faces around to each other

    eTop = toExtrude.beginEdge();
    do
    {
        toExtrude.connectEdges(eTop.twin().next(), eTop.prev().twin().prev());
    }
    while(eTop.toNextInFace());
}

//....... declare maximum range for this extrude, also
void
Extrude(tMesh_Simple3D& hull, long amount)
{
    assertD(amount > 0);
    assertD(!hull.empty());

    if(hull.faces() > 1)
    {
        std::vector<tVertex_Simple3D> verticesBelow;
        SplitHullForExtrusion(hull, verticesBelow);
        tPoint3 offset(0, 0, -amount, amount);
        long i;
        for(i = 0; i != SizeL(verticesBelow); ++i)
        {
            verticesBelow[i]->_p += offset;
        }
        return;
    }

// special case handling for extrusion of single face operands

    eSide facingDirection = FacingDirection(hull.beginFace());
    // SIDE_RIGHT = facing up
    // SIDE_LEFT = facing down
    // SIDE_CENTRE = vertical

    if(facingDirection == SIDE_CENTRE)
    {
        ExtrudeVerticalFace(hull, amount);
        return;
    }

    if(facingDirection == SIDE_LEFT)
    {
        FlipFace(hull.beginFace());
    }

    ExtrudeUpwardFacingFace(hull, amount);
}
