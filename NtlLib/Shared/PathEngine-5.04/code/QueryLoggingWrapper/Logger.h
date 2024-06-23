//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER__LOGGER_INCLUDED
#define QUERY_LOGGING_WRAPPER__LOGGER_INCLUDED

#include "platform_common/Mutex.h"
#include "common/STL/vector.h"
#include <stdio.h>

class iFaceVertexMesh;
class iAgent;
class cPosition;

class cLogger
{
    cMutex _mutex;
    FILE* _file;
    std::vector<char> _buffer;
    unsigned long _queryCount;
    unsigned long _fileIndex_DataBuffer;
    unsigned long _fileIndex_SourceContent;

    void writeBuffer();
    void signedToBuffer(long value, long tokenSize);
    void unsignedToBuffer(unsigned long value, long tokenSize);
    void boolToBuffer(bool value);
    void pointerToBuffer(void* voidPointer);

public:

    cLogger() :
      _buffer(500)
    {
        _buffer.resize(0);
    }

    void initialise();
    void shutdown();

    unsigned long methodEntry(long interfaceIndex, long methodIndex, void* interfacePointer);
    void methodEntry_Completed();
    void methodExit(long interfaceIndex, long methodIndex, unsigned long queryCount, void* interfacePointer);
    void methodExit_Completed();

    void log(bool value);
    void log(long value);
    void log(unsigned long value);
    void log(float value);
    void log(const char* string);
    void log(const char *const* attributes);
    void log(const long* point);
    void log(const float* point);
    void log(const cPosition& position);
    void log(void* voidPointer);
    void logCollidingLine(long* collidingLine);
    void logCollisionShape(long numberOfPoints, const long* coordinateData);
    void logDataBuffer(const char* dataBuffer, unsigned long bufferSize);
    void logPathBuffers(const cPosition* positionsBuffer, const long* connectionIndicesBuffer, long pathLength);
    void logSourceContent(const iFaceVertexMesh* const* geometryObjectPointers, long numberOfGeometryObjects);
    void logAgentVector(iAgent** resultsBuffer, long& numberOverlapped);
};

extern cLogger gLogger;

#endif
