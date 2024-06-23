//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IPATHENGINE_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IPATHENGINE_WRAPPER_INCLUDED

#include "i_pathengine.h"

class iPathEngine_Wrapper : public iPathEngine
{
public:

    typedef iPathEngine tWrapped;

	iPathEngine* _target;

    static long interfaceIndex()
    {
        return 0;
    }

    const char *const*
    getVersionAttributes() const;

    long
    getInterfaceMajorVersion() const;

    long
    getInterfaceMinorVersion() const;

    void
    getReleaseNumbers(long& majorReleaseNumber, long& minorReleaseNumber, long& internalReleaseNumber) const;

    void
    setErrorHandler(iErrorHandler* newHandler);

    iErrorHandler*
    getErrorHandler();

    void
    setPathPoolParameters(long pathsPooled, long pointsPerPath);

    bool
    shapeIsValid(long numberOfPoints, const long* coordinateData);

    iShape*
    newShape(long numberOfPoints, const long* coordinateData);

    iMesh*
    loadMeshFromBuffer(const char* format, const char* dataBuffer, unsigned long bufferSize, const char *const* options);

    iMesh*
    buildMeshFromContent(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char *const* options);

    void
    saveContentData(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects, const char* format, iOutputStream* os);

    unsigned long
    totalMemoryAllocated();

    unsigned long
    maximumMemoryAllocated();

    void
    resetMaximumMemoryAllocated();

    void
    setRandomSeed(unsigned long value);

    bool
    collisionPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize);

    bool
    pathfindPreprocessVersionIsCompatible(const char* dataBuffer, unsigned long bufferSize);

    void
    deleteAllObjects();

    iContentChunk*
    newContentChunk(const iFaceVertexMesh* ground, const iAnchorsAndPinnedShapes* anchorsAndShapes, const char *const* attributes);

    iContentChunk*
    loadContentChunk(const char* dataBuffer, unsigned long bufferSize);
};

#endif
