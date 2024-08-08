//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ICOLLISIONCONTEXT_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ICOLLISIONCONTEXT_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iCollisionContext_Wrapper : public iCollisionContext, public iAPIObjectWrapper
{
public:

    typedef iCollisionContext tWrapped;

	iCollisionContext* _target;

    static long interfaceIndex()
    {
        return 4;
    }

    void
    destroy();

    void
    addAgent(iAgent* agent);

    void
    removeAgent(iAgent* agent);

    long
    getNumberOfAgents() const;

    iAgent*
    getAgent(long index) const;

    bool
    includes(iAgent* agent) const;

    void
    addObstacleSet(iObstacleSet* context);

    void
    removeObstacleSet(iObstacleSet* context);

    long
    getNumberOfObstacleSets() const;

    iObstacleSet*
    getObstacleSet(long index) const;

    void
    updateCollisionPreprocessFor(iShape* shape) const;

    void
    updatePathfindingPreprocessFor(iShape* shape) const;

    void
    setSurfaceTypeTraverseCost(long surfaceType, float cost);

    void
    setSurfaceTypeTraverseCostDirection(long surfaceType, long directionVectorX, long directionVectorY);

    void
    setQueryBounds(long startX, long startY, long endX, long endY);

    void
    clearQueryBounds();

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
