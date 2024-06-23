//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ICONTENTCHUNK_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ICONTENTCHUNK_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iContentChunk_Wrapper : public iContentChunk, public iAPIObjectWrapper
{
public:

    typedef iContentChunk tWrapped;

	iContentChunk* _target;

    static long interfaceIndex()
    {
        return 10;
    }

    void
    destroy();

    bool
    preValidate(long maximumScale, long translationRange);

    void
    save(iOutputStream* outputStream) const;

    iContentChunkInstance*
    instantiate(long rotation, long scale, long translationX, long translationY, float translationZ, long sectionID);

    iContentChunkInstance*
    instantiate_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ, long sectionID);

    void
    deleteAllInstances();

    bool
    hasGroundComponent() const;

    bool
    hasAnchorsAndShapesComponent() const;

    long
    numberOfConnectingEdges() const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
