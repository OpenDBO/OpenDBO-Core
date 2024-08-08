//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TRIANGULATE_FACE_INCLUDED
#define TRIANGULATE_FACE_INCLUDED

#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Geometry/interface/InsideCorner.h"
#include "common/TemplateMesh/Helper.h"

template <class tMesh>
void TriangulateFace(tMesh &mesh, const typename tMesh::face &f)
{
//    assertD(FaceIsConvex(tMesh::beginEdge(f))); //... inline corners are permitted here
#ifdef ASSERTIONS_ON
    {
        typename tMesh::edge e;
        e = tMesh::beginEdge(f);
        do
        {
            eDirection dir = GetEdgeAxis(e).direction(GetEdgeAxis(e.next()));
            assertD(dir == DIRECTION_FORWARD || dir == DIRECTION_RIGHT);
        }
        while(e.toNextInFace());
    }
#endif
    typename tMesh::edge e = tMesh::beginEdge(f);
    long edges = CountEdgesAround(e);
    long i,j;
    for(i=0;i<edges;i++,++e)
    {
        if(!GetVertexPoint(e).isPoint())
            continue;
        typename tMesh::edge e2=e.next().next();
        for(j=0;j<edges-3;j++,++e2)
        {
            if(!GetVertexPoint(e2).isPoint())
                continue;
            if(GetEdgeLine(e).direction(GetEdgeLine(e2.prev()))==DIRECTION_FORWARD)
                continue;
            if(GetEdgeLine(e.prev()).direction(GetEdgeLine(e2))==DIRECTION_FORWARD)
                continue;
            tLine line(GetVertexPoint(e).approximatePoint(),GetVertexPoint(e2).approximatePoint());

            mesh.splitFace(e,e2,MakeEdge(e,line),MakeEdge(e2,line.reversed()));

            TriangulateFace(mesh,e.face());
            TriangulateFace(mesh,e2.face());
            return;
        }
    }
}

template <class tMesh> bool
CheckForSplitCrossings(
        const tLine &line,
        typename tMesh::edge begin_e,
        typename tMesh::edge end_e
        )
{
    while(begin_e != end_e)
    {
        tLine test = GetEdgeLine(begin_e);
        if(line.side(test.start()) != line.side(test.end()))
        {
            if(test.side(line.start()) == SIDE_RIGHT
                && test.side(line.end()) == SIDE_LEFT)
            {
                return true;
            }
            if(test.side(line.start()) == SIDE_LEFT
                && test.side(line.end()) == SIDE_RIGHT)
            {
                return true;
            }
        }
        ++begin_e;
    }
    return false;
}

template <class tMesh> bool
LineIsValidSplit(
        typename tMesh::edge e,
        typename tMesh::edge e2,
        const tLine &line
        )
{
    if(!InsideCorner(GetEdgeLine(e.prev()), GetEdgeLine(e), line.end()))
    {
        return false;
    }
    if(!InsideCorner(GetEdgeLine(e2.prev()), GetEdgeLine(e2), line.start()))
    {
        return false;
    }
    if(CheckForSplitCrossings<tMesh>(line, e.next(), e2.prev()))
    {
        return false;
    }
    if(CheckForSplitCrossings<tMesh>(line.reversed(), e2.next(), e.prev()))
    {
        return false;
    }
    return true;
}

template <class tMesh> bool
TriangulateFace_NonConvex(tMesh& mesh, typename tMesh::face f)
{
    long i, j;
    typename tMesh::edge e = tMesh::beginEdge(f);
    long edges = CountEdgesAround(e);
    for(i = 0; i < edges; i++, ++e)
    {
        typename tMesh::edge e2 = e.next().next();
        for(j = 0; j < edges - 3; j++, ++e2)
        {
            tLine line(GetVertexPoint(e), GetVertexPoint(e2));

        // test potential split for collisions

            if(!LineIsValidSplit<tMesh>(e, e2, line))
            {
                continue;
            }

            mesh.splitFace(e,e2,MakeEdge(e,line),MakeEdge(e2,line.reversed()));

            return TriangulateFace_NonConvex(mesh, e.face()) &&
                TriangulateFace_NonConvex(mesh, e2.face());
        }
    }
    return CountEdgesInFace(mesh, f) == 3;
}

#endif
