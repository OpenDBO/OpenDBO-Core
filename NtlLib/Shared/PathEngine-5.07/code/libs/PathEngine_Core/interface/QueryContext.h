//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_CONTEXT_INCLUDED
#define QUERY_CONTEXT_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
class cFlagVector;
class cAStarHeapPool;
class cBitVector;

class cQueryContext
{
    long _currentQuery, _nextQuery;

    const tMesh& _mesh;
    bool _meshHasInternalVertices;
    long _numberOfInternalFaces;
    long _maxElementCountRequirement;

    cFlagVector* _flagVector;
    bool _flagVectorLocked;

    char* _generalPurposeBuffer;
    long _generalPurposeBufferSize;
    bool _generalPurposeBufferLocked;

    cBitVector* _elementBitBuffer;
    bool _elementBitBufferLocked;

public:
    
    cAStarHeapPool& _heapPool;

    cQueryContext(tMesh& mesh, long numberOfInternalFaces, bool meshHasInternalVertices, cAStarHeapPool& heapPool);
    ~cQueryContext();

    void enter();
    void exit();

    bool insideQuery() const
    {
        return _currentQuery >= 0;
    }

//.. add when required
//    void addFlagsSizeRequirement(long maxSize);
//    void addGeneralPurposeSizeRequirement(long maxSize);

    void addElementCountRequirement(long elementCount);

// interface provided for use by code inside a query

    //... support const meshes
    tMesh& refMesh() const
    {
        assertD(insideQuery());
        return const_cast<tMesh&>(_mesh);
    }
    long getNumberOfInternalFaces() const
    {
        return _numberOfInternalFaces;
    }
    bool meshHasInternalVertices() const
    {
        return _meshHasInternalVertices;
    }

    long getMaxElementCount() const
    {
        return _maxElementCountRequirement;
    }

    cFlagVector& lockFlagVector(long sizeRequired);
    cFlagVector& lockFaceFlagVector();
    cFlagVector& lockElementFlagVector();
    void releaseFlagVector();

    char* lockGeneralPurposeBuffer(long sizeRequired);
    tFace* lockFaceBuffer();
    void releaseGeneralPurposeBuffer();

    cBitVector& lockElementBitBuffer();
    void releaseElementBitBuffer();
};

#endif
