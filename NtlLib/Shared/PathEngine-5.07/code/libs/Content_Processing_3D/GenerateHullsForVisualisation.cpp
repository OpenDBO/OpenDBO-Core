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
#include "interface/GenerateHullsForVisualisation.h"
#include "BuildSolidObjects.h"
#include "tMesh_Simple3D.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"

static void
CopyMeshContents(
        tMesh_Simple3D& fromMesh,
        tMesh_3D& toMesh
        )
{
    tEdge_Simple3D fromE = fromMesh.beginEdge();
    tEdge_3D toE = toMesh.beginEdge();
    do
    {
        toE->_z = static_cast<float>(fromE.vertex()->_p.getZ());
        toE.toNextInMesh();
    }
    while(fromE.toNextInMesh());
    tVertex_Simple3D fromV = fromMesh.beginVertex();
    tVertex_3D toV = toMesh.beginVertex();
    do
    {
        toV->_x = fromV->_p.getX();
        toV->_y = fromV->_p.getY();
        ++toV;
    }
    while((++fromV).valid());
}

void
GenerateHullsForVisualisation(
        const long* solidObjectsData,
        long solidObjectsDataSize,
        tMesh_3D& result
        )
{
    assertR(result.empty());
    std::vector<tMesh_Simple3D*> solidObjects;
    BuildSolidObjects(solidObjectsData, solidObjectsDataSize, solidObjects);
    long i;
    for(i = 0; i != SizeL(solidObjects); ++i)
    {
        tMesh_3D objectMesh3D;
        objectMesh3D.copyStructure(*solidObjects[i]);
        CopyMeshContents(*solidObjects[i], objectMesh3D);
        result.append(objectMesh3D);
    }
}

