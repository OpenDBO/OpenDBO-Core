//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_3D_WRAPPER_INCLUDED
#define MESH_3D_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

class cMesh3DWrapper : public iFaceVertexMesh
{
    tMesh_3D_FaceTracked _mesh;
    std::vector<long> _surfaceTypes;
    std::vector<long> _sectionIDs;
    std::vector<long> _userData;
    std::vector<long> _attributesMapping;

public:

    // the mesh passed in is copied and vertical discontinuities split
    // the resulting wrapper is then a stand-alone object
    cMesh3DWrapper(tMesh_3D& meshToWrap,
            const std::vector<long>* surfaceTypes,
            const std::vector<long>* sectionIDs,
            const std::vector<long>* userData
            );

// iFaceVertexMesh interface

    long faces() const
    {
        return _mesh.faces();
    }
    long vertices() const
    {
        return _mesh.vertices();
    }
    long vertexIndex(long face, long vertexInFace) const
    {
        assertD(vertexInFace >= 0 && vertexInFace < 3);
        tFace_3D_FaceTracked f = _mesh.faceAt(face);
        tEdge_3D_FaceTracked e = tMesh_3D_FaceTracked::beginEdge(f);
        while(vertexInFace--)
        {
            ++e;
        }
        return e.vertex().index();
    }
    long vertexX(long vertex) const
    {
        tVertex_3D_FaceTracked v = _mesh.vertAt(vertex);
        return GetVertexPoint(v).getX();
    }
    long vertexY(long vertex) const
    {
        tVertex_3D_FaceTracked v = _mesh.vertAt(vertex);
        return GetVertexPoint(v).getY();
    }
    float vertexZ(long vertex) const
    {
        tVertex_3D_FaceTracked v = _mesh.vertAt(vertex);
        tEdge_3D_FaceTracked e = tMesh_3D_FaceTracked::firstEdge(v);
        assertD(e.valid());
        return e->getZ();
    }
    long faceAttribute(long face, long attributeIndex) const;
};

#endif
