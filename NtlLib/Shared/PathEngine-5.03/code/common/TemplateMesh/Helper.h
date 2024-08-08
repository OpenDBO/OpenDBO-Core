//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TEMPLATE_MESH_HELPER_INCLUDED
#define TEMPLATE_MESH_HELPER_INCLUDED

template <class tEdge> long
CountEdgesAround(tEdge startEdge)
{
    long result = 1;
    tEdge e = startEdge.next();
    while(e != startEdge)
    {
        result++;
        ++e;
    }
    return result;
}

template <class tMesh, class tFace> long
CountEdgesInFace(tMesh& mesh, tFace f)
{
    return CountEdgesAround(mesh.beginEdge(f));
}

#endif
