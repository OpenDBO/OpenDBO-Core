//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IQUERYCALLBACK_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IQUERYCALLBACK_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iQueryCallBack_Wrapper : public iQueryCallBack, public iAPIObjectWrapper
{
public:

    typedef iQueryCallBack tWrapped;

	iQueryCallBack* _target;

    static long interfaceIndex()
    {
        return 9;
    }

    long
    desiredCallBackFrequency();

    bool
    abort();

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
