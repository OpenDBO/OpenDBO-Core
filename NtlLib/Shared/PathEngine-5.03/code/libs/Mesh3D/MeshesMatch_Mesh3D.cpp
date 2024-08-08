//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/MeshesMatch_Mesh3D.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

static bool
EpsilonEqual(float value1, float value2, float epsilon)
{
    return value2 >= value1 - epsilon && value2 <= value1 + epsilon;
}

static bool
VerticesMatch(tVertex_3D v1, tVertex_3D v2)
{
    return v1->_x == v2->_x && v1->_y == v2->_y;
}

static bool
EdgesMatch(tEdge_3D e1, tEdge_3D e2, float epsilon)
{
    return EpsilonEqual(e1->getZ(), e2->getZ(), epsilon)
          && VerticesMatch(e1.vertex(), e2.vertex())
          && VerticesMatch(e1.next().vertex(), e2.next().vertex())
          ;
}

static bool
FacesMatch(tFace_3D f1, tFace_3D f2, float epsilon)
{
    return f1->isExternal() == f2->isExternal()
        && EpsilonEqual(f1->_heightAtOrigin, f2->_heightAtOrigin, epsilon)
        && EpsilonEqual(f1->_heightForX, f2->_heightForX, epsilon)
        && EpsilonEqual(f1->_heightForY, f2->_heightForY, epsilon)
        ;
}

bool
MeshesMatch_Mesh3D(tMesh_3D& mesh1, tMesh_3D& mesh2, float epsilon)
{
    typedef tMesh_3D tMesh;
    typedef tMesh_3D::face tFace;
    typedef tMesh_3D::edge tEdge;
    typedef tMesh_3D::vertex tVertex;

    if(mesh1.faces() != mesh2.faces()
     || mesh1.edges() != mesh2.edges()
     || mesh1.vertices() != mesh2.vertices()
     )
    {
        return false;
    }

    std::vector<long> edgeMapping(mesh1.edges());
    cBitVector edgeMapped(mesh2.edges(), false);
    long i;
    for(i = 0; i != mesh1.edges(); ++i)
    {
        long j;
        for(j = 0; j != mesh2.edges(); ++j)
        {
            if(edgeMapped[j])
            {
                continue;
            }
            if(EdgesMatch(mesh1.edgeAt(i), mesh2.edgeAt(j), epsilon))
            {
                edgeMapped.setBit(j, true);
                edgeMapping[i] = j;
                break;
            }
        }
        if(j == mesh2.edges())
        {
            return false; // failed to find an edge matching edge i in the source mesh
        }
    }

    for(i = 0; i != mesh1.faces(); ++i)
    {
        tFace f = mesh1.faceAt(i);
        tEdge e1 = tMesh_3D::beginEdge(f);
        tEdge e2 = mesh2.edgeAt(edgeMapping[e1.index()]);
        if(!FacesMatch(f, e2.face(), epsilon))
        {
            return false;
        }
        if(e1.twin().valid())
        {
            if(!e2.twin().valid())
            {
                return false;
            }
            if(edgeMapping[e1.twin().index()] != e2.twin().index())
            {
                return false;
            }
        }
        while(e1.toNextInFace())
        {
            ++e2;
            if(edgeMapping[e1.index()] != e2.index())
            {
                return false;
            }
        }
    }
    
    for(i = 0; i != mesh1.vertices(); ++i)
    {
        tVertex v = mesh1.vertAt(i);
        tEdge e1 = tMesh_3D::firstEdge(v);
        tEdge e2 = mesh2.edgeAt(edgeMapping[e1.index()]);
        while(e1.toNextAroundVertex())
        {
            e2 = e2.prev().twin();
            if(!e2.valid())
            {
                return false;
            }
            if(edgeMapping[e1.index()] != e2.index())
            {
                return false;
            }
        }
    }
    return true;
}
