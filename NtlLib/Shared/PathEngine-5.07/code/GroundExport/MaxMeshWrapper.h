//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MAX_MESH_WRAPPER_INCLUDED
#define MAX_MESH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"
class Mesh;
class Matrix3;

class cMaxMeshWrapper : public iFaceVertexMesh
{
    const Mesh& _mesh;
    std::vector<long> _transformedVertexXY;
    std::vector<float> _transformedVertexZ;
    std::vector<long> _surfaceTypes;
    std::vector<long> _sectionIDs;
    std::vector<long> _markerTypes;

public:

    cMaxMeshWrapper(const Mesh& mesh, const Matrix3& matrix,
     bool useMaterialIDs, bool setTerrainLayer, bool externalShapeAndPortalMarkers,
     float scale);

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
