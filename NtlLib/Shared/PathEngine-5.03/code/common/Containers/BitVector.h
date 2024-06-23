//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BIT_VECTOR_INCLUDED
#define BIT_VECTOR_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include "common/STL_Helpers.h"

class cBitVector
{
    long _size;
    std::vector<unsigned char> _v;

    static long byteSize(long size)
    {
        return (size + 7) / 8;
    }
    static long byteIndex(long index)
    {
        return index / 8;
    }
    static unsigned char maskFor(long index)
    {
        return 1 << (index & 7);
    }

public:

    cBitVector() :
      _size(0)
    {
    }

    cBitVector(long size) :
     _size(size),
     _v(byteSize(size))
    {
    }
    cBitVector(long size, bool initialiser) :
     _size(size),
     _v(byteSize(size), initialiser ? 0xff : 0)
    {
    }

    void
    clear()
    {
        _v.resize(0);
        _size = 0;
    }
    void
    completelyClear()
    {
        CompletelyClear(_v);
    }

    void swap(cBitVector& rhs)
    {
        std::swap(_size, rhs._size);
        _v.swap(rhs._v);
    }

    bool empty() const {return _size == 0;}
    long size() const {return _size;}

    bool operator[](long i) const
    {
        return (_v[byteIndex(i)] & maskFor(i)) != 0;
    }

    void push_back(bool value)
    {
        _v.resize(byteSize(_size + 1));
        _size++;
        setBit(_size - 1, value);
    }
    void pop_back()
    {
        _size--;
        _v.resize(byteSize(_size));
    }
    void resize(long newSize)
    {
        _v.resize(byteSize(newSize));
        _size = newSize;
    }
    void resize(long newSize, bool initialiser)
    {
        long sizeBefore = _size;
        _v.resize(byteSize(newSize), initialiser ? 0xff : 0);
        _size = newSize;
        while((sizeBefore & 7) && sizeBefore < newSize)
        {
            setBit(sizeBefore, initialiser);
            sizeBefore++;
        }
    }

    void setBit(long i, bool value)
    {
        if(value)
        {
            _v[byteIndex(i)] |= maskFor(i);
        }
        else
        {
            unsigned char inverseMask = ~maskFor(i);
            _v[byteIndex(i)] &= inverseMask;
        }
    }
    void setLastBit(bool value)
    {
        setBit(_size - 1, value);
    }
    void fill(bool value)
    {
        unsigned long i;
        unsigned char fillWith = value ? 0xff : 0;
        for(i = 0; i != _v.size(); ++i)
        {
            _v[i] = fillWith;
        }
    }
};

#endif
