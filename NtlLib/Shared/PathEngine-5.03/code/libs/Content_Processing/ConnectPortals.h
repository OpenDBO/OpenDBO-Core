//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "common/STL/vector.h"

class cExternalShapeInfo;

class iFaceIsOnTerrainCallBack
{
public:
    virtual bool faceIsOnTerrain(tFace_3D_FaceTracked f) = 0;
};

void
ConnectPortals(
        tMesh_3D_FaceTracked& baseMesh,
        iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack,
        const std::vector<cExternalShapeInfo*>& externalShapes,
        bool ignoreConnectionFailure
        );
