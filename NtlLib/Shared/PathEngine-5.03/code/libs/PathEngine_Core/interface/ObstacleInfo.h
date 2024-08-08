//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef OBSTACLE_INFO_INCLUDED
#define OBSTACLE_INFO_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh2D/interface/InternalPosition.h"

class cCollisionShape;
class iOutputStream;

class cObstacleInfo
{
public:

    cInternalPosition _position;
//    const cCollisionShape* _shape;
    float _costToTraverse;
    tPoint _directionForCostToTraverse;

    cObstacleInfo()
    {
    }

    cObstacleInfo(
        const cInternalPosition& position,
//        const cCollisionShape* shape,
        float costToTraverse,
        const tPoint& directionForCostToTraverse
        ) :
     _position(position),
//     _shape(shape),
     _costToTraverse(costToTraverse),
     _directionForCostToTraverse(directionForCostToTraverse)
    {
    }

    // shape is passed in here to avoid changing the checksum
    // (since this shape is now stored elsewhere)
    void checksum(iOutputStream& os, const cCollisionShape& shape) const;
};

#endif
