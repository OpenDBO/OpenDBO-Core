//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IPATH_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IPATH_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iPath_Wrapper : public iPath, public iAPIObjectWrapper
{
public:

    typedef iPath tWrapped;

	iPath* _target;

    static long interfaceIndex()
    {
        return 6;
    }

    void
    destroy();

    long
    size() const;

    cPosition
    position(long pointIndex) const;

    long
    connectionIndex(long segmentIndex) const;

    const cPosition*
    getPositionArray() const;

    const long*
    getConnectionIndexArray() const;

    unsigned long
    getLength() const;

    iCollisionInfo*
    advanceAlong(iShape* shape, float distance, iCollisionContext* context, float& precisionX, float& precisionY);

    void
    renderOnGround(long originX, long originY, iRender3DLinesCallBack& callBack) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
