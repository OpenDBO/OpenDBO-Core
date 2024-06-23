//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iObstacleSet_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iObstacleSet_Wrapper::destroy()
{
    unsigned long queryCount = gLogger.methodEntry(5, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(5, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iObstacleSet_Wrapper::addAgent(iAgent* agent)
{
    unsigned long queryCount = gLogger.methodEntry(5, 1, (void*)this);
    gLogger.log((void*)agent);
    gLogger.methodEntry_Completed();
    _target->addAgent(ExtractWrapped<iAgent_Wrapper>(agent));
    gLogger.methodExit(5, 1, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iObstacleSet_Wrapper::removeAgent(iAgent* agent)
{
    unsigned long queryCount = gLogger.methodEntry(5, 2, (void*)this);
    gLogger.log((void*)agent);
    gLogger.methodEntry_Completed();
    _target->removeAgent(ExtractWrapped<iAgent_Wrapper>(agent));
    gLogger.methodExit(5, 2, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

long
iObstacleSet_Wrapper::getNumberOfAgents() const
{
    unsigned long queryCount = gLogger.methodEntry(5, 3, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->getNumberOfAgents();
    gLogger.methodExit(5, 3, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iAgent*
iObstacleSet_Wrapper::getAgent(long index) const
{
    unsigned long queryCount = gLogger.methodEntry(5, 4, (void*)this);
    gLogger.log(index);
    gLogger.methodEntry_Completed();
    iAgent* result = _target->getAgent(index);
    ExtractWrapper<iAgent_Wrapper>(result);
    gLogger.methodExit(5, 4, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iObstacleSet_Wrapper::includes(iAgent* agent) const
{
    unsigned long queryCount = gLogger.methodEntry(5, 5, (void*)this);
    gLogger.log((void*)agent);
    gLogger.methodEntry_Completed();
    bool result = _target->includes(ExtractWrapped<iAgent_Wrapper>(agent));
    gLogger.methodExit(5, 5, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

long
iObstacleSet_Wrapper::size() const
{
    unsigned long queryCount = gLogger.methodEntry(5, 6, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->size();
    gLogger.methodExit(5, 6, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iObstacleSet_Wrapper::empty() const
{
    unsigned long queryCount = gLogger.methodEntry(5, 7, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->empty();
    gLogger.methodExit(5, 7, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iObstacleSet_Wrapper::updateCollisionPreprocessFor(iShape* shape) const
{
    unsigned long queryCount = gLogger.methodEntry(5, 8, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->updateCollisionPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(5, 8, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iObstacleSet_Wrapper::updatePathfindingPreprocessFor(iShape* shape) const
{
    unsigned long queryCount = gLogger.methodEntry(5, 9, (void*)this);
    gLogger.log((void*)shape);
    gLogger.methodEntry_Completed();
    _target->updatePathfindingPreprocessFor(ExtractWrapped<iShape_Wrapper>(shape));
    gLogger.methodExit(5, 9, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}
void iObstacleSet_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
