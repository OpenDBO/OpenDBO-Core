//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ObstacleInfo.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/CollisionShape.h"
#include "common/interface/iOutputStream_Operators.h"
#include "platform_common/FloatPersistence.h"

void
cObstacleInfo::checksum(iOutputStream& os, const cCollisionShape& shape) const
{
    os << _position.f.index();
    os << _position.p.getX();
    os << _position.p.getY();
    shape.checksum(os);
    std::string persistent = FloatToPersistent(_costToTraverse);
    os << persistent;
    os << _directionForCostToTraverse.getX();
    os << _directionForCostToTraverse.getY();
}

