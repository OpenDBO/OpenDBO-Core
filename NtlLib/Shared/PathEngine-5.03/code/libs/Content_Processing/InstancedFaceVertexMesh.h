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

class cInstancedFaceVertexMesh : public iFaceVertexMesh
{
    const iFaceVertexMesh* _base;
    long _sinOf, _cosOf;
    long _scale;
    float _scaleF;
    long _translationX, _translationY;
    float _translationZ;

public:

    cInstancedFaceVertexMesh(
        const iFaceVertexMesh* base,
        long rotation,
        long scale,
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
