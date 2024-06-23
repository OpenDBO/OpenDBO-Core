//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ObstructionsLoader.h"
#include "libs/Geometry/interface/CollisionShape.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Attributes.h"
#include "common/interface/Assert.h"
#include "common/CLib/stdlib.h"
#include "common/STL/vector.h"

// TODO organise conversions to and from strings better
static void
PositionFromString(const char* str, cPosition& position)
{
    char *ptr;
    position.cell=strtol(str,&ptr,10);
    if(position.cell==-1)
    {
        assertD(*ptr==0);
        return;
    }
    assertD(*ptr==':');
    str=ptr+1;
    position.x=strtol(str,&ptr,10);
    assertD(*ptr==',');
    str=ptr+1;
    position.y=strtol(str,&ptr,10);
    assertD(*ptr==0);
}

iXMLHandler*
cObstructionsLoader::element(const char* el, const char *const* attributes)
{
    if(strcmp(el, "obstacle") != 0)
    {
        return 0;
    }

    const char* position = GetValueForAttribute(attributes, "position");
    assertR(position);
    cPosition p;
    PositionFromString(position, p);
    _loadedPositions.push_back(p);
//    if(!_mesh.positionIsValid(p))
//    {
//        Error("NonFatal", "The position for an obstacle being loaded is not valid. The obstacle will be skipped.");
//        return 0;
//    }

    const char* id = GetValueForAttribute(attributes, "id");
    if(id)
    {
        _ids.push_back(id);
    }
    else
    {
        _ids.push_back(std::string());
    }

    std::vector<long> coordsVector;
    const char* coordsString = GetValueForAttribute(attributes, "vertices");

    while(1)
    {
        char* nextPtr;
        coordsVector.push_back(strtol(coordsString, &nextPtr, 10));
        assertD(*nextPtr == ',');
        coordsString = nextPtr + 1;
        coordsVector.push_back(strtol(coordsString, &nextPtr, 10));
        if(*nextPtr == 0)
        {
            break;
        }
        assertD(*nextPtr == ',');
        coordsString = nextPtr + 1;
    }

    cObstacleInfo info;

//    _mesh.convertPosition(p, info._position);
    _shapes.push_back(cCollisionShape(SizeL(coordsVector) / 2, &coordsVector.front()));
    info._costToTraverse = ValueAsPersistentFloatWithDefault(attributes, "costToTraverse", -1.f);
    {
        long x = ValueAsLongWithDefault(attributes, "costDirectionX", 0);
        long y = ValueAsLongWithDefault(attributes, "costDirectionY", 0);
        info._directionForCostToTraverse = tPoint(x, y, DIRECTION_VECTOR_RANGE);
    }

    _infos.push_back(info);

    return 0;
}
