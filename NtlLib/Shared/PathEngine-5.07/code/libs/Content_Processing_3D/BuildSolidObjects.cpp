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
#include "BuildSolidObjects.h"
#include "ConvexHull3D.h"
#include "tMesh_Simple3D.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "common/interface/Error.h"
#include "common/interface/Assert.h"
#include "platform_common/CoordinateRanges.h"

void
BuildSolidObjects(
        const long* solidObjectsData,
        long solidObjectsDataSize,
        std::vector<tMesh_Simple3D*>& operands
        )
{
    INTERFACE_ASSERT(solidObjectsDataSize >= 1);
    std::vector<tPoint3> points;
    bool emittedOutOfRangeError = false;
    long position = 0;
    while(1)
    {
        long numberOfPoints = solidObjectsData[position++];
        if(numberOfPoints == 0)
        {
          // reached end of data stream
            if(position != solidObjectsDataSize)
            {
                Error("NonFatal", "BuildMeshFrom3DContent() - Object data end encountered before specified data size.");
            }
            return;
        }
        assertR(solidObjectsDataSize >= position + numberOfPoints * 3 + 1);
        if(solidObjectsDataSize < position + numberOfPoints * 3 + 1)
        {
            Error("NonFatal", "BuildMeshFrom3DContent() - Bad solid objects data encountered, ignoring remainder of buffer");
            return;
        }
        const long* pointData = solidObjectsData + position;
        position += numberOfPoints * 3;
        points.resize(0);
        long i;
        for(i = 0; i != numberOfPoints * 3; ++i)
        {
            if(pointData[i] < -WORLD_RANGE || pointData[i] > WORLD_RANGE)
            {
                if(!emittedOutOfRangeError)
                {
                    Error("NonFatal", "BuildMeshFrom3DContent() - Out of range coordinates encountered in solid object data. Objects with out of range coordinates will be skipped.");
                    emittedOutOfRangeError = true;
                }
                break;
            }
        }
        if(i != numberOfPoints * 3)
        {
            continue;
        }
        for(i = 0; i != numberOfPoints; ++i)
        {
            points.push_back(tPoint3(pointData[i * 3], pointData[i * 3 + 1], pointData[i * 3 + 2], WORLD_RANGE));
        }
        operands.push_back(new tMesh_Simple3D);
        ConvexHull3D(SizeL(points), &points.front(), *operands.back());
        operands.back()->consolidate();
    }
}
