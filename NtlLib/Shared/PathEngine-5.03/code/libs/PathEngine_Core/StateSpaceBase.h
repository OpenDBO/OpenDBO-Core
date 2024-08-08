//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef STATE_SPACE_BASE_INCLUDED
#define STATE_SPACE_BASE_INCLUDED

#include "interface/OffMeshConnections.h"
#include "interface/ShapesCostCalculator.h"
#include "libs/Geometry/interface/tLine.h"
#include "SilhouetteZone.h"

template <class tCorners>
class bStateSpace
{
    const cOffMeshConnections& _connections;
    const tCorners& _corners;

protected:

    cInternalPosition _goal;
    cInternalPosition _start;

public:

    bStateSpace(
        const cOffMeshConnections& connections,
        const tCorners& corners) :
     _connections(connections),
     _corners(corners)
    {
    }

    void setGoal(const cInternalPosition& value)
    {
        _goal = value;
    }
    void setStart(const cInternalPosition& value)
    {
        _start = value;
    }

// define ordering for points

    long points() const
    {
        return 2 + _connections.numberOfEndPoints() + _corners.size();
//        return 2 + _corners.size();
    }

    cInternalPosition getPosition(long index) const
    {
        if(index < 2)
        {
            if(index == 0)
            {
                return _goal;
            }
            return _start;
        }
        index -= 2;
        if(index < _connections.numberOfEndPoints())
        {
            return _connections.refEndPoint(index);
        }
        index -= _connections.numberOfEndPoints();
        return cInternalPosition(_corners.cornerFace(index), _corners.cornerPoint(index));
    }
    bool pointIsCorner(long index) const
    {
        return index >= 2 + _connections.numberOfEndPoints();
//        return index >= 2;
    }

    enum ePointType
    {
        GOAL,
        START,
        END_POINT,
        CORNER,
    };

    ePointType getPointTypeAndSubIndex(long point, long& indexWithinType) const
    {
        indexWithinType = point;
        if(indexWithinType < 2)
        {
            if(indexWithinType == 0)
            {
                return GOAL;
            }
            indexWithinType--;
            return START;
        }
        indexWithinType -= 2;
        if(indexWithinType < _connections.numberOfEndPoints())
        {
            return END_POINT;
        }
        indexWithinType -= _connections.numberOfEndPoints();
        return CORNER;
    }

    long pointForEndPoint(long endPoint) const
    {
        return 2 + endPoint;
    }
    long endPointForPoint(long point) const
    {
        return point - 2;
    }

// define ordering for states

    long states() const
    {
        return points() * 2;
    }
    long pointForState(long state) const
    {
        return state / 2;
    }
    cInternalPosition positionForState(long state) const
    {
        return getPosition(state / 2);
    }

    long encodeEndPoint(long pointIndex, bool departing) const
    {
        pointIndex <<= 1;
        if(!departing)
        {
            pointIndex++;
        }
        return pointIndex;
    }
    long encodeCorner(long pointIndex, eSilhouetteZone approachZone) const
    {
        pointIndex <<= 1;
        if(approachZone == SZ_RIGHTSILHOUETTE)
        {
            pointIndex++;
        }
        return pointIndex;
    }

    bool stateIsCorner(long state) const
    {
        return pointIsCorner(pointForState(state));
    }

    void decodeEndPoint(long state, long& pointIndex, bool& departing) const
    {
        pointIndex = (state>>1);
        departing = ((state & 1) == 0);
    }
    void decodeCorner(long state, long& pointIndex, eSilhouetteZone& approachZone) const
    {
        pointIndex = (state>>1);
        approachZone = SZ_LEFTSILHOUETTE;
        if(state & 1)
        {
            approachZone = SZ_RIGHTSILHOUETTE;
        }
    }
    bool decodeIsDeparting(long state) const
    {
        return ((state & 1) == 0);
    }
    eSilhouetteZone decodeApproachZone(long state) const
    {
        if(state & 1)
        {
            return SZ_RIGHTSILHOUETTE;
        }
        return SZ_LEFTSILHOUETTE;
    }

    long reverseState(long state) const
    {
        state ^= 1;
        return state;
    }

    long startState() const
    {
        return 3;
    }

// silhouette zone helpers

    eSilhouetteZone zoneFor(long pointIndex, const tPoint &target) const
    {
        long cornerIndex = pointIndex - (2 + _connections.numberOfEndPoints());
//        long cornerIndex = pointIndex - 2;
        return ZoneFor(
            _corners.axisBefore(cornerIndex),
            _corners.axisAfter(cornerIndex),
            target - _corners.cornerPoint(cornerIndex));
    }

    bool checkSourceCornerSilhouetteConditions(long previousState, long sourceState, const tPoint& targetP) const
    {
        if(!stateIsCorner(sourceState))
        {
            return true;
        }
        // assume that start start is not a corner, so previousState is valid if we get here

        tLine line(positionForState(previousState).p, positionForState(sourceState).p);
        
        long pointIndex;
        eSilhouetteZone approachZone;
        decodeCorner(sourceState, pointIndex, approachZone);
        if(approachZone == SZ_LEFTSILHOUETTE)
        {
            return line.side(targetP) == SIDE_RIGHT && zoneFor(pointIndex, targetP) == SZ_RIGHTSILHOUETTE;
        }
        else
        {
            return line.side(targetP) == SIDE_LEFT && zoneFor(pointIndex, targetP) == SZ_LEFTSILHOUETTE;
        }
    }

// other helpers

    unsigned long getCost(const cInternalPosition& from, const cInternalPosition& to) const
    {
        return Length(to.p - from.p);
    }

// (partial) interface for AStar

    long getNumberOfStates() const
    {
        return states();
    }
    bool isGoal(long index) const
    {
        return index == 0;
    }
    unsigned long getMinimumCostToGoal(long state) const
    {
        return Length(_goal.p - positionForState(state).p);
    }

// path building helpers

    long getConnectionIndex(long fromState, long toState) const
    {
        long fromPoint = pointForState(fromState);
        long toPoint = pointForState(toState);
        long fromIndex;
        ePointType fromType = getPointTypeAndSubIndex(fromPoint, fromIndex);
        if(fromType != END_POINT)
        {
            return -1;
        }
        if(!decodeIsDeparting(fromState))
        {
            return -1;
        }
        long toIndex;
        ePointType toType = getPointTypeAndSubIndex(toPoint, toIndex);
        if(toType != END_POINT)
        {
            return -1;
        }
        long result = _connections.lookupConnection(fromIndex, toIndex);
        assertD(result != -1);
        return result;
    }
};

#endif

