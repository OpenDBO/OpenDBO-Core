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
#include "interface/BuildMeshFrom3DContent.h"
#include "BuildSolidObjects.h"
#include "Extrude.h"
#include "BSP3D.h"
#include "OperandsFromFaceVertex.h"
#include "AddVolumeToVerticalFace.h"
#include "BSPStats.h"
#include "BSPVertices.h"
#include "Approximate.h"
#include "ConvertToMesh3D.h"
#include "libs/PathEngine_Interface/interface/cPathEngine.h"
#include "libs/PathEngine_Interface/interface/Singletons.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/interface/Error.h"
#include "platform_common/CoordinateRanges.h"

iMesh*
BuildMeshFrom3DContent(
        const iFaceVertexMesh *const * meshObjectPointers,
        long numberOfMeshObjects,
        const long* solidObjectsData,
        long solidObjectsDataSize,
        long extrudeHeight,
        long maxStepHeight,
        long maxSlope,
        const char *const* options,
        iProgressCallBack* progressCallBack
        )
{
    INTERFACE_ASSERT(extrudeHeight >= 0);
    if(extrudeHeight < 0)
    {
        extrudeHeight = 0;
    }
    if(extrudeHeight > 8191)
    {
        const char *const attributes[] =
        {
            "supportedRange", "8191",
            0
        };
        Error("NonFatal", "BuildMeshFrom3DContent() - The extrude height specified is outside the supported range and will be reduced to fit this range.", attributes);
        extrudeHeight = 8191;
    }
    INTERFACE_ASSERT(numberOfMeshObjects >= 0);
    if(numberOfMeshObjects < 0)
    {
        numberOfMeshObjects = 0;
    }

    std::vector<tMesh_Simple3D*> operands;
    long i;
    for(i = 0; i != numberOfMeshObjects; ++i)
    {
        OperandsFromFaceVertex(*meshObjectPointers[i], operands);
    }

    if(solidObjectsDataSize)
    {
        BuildSolidObjects(solidObjectsData, solidObjectsDataSize, operands);
    }

    if(extrudeHeight != 0)
    {
        for(i = 0; i != SizeL(operands); ++i)
        {
            Extrude(*operands[i], extrudeHeight);
            if(operands[i]->faces() == 1)
            {
                AddVolumeToVerticalFace(*operands[i]);
            }
            operands[i]->consolidate();
        }
    }

    cDynamicFraction maxSlopeFraction(maxSlope);
    maxSlopeFraction /= 100;
    cBSPStats stats;
    cBSPVertices resultVertices;
    tMesh_Dynamic resultDynamic;
    CombineOperandsAndExtractGround(operands, resultDynamic, resultVertices, stats, maxStepHeight, maxSlopeFraction, progressCallBack);

    if(resultDynamic.empty())
    {
        return 0;
    }

    tMesh_3D resultMesh3D;
    ConvertToMesh3D(resultVertices, resultDynamic, resultMesh3D);
    iMesh* result = nSingletons::pathEngine().constructMeshFrom3D(
            resultMesh3D,
            0, 0, 0, // 3d data channels
            options
            );
    return result;
}
