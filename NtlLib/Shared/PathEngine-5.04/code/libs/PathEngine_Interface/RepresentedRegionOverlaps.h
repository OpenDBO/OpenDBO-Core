//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef REPRESENTED_REGION_OVERLAPS_INCLUDED
#define REPRESENTED_REGION_OVERLAPS_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/Containers/CollapsedVectorVector.h"

class iXMLOutputStream;

class cRepresentedRegionOverlaps
{
    friend class cRepresentedRegionOverlapsLoader;
    cCollapsedVectorVector<long> _overlaps;

    cRepresentedRegionOverlaps()
    {
    }

public:

    cRepresentedRegionOverlaps(
            const cCollapsedVectorVector<long>& subsetsLookup,
            long subsetIndex
            );

    void save(iXMLOutputStream& os) const;

    long lookupOverlappedFace(long sourceFaceIndex, long targetRegion) const
    {
        assertD(sourceFaceIndex >= 0 && sourceFaceIndex < _overlaps.size());
        const long *const subVector = _overlaps[sourceFaceIndex];
        for(long i = 0; i != _overlaps.subVectorSize(sourceFaceIndex); i += 2)
        {
            if(subVector[i] == targetRegion)
            {
                return subVector[i + 1];
            }
        }
        invalid();
        return -1;
    }
};

class cRepresentedRegionOverlapsLoader : public iXMLHandler
{
    cRepresentedRegionOverlaps* _result;
    long _loadedSize;
    long _currentFace;

public:

    cRepresentedRegionOverlapsLoader();
    ~cRepresentedRegionOverlapsLoader();

    const cRepresentedRegionOverlaps* takeResult();

// interface for iXMLHandler

    void init(const char *const* attr);
    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
