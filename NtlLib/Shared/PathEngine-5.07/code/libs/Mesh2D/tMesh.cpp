//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "interface/tMesh.h"
#include "common/TemplateMesh/ElementIndexing.h"

cV::cV(const tPoint& p) :
 _elementOn3D(-1),
 _p(p)
{
//    vertexindex = -1;
}

long
cV::getTargetVertexIndex() const
{
    bMeshElement::eType type;
    long subIndex;
    TypeAndSubIndexForElementIndex(_elementOn3D, type, subIndex);
    assertD(type == bMeshElement::VERTEX);
    return subIndex;
}

//long GetFaceIndex(tFace f)
//{
//    return f->index;
//}
//long GetEdgeIndex(tEdge e)
//{
//    return e->edgeindex;
//}
