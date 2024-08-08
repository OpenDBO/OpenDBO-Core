//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TRIANGULATE_INCLUDED
#define TRIANGULATE_INCLUDED

#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/TriangulateFace.h"
#include "libs/Mesh_Common/interface/Vertex.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"
#include "common/TemplateMesh/Helper.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

template <class T>
class cNullVertexFilter
{
public:
    bool operator()(T)
    {
        return false;
    }
};

template <class tMesh>
void Triangulate(tMesh &mesh)
{
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        if(!f->isExternal())
            TriangulateFace(mesh,f);
        f++;
    }
}

template <class tEdge> bool
EdgeCanBeFlipped(tEdge e)
{
    tLine line(GetVertexPoint(e.prev()), GetVertexPoint(e.twin().prev()));
    return line.side(GetVertexPoint(e)) == SIDE_LEFT && line.side(GetVertexPoint(e.twin())) == SIDE_RIGHT;
}

// assumes that the mesh is a triangulation
template <class tMesh> void
RemoveInternalVertex(tMesh& mesh, typename tMesh::vertex v)
{
    typedef typename tMesh::edge tEdge;

    std::list<tEdge> spokes;
    tEdge e = tMesh::firstEdge(v);
    do
    {
        assertD(e.next().next().next() == e);
        spokes.push_back(e);
    }
    while(e.toNextAroundVertex());

    while(spokes.size() > 3)
    {
        typename std::list<tEdge>::iterator i = spokes.begin();
        while(!EdgeCanBeFlipped(*i))
        {
            ++i;
            if(i == spokes.end())
            {
                assertD(spokes.size() == 4);
              // special case where one edge in the triangulation must cross the vertex
              // e.g. when the vertex is at the centre of a square
              // in this case, we get two spokes diametrically opposite which stop us resolving to a triangle
                tEdge toJoin1, toJoin2;
                tEdge toUnsplit;
                i = spokes.begin();
                ++i;
                if(GetEdgeLine(*i).side(GetEdgeLine(spokes.back())) == SIDE_CENTRE)
                {
                  // spokes[1] and spokes[3]
                    toUnsplit = *i;
                    toJoin1 = spokes.front();
                    ++i;
                    toJoin2 = *i;
                }
                else
                {
                  // spokes[0] and spokes[2]
                    toUnsplit = spokes.front();
                    toJoin1 = *i;
                    ++i;
                    assertD(GetEdgeLine(spokes.front()).side(GetEdgeLine(*i)) == SIDE_CENTRE);
                    toJoin2 = spokes.back();
                }
                mesh.joinFaces(toJoin1);
                mesh.joinFaces(toJoin2);
                mesh.unsplitEdge(toUnsplit.twin());
                return;
            }
        }

        mesh.flipEdge(*i);
        spokes.erase(i);
    }
    assertD(spokes.size() == 3);

    mesh.joinFaces(v);
}

template <class tMesh>
void RemoveInternalVertices(tMesh &mesh)
{
    long numberremoved=0;
restart:
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        typename tMesh::edge e=tMesh::beginEdge(f);
        do
        {
            if(VertexIsExternal(e))
                continue;

            RemoveInternalVertex(mesh, e.vertex());
//            assertD(ValidateMeshConnections(mesh));

            numberremoved++;
            goto restart;
        }
        while(++e!=tMesh::beginEdge(f));
        f++;
    }
}

// assumes that the mesh is a triangulation
template <class tMesh> void
RemoveInlineEdgeVertex(tMesh& mesh, typename tMesh::edge firstInternal)
{
    typedef typename tMesh::edge tEdge;

    std::list<tEdge> spokes;

    assertD(firstInternal.next().next().next() == firstInternal);

    tEdge e = firstInternal.prev().twin();
    while(!e.face()->isExternal())
    {
        assertD(e.next().next().next() == e);
        spokes.push_back(e);
        e = e.prev().twin();
    }

    while(spokes.size() > 1)
    {
        typename std::list<tEdge>::iterator i = spokes.begin();
        while(!EdgeCanBeFlipped(*i))
        {
            ++i;
            assertD(i != spokes.end());
        }

        mesh.flipEdge(*i);
        spokes.erase(i);
    }
    assertD(spokes.size() == 1);

    mesh.joinFaces(spokes.front());
    mesh.unsplitEdge(firstInternal.prev());
}



template <class tMesh, class tVertexFilter>
void RemoveInlineEdgeVertices(tMesh &mesh, tVertexFilter filter)
{
restart:
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        typename tMesh::edge e=tMesh::beginEdge(f);
        do
        {
            if(!e.twin().face()->isExternal())
                continue;

            if(filter(e.vertex()))
            {
                continue;
            }
            
            if(!ExternalVertexIsInLine(e.twin().next()))
            {
                continue;
            }

            RemoveInlineEdgeVertex(mesh, e);
//            assertD(ValidateMeshConnections(mesh));

            goto restart;
        }
        while(++e!=tMesh::beginEdge(f));
        f++;
    }
}

template <class tMesh, class tEdgeFilter>
void SimplifyMesh(tMesh &mesh, tEdgeFilter filter)
{
    typename tMesh::face f=mesh.beginFace();
    while(f.valid())
    {
        typename tMesh::edge next(mesh.beginEdge(f));
        bool joinedaface=false;
        do
        {
            typename tMesh::edge e(next);
            ++next;

            if(filter(e))
            {
                continue;
            }

            eDirection directionleft=GetEdgeLine(e.prev()).direction(GetEdgeLine(e.twin().next()));
            if(directionleft!=DIRECTION_RIGHT)
                continue;
            eDirection directionright=GetEdgeLine(e.twin().prev()).direction(GetEdgeLine(e.next()));
            if(directionright!=DIRECTION_RIGHT)
                continue;

            typename tMesh::edge beforeleft(e.prev());
            typename tMesh::edge beforeright(e.twin().prev());
            mesh.joinFaces(e);

            //.... on a mesh with external faces added, all twins are valid
            //.... so check if we can simplify this
            if(directionleft==DIRECTION_FORWARD)
            {
                if(
                    (!beforeleft.twin().valid() && !beforeleft.next().twin().valid())
                    ||
                    (beforeleft.twin().valid() && beforeleft.next().twin().valid() && beforeleft.twin().prev()==beforeleft.next().twin())
                    )
                    mesh.unsplitEdge(beforeleft);
            }
            if(directionright==DIRECTION_FORWARD)
            {
                if(
                    (!beforeright.twin().valid() && !beforeright.next().twin().valid())
                    ||
                    (beforeright.twin().valid() && beforeright.next().twin().valid() && beforeright.twin().prev()==beforeright.next().twin())
                    )
                    mesh.unsplitEdge(beforeright);
            }

            joinedaface=true;
            break;
        }
        while(next!=mesh.beginEdge(f));

        if(!joinedaface)
            ++f;
    }
}

template <class tMesh>
bool Attempt_RemoveInternalVertex(  tMesh &mesh, 
                                    const typename tMesh::edge &edge, 
                                    long max_error)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;
    typedef typename tMesh::tFaceData tFaceData;

    tPoint p=GetVertexPoint(edge);
    long heightBefore=GetVertexHeight(edge);

    std::vector<tEdge> edges_out;
    tFace f=RemoveInternalVertex<tMesh>(mesh,edge,edges_out,tFaceData());

    std::vector<tEdge> edges_in;
    {
        tEdge e=tMesh::beginEdge(f);
        do
            edges_in.push_back(e.twin());
        while(e.nextInFace());
    }

    std::vector<tFace> faces_added;
    TriangulateFace_NonConvex<tMesh>(mesh,f,faces_added);

    tFace containing_f;
    {
        containing_f.clear();
        size_t i;
        for(i=0;i<faces_added.size();i++)
        {
            if(PointInFace_Inclusive(tMesh::beginEdge(faces_added[i]),p))
            {
                containing_f=faces_added[i];
                break;
            }
        }
    }
    assertD(containing_f);

    CalculatePlaneEquation(containing_f);
    long heightAfter=containing_f->heightFor(p);

    long difference=heightAfter-heightBefore;
    if(difference<0)
        difference=-difference;
    if(difference<=max_error)
    {
        // finalise the change
        EraseInternalVertex(mesh,edge);
        return true;
    }
    else
    {
        // undo the change
        size_t i;
        for(i=0;i<faces_added.size();i++)
            mesh.eraseFace(faces_added[i]);
        assertD(edges_in.size()==edges_out.size());
        for(i=0;i<edges_in.size();i++)
            edges_in[i].connect(edges_out[i]);
        return false;
    }
}

template <class tMesh>
bool Attempt_RemoveExternalVertex(  tMesh &mesh, 
                                    const typename tMesh::edge &edge, 
                                    long max_error)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::face tFace;
    typedef typename tMesh::tFaceData tFaceData;

    assertD(edge.twin().face()->isExternal());

    tLine line(
        GetVertexPoint(edge.twin().next().next()),
        GetVertexPoint(edge.next())
        );

    if(!ExternalVertexIsInLine(edge.twin().next()))
    {
        return false;
    }

    tPoint p=GetVertexPoint(edge);
    long heightBefore=GetVertexHeight(edge);

    long longest_axis=line.axis().longestAxis();
    tPoint offset=p-line.start();
    tPoint::T n,d;
    n=offset.getPtr()[longest_axis];
    d=line.axis().getPtr()[longest_axis];
    if(d<0)
    {
        n=-n;
        d=-d;
    }

    long startz=GetVertexHeight(edge.twin().next().next());
    long endz=GetVertexHeight(edge.next());
    long zmid=heightBefore-startz;
    long ztotal=endz-startz;
    long maxAllowed=zmid+max_error;
    long minAllowed=zmid-max_error;

    tPoint::tDistance lhs,rhs;
    lhs=ztotal;
    lhs*=n;
    rhs=maxAllowed;
    rhs*=d;
    if(lhs>rhs)
        return false;
    rhs=minAllowed;
    rhs*=d;
    if(lhs<rhs)
        return false;

    RemoveEdgeVertex(mesh,edge,line);
    return true;
}

template <class tMesh> typename tMesh::vertex
AddVertexBySplittingEdge(tMesh& mesh, typename tMesh::edge edge, const typename tMesh::tVertexData& vertexData)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::tEdgeData tEdgeData;

    mesh.splitEdge(edge, vertexData, tEdgeData());
    {
        tEdge after1 = edge.next();
        tEdge after2 = after1.next().next();
        mesh.splitFace(after1, after2, *after1, *after2);
    }
    {
        tEdge after1 = edge.twin();
        tEdge after2 = after1.next().next();
        mesh.splitFace(after1, after2, *after1, *after2);
    }
    return edge.next().vertex();
}

template <class tMesh> typename tMesh::vertex
AddVertexInsideFace(tMesh& mesh, typename tMesh::face f, const typename tMesh::tVertexData& vertexData)
{
    typedef typename tMesh::edge tEdge;
    typedef typename tMesh::tEdgeData tEdgeData;

    tEdge e = tMesh::beginEdge(f);
    long numberOfSplits = CountEdgesAround(e);
    std::vector<tEdgeData> edgeData(numberOfSplits * 2);
    std::vector<tEdge> after(numberOfSplits);

    size_t i = 0;
    do
    {
        edgeData[i * 2 + 1] = *(e.next());
        after[i++] = e;
    }
    while(e.toNextInFace());

    return mesh.splitFaceWithVertex(
        f, numberOfSplits, vertexData, &edgeData.front(), &after.front());
}

#endif
