//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_SWAP_OUT_SUBSCRIBER
#define I_SWAP_OUT_SUBSCRIBER

class iSwapOutSubscriber
{
public:
    virtual ~iSwapOutSubscriber() {}
    virtual void swapOut(long index) = 0;
    virtual void pushBack() = 0;
};

#endif
