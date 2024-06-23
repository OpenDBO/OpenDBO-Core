//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ICONTENTCHUNKINSTANCE_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ICONTENTCHUNKINSTANCE_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iContentChunkInstance_Wrapper : public iContentChunkInstance, public iAPIObjectWrapper
{
public:

    typedef iContentChunkInstance tWrapped;

	iContentChunkInstance* _target;

    static long interfaceIndex()
    {
        return 11;
    }

    void
    destroy();

    iContentChunk*
    getContentChunk() const;

    const iFaceVertexMesh*
    getGroundPointer() const;

    const iAnchorsAndPinnedShapes*
    getAnchorsAndShapesPointer() const;

    void
    addAnchorsAndShapes(iMesh* mesh, const char* idPrefix, long numberOfTerrainLayers) const;

    void
    getHorizontalRange(cHorizontalRange& result) const;

    long
    numberOfConnectingEdges() const;

    void
    getConnectingEdgeGeometry(long connectingEdgeIndex, long& startX, long& startY, float& startZ, long& endX, long& endY, float& endZ) const;

    bool
    edgesCanConnect(long connectingEdgeOnThis, iContentChunkInstance* targetInstance, long connectingEdgeOnTarget) const;

    void
    makeEdgeConnection(long connectingEdgeOnThis, iContentChunkInstance* targetInstance, long connectingEdgeOnTarget) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
