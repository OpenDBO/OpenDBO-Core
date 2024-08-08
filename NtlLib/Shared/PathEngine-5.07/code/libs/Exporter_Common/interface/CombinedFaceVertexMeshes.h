//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPORTER_COMMON__COMBINED_FACE_VERTEX_MESHES_INCLUDED
#define EXPORTER_COMMON__COMBINED_FACE_VERTEX_MESHES_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"

class cCombinedFaceVertexMeshes : public iFaceVertexMesh
{
    const std::vector<iFaceVertexMesh*>& _baseMeshes;
    std::vector<long> _baseMeshForFace;
    std::vector<long> _baseMeshForVertex;
    std::vector<long> _startFace;
    std::vector<long> _startVertex;

public:

    // base meshes and vector passed in here must remain valid (and unmodified) for the lifetime of this object
    cCombinedFaceVertexMeshes(const std::vector<iFaceVertexMesh*>& baseMeshes);

    void getOriginalMeshAndIndexForFace(long face, long& originalMesh, long& originalIndex) const
    {
        originalMesh = _baseMeshForFace[face];
        originalIndex = face - _startFace[originalMesh];
    }
    void getOriginalMeshAndIndexForVertex(long vertex, long& originalMesh, long& originalIndex) const
    {
        originalMesh = _baseMeshForVertex[vertex];
        originalIndex = vertex - _startVertex[originalMesh];
    }

// iFaceVertexMesh interface

    long faces() const
    {
        return SizeL(_baseMeshForFace);
    }
    long vertices() const
    {
        return SizeL(_baseMeshForVertex);
    }
    long vertexIndex(long face, long vertexInFace) const
    {
        long baseMesh = _baseMeshForFace[face];
        long baseFace = face - _startFace[baseMesh];
        return _baseMeshes[baseMesh]->vertexIndex(baseFace, vertexInFace) + _startVertex[baseMesh];
    }
    long vertexX(long vertex) const
    {
        long baseMesh = _baseMeshForVertex[vertex];
        long baseVertex = vertex - _startVertex[baseMesh];
        return _baseMeshes[baseMesh]->vertexX(baseVertex);
    }
    long vertexY(long vertex) const
    {
        long baseMesh = _baseMeshForVertex[vertex];
        long baseVertex = vertex - _startVertex[baseMesh];
        return _baseMeshes[baseMesh]->vertexY(baseVertex);
    }
    float vertexZ(long vertex) const
    {
        long baseMesh = _baseMeshForVertex[vertex];
        long baseVertex = vertex - _startVertex[baseMesh];
        return _baseMeshes[baseMesh]->vertexZ(baseVertex);
    }
    long faceAttribute(long face, long attributeIndex) const
    {
        long baseMesh = _baseMeshForFace[face];
        long baseFace = face - _startFace[baseMesh];
        return _baseMeshes[baseMesh]->faceAttribute(baseFace, attributeIndex);
    }
};

#endif
