//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IMESHFEDERATION_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IMESHFEDERATION_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iMeshFederation_Wrapper : public iMeshFederation, public iAPIObjectWrapper
{
public:

    typedef iMeshFederation tWrapped;

	iMeshFederation* _target;

    static long interfaceIndex()
    {
        return 12;
    }

    void
    destroy();

    void
    save(const char* format, iOutputStream* outputStream) const;

    long
    size() const;

    bool
    coordinatesAreInsideRange(long x, long y) const;

    long
    tileForQuery(long queryStartX, long queryStartY) const;

    void
    getTileCentre(long tileIndex, long& centreX, long& centreY) const;

    void
    getHandledRegion_World(long tileIndex, cHorizontalRange& result) const;

    void
    getRepresentedRegion_World(long tileIndex, cHorizontalRange& result) const;

    void
    getHandledRegion_Local(long tileIndex, cHorizontalRange& result) const;

    void
    getRepresentedRegion_Local(long tileIndex, cHorizontalRange& result) const;

    cPosition
    translatePosition(long fromTileIndex, const iMesh* fromMesh, long toTileIndex, const iMesh* toMesh, const cPosition& fromPosition) const;

    void
    getRepresentedRegionsOverlapped(const cHorizontalRange& queryRegion, long* resultsBuffer, long& numberOverlapped) const;

    iMesh*
    buildTileMeshFromContent(long tileIndex, const iContentChunkInstance* const* instancePointers, long numberOfInstances, const char *const* options, iOutputStream* os);

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
