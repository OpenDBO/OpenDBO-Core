//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VECTOR_WITH_FREE_LIST_INCLUDED
#define VECTOR_WITH_FREE_LIST_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include "common/STL_Helpers.h"

template <class T>
class cVectorWithFreeList
{
    std::vector<T> _v;
    long _firstFree;
    std::vector<long> _nextFree;
    long _numberInUse;

public:

    cVectorWithFreeList()
    {
        _firstFree = -1;
        _numberInUse = 0;
    }

    void
    completelyClear()
    {
        CompletelyClear(_v);
        CompletelyClear(_nextFree);
        _firstFree = -1;
        _numberInUse = 0;
    }

    long size() const
    {
        return SizeL(_v);
    }
    const T& operator[](long index) const
    {
        return _v[index];
    }
    T& operator[](long index)
    {
        return _v[index];
    }

    bool indexIsInUse(long index) const
    {
        assertD(index >= 0);
        assertD(index < static_cast<long>(_v.size()));
        return _nextFree[index] == -2;
    }
    
    long numberInUse() const
    {
        return _numberInUse;
    }

    long reuseOrPushBack(const T& value)
    {
        long result;
        if(_firstFree == -1)
        {
            result = SizeL(_v);
            _v.push_back(value);
            _nextFree.push_back(-2);
        }
        else
        {
            result = _firstFree;
            assertD(_nextFree[result] >= -1);
            _firstFree = _nextFree[result];
            _nextFree[result] = -2;
            _v[result] = value;
        }
        _numberInUse++;
        return result;
    }
    void release(long index)
    {
        assertD(_nextFree[index] == -2);
        _nextFree[index] = _firstFree;
        _firstFree = index;
        _numberInUse--;
    }
};

#endif
