//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MAYA_MESH_WRAPPER_INCLUDED
#define MAYA_MESH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"

class MFnMesh;
class MDagPath;
class MColorArray;
class MFnDependencyNode;
class MObjectArray;
class MIntArray;

class cMayaMeshWrapper : public iFaceVertexMesh
{
    MFnMesh* _mesh;
    std::vector<long> _transformedVertexXY;
    std::vector<float> _transformedVertexZ;
    std::vector<long> _surfaceTypes;
    std::vector<long> _sectionIDs;
    std::vector<long> _markerTypes;

    void extractAttributesFromVertexColours(MColorArray&);
    void extractAttributesFromMaterial(const MFnDependencyNode& material, long faceIndex);
    void extractAttributesFromShaders(const MObjectArray& shaders, const MIntArray& indices);

public:

    static bool checkCanConstructFrom(const MDagPath& dagPath);

    cMayaMeshWrapper(const MDagPath& dagPath, float scale);
    ~cMayaMeshWrapper();

// iFaceVertexMesh interface

    long faces() const;
    long vertices() const;

    long vertexIndex(long face, long vertexInFace) const;
    long vertexX(long vertex) const;
    long vertexY(long vertex) const;
    float vertexZ(long vertex) const;
    long surfaceType(long face) const;
    long faceAttribute(long face, long attributeIndex) const;
};

#endif
