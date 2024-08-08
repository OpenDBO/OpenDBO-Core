//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iMeshFederation_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iMeshFederation_Wrapper::destroy()
{
    unsigned long queryCount = gLogger.methodEntry(11, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(11, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iMeshFederation_Wrapper::save(const char* format, iOutputStream* outputStream) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 1, (void*)this);
    gLogger.log(format);
    gLogger.methodEntry_Completed();
    _target->save(format, outputStream);
    gLogger.methodExit(11, 1, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

long
iMeshFederation_Wrapper::size() const
{
    unsigned long queryCount = gLogger.methodEntry(11, 2, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->size();
    gLogger.methodExit(11, 2, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iMeshFederation_Wrapper::coordinatesAreInsideRange(long x, long y) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 3, (void*)this);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    bool result = _target->coordinatesAreInsideRange(x, y);
    gLogger.methodExit(11, 3, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

long
iMeshFederation_Wrapper::tileForQuery(long queryStartX, long queryStartY) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 4, (void*)this);
    gLogger.log(queryStartX);
    gLogger.log(queryStartY);
    gLogger.methodEntry_Completed();
    long result = _target->tileForQuery(queryStartX, queryStartY);
    gLogger.methodExit(11, 4, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iMeshFederation_Wrapper::getTileCentre(long tileIndex, long& centreX, long& centreY) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 5, (void*)this);
    gLogger.log(tileIndex);
    gLogger.methodEntry_Completed();
    _target->getTileCentre(tileIndex, centreX, centreY);
    gLogger.methodExit(11, 5, queryCount, (void*)this);
    gLogger.log(centreX);
    gLogger.log(centreY);
    gLogger.methodExit_Completed();
}

void
iMeshFederation_Wrapper::getHandledRegion_World(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 6, (void*)this);
    gLogger.log(tileIndex);
    gLogger.methodEntry_Completed();
    _target->getHandledRegion_World(tileIndex, minX, minY, maxX, maxY);
    gLogger.methodExit(11, 6, queryCount, (void*)this);
    gLogger.log(minX);
    gLogger.log(minY);
    gLogger.log(maxX);
    gLogger.log(maxY);
    gLogger.methodExit_Completed();
}

void
iMeshFederation_Wrapper::getRepresentedRegion_World(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 7, (void*)this);
    gLogger.log(tileIndex);
    gLogger.methodEntry_Completed();
    _target->getRepresentedRegion_World(tileIndex, minX, minY, maxX, maxY);
    gLogger.methodExit(11, 7, queryCount, (void*)this);
    gLogger.log(minX);
    gLogger.log(minY);
    gLogger.log(maxX);
    gLogger.log(maxY);
    gLogger.methodExit_Completed();
}

void
iMeshFederation_Wrapper::getHandledRegion_Local(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 8, (void*)this);
    gLogger.log(tileIndex);
    gLogger.methodEntry_Completed();
    _target->getHandledRegion_Local(tileIndex, minX, minY, maxX, maxY);
    gLogger.methodExit(11, 8, queryCount, (void*)this);
    gLogger.log(minX);
    gLogger.log(minY);
    gLogger.log(maxX);
    gLogger.log(maxY);
    gLogger.methodExit_Completed();
}

void
iMeshFederation_Wrapper::getRepresentedRegion_Local(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 9, (void*)this);
    gLogger.log(tileIndex);
    gLogger.methodEntry_Completed();
    _target->getRepresentedRegion_Local(tileIndex, minX, minY, maxX, maxY);
    gLogger.methodExit(11, 9, queryCount, (void*)this);
    gLogger.log(minX);
    gLogger.log(minY);
    gLogger.log(maxX);
    gLogger.log(maxY);
    gLogger.methodExit_Completed();
}

cPosition
iMeshFederation_Wrapper::translatePosition(long fromTileIndex, const iMesh* fromMesh, long toTileIndex, const iMesh* toMesh, const cPosition& fromPosition) const
{
    unsigned long queryCount = gLogger.methodEntry(11, 10, (void*)this);
    gLogger.log(fromTileIndex);
    gLogger.log((void*)fromMesh);
    gLogger.log(toTileIndex);
    gLogger.log((void*)toMesh);
    gLogger.log(fromPosition);
    gLogger.methodEntry_Completed();
    cPosition result = _target->translatePosition(fromTileIndex, ExtractWrapped<const iMesh_Wrapper>(fromMesh), toTileIndex, ExtractWrapped<const iMesh_Wrapper>(toMesh), fromPosition);
    gLogger.methodExit(11, 10, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}
void iMeshFederation_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
