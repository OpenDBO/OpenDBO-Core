//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "PlueckerLine.h"

cPlueckerLine::cPlueckerLine(const cVector3F& start, const cVector3F& end)
{
    axis = end - start;
    product = axis.crossProduct(start);
}

