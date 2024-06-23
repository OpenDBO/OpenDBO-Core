//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"

void ApproximatePointsAlongLine(const tLine& line,
                                    long line_length,
                                    float distance_along,
                                    bool& exactly_on_line,
                                    tPoint& point1,
                                    tPoint& point2);

void ApproximatePointsAlongLine(const tLine& line,
                                    float distance_along,
                                    bool& exactly_on_line,
                                    tPoint& point1,
                                    tPoint& point2);
