//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iCollisionContext_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iCollisionContext_Wrapper::destroy()
{
    unsigned long queryCount = gLogger.methodEntry(4, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(4, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::addAgent(iAgent* agent)
{
    unsigned long queryCount = gLogger.methodEntry(4, 1, (void*)this);
    gLogger.log((void*)agent);
    gLogger.methodEntry_Completed();
    _target->addAgent(ExtractWrapped<iAgent_Wrapper>(agent));
    gLogger.methodExit(4, 1, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::removeAgent(iAgent* agent)
{
    unsigned long queryCount = gLogger.methodEntry(4, 2, (void*)this);
    gLogger.log((void*)agent);
    gLogger.methodEntry_Completed();
    _target->removeAgent(ExtractWrapped<iAgent_Wrapper>(agent));
    gLogger.methodExit(4, 2, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

long
iCollisionContext_Wrapper::getNumberOfAgents() const
{
    unsigned long queryCount = gLogger.methodEntry(4, 3, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->getNumberOfAgents();
    gLogger.methodExit(4, 3, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iAgent*
iCollisionContext_Wrapper::getAgent(long index) const
{
    unsigned long queryCount = gLogger.methodEntry(4, 4, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    iAgent* result = _target->getAgent(index);
    ExtractWrapper<iAgent_Wrapper>(result);
    gLogger.methodExit(4, 4, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iCollisionContext_Wrapper::includes(iAgent* agent) const
{
    unsigned long queryCount = gLogger.methodEntry(4, 5, (void*)this);
    gLogger.log((void*)agent);
    gLogger.methodEntry_Completed();
    bool result = _target->includes(ExtractWrapped<iAgent_Wrapper>(agent));
    gLogger.methodExit(4, 5, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iCollisionContext_Wrapper::addObstacleSet(iObstacleSet* context)
{
    unsigned long queryCount = gLogger.methodEntry(4, 6, (void*)this);
    gLogger.log((void*)context);
    gLogger.methodEntry_Completed();
    _target->addObstacleSet(ExtractWrapped<iObstacleSet_Wrapper>(context));
    gLogger.methodExit(4, 6, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::removeObstacleSet(iObstacleSet* context)
{
    unsigned long queryCount = gLogger.methodEntry(4, 7, (void*)this);
    gLogger.log((void*)context);
    gLogger.methodEntry_Completed();
    _target->removeObstacleSet(ExtractWrapped<iObstacleSet_Wrapper>(context));
    gLogger.methodExit(4, 7, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

long
iCollisionContext_Wrapper::getNumberOfObstacleSets() const
{
    unsigned long queryCount = gLogger.methodEntry(4, 8, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->getNumberOfObstacleSets();
    gLogger.methodExit(4, 8, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iObstacleSet*
iCollisionContext_Wrapper::getObstacleSet(long index) const
{
    unsigned long queryCount = gLogger.methodEntry(4, 9, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    iObstacleSet* result = _target->getObstacleSet(index);
    ExtractWrapper<iObstacleSet_Wrapper>(result);
    gLogger.methodExit(4, 9, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iCollisionContext_Wrapper::updateCollisionPreprocessFor(iShape* shape) const
{
    unsigned long queryCount = gLogger.methodEntry(4, 10, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->updateCollisionPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(4, 10, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::updatePathfindingPreprocessFor(iShape* shape) const
{
    unsigned long queryCount = gLogger.methodEntry(4, 11, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->updatePathfindingPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(4, 11, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::setSurfaceTypeTraverseCost(long surfaceType, float cost)
{
    unsigned long queryCount = gLogger.methodEntry(4, 12, (void*)this);
    gLogger.log(surfaceType);
    gLogger.log(cost);
    gLogger.methodEntry_Completed();
    _target->setSurfaceTypeTraverseCost(surfaceType, cost);
    gLogger.methodExit(4, 12, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::setSurfaceTypeTraverseCostDirection(long surfaceType, long directionVectorX, long directionVectorY)
{
    unsigned long queryCount = gLogger.methodEntry(4, 13, (void*)this);
    gLogger.log(surfaceType);
    gLogger.log(directionVectorX);
    gLogger.log(directionVectorY);
    gLogger.methodEntry_Completed();
    _target->setSurfaceTypeTraverseCostDirection(surfaceType, directionVectorX, directionVectorY);
    gLogger.methodExit(4, 13, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::setQueryBounds(long startX, long startY, long endX, long endY)
{
    unsigned long queryCount = gLogger.methodEntry(4, 14, (void*)this);
    gLogger.log(startX);
    gLogger.log(startY);
    gLogger.log(endX);
    gLogger.log(endY);
    gLogger.methodEntry_Completed();
    _target->setQueryBounds(startX, startY, endX, endY);
    gLogger.methodExit(4, 14, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iCollisionContext_Wrapper::clearQueryBounds()
{
    unsigned long queryCount = gLogger.methodEntry(4, 15, (void*)this);
    gLogger.methodEntry_Completed();
    _target->clearQueryBounds();
    gLogger.methodExit(4, 15, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}
void iCollisionContext_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
