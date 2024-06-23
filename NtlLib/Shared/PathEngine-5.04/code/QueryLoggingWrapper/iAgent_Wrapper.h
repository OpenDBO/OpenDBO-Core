//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IAGENT_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IAGENT_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iAgent_Wrapper : public iAgent, public iAPIObjectWrapper
{
public:

    typedef iAgent tWrapped;

	iAgent* _target;

    static long interfaceIndex()
    {
        return 3;
    }

    void
    destroy();

    iMesh*
    getMesh() const;

    iShape*
    getShape() const;

    void
    setUserData(void* voidPointer);

    void*
    getUserData() const;

    cPosition
    getPosition() const;

    void
    moveTo(const cPosition& newPosition);

    bool
    canCollide() const;

    bool
    canPathfind() const;

    bool
    testCollisionAt(iCollisionContext* context, const cPosition& newPosition) const;

    bool
    testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const;

    bool
    testCollisionTo_XY(iCollisionContext* context, long x, long y, long& cell) const;

    iCollisionInfo*
    firstCollisionTo(iCollisionContext* context, long x, long y, long& cell) const;

    cPosition
    findClosestUnobstructedPosition(iCollisionContext* context, long maximumDistance) const;

    void
    getAllAgentsOverlapped(iCollisionContext* context, iAgent** resultsBuffer, long& numberOverlapped) const;

    bool
    testCollisionDirectlyAgainstPlacedShape(iShape* shape, const cPosition& shapePosition) const;

    iPath*
    findShortestPathTo(iCollisionContext* context, const cPosition& target) const;

    iPath*
    findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const;

    iPath*
    findPathAway(iCollisionContext* context, const cPosition& awayFrom, long distanceAway) const;

    iPath*
    findPathAway_WithQueryCallBack(iCollisionContext* context, const cPosition& awayFrom, long distanceAway, iQueryCallBack* queryCallBack) const;

    iCollisionInfo*
    advanceAlongPath(iPath* path, float distance, iCollisionContext* context);

    iCollisionInfo*
    advanceAlongPathWithPrecision(iPath* path, float distance, iCollisionContext* context, float& precisionX, float& precisionY);

    iPath*
    generateCurvedPath(iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;

    void
    setTraverseCost(float cost);

    void
    setTraverseCostDirection(long directionVectorX, long directionVectorY);

    float
    getTraverseCost() const;

    void
    getTraverseCostDirection(long& directionVectorX, long& directionVectorY) const;

    iPath*
    generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, long startVectorX, long startVectorY, long endVectorX, long endVectorY, long sectionLength, float turnRatio1, float turnRatio2) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
