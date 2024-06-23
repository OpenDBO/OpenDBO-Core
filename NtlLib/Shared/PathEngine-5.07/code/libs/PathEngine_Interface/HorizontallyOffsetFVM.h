//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef HORIZONTALLY_OFFSET_FVM__INCLUDED
#define HORIZONTALLY_OFFSET_FVM__INCLUDED

#include "i_pathengine.h"

class cHorizontallyOffsetFVM : public iFaceVertexMesh
{
    const iFaceVertexMesh* _base;
    long _offsetX, _offsetY;

public:

    cHorizontallyOffsetFVM(const iFaceVertexMesh* base, long offsetX, long offsetY);

// iFaceVertexMesh interface

    long faces() const;
    long vertices() const;
    long vertexIndex(long face, long vertexInFace) const;
    long vertexX(long vertex) const;
    long vertexY(long vertex) const;
    float vertexZ(long vertex) const;
    long faceAttribute(long face, long attributeIndex) const;
};

#endif
