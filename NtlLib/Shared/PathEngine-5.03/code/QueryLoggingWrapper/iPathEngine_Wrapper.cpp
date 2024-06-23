//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iPathEngine_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
const char *const*
iPathEngine_Wrapper::getVersionAttributes() const
{
    unsigned long queryCount = gLogger.methodEntry(0, 0, (void*)this);
    gLogger.methodEntry_Completed();
    const char *const* result = _target->getVersionAttributes();
    gLogger.methodExit(0, 0, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

long
iPathEngine_Wrapper::getInterfaceMajorVersion() const
{
    unsigned long queryCount = gLogger.methodEntry(0, 1, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->getInterfaceMajorVersion();
    gLogger.methodExit(0, 1, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

long
iPathEngine_Wrapper::getInterfaceMinorVersion() const
{
    unsigned long queryCount = gLogger.methodEntry(0, 2, (void*)this);
    gLogger.methodEntry_Completed();
    long result = _target->getInterfaceMinorVersion();
    gLogger.methodExit(0, 2, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::getReleaseNumbers(long& majorReleaseNumber, long& minorReleaseNumber, long& internalReleaseNumber) const
{
    unsigned long queryCount = gLogger.methodEntry(0, 3, (void*)this);
    gLogger.methodEntry_Completed();
    _target->getReleaseNumbers(majorReleaseNumber, minorReleaseNumber, internalReleaseNumber);
    gLogger.methodExit(0, 3, queryCount, (void*)this);
    gLogger.log(majorReleaseNumber);
    gLogger.log(minorReleaseNumber);
    gLogger.log(internalReleaseNumber);
    gLogger.methodExit_Completed();
}

void
iPathEngine_Wrapper::setErrorHandler(iErrorHandler* newHandler)
{
    unsigned long queryCount = gLogger.methodEntry(0, 4, (void*)this);
    gLogger.methodEntry_Completed();
    _target->setErrorHandler(newHandler);
    gLogger.methodExit(0, 4, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iErrorHandler*
iPathEngine_Wrapper::getErrorHandler()
{
    unsigned long queryCount = gLogger.methodEntry(0, 5, (void*)this);
    gLogger.methodEntry_Completed();
    iErrorHandler* result = _target->getErrorHandler();
    gLogger.methodExit(0, 5, queryCount, (void*)this);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::setPathPoolParameters(long pathsPooled, long pointsPerPath)
{
    unsigned long queryCount = gLogger.methodEntry(0, 6, (void*)this);
    gLogger.log(pathsPooled);
    gLogger.log(pointsPerPath);
    gLogger.methodEntry_Completed();
    _target->setPathPoolParameters(pathsPooled, pointsPerPath);
    gLogger.methodExit(0, 6, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iPathEngine_Wrapper::shapeIsValid(long numberOfPoints, const long* coordinateData)
{
    unsigned long queryCount = gLogger.methodEntry(0, 7, (void*)this);
    gLogger.logCollisionShape(numberOfPoints, coordinateData);
    gLogger.methodEntry_Completed();
    bool result = _target->shapeIsValid(numberOfPoints, coordinateData);
    gLogger.methodExit(0, 7, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iShape*
iPathEngine_Wrapper::newShape(long numberOfPoints, const long* coordinateData)
{
    unsigned long queryCount = gLogger.methodEntry(0, 8, (void*)this);
    gLogger.logCollisionShape(numberOfPoints, coordinateData);
    gLogger.methodEntry_Completed();
    iShape* result = _target->newShape(numberOfPoints, coordinateData);
    result = WrapNew<iShape_Wrapper>(result);
    gLogger.methodExit(0, 8, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iMesh*
iPathEngine_Wrapper::loadMeshFromBuffer(const char* format, const char* dataBuffer, unsigned long bufferSize, const char *const* options)
{
    unsigned long queryCount = gLogger.methodEntry(0, 9, (void*)this);
    gLogger.log(format);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.log(options);
    gLogger.methodEntry_Completed();
    iMesh* result = _target->loadMeshFromBuffer(format, dataBuffer, bufferSize, options);
    result = WrapNew<iMesh_Wrapper>(result);
    gLogger.methodExit(0, 9, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iMesh*
iPathEngine_Wrapper::buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char *const* options)
{
    unsigned long queryCount = gLogger.methodEntry(0, 10, (void*)this);
    gLogger.logSourceContent(geometryObjectPointers, numberOfGeometryObjects);
    gLogger.log(options);
    gLogger.methodEntry_Completed();
    iMesh* result = _target->buildMeshFromContent(geometryObjectPointers, numberOfGeometryObjects, options);
    result = WrapNew<iMesh_Wrapper>(result);
    gLogger.methodExit(0, 10, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char* format, iOutputStream* os)
{
    unsigned long queryCount = gLogger.methodEntry(0, 11, (void*)this);
    gLogger.logSourceContent(geometryObjectPointers, numberOfGeometryObjects);
    gLogger.log(format);
    gLogger.methodEntry_Completed();
    _target->saveContentData(geometryObjectPointers, numberOfGeometryObjects, format, os);
    gLogger.methodExit(0, 11, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

unsigned long
iPathEngine_Wrapper::totalMemoryAllocated()
{
    unsigned long queryCount = gLogger.methodEntry(0, 12, (void*)this);
    gLogger.methodEntry_Completed();
    unsigned long result = _target->totalMemoryAllocated();
    gLogger.methodExit(0, 12, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

unsigned long
iPathEngine_Wrapper::maximumMemoryAllocated()
{
    unsigned long queryCount = gLogger.methodEntry(0, 13, (void*)this);
    gLogger.methodEntry_Completed();
    unsigned long result = _target->maximumMemoryAllocated();
    gLogger.methodExit(0, 13, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::resetMaximumMemoryAllocated()
{
    unsigned long queryCount = gLogger.methodEntry(0, 14, (void*)this);
    gLogger.methodEntry_Completed();
    _target->resetMaximumMemoryAllocated();
    gLogger.methodExit(0, 14, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iPathEngine_Wrapper::setRandomSeed(unsigned long value)
{
    unsigned long queryCount = gLogger.methodEntry(0, 15, (void*)this);
    gLogger.log(value);
    gLogger.methodEntry_Completed();
    _target->setRandomSeed(value);
    gLogger.methodExit(0, 15, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iPathEngine_Wrapper::collisionPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize)
{
    unsigned long queryCount = gLogger.methodEntry(0, 16, (void*)this);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    bool result = _target->collisionPreprocessVersionIsCompatible(dataBuffer, bufferSize);
    gLogger.methodExit(0, 16, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iPathEngine_Wrapper::pathfindPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize)
{
    unsigned long queryCount = gLogger.methodEntry(0, 17, (void*)this);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    bool result = _target->pathfindPreprocessVersionIsCompatible(dataBuffer, bufferSize);
    gLogger.methodExit(0, 17, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iPathEngine_Wrapper::deleteAllObjects()
{
    unsigned long queryCount = gLogger.methodEntry(0, 18, (void*)this);
    gLogger.methodEntry_Completed();
    _target->deleteAllObjects();
    gLogger.methodExit(0, 18, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iContentChunk*
iPathEngine_Wrapper::newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes)
{
    unsigned long queryCount = gLogger.methodEntry(0, 19, (void*)this);
    gLogger.log(attributes);
    gLogger.methodEntry_Completed();
    iContentChunk* result = _target->newContentChunk(ground, anchorsAndShapes, attributes);
    result = WrapNew<iContentChunk_Wrapper>(result);
    gLogger.methodExit(0, 19, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iContentChunk*
iPathEngine_Wrapper::loadContentChunk(const char* dataBuffer, unsigned long bufferSize)
{
    unsigned long queryCount = gLogger.methodEntry(0, 20, (void*)this);
    gLogger.logDataBuffer(dataBuffer, bufferSize);
    gLogger.methodEntry_Completed();
    iContentChunk* result = _target->loadContentChunk(dataBuffer, bufferSize);
    result = WrapNew<iContentChunk_Wrapper>(result);
    gLogger.methodExit(0, 20, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}
