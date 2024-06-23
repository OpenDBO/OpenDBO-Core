//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "interface/ApproximatePointsAlongLine.h"
#include "common/interface/Assert.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/tLine.h"

void ApproximatePointsAlongLine(const tLine& line,
                                    long lineLength,
                                    float distanceAlong,
                                    bool& exactly_on_line,
                                    tPoint& point1,
                                    tPoint& point2)
{
    tPoint vector = line.axis();
    eSide side;
    long absoluteX, absoluteY;
    long correction;

    tPoint divided = tPoint(
                    static_cast<long>(vector.getX() * distanceAlong / lineLength),
                    static_cast<long>(vector.getY() * distanceAlong / lineLength),
                    vector.getRange());

    side = vector.side(divided);
    
    if(side==SIDE_CENTRE)
    {
        point1=line.start()+divided;
        exactly_on_line=true;
        return;
    }

    exactly_on_line=false;

    if(side==SIDE_LEFT)
        point1=divided;
    else
        point2=divided;

    absoluteX=vector.getX();
    if(absoluteX<0)
        absoluteX=-absoluteX;
    absoluteY=vector.getY();
    if(absoluteY<0)
        absoluteY=-absoluteY;
    
    // horizontal, vertical, and 45 degree vectors will not have rounding problems
    assertD(absoluteX);
    assertD(absoluteY);
    assertD(absoluteX!=absoluteY);

    correction=1;
    if(side==SIDE_RIGHT)
        correction=-1;

    // force rounding in axis with smaller component
    if(absoluteY>absoluteX)
    {
        if(vector.getY()>0)
            divided+=tPoint(correction,0,1);
        else
            divided-=tPoint(correction,0,1);
    }
    else
    {
        if(vector.getX()<0)
            divided+=tPoint(0,correction,1);
        else
            divided-=tPoint(0,correction,1);
    }
    assertD(vector.side(divided)!=side);

    if(side==SIDE_LEFT)
        point2=divided;
    else
        point1=divided;

    point1+=line.start();
    point2+=line.start();
}

void ApproximatePointsAlongLine(const tLine& line,
                                    float distanceAlong,
                                    bool& exactly_on_line,
                                    tPoint& point1,
                                    tPoint& point2)
{
    long lineLength = Length(line.axis());
    ApproximatePointsAlongLine(line, lineLength, distanceAlong, exactly_on_line,
                point1, point2);
}
