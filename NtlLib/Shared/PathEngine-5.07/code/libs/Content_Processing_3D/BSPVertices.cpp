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
#include "BSPVertices.h"

void
cBSPVertices::approximateX()
{
    long i;
    for(i = SizeL(_originalPoints); i != SizeL(_dynamicPoints); ++i)
    {
        _dynamicPoints[i].x() = _dynamicPoints[i].x().asLong_RoundToNearest();
    }
}

void
cBSPVertices::approximateY()
{
    long i;
    for(i = SizeL(_originalPoints); i != SizeL(_dynamicPoints); ++i)
    {
        _dynamicPoints[i].y() = _dynamicPoints[i].y().asLong_RoundToNearest();
    }
}

void
cBSPVertices::approximateZ()
{
    long i;
    for(i = SizeL(_originalPoints); i != SizeL(_dynamicPoints); ++i)
    {
        _dynamicPoints[i].z() = _dynamicPoints[i].z().asLong_RoundToNearest();
    }
}

