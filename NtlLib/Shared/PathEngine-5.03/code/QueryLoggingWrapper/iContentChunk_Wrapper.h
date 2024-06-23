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

    const iFaceVertexMesh*
    instantiate(long rotation, long scale, long translationX, long translationY, float translationZ);

    const iFaceVertexMesh*
    instantiate_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ);

    void
    deleteAllInstances();

    void
    addAnchorsAndShapes(iMesh* mesh, long rotation, long scale, long translationX, long translationY, float translationZ, const char* idPrefix);

    void
    addAnchorsAndShapes_FloatRotateAndScale(iMesh* mesh, float rotation, float scale, long translationX, long translationY, float translationZ, const char* idPrefix);

    bool
    hasGroundComponent() const;

    bool
    hasAnchorsAndShapesComponent() const;

    const iAnchorsAndPinnedShapes*
    instantiateAnchorsAndShapes(long rotation, long scale, long translationX, long translationY, float translationZ);

    const iAnchorsAndPinnedShapes*
    instantiateAnchorsAndShapes_FloatRotateAndScale(float rotation, float scale, long translationX, long translationY, float translationZ);

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
