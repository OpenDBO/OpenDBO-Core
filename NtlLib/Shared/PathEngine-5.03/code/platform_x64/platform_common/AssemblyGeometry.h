//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ASSEMBLY_GEOMETRY_INCLUDED
#define ASSEMBLY_GEOMETRY_INCLUDED

#include "libs/Geometry/interface/SideEnum.h"
#include "platform_support/LargeIntegers.h"

namespace nAssemblyGeometry
{

inline eSide
side(long baseX, long baseY, long testX, long testY)
{
//    long lhs, rhs;
//    _asm
//    {
//        mov eax, baseY
//        imul testX
//        jc asmlabel_multipliedoutof32bits
//        mov lhs, eax
//        mov eax, baseX
//        imul testY
//        jc asmlabel_multipliedoutof32bits
//        mov rhs, eax
//    }
//    if(lhs < rhs)
//    {
//        return SIDE_LEFT;
//    }
//    if(lhs == rhs)
//    {
//        return SIDE_CENTRE;
//    }
//    return SIDE_RIGHT;
//asmlabel_multipliedoutof32bits:
    __int64 lhs, rhs;
    lhs = testX;
    lhs *= baseY;
    rhs = testY;
    rhs *= baseX;
    if(lhs < rhs)
    {
        return SIDE_LEFT;
    }
    if(lhs == rhs)
    {
        return SIDE_CENTRE;
    }
    return SIDE_RIGHT;
}

}

#endif
