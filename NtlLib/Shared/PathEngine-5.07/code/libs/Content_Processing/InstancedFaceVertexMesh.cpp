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
#include "InstancedFaceVertexMesh.h"
#include "QuantisedRotation.h"

cInstancedFaceVertexMesh::cInstancedFaceVertexMesh(
        const iFaceVertexMesh* base,
        long rotation,
        long scale,
        long translationX, long translationY,
        float translationZ,
        long sectionID
        )
{
    _base = base;
    QuantisedRotationCoefficients(rotation, _sinOf, _cosOf);
    _scale = scale;
    _scaleF = static_cast<float>(scale);
    _translationX = translationX;
    _translationY = translationY;
    _translationZ = translationZ;
    _sectionIDOverride = sectionID;
}

// iFaceVertexMesh interface

long
cInstancedFaceVertexMesh::faces() const
{
    return _base->faces();
}
long
cInstancedFaceVertexMesh::vertices() const
{
    return _base->vertices();
}
long
cInstancedFaceVertexMesh::vertexIndex(long face, long vertexInFace) const
{
    return _base->vertexIndex(face, vertexInFace);
}
long
cInstancedFaceVertexMesh::vertexX(long vertex) const
{
    return (_base->vertexX(vertex) * _cosOf + _base->vertexY(vertex) * _sinOf) / QuantisedRotationDivisor() * _scale + _translationX;
}
long
cInstancedFaceVertexMesh::vertexY(long vertex) const
{
    return (_base->vertexY(vertex) * _cosOf - _base->vertexX(vertex) * _sinOf) / QuantisedRotationDivisor() * _scale + _translationY;
}
float
cInstancedFaceVertexMesh::vertexZ(long vertex) const
{
    return _base->vertexZ(vertex) * _scaleF + _translationZ;
}
long
cInstancedFaceVertexMesh::faceAttribute(long face, long attributeIndex) const
{
    if(attributeIndex == PE_FaceAttribute_SectionID && _sectionIDOverride != -1)
    {
        return _sectionIDOverride;
    }
    return _base->faceAttribute(face, attributeIndex);
}
