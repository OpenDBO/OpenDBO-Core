//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_FEDERATION_INCLUDED
#define MESH_FEDERATION_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/Containers/ReplacementList.h"
#include "i_pathengine.h"

class iAPIObjectWrapper;
class iOutputStream;
class iXMLOutputStream;
class cSimpleDOM;
class cMesh;
template <class T> class cCollapsedVectorVector;

class cMeshFederation : public iMeshFederation
{
  // immutable, except on construction and deletion
    cList_NoSize<cMeshFederation*>::iterator _heldNode;
    long _startX, _startY;
    long _width, _height;
    long _tileSize, _overlap;
    long _tilesInX, _tilesInY, _tiles;

    void pushBackSubsets(const tPoint& bottomLeft, const tPoint& topRight, cCollapsedVectorVector<long>& result);
    void buildSubsetsLookup(const cMesh& worldMesh, cCollapsedVectorVector<long>& result);

    bool isInHandledRegion_Local(long tileIndex, long x, long y) const;

    void doSave(iXMLOutputStream& xos) const;

    void constructorCommon();

public:

    iAPIObjectWrapper* _wrapper;

    cMeshFederation(
            const cMesh& worldMesh,
            long tileSize, long overlap,
            iMeshFederationTileGeneratedCallBack& tileGeneratedCallBack,
            cList_NoSize<cMeshFederation*>::iterator heldNode
            );
    cMeshFederation(
            const char* format, const char* dataBuffer, unsigned long bufferSize,
            cList_NoSize<cMeshFederation*>::iterator heldNode
            );
    ~cMeshFederation();

// iMeshFederation

    void destroy();
    void save(const char* format, iOutputStream* outputStream) const;
    long size() const;
    bool coordinatesAreInsideRange(long x, long y) const;
    long tileForQuery(long queryStartX, long queryStartY) const;
    void getTileCentre(long tileIndex, long& centreX, long& centreY) const;
    void getHandledRegion_World(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const;
    void getRepresentedRegion_World(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const;
    void getHandledRegion_Local(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const;
    void getRepresentedRegion_Local(long tileIndex, long& minX, long& minY, long& maxX, long& maxY) const;
    cPosition translatePosition(long fromTileIndex, const iMesh* fromMesh, long toTileIndex, const iMesh* toMesh, const cPosition& fromPosition) const;
    void operator delete(void* p) {::operator delete(p);}

//==================
};

inline cMeshFederation* FromI(iMeshFederation* ptr)
{
    return static_cast<cMeshFederation*>(ptr);
}

#endif
