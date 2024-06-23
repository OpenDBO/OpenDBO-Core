//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_CHUNK_INSTANCE_INCLUDED
#define CONTENT_CHUNK_INSTANCE_INCLUDED

#include "i_pathengine.h"
#include "common/STL/vector.h"

class cContentChunk;
class iAPIObjectWrapper;
class cBridgingModifier;

class cContentChunkInstance : public iContentChunkInstance
{
    cContentChunk* _base;
    long _indexInBase;
    const iFaceVertexMesh* _faceVertex;
    cBridgingModifier* _bridgingModifier;
    const iFaceVertexMesh* _beforeBridgingModifier;
    const iAnchorsAndPinnedShapes* _anchorsAndShapes;
    cHorizontalRange _horizontalRange;
    bool _hasGroundFootprint;
    cHorizontalRange _footprintRange;
    long _sectionID;

public:

    iAPIObjectWrapper* _wrapper;

    cContentChunkInstance(
            cContentChunk* base, long indexInBase,
            const iFaceVertexMesh* faceVertex,
            const std::vector<long>& connectingEdgeInfo,
            const iAnchorsAndPinnedShapes* anchorsAndShapes,
            long sectionID
            );
    ~cContentChunkInstance();

    long getSectionID() const
    {
        return _sectionID;
    }

    bool hasGroundFootprint() const
    {
        return _hasGroundFootprint;
    }
    void getGroundFootprintRange(cHorizontalRange& result) const;

// iContentChunkInstance

    void destroy();
    iContentChunk* getContentChunk() const;
    const iFaceVertexMesh* getGroundPointer() const;
    const iAnchorsAndPinnedShapes* getAnchorsAndShapesPointer() const;
    void addAnchorsAndShapes(iMesh* mesh, const char* idPrefix, long numberOfTerrainLayers) const;
    void getHorizontalRange(cHorizontalRange& result) const;
    long numberOfConnectingEdges() const;
    void getConnectingEdgeGeometry(long connectingEdgeIndex, long& startX, long& startY, float& startZ, long& endX, long& endY, float& endZ) const;
    bool edgesCanConnect(long connectingEdgeOnThis, iContentChunkInstance* targetInstance, long connectingEdgeOnTarget) const;
    void makeEdgeConnection(long connectingEdgeOnThis, iContentChunkInstance* targetInstance, long connectingEdgeOnTarget) const;
    void operator delete(void* p) {::operator delete(p);}

//==================
};

inline cContentChunkInstance* FromI(iContentChunkInstance* ptr)
{
    return static_cast<cContentChunkInstance*>(ptr);
}
inline const cContentChunkInstance* FromI(const iContentChunkInstance* ptr)
{
    return static_cast<const cContentChunkInstance*>(ptr);
}

#endif
