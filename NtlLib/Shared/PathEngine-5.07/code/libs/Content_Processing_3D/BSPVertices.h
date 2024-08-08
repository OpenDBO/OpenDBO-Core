//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_VERTICES_INCLUDED
#define BSP_VERTICES_INCLUDED

#include "DynamicPoint3.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "common/STL/vector.h"

class cBSPVertices
{
    std::vector<tPoint3> _originalPoints;
    std::vector<cDynamicPoint3> _dynamicPoints;
 
public:

    void
    swap(cBSPVertices& rhs)
    {
        _originalPoints.swap(rhs._originalPoints);
        _dynamicPoints.swap(rhs._dynamicPoints);
    }

    long
    addOriginal(const tPoint3& p)
    {
        assertD(_dynamicPoints.size() == _originalPoints.size());
        long index = SizeL(_originalPoints);
        _originalPoints.push_back(p);
        _dynamicPoints.push_back(p);
        return index;
    }

    long
    addDynamic(const cDynamicPoint3& p)
    {
        long index = SizeL(_dynamicPoints);
        _dynamicPoints.push_back(p);
        return index;
    }

    bool
    isOriginal(long index) const
    {
        return index < SizeL(_originalPoints);
    }
    const
    tPoint3& lookupOriginal(long index) const
    {
        return _originalPoints[index];
    }
    const
    cDynamicPoint3& lookupDynamic(long index) const
    {
        return _dynamicPoints[index];
    }

    void approximateX();
    void approximateY();
    void approximateZ();
};

#endif
