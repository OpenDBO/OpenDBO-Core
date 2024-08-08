//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_ISHAPE_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_ISHAPE_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iShape_Wrapper : public iShape, public iAPIObjectWrapper
{
public:

    typedef iShape tWrapped;

	iShape* _target;

    static long interfaceIndex()
    {
        return 1;
    }

    void
    destroy();

    long
    size() const;

    void
    vertex(long index, long& x, long& y) const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
