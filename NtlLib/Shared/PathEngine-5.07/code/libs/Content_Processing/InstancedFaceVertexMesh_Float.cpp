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
#include "interface/InstancedFaceVertexMesh_Float.h"
#include <math.h>

cInstancedFaceVertexMesh_Float::cInstancedFaceVertexMesh_Float(
        const iFaceVertexMesh* base,
        float rotation,
        float scale,
        long translationX, long translationY,
        float translationZ,
        long sectionID
        )
{
    _base = base;
    _cosOf = cosf(rotation);
    _sinOf = sinf(rotation);
    _scale = scale;
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
    _sectionIDOverride = sectionID;
}

// iFaceVertexMesh interface

long
cInstancedFaceVertexMesh_Float::faces() const
{
    return _base->faces();
}
long
cInstancedFaceVertexMesh_Float::vertices() const
{
    return _base->vertices();
}
long
cInstancedFaceVertexMesh_Float::vertexIndex(long face, long vertexInFace) const
{
    return _base->vertexIndex(face, vertexInFace);
}
long
cInstancedFaceVertexMesh_Float::vertexX(long vertex) const
{
    return static_cast<long>((_base->vertexX(vertex) * _cosOf + _base->vertexY(vertex) * _sinOf) * _scale) + _translationX;
}
long
cInstancedFaceVertexMesh_Float::vertexY(long vertex) const
{
    return static_cast<long>((_base->vertexY(vertex) * _cosOf - _base->vertexX(vertex) * _sinOf) * _scale) + _translationY;
}
float
cInstancedFaceVertexMesh_Float::vertexZ(long vertex) const
{
    return _base->vertexZ(vertex) * _scale + _translationZ;
}
long
cInstancedFaceVertexMesh_Float::faceAttribute(long face, long attributeIndex) const
{
    if(attributeIndex == PE_FaceAttribute_SectionID && _sectionIDOverride != -1)
    {
        return _sectionIDOverride;
    }
    return _base->faceAttribute(face, attributeIndex);
}
