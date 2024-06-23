//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FLAG_VECTOR_INCLUDED
#define FLAG_VECTOR_INCLUDED

#include "common/interface/Assert.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

class cFlagVector
{
    cBitVector _flags;
    std::vector<long> _list;
    long _numberSet;

public:

    cFlagVector(long size) :
     _flags(size, false),
     _list(size),
     _numberSet(0)
    {
    }

    long size() const
    {
        return static_cast<long>(_flags.size());
    }
    void resize(long newSize)
    {
        assertD(newSize > size());
        _flags.resize(newSize, false);
        _list.resize(newSize);
    }

    void clear()
    {
        long i;
        for(i = 0; i < _numberSet; i++)
        {
            _flags.setBit(_list[i], false);
        }
        _numberSet = 0;
    }

    void set(long index)
    {
        assertD(index >= 0 && index < SizeL(_flags));
        assertD(!_flags[index]);
        _flags.setBit(index, true);
        _list[_numberSet++] = index;
    }

    void setIfNotAlreadySet(long index)
    {
        assertD(index >= 0 && index < SizeL(_flags));
        if(!_flags[index])
        {
            _flags.setBit(index, true);
            _list[_numberSet++] = index;
        }
    }

    bool operator[](long index) const
    {
        return _flags[index] != 0;
    }
    long numberSet() const
    {
        return _numberSet;
    }
    long indexSet(long i) const
    {
        return _list[i];
    }
};

#endif
