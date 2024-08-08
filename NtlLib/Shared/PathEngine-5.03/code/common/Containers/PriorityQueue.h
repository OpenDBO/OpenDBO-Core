//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PRIORITY_QUEUE_INCLUDED
#define PRIORITY_QUEUE_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include "common/STL/algorithm.h"

template <class T, class tPredicate>
class cPriorityQueue
{
    std::vector<T> _heap;
    tPredicate* _predicate;

public:

    cPriorityQueue(long reservedSize) :
      _heap(reservedSize)
    {
        _heap.resize(0);
    }

    // note that it looks like this object will then be copied in as a parameter in the stl methods below
    void setPredicate(tPredicate* predicate)
    {
        _predicate = predicate;
    }

    void clear()
    {
        _heap.resize(0);
    }
    void swap(cPriorityQueue& rhs)
    {
        _heap.swap(rhs._heap);
    }

    bool empty() const {return _heap.empty();}
    long size() const {return static_cast<long>(_heap.size());}

    const T& front() const
    {
        return _heap.front();
    }

    const T& operator[](long index) const
    {
        return _heap[index];
    }

    void insert(T value)
    {
        _heap.push_back(value);
        std::push_heap(_heap.begin(), _heap.end(), *_predicate);
    }
    T popFront()
    {
        assertD(!empty());
        T result = _heap.front();
        std::pop_heap(_heap.begin(), _heap.end(), *_predicate);
        _heap.pop_back();
        return result;
    }
    void updateValueForwards(T value)
    {
        typename std::vector<T>::iterator it;
        for(it = _heap.begin(); it != _heap.end(); ++it)
        {
            if((*it) == value)
            {
                std::push_heap(_heap.begin(), it + 1, *_predicate);
                return;
            }
        }
        invalid();
    }
};

#endif
