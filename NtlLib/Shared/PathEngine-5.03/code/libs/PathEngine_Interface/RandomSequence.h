//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef RANDOM_SEQUENCE_INCLUDED
#define RANDOM_SEQUENCE_INCLUDED

#include "common/interface/iRandomSequence.h"

class cRandomSequence : public iRandomSequence
{
    long _seed;

public:

    cRandomSequence();

// iRandomSequence

    long getSeed() const;
    void setSeed(long value);
    long random15();
    long random31();
};

#endif
