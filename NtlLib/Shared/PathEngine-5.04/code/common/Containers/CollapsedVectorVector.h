//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COLLAPSED_VECTOR_VECTOR_INCLUDED
#define COLLAPSED_VECTOR_VECTOR_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"

template <class T>
class cCollapsedVectorVector
{
    long _size;
    std::vector<T> _v;
    std::vector<long> _index;

    // prevent copying and assignment
    cCollapsedVectorVector(const cCollapsedVectorVector&);
    const cCollapsedVectorVector& operator=(const cCollapsedVectorVector&);

public:

    cCollapsedVectorVector() :
     _size(0),
     _index(_size + 1)
    {
        _index[0] = 0;
    }

    cCollapsedVectorVector(const std::vector<std::vector<T> >& buildFrom) :
     _size(SizeL(buildFrom)),
     _index(_size + 1)
    {
        _index.resize(0);
        _index.push_back(0);
        long i;
        for(i = 0; i != _size; ++i)
        {
            assertD(SizeL(_index) <= _size);
            _index.push_back(_index.back() + SizeL(buildFrom[i]));
        }
        _v.resize(_index.back());
        long vIndex = 0;
        for(i = 0; i != _size; ++i)
        {
            for(long j = 0; j != subVectorSize(i); ++j)
            {
                _v[vIndex++] = buildFrom[i][j];
            }
        }
        assertD(vIndex == SizeL(_v));
    }

    cCollapsedVectorVector(const std::vector<long> entriesPerSubVector) :
     _size(SizeL(entriesPerSubVector)),
     _index(_size + 1)
    {
        _index.resize(0);
        _index.push_back(0);
        for(long i = 0; i != SizeL(entriesPerSubVector); ++i)
        {
            assertD(SizeL(_index) <= _size);
            _index.push_back(_index.back() + entriesPerSubVector[i]);
        }
        _v.resize(_index.back());
    }

    cCollapsedVectorVector(long size) :
     _size(size),
     _index(_size + 1, 0)
    {
    }

    void
    reconstruct(const std::vector<long> entriesPerSubVector)
    {
        _size = SizeL(entriesPerSubVector);
        _index.resize(_size + 1);
        _index.resize(0);
        _index.push_back(0);
        for(long i = 0; i != SizeL(entriesPerSubVector); ++i)
        {
            assertD(SizeL(_index) <= _size);
            _index.push_back(_index.back() + entriesPerSubVector[i]);
        }
        _v.resize(_index.back());
    }

    void
    swap(cCollapsedVectorVector& rhs)
    {
        std::swap(_size, rhs._size);
        _v.swap(rhs._v);
        _index.swap(rhs._index);
    }

    void clear()
    {
        _index.resize(1);
    }

    void pushBackSubVector()
    {
        _size++;
        _index.push_back(_index.back());
    }
    void pushBackInLastSubVector(const T& value)
    {
        assertD(_size > 0);
        _index.back()++;
        _v.push_back(value);
    }

// note that the following is not needed for directly constructed instances because both vectors are reserved to exactly the right size
    void shrinkToFit()
    {
        if(_v.capacity() != _v.size())
        {
            std::vector<T>(_v).swap(_v);
        }
        if(_index.capacity() != _index.size())
        {
            std::vector<long>(_index).swap(_index);
        }
    }

    long size() const
    {
        return _size;
    }
    const T* operator[](long i) const
    {
        return &_v[_index[i]];
    }
    T* operator[](long i)
    {
        return &_v[_index[i]];
    }

    long subVectorSize(long i) const
    {
        return _index[i + 1] - _index[i];
    }
};

#endif
