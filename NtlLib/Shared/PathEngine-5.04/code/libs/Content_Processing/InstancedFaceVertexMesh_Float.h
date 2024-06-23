//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"

class cInstancedFaceVertexMesh_Float : public iFaceVertexMesh
{
    const iFaceVertexMesh* _base;
    float _cosOf, _sinOf;
    float _scale;
    long _translationX, _translationY;
    float _translationZ;

public:

    cInstancedFaceVertexMesh_Float(
        const iFaceVertexMesh* base,
        float rotation,
        float scale,
        long translationX, long translationY,
        float translationZ
        );

// iFaceVertexMesh interface

    long faces() const;
    long vertices() const;
    long vertexIndex(long face, long vertexInFace) const;
    long vertexX(long vertex) const;
    long vertexY(long vertex) const;
    float vertexZ(long vertex) const;
    long faceAttribute(long face, long attributeIndex) const;
};
