//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOADED_FACE_VERTEX_MESH_INCLUDED
#define LOADED_FACE_VERTEX_MESH_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "i_pathengine.h"
#include "common/AttributesBuilder.h"
#include "common/STL/vector.h"

class cLoadedFaceVertexMesh : public iXMLHandler, public iFaceVertexMesh
{
    std::vector<long> _vertexXYs;
    std::vector<float> _vertexZs;
    std::vector<long> _faceVerts;
    std::vector<long> _faceAttributes;

public:

// direct construction

    long addVertex(long x, long y, float z);
    long addFace(long v0, long v1, long v2);
    void setAttribute(long face, long atttribute, long value);

// iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attr);

// iFaceVertexMesh interface

    long faces() const
    {
        return SizeL(_faceVerts) / 3;
    }
    long vertices() const
    {
        return SizeL(_vertexZs);
    }
    long vertexIndex(long face, long vertexInFace) const
    {
        return _faceVerts[face * 3 + vertexInFace];
    }
    long vertexX(long vertex) const
    {
        return _vertexXYs[vertex * 2 + 0];
    }
    long vertexY(long vertex) const
    {
        return _vertexXYs[vertex * 2 + 1];
    }
    float vertexZ(long vertex) const
    {
        return _vertexZs[vertex];
    }
    long faceAttribute(long face, long attributeIndex) const;
};

#endif
