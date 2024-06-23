//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef I_RANDOM_SEQUENCE_HAS_BEEN_INCLUDED
#define I_RANDOM_SEQUENCE_HAS_BEEN_INCLUDED
class iRandomSequence
{
public:

    virtual ~iRandomSequence()
    {
    }
    
    virtual long getSeed() const = 0;
    virtual void setSeed(long value) = 0;
    virtual long random15() = 0;
    virtual long random31() = 0;
};
#endif
