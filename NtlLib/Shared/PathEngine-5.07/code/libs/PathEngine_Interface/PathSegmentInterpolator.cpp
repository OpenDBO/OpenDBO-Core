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
#include "interface/PathSegmentInterpolator.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/STL/algorithm.h"

long
cPathSegmentInterpolator::interpolatorValueAtPoint(const tPoint& p) const
{
    switch(_interpolationType)
    {
    default:
        invalid();
    case X_POS:
        return p.getX() - _start.getX();
    case X_NEG:
        return _start.getX() - p.getX();
    case Y_POS:
        return p.getY() - _start.getY();
    case Y_NEG:
        return _start.getY() - p.getY();
    }
}
long
cPathSegmentInterpolator::otherAxisValueAtPoint(const tPoint& p) const
{
    if(_interpolationType == X_POS || _interpolationType == X_NEG)
    {
        return p.getY();
    }
    return p.getX();
}
tPoint
cPathSegmentInterpolator::pointFor(long interpolatorValue, long otherAxisValue) const
{
    long effectiveRange;
    effectiveRange = _start.getRange();
    if(_end.getRange() > effectiveRange)
    {
        effectiveRange = _end.getRange();
    }
    switch(_interpolationType)
    {
    default:
        invalid();
    case X_POS:
        return tPoint(_start.getX() + interpolatorValue, otherAxisValue, effectiveRange);
    case X_NEG:
        return tPoint(_start.getX() - interpolatorValue, otherAxisValue, effectiveRange);
    case Y_POS:
        return tPoint(otherAxisValue, _start.getY() + interpolatorValue, effectiveRange);
    case Y_NEG:
        return tPoint(otherAxisValue, _start.getY() - interpolatorValue, effectiveRange);
    }
}

void
cPathSegmentInterpolator::initOrReinit(const tPoint& start, const tPoint& end)
{
    _start = start;
    _end = end;

    tLine line(_start, _end);
    tPoint axis = line.axis();
    _length = Length(axis);

    long absoluteX = axis.getX();
    if(absoluteX < 0)
    {
        absoluteX = -absoluteX;
    }
    long absoluteY = axis.getY();
    if(absoluteY < 0)
    {
        absoluteY = -absoluteY;
    }
    
    assertD(absoluteX || absoluteY);
    if(absoluteX >= absoluteY)
    {
        if(axis.getX() > 0)
        {
            _interpolationType = X_POS;
        }
        else
        {
            _interpolationType = X_NEG;
        }
    }
    else
    {
        if(axis.getY() > 0)
        {
            _interpolationType = Y_POS;
        }
        else
        {
            _interpolationType = Y_NEG;
        }
    }
}

const tPoint&
cPathSegmentInterpolator::refStart() const
{
    return _start;
}
long
cPathSegmentInterpolator::segmentLength() const
{
    return _length;
}
long
cPathSegmentInterpolator::interpolatorMax() const
{
    return interpolatorValueAtPoint(_end);
}

void
cPathSegmentInterpolator::interpolate(
        const tPoint& currentPosition,
        long interpolatorTarget,
        bool& exactlyOnLine, tPoint& point1, tPoint& point2
        ) const
{
    assertD(interpolatorTarget >= 0);
    assertD(interpolatorTarget < interpolatorMax());
    
    long interpolatorBase = interpolatorValueAtPoint(currentPosition);
    assertD(interpolatorBase >= 0);
    assertD(interpolatorBase < interpolatorMax());
    assertD(interpolatorTarget >= interpolatorBase);
    if(interpolatorTarget == interpolatorBase)
    {
        exactlyOnLine = true;
        point1 = currentPosition;
        return;
    }

    long otherAxisBase = otherAxisValueAtPoint(currentPosition);
    long otherAxisToEnd = otherAxisValueAtPoint(_end) - otherAxisBase;

    long interpolaterToEnd = interpolatorMax() - interpolatorBase;
    long interpolaterToTarget = interpolatorTarget - interpolatorBase;

    tPoint::tDistance otherAxisToTarget_Multiplied;
    tMultiplier::multiply(otherAxisToEnd, interpolaterToTarget, otherAxisToTarget_Multiplied);
    long otherAxisToTarget;
    tMultiplier::divide(otherAxisToTarget_Multiplied, interpolaterToEnd, otherAxisToTarget);

    point1 = pointFor(interpolatorTarget, otherAxisBase + otherAxisToTarget);

    tLine l(currentPosition, _end);
    eSide side1 = l.side(point1);
    if(side1 == SIDE_CENTRE)
    {
        exactlyOnLine = true;
        return;
    }

    exactlyOnLine = false;
    point2 = pointFor(interpolatorTarget, otherAxisBase + otherAxisToTarget + 1);
    eSide side2 = l.side(point2);
    assertD(side2 != SIDE_CENTRE);
    if(side2 == side1)
    {
        point2 = pointFor(interpolatorTarget, otherAxisBase + otherAxisToTarget - 1);
        side2 = l.side(point2);
        assertD(side2 == OtherSide(side1));
    }
    
    eSide preferredSide = l.side(_start); // prefer to make point1 approximate in direction to negate drift for current position
    if(preferredSide == SIDE_CENTRE)
    { 
        preferredSide = SIDE_LEFT; // in the absence of drift this is chosen arbitrarily to make the resulting points deterministic
    }

    if(side1 != preferredSide)
    {
        std::swap(point1, point2);    
    }
}
