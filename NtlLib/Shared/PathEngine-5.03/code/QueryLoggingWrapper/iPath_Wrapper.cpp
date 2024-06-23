//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iPath_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iPath_Wrapper::destroy()
{
    unsigned long queryCount = gLogger.methodEntry(6, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(6, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

long
iPath_Wrapper::size() const
{
    unsigned long queryCount = gLogger.methodEntry(6, 1, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->size();
    gLogger.methodExit(6, 1, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iPath_Wrapper::position(long pointIndex) const
{
    unsigned long queryCount = gLogger.methodEntry(6, 2, (void*)this);
    gLogger.log(pointIndex);
    gLogger.methodEntry_Completed();
    cPosition result = _target->position(pointIndex);
    gLogger.methodExit(6, 2, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

long
iPath_Wrapper::connectionIndex(long segmentIndex) const
{
    unsigned long queryCount = gLogger.methodEntry(6, 3, (void*)this);
    gLogger.log(segmentIndex);
    gLogger.methodEntry_Completed();
    long result = _target->connectionIndex(segmentIndex);
    gLogger.methodExit(6, 3, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

const cPosition*
iPath_Wrapper::getPositionArray() const
{
    unsigned long queryCount = gLogger.methodEntry(6, 4, (void*)this);
    gLogger.methodEntry_Completed();
    const cPosition* result = _target->getPositionArray();
    gLogger.methodExit(6, 4, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}

const long*
iPath_Wrapper::getConnectionIndexArray() const
{
    unsigned long queryCount = gLogger.methodEntry(6, 5, (void*)this);
    gLogger.methodEntry_Completed();
    const long* result = _target->getConnectionIndexArray();
    gLogger.methodExit(6, 5, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}

unsigned long
iPath_Wrapper::getLength() const
{
    unsigned long queryCount = gLogger.methodEntry(6, 6, (void*)this);
    gLogger.methodEntry_Completed();
    unsigned long result = _target->getLength();
    gLogger.methodExit(6, 6, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iCollisionInfo*
iPath_Wrapper::advanceAlong(iShape* shape, float distance, iCollisionContext* context, float& precisionX, float& precisionY)
{
    unsigned long queryCount = gLogger.methodEntry(6, 7, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log(distance);
    gLogger.log((void*)context);
    gLogger.methodEntry_Completed();
    iCollisionInfo* result = _target->advanceAlong(ExtractWrapped<iShape_Wrapper>(shape), distance, ExtractWrapped<iCollisionContext_Wrapper>(context), precisionX, precisionY);
    result = WrapNew<iCollisionInfo_Wrapper>(result);
    gLogger.methodExit(6, 7, queryCount, (void*)this);
    gLogger.log(precisionX);
    gLogger.log(precisionY);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPath_Wrapper::renderOnGround(long originX, long originY, iRender3DLinesCallBack& callBack) const
{
    unsigned long queryCount = gLogger.methodEntry(6, 8, (void*)this);
    gLogger.log(originX);
    gLogger.log(originY);
    gLogger.methodEntry_Completed();
    _target->renderOnGround(originX, originY, callBack);
    gLogger.methodExit(6, 8, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}
void iPath_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
