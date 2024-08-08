//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "HorizontallyOffsetFVM.h"

cHorizontallyOffsetFVM::cHorizontallyOffsetFVM(const iFaceVertexMesh* base, long offsetX, long offsetY) :
 _base(base),
 _offsetX(offsetX),
 _offsetY(offsetY)
{
}

long
cHorizontallyOffsetFVM::faces() const
{
    return _base->faces();
}
long
cHorizontallyOffsetFVM::vertices() const
{
    return _base->vertices();
}
long
cHorizontallyOffsetFVM::vertexIndex(long face, long vertexInFace) const
{
    return _base->vertexIndex(face, vertexInFace);
}
long
cHorizontallyOffsetFVM::vertexX(long vertex) const
{
    return _base->vertexX(vertex) + _offsetX;
}
long
cHorizontallyOffsetFVM::vertexY(long vertex) const
{
    return _base->vertexY(vertex) + _offsetY;
}
float
cHorizontallyOffsetFVM::vertexZ(long vertex) const
{
    return _base->vertexZ(vertex);
}
long
cHorizontallyOffsetFVM::faceAttribute(long face, long attributeIndex) const
{
    return _base->faceAttribute(face, attributeIndex);
}
