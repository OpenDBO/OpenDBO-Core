/**********************************************************************

Filename    :   GTLTypes.h
Content     :   General template types and functions
Created     :   August 20, 2001
Authors     :   Michael Antonov
Copyright   :   (c) 2001-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GTLTYPES_H
#define INC_GTLTYPES_H

#include "GTypes.h"
#include "GDebug.h"
#include "GMemory.h"
#include "GMath.h"
#include <string.h>
#include "GRefCount.h" // for gpaged_heap

#define GFC_HASH_MIN_SIZE     8


// Pragma to prevent long name warnings in in VC++
#if defined(GFC_CC_MSVC)
#pragma warning(disable : 4786)
#endif



// ***** Add support for placement new

// Calls constructor on own memory created with "new(ptr) type"
#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE

#if defined(GFC_CC_MWERKS) || defined(GFC_CC_BORLAND)
#include <new>
#else

#ifndef GFC_CC_GNU
GINLINE void*   operator new        (size_t n, void *ptr)   { return ptr; }
GINLINE void    operator delete     (void *ptr, void *ptr2) { }
#else
#include <new>
#endif // GFC_CC_GNU

#endif // GFC_CC_MWERKS | GFC_CC_BORLAND

#endif // __PLACEMENT_NEW_INLINE




// ***** Operator extensions

namespace GTL {

    
// ***** Construct / Destruct

#if defined(GFC_BUILD_DEFINE_NEW) && defined(GFC_DEFINE_NEW)
#undef new
#endif

template <class T>
inline T*  gconstruct(void *p)
{
    return ::new(p) T;
}

template <class T>
inline T*  gconstruct(void *p, const T& source)
{
    return ::new(p) T(source);
}

// Same as above, but allows for a different type of constructor.
template <class T, class S>
inline T*  gconstruct_alt(void *p, const S& source)
{
    return ::new(p) T(source);
}

template <class T>
GINLINE void gconstruct_array(void *p, UPInt count)
{
    UByte *pdata = (UByte*)p;
    for (UPInt i=0; i< count; i++, pdata += sizeof(T))
    {
        gconstruct<T>(pdata);
    }
}

template <class T>
GINLINE void gconstruct_array(void *p, UPInt count, const T& source)
{
    UByte *pdata = (UByte*)p;
    for (UPInt i=0; i< count; i++, pdata += sizeof(T))
    {
        gconstruct<T>(pdata, source);
    }
}

template <class T>
GINLINE void gdestruct(T *pobj)
{
    pobj->~T();
}

template <class T>
GINLINE void gdestruct_array(T *pobj, UPInt count)
{   
    for (UPInt i=0; i<count; i++, pobj++)        
        pobj->~T();
}


// ***** Custom Containers

// in MSVC 7.1, warning about placement new POD default initializer
#ifdef GFC_CC_MSVC
#pragma warning(disable : 4345) 
#endif 


// Resizable garray.  Don't put anything in here that can't be moved
// around by bitwise copy.  Don't keep the address of an element; the
// garray contents will move around as it gets resized.
//
// Default constructor and destructor get called on the elements as
// they are added or removed from the active part of the garray.
template<class C, class Base>
class garray_base
    {
public:
    GFC_MEMORY_REDEFINE_NEW(garray)

    garray_base()
        : Buffer(0), Size(0), BufferSize(0)     {   }
    garray_base(int size_hint)
        : Buffer(0), Size(0), BufferSize(0)     { resize(size_hint);  }
    garray_base(const garray_base<C, Base>& a)
        : Buffer(0), Size(0), BufferSize(0)     { operator=(a); }
    ~garray_base()
    { 
         clear();
         if (Buffer)
             GFREE(Buffer);
    }

    // Buffer size policy bit assignments.
    enum BufferPolicy
    {
        // Use high bits of buffer size to store resize policy.
        Buffer_NoShrink     = 2,
        Buffer_NoClear      = Buffer_NoShrink >> 1,        
        Buffer_PolicyMask   = (Buffer_NoShrink|Buffer_NoClear),
        // Shift for mask
        Buffer_PolicyShift  = (sizeof(UPInt)*8 - 2)
        // Mask for buffer size. (not compatible w/x64)
        //Buffer_SizeMask     = ~(Buffer_PolicyMask << Buffer_PolicyShift)
    };
    
    BufferPolicy get_size_policy() const
    {
        return (BufferPolicy) ((BufferSize >> Buffer_PolicyShift) & Buffer_PolicyMask);
    }
    void    set_size_policy(BufferPolicy policy)
    {
        BufferSize = (BufferSize & (~(UPInt(Buffer_PolicyMask) << Buffer_PolicyShift))) |
                     (UPInt(policy & Buffer_PolicyMask) << Buffer_PolicyShift);
    }

    bool    is_noshrink() const { return (get_size_policy() & Buffer_NoShrink) != 0;  }
    bool    is_noclear() const  { return (get_size_policy() & Buffer_NoClear) != 0;  }


    // Basic garray access.
    C&  operator[](UPInt index)
    {
        GASSERT(index < Size);
        return Buffer[index]; 
    }
    const C&    operator[](UPInt index) const
    {
        GASSERT(index < Size);
        return Buffer[index]; 
    }
    UPInt    size() const
        { return Size;  }

    UPInt    capacity() const
        { return BufferSize & (~(UPInt(Buffer_PolicyMask) << Buffer_PolicyShift));  }

    // Insert the given element at the end of the garray.
    void    push_back(const C& val)
    {
        // DO NOT pass elements of your own vector into
        // push_back()!  Since we're using references,
        // resize() may munge the element storage!
        // GASSERT(&val < &Buffer[0] || &val > &Buffer[BufferSize]);

        UPInt new_size = Size + 1;
        resize(new_size);
        (*this)[new_size-1] = val;
    }

    // Remove the last element.
    void    pop_back()
    {
        GASSERT(Size > 0);
        resize(Size - 1);
    }

    // Access the first element.
    C&  front()                 { return (*this)[0];    }
    const C&    front() const   { return (*this)[0];    }

    // Access the last element.
    C&  back()                  { return (*this)[Size-1];   }
    const C&    back() const    { return (*this)[Size-1];   }

    
    // Empty and destruct all elements.
    void    clear()
    {
        resize(0);
    }

    void    release()
        { clear();  }

    // Array copy.  Copies the contents of a into this garray.
    void    operator=(const garray_base<C, Base>& a)   
    {
        resize(a.size());
        for (UPInt i = 0; i < Size; i++) {
            *(Buffer + i) = a[i];
        }
    }


    // Removing multiple elements from the garray.
    void    removeMultiple(UPInt index, UPInt num)
    {
        GASSERT(index + num <= Size);

        if (Size == num)
        {
            clear();
        }
        else
        {
            for (UPInt i = 0; i < num; ++i)
               Buffer[index + i].~C(); // destructor

            memmove(Buffer+index, Buffer+index+num, sizeof(C) * (Size - num - index));
            Size -= num;
        }
    }

    // Removing an element from the garray is an expensive operation!
    // It compacts only after removing the last element.
    void    remove(UPInt index)
    {
        GASSERT(index < Size);

        if (Size == 1)
        {
            clear();
        }
        else
        {
            Buffer[index].~C(); // destructor

            memmove(Buffer+index, Buffer+index+1, sizeof(C) * (Size - 1 - index));
            Size--;
        }
    }


    // Insert the given object at the given index shifting all the elements up.
    void    insert(UPInt index, const C& val = C())
    {
        GASSERT(index <= Size);

        resize(Size + 1);

        if (index < Size - 1)
        {
            // is it safe to use memmove?
            memmove(Buffer+index+1, Buffer+index, sizeof(C) * (Size - 1 - index));
        }

        // Copy-construct into the newly opened slot.
        new (Buffer + index) C(val);
    }

    // Insert the given object at the given index shifting all the elements up.
    void    insertMultiple(UPInt index, UPInt num, const C& val = C())
    {
        GASSERT(index <= Size);

        resize(Size + num);

        if (index < Size - num)
        {
            // is it safe to use memmove?
            memmove(Buffer+index+num, Buffer+index, sizeof(C) * (Size - num - index));
        }

        // Copy-construct into the newly opened slots.
        for (UPInt i = 0; i < num; ++i)
            new (Buffer + index + i) C(val);
    }

    // Append the given data to our garray.
    void    append(const garray_base<C, Base>& other)
    {
        append(other.Buffer, other.size());
    }

    void    append(const C& value)
    {
        push_back(value);
    }


    // Append the given data to our garray.
    void    append(const C other[], UPInt count)
    {
        if (count > 0)
        {
            UPInt size0 = Size;
            resize(Size + count);
            // Must use operator=() to copy elements, in case of side effects (e.g. ref-counting).
            for (UPInt i = 0; i < count; i++)
            {
                Buffer[i + size0] = other[i];
            }
        }
    }


    // Preserve existing elements via realloc.
    // 
    // Newly created elements are initialized with default element
    // of C.  Removed elements are destructed.
    void    resize(UPInt new_size)
    {
       ((Base*)this)->resize_impl(new_size);
    }


    void    reserve(UPInt rsize)
    {
        //UPInt old_size = BufferSize;
        //old_size = old_size;    // don't warn that this is unused.

        if (get_size_policy() != 0)
        {
            if (is_noshrink() && rsize < capacity())
                return;
            if (is_noclear() && (rsize == 0))
                return;
        }

        // Set buffer size
        set_buffer_size(rsize);

        // Resize the buffer.
        if (rsize == 0)
        {
            if (Buffer)
                GFREE(Buffer);
            Buffer = 0;
        }
        else
        {
            if (Buffer)
            {
                Buffer = (C*) GREALLOC(Buffer, sizeof(C) * rsize);
            }
            else
            {
                Buffer = (C*) GALLOC(sizeof(C) * rsize);
                memset(Buffer, 0, (sizeof(C) * rsize));
            }
            // GASSERT(Buffer); // need to throw (or something) on alloc failure!
        }
    }

    class iterator
    {
        garray_base<C, Base>*   pArray;
        int                     CurIndex;
    public:
        iterator() : pArray(NULL), CurIndex(-1) {}
        iterator(garray_base<C, Base>* parr) : pArray(parr), CurIndex(0) {}
        iterator(garray_base<C, Base>* parr, int idx) : pArray(parr), CurIndex(idx) {}
        iterator(const iterator& it) : pArray(it.pArray), CurIndex(it.CurIndex) {}

        iterator& operator=(const iterator& it)
        {
            pArray = it.pArray;
            CurIndex = it.CurIndex;
            return *this;
        }

        bool operator==(const iterator& it) const { return pArray == it.pArray && CurIndex == it.CurIndex; }
        bool operator!=(const iterator& it) const { return pArray != it.pArray || CurIndex != it.CurIndex; }

        iterator& operator++()
        {
            if (pArray)
            {
                if (CurIndex < (int)pArray->size())
                    ++CurIndex;
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator it(*this);
            operator++();
            return it;
        }
        iterator& operator--()
        {
            if (pArray)
            {
                if (CurIndex >= 0)
                    --CurIndex;
            }
            return *this;
        }
        iterator operator--(int)
        {
            iterator it(*this);
            operator--();
            return it;
        }
        iterator operator+(int delta) const
        {
            return iterator(pArray, CurIndex + delta);
        }
        iterator operator-(int delta) const
        {
            return iterator(pArray, CurIndex - delta);
        }
        SPInt operator-(const iterator& right) const
        {
            GASSERT(pArray == right.pArray);
            return CurIndex - right.CurIndex;
        }
        C& operator*() const    { GASSERT(pArray); return (*pArray)[CurIndex]; }
        C* operator->() const   { GASSERT(pArray); return &(*pArray)[CurIndex]; }
        C* GetPtr() const       { GASSERT(pArray); return &(*pArray)[CurIndex]; }

        bool IsFinished() const { return !pArray || CurIndex < 0 || CurIndex >= (int)pArray->size(); }

        void remove()
        {
            if (!IsFinished())
                pArray->remove(CurIndex);
        }

        int get_index() const { return CurIndex; }
    };
    typedef iterator Iterator;
    iterator begin() { return iterator(this); }
    iterator end()   { return iterator(this, (int)size()); }
    iterator Begin() { return begin(); }
    iterator End()   { return end(); }
    iterator Last()  { return iterator(this, (int)size() - 1); }

    class const_iterator
    {
        const garray_base<C, Base>* pArray;
        int                         CurIndex;
    public:
        const_iterator() : pArray(NULL), CurIndex(-1) {}
        const_iterator(const garray_base<C, Base>* parr) : pArray(parr), CurIndex(0) {}
        const_iterator(const garray_base<C, Base>* parr, int idx) : pArray(parr), CurIndex(idx) {}
        const_iterator(const iterator& it) : pArray(it.pArray), CurIndex(it.CurIndex) {}

        const_iterator& operator=(const const_iterator& it)
        {
            pArray = it.pArray;
            CurIndex = it.CurIndex;
            return *this;
        }

        bool operator==(const const_iterator& it) const { return pArray == it.pArray && CurIndex == it.CurIndex; }
        bool operator!=(const const_iterator& it) const { return pArray != it.pArray || CurIndex != it.CurIndex; }

        const_iterator& operator++()
        {
            if (pArray)
            {
                if (CurIndex < (int)pArray->size())
                    ++CurIndex;
            }
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator it(*this);
            operator++();
            return it;
        }
        const_iterator& operator--()
        {
            if (pArray)
            {
                if (CurIndex >= 0)
                    --CurIndex;
            }
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator it(*this);
            operator--();
            return it;
        }
        const_iterator operator+(int delta) const
        {
            return const_iterator(pArray, CurIndex + delta);
        }
        const_iterator operator-(int delta) const
        {
            return const_iterator(pArray, CurIndex - delta);
        }
        SPInt operator-(const const_iterator& right) const
        {
            GASSERT(pArray == right.pArray);
            return CurIndex - right.CurIndex;
        }
        const C& operator*() const  { GASSERT(pArray); return (*pArray)[CurIndex]; }
        const C* operator->() const { GASSERT(pArray); return &(*pArray)[CurIndex]; }
        const C* GetPtr() const     { GASSERT(pArray); return &(*pArray)[CurIndex]; }

        bool IsFinished() const { return !pArray || CurIndex < 0 || CurIndex >= (int)pArray->size(); }

        int get_index() const { return CurIndex; }
    };
    typedef const_iterator ConstIterator;
    const_iterator begin() const { return const_iterator(this); }
    const_iterator end() const   { return const_iterator(this, (int)size()); }
    const_iterator Begin() const { return begin(); }
    const_iterator End() const   { return end(); }
    const_iterator Last() const  { return const_iterator(this, (int)size() - 1); }

protected:
    C*      Buffer;
    UPInt   Size;
    UPInt   BufferSize;

    // Assigns buffer size only.
    void    set_buffer_size(UPInt capacity)
    {
        GASSERT(capacity < (UPInt(Buffer_PolicyMask) << Buffer_PolicyShift));
        BufferSize = (BufferSize & (UPInt(Buffer_PolicyMask) << Buffer_PolicyShift)) | capacity;
    }
};



// Basic array implementation. Uses default constructor for elements
// created by resize.
template<class C>
class garray : public garray_base<C, garray<C> >
{
public:
  
    garray()
        : garray_base<C, garray<C> >() { }
    garray(int size_hint)
        : garray_base<C, garray<C> >(size_hint) { }
    garray(const garray<C>& a)
        : garray_base<C, garray<C> >(a) { }
            
    ~garray()
    {  }  
     
    // Array copy.  Copies the contents of a into this garray.
    void    operator = (const garray<C>& a)   
    {
        garray_base<C, garray<C> >::operator = (a);      
    }
    
    // Preserve existing elements via realloc.
    // 
    // Newly created elements are initialized with default element
    // of C.  Removed elements are destructed.
    // Use 'this->' to avoid compiler warnings about unavailable functions.
    void    resize_impl(UPInt new_size)
    {
        UPInt    old_size = this->Size;

        this->Size = new_size;

        // Destruct old elements (if we're shrinking).
        UPInt i;     
        for (i = new_size; i < old_size; i++) 
            (this->Buffer + i)->~C();
        
        UPInt bufferSize = this->capacity();

        if (new_size == 0)
        {
            this->reserve(0);
        }
        else if (this->Size <= bufferSize && this->Size > bufferSize >> 1)
        {
            // don't compact yet.
            // GASSERT(Buffer != 0);
        }
        else
        {
            UPInt new_buffer_size = this->Size + (this->Size >> 2);
            this->reserve(new_buffer_size);
        }

        // Copy default C into new elements.
        for (i = old_size; i < new_size; i++)
            gconstruct<C>(this->Buffer + i);  // placement new            
      }
          
};


// An array that always uses copy constructor for elements created by resize.
template<class C>
class garray_cc : public garray_base<C, garray_cc<C> >
{
protected:
    C   default_value;
public:
  
    garray_cc(const C& defval)
        : garray_base<C, garray_cc<C> >(), default_value(defval) { }
    garray_cc(const C& defval, int size_hint)
        : garray_base<C, garray_cc<C> >(size_hint), default_value(defval) { }
    garray_cc(const garray_cc<C>& a)
        : garray_base<C, garray_cc<C> >(a), default_value(a.default_value) { }
    ~garray_cc()
     { }
     
    // Array copy.  Copies the contents of a into this garray.
    void    operator = (const garray_cc<C>& a)   
    {
        default_value = a.default_value;
        garray_base<C, garray_cc<C> >::operator = (a);       
    }
    
    // Preserve existing elements via realloc.
    // 
    // Newly created elements are initialized with default element
    // of C.  Removed elements are destructed.
    void    resize_impl(UPInt new_size)
    {
        UPInt    old_size = this->Size;

        this->Size = new_size;

        // Destruct old elements (if we're shrinking).
        UPInt i;     
        for (i = new_size; i < old_size; i++) 
            (this->Buffer + i)->~C();

        UPInt bufferSize = this->capacity();

        if (new_size == 0)
        {
            this->reserve(0);
        }
        else if (this->Size <= bufferSize && this->Size > bufferSize >> 1)
        {
            // don't compact yet.
            // GASSERT(Buffer != 0);
        }
        else
        {
            UPInt new_buffer_size = this->Size + (this->Size >> 2);
            this->reserve(new_buffer_size);
        }

        // Copy default C into new elements.
        for (i = old_size; i < new_size; i++)
            gconstruct<C>(this->Buffer + i, default_value);
      }
          
};




// ***** Hash-table implementation


// GTL::ghash_set and GTL::ghash.
//
// Hash table, linear probing, internal chaining.  One  interesting/nice thing
// about this implementation is that the table itself is a flat chunk of memory
// containing no pointers, only relative indices. If the key and value types
// of the ghash contain no pointers, then the ghash can be serialized using raw IO.
//
// Never shrinks, unless you explicitly clear() it.  Expands on
// demand, though.  For best results, if you know roughly how big your
// table will be, default it to that size when you create it.
//
// Key usability feature:
//
//   1. Allows node hash values to either be cached or not.
//
//   2. Allows for alternative keys with methods such as get_alt(). Handy
//      if you need to search nodes by their components; no need to create
//      temporary nodes.
//


// *** Hash functors:
//
//  gidentity_hash   - use when the key is already a good hash
//  gfixed_size_hash - general hash based on object's in-memory representation.


// Hash is just the input value; can use this for integer-indexed hash tables.
template<class C>
class gidentity_hash
{
public:
    size_t  operator()(const C& data) const
    { return (size_t) data; }
};

// Computes a hash of an object's representation.
template<class C>
class gfixed_size_hash
{
public:
    
    // Alternative: "sdbm" hash function, suggested at same web page
    // above, http::/www.cs.yorku.ca/~oz/hash.html
    // This is somewhat slower then Bernstein, but it works way better than the above
    // hash function for hashing large numbers of 32-bit ints.
    static GINLINE size_t   sdbm_hash(const void* data_in, size_t size, size_t seed = 5381)     
    {
        const UByte*    data = (const UByte*) data_in;
        size_t          h = seed;
        while (size > 0)
        {
            size--;
            h = (h << 16) + (h << 6) - h + (size_t)data[size];
        }   
        return h;
    }


    size_t  operator()(const C& data) const
    {
        unsigned char*  p = (unsigned char*) &data;
        int size = sizeof(C);

        return sdbm_hash(p, size);
    }
};



// *** GTL::ghashset_entry entry types. 

// Compact hash table entry type that re-computes hash keys during hash traversal.
// Good to use if the hash function is cheap or the hash value is already cached in C.
template<class C, class hashf>
class ghashset_entry
{
public:
    // Internal chaining for collisions.
    ptrdiff_t   Next_in_chain;
    C           value;

    ghashset_entry()
        : Next_in_chain(-2) { }
    ghashset_entry(const ghashset_entry& e)
        : Next_in_chain(e.Next_in_chain), value(e.value) { }
    ghashset_entry(const C& key, ptrdiff_t next_in_chain)
        : Next_in_chain(next_in_chain), value(key) { }

    bool    is_empty() const            { return Next_in_chain == -2;  }
    bool    is_end_of_chain() const     { return Next_in_chain == -1;  }

    // Cached hash value access - can be optimized bu storing hash locally.
    // Mask value only needs to be used if set_cached_hash is not implemented.
    size_t  get_cached_hash(size_t maskValue) const  { return hashf()(value) & maskValue; }
    void    set_cached_hash(size_t hashValue)        { GUNUSED(hashValue); }

    void    clear()
    {        
        value.~C(); // placement delete
        Next_in_chain = -2;
    }
};

// Hash table entry type that caches the entry hash value for nodes, so that it
// does not need to be re-computed during access.
template<class C, class hashf>
class ghashset_cached_entry
{
public:
    // Internal chaining for collisions.
    ptrdiff_t   Next_in_chain;
    size_t      HashValue;
    C           value;

    ghashset_cached_entry()
        : Next_in_chain(-2) { }
    ghashset_cached_entry(const ghashset_cached_entry& e)
        : Next_in_chain(e.Next_in_chain), HashValue(e.HashValue), value(e.value) { }
    ghashset_cached_entry(const C& key, ptrdiff_t next_in_chain)
        : Next_in_chain(next_in_chain), value(key) { }

    bool    is_empty() const            { return Next_in_chain == -2;  }
    bool    is_end_of_chain() const     { return Next_in_chain == -1;  }

    // Cached hash value access - can be optimized bu storing hash locally.
    // Mask value only needs to be used if set_cached_hash is not implemented.
    size_t  get_cached_hash(size_t maskValue) const  { GUNUSED(maskValue); return HashValue; }
    void    set_cached_hash(size_t hashValue)        { HashValue = hashValue; }

    void    clear()
    {
        value.~C();
        Next_in_chain = -2;
    }
};



// *** HTL::ghash_set implementation - relies on either cached or regular entries.
//
// Use: entry = ghashset_cached_entry<C, hashf> if hashes are expensive to
//              compute and thus need caching in entries.
//      entry = ghashset_entry<C, hashf> if hashes are already externally cached.
//
template<class C, class hashf = gfixed_size_hash<C>,
         class althashf = hashf,
         class entry = ghashset_cached_entry<C, hashf> >
class ghash_set
{
public:
    GFC_MEMORY_REDEFINE_NEW(ghash_set)

    typedef ghash_set<C, hashf, althashf, entry>    self;

    ghash_set() : pTable(NULL)                  {   }
    ghash_set(int size_hint) : pTable(NULL)     { set_capacity(size_hint);  }
    ghash_set(const self& src) : pTable(NULL)   { *this = src; }
    ~ghash_set()                                { clear();  }


    void    operator = (const self& src)
    {
        clear();
        if (src.is_empty() == false)
        {
            set_capacity(src.size());

            for (const_iterator it = src.begin(); it != src.end(); ++it)
            {
                add(*it);
            }
        }
    }


    // Remove all entries from the ghash_set table.
    void    clear() 
    {
        if (pTable)
        {
            // Delete the entries.
            for (size_t i = 0, n = pTable->SizeMask; i <= n; i++)
            {
                entry*  e = &E(i);
                if (!e->is_empty())             
                    e->clear();
            }
            GFREE(pTable);
            pTable = NULL;
        }
    }

    // Returns true if the ghash_set is empty.
    bool    is_empty() const
    {
        return pTable == NULL || pTable->EntryCount == 0;
    }


    // Set a new or existing value under the key, to the value.
    // Pass a different class of 'key' so that assignment reference object
    // can be passed instead of the actual object.
    template<class CRef>
    void    set(const CRef& key)
    {
        size_t  hash_value = hashf()(key);
        ptrdiff_t  index      = (ptrdiff_t)-1;

        if (pTable != NULL)
            index = find_index_core(key, hash_value & pTable->SizeMask);

        if (index >= 0)
        {            
            E(index).value = key;
        }
        else
        {
            // Entry under key doesn't exist.
            add(key, hash_value);
        }
    }

    template<class CRef>
    inline void    add(const CRef& key)
    {
        size_t  hash_value = hashf()(key);
        add(key, hash_value);
    }


    // Remove by alternative key.
    template<class K>
    void    remove_alt(const K& key)
    {   
        if (pTable == NULL)
            return;

        size_t       hash_value = althashf()(key);
        ptrdiff_t    index      = hash_value & pTable->SizeMask;

        entry*  e = &E(index);

        // If empty node or occupied by collider, we have nothing to remove.
        if (e->is_empty() || (e->get_cached_hash(pTable->SizeMask) != (size_t)index))
            return;        

        // Save index
        ptrdiff_t   naturalIndex = index;
        ptrdiff_t   prevIndex    = -1;

        while ((e->get_cached_hash(pTable->SizeMask) != (size_t)naturalIndex) || !(e->value == key))
        {
            // Keep looking through the chain.
            prevIndex   = index;
            index       = e->Next_in_chain;
            if (index == -1)
                return; // End of chain, item not found
            e = &E(index);
        }

        // Found it - our item is at index
        if (naturalIndex == index)
        {
            // If we have a follower, move it to us
            if (!e->is_end_of_chain())
            {               
                entry*  enext = &E(e->Next_in_chain);
                e->clear();
                new (e) entry(*enext);
                // Point us to the follower's cell that will be cleared
                e = enext;
            }
        }
        else
        {
            // We are not at natural index, so deal with the prev items next index
            E(prevIndex).Next_in_chain = e->Next_in_chain;
        }

        // Clear us, of the follower cell that was moved.
        e->clear();
        pTable->EntryCount --;
        // Should we check the size to condense hash? ...
    }

    // Remove by main key.
    template<class CRef>
    void    remove(const CRef& key)
    {
        remove_alt(key);
    }



    // Retrieve the pointer to a value under the given key.
    //  - If there's no value under the key, then return NULL.    
    //  - If there is a value, return the pointer.    
    template<class K>
    C*  get(const K& key)
    {
        ptrdiff_t   index = find_index(key);
        if (index >= 0)        
            return &E(index).value;        
        return 0;
    }   

    template<class K>
    const C* get(const K& key) const
    {
        ptrdiff_t   index = find_index(key);
        if (index >= 0)        
            return &E(index).value;        
        return 0;
    }

    // Alternative key versions of get. Used by Hash.
    template<class K>
    const C* get_alt(const K& key) const
    {
        ptrdiff_t   index = find_index_alt(key);
        if (index >= 0)        
            return &E(index).value;
        return 0;
    }

    template<class K>
    C*  get_alt(const K& key)
    {
        ptrdiff_t   index = find_index_alt(key);
        if (index >= 0)        
            return &E(index).value;
        return 0;
    }   

    template<class K>
    bool  get_alt(const K& key, C* pval) const
    {
        ptrdiff_t   index = find_index_alt(key);
        if (index >= 0)
        {
            if (pval)
                *pval = E(index).value;
            return 1;
        }
        return 0;
    }


    UPInt    size() const
    {
        return pTable == NULL ? 0 : (UPInt)pTable->EntryCount;
    }


    // Resize the ghash_set table to fit one more entry.  Often this
    // doesn't involve any action.
    void    check_expand()  
    {
        if (pTable == NULL)
        {
            // Initial creation of table.  Make a minimum-sized table.
            set_raw_capacity(GFC_HASH_MIN_SIZE);
        }
        else if (pTable->EntryCount * 5 > (pTable->SizeMask + 1) * 4)
        {
            // pTable is more than 5/4 ths full.  Expand.
            set_raw_capacity((pTable->SizeMask + 1) * 2);
        }
    }


    // Hint the bucket count to >= n.
    void    resize(size_t n)    
    {
        // Not really sure what this means in relation to
        // STLport's hash_map... they say they "increase the
        // bucket count to at least n" -- but does that mean
        // their real capacity after resize(n) is more like
        // n*2 (since they do linked-list chaining within
        // buckets?).
        set_capacity(n);
    }

    // Size the ghash_set so that it can comfortably contain the given
    // number of elements.  If the ghash_set already contains more
    // elements than new_size, then this may be a no-op.
    void    set_capacity(size_t new_size)
    {
        size_t new_raw_size = (new_size * 5) / 4;
        if (new_raw_size < size())
            return;
        set_raw_capacity(new_raw_size);
    }


    // Disable inappropriate 'operator ->' warning on MSVC6.
#ifdef GFC_CC_MSVC
#if (GFC_CC_MSVC < 1300)
# pragma warning(disable : 4284)
#endif
#endif


    // Iterator API, like STL.
    struct const_iterator
    {   
        const C&    operator * () const
        {            
            return pHash->E(Index).value;
        }

        const C*    operator -> () const
        {
            return &pHash->E(Index).value;
        }

        void    operator ++ ()
        {
            // Find next non-empty entry.
            if (Index <= pHash->pTable->SizeMask)
            {
                Index++;
                while (Index <= pHash->pTable->SizeMask &&
                    pHash->E(Index).is_empty())
                {
                    Index++;
                }
            }
        }

        bool    operator == (const const_iterator& it) const
        {
            if (is_end() && it.is_end())
            {
                return true;
            }
            else
            {
                return (pHash == it.pHash) && (Index == it.Index);
            }
        }

        bool    operator != (const const_iterator& it) const
        {
            return ! (*this == it);
        }


        bool    is_end() const
        {
            return (pHash == NULL) || 
                (pHash->pTable == NULL) || 
                (Index > pHash->pTable->SizeMask);
        }

        const_iterator()
            : pHash(NULL), Index(0)
        { }

    protected:
        friend class ghash_set<C, hashf, althashf, entry>;

        const_iterator(const ghash_set* h, ptrdiff_t index)
            : pHash(h), Index(index)
        { }

        const ghash_set*    pHash;
        size_t              Index;
    };

    friend struct const_iterator;


    // Non-const iterator; get most of it from const_iterator.
    struct iterator : public const_iterator
    {      
        // Allow non-const access to entries.
        C&  operator*() const
        {            
            return const_cast<ghash_set*>(const_iterator::pHash)->E(const_iterator::Index).value;
        }    

        C*  operator->() const 
        {
            return &(operator*());
        }

        iterator()
            : const_iterator(NULL, 0)
        { }

    private:
        friend class ghash_set<C, hashf, althashf, entry>;

        iterator(ghash_set* h, ptrdiff_t i0)
            : const_iterator(h, i0)
        { }
    };

    friend struct iterator;

    iterator    begin()
    {
        if (pTable == 0)
            return iterator(NULL, 0);

        // Scan till we hit the first valid entry.
        size_t  i0 = 0;
        while (i0 <= pTable->SizeMask && E(i0).is_empty())
        {
            i0++;
        }
        return iterator(this, i0);
    }
    iterator    end()               { return iterator(NULL, 0); }

    const_iterator  begin() const   { return const_cast<ghash_set*>(this)->begin();     }
    const_iterator  end() const     { return const_cast<ghash_set*>(this)->end();   }

    template<class K>
    iterator    find(const K& key)
    {
        ptrdiff_t   index = find_index(key);
        if (index >= 0)        
            return iterator(this, index);        
        return iterator(NULL, 0);
    }
    template<class K>
    iterator    find_alt(const K& key)
    {
        ptrdiff_t   index = find_index_alt(key);
        if (index >= 0)        
            return iterator(this, index);        
        return iterator(NULL, 0);
    }


    template<class K>
    const_iterator  find(const K& key) const        { return const_cast<ghash_set*>(this)->find(key);   }
    template<class K>
    const_iterator  find_alt(const K& key) const    { return const_cast<ghash_set*>(this)->find_alt(key);   }

private:

    // Find the index of the matching entry.  If no match, then return -1.
    template<class K>
    ptrdiff_t   find_index(const K& key) const
    {
        if (pTable == NULL)
            return -1;
        size_t hash_value = hashf()(key) & pTable->SizeMask;
        return find_index_core(key, hash_value);
    }

    template<class K>
    ptrdiff_t   find_index_alt(const K& key) const
    {
        if (pTable == NULL)
            return -1;
        size_t hash_value = althashf()(key) & pTable->SizeMask;
        return find_index_core(key, hash_value);
    }

    // Find the index of the matching entry.  If no match, then return -1.
    template<class K>
    ptrdiff_t   find_index_core(const K& key, size_t hash_value) const
    {
        // Table must exist.
        GASSERT(pTable != 0);
        // Hash key must be 'and-ed' by the caller.
        GASSERT((hash_value & ~pTable->SizeMask) == 0);

        size_t          index = hash_value;
        const entry*    e     = &E(index);

        // If empty or occupied by a collider, not found.
        if (e->is_empty() || (e->get_cached_hash(pTable->SizeMask) != index))
            return -1;

        while(1)
        {
            GASSERT(e->get_cached_hash(pTable->SizeMask) == hash_value);

            if (e->get_cached_hash(pTable->SizeMask) == hash_value && e->value == key)
            {
                // Found it.
                return index;
            }
            // Values can not be equal at this point.
            // That would mean that the hash key for the same value differs.
            GASSERT(!(e->value == key));

            // Keep looking through the chain.
            index = e->Next_in_chain;
            if (index == (size_t)-1)
                break; // end of chain

            e = &E(index);
        }
        return -1;
    }


    // Add a new value to the ghash_set table, under the specified key.
    template<class CRef>
    void    add(const CRef& key, size_t hash_value)
    {
        check_expand();
        hash_value &= pTable->SizeMask;

        pTable->EntryCount++;

        ptrdiff_t   index         = hash_value;
        entry*      natural_entry = &(E(index));

        if (natural_entry->is_empty())
        {
            // Put the new entry in.
            new (natural_entry) entry(key, -1);
        }
        else
        {
            // Find a blank spot.
            ptrdiff_t   blank_index = index;
            do {
                blank_index = (blank_index + 1) & pTable->SizeMask;
            } while(!E(blank_index).is_empty());

            entry*  blank_entry = &E(blank_index);

            if (natural_entry->get_cached_hash(pTable->SizeMask) == (size_t)index)
            {
                // Collision.  Link into this chain.

                // Move existing list head.
                new (blank_entry) entry(*natural_entry);    // placement new, copy ctor

                // Put the new info in the natural entry.
                natural_entry->value         = key;
                natural_entry->Next_in_chain = blank_index;
            }
            else
            {
                // Existing entry does not naturally
                // belong in this slot.  Existing
                // entry must be moved.

                // Find natural location of collided element (i.e. root of chain)
                ptrdiff_t   collided_index = natural_entry->get_cached_hash(pTable->SizeMask);
                GASSERT(collided_index >= 0 && collided_index <= (ptrdiff_t)pTable->SizeMask);
                for (;;)
                {
                    entry*  e = &E(collided_index);
                    if (e->Next_in_chain == index)
                    {
                        // Here's where we need to splice.
                        new (blank_entry) entry(*natural_entry);
                        e->Next_in_chain = blank_index;
                        break;
                    }
                    collided_index = e->Next_in_chain;
                    GASSERT(collided_index >= 0 && collided_index <= (ptrdiff_t)pTable->SizeMask);
                }

                // Put the new data in the natural entry.
                natural_entry->value         = key;
                natural_entry->Next_in_chain = -1;                
            }            
        }

        // Record hash value: has effect only if cached node is used.
        natural_entry->set_cached_hash(hash_value);
    }


    // Index access helpers.
    entry&  E(size_t index)
    {
        // Must have pTable and access needs to be within bounds.
        GASSERT(index <= pTable->SizeMask);
        return *(((entry*) (pTable + 1)) + index);
    }
    const entry&    E(size_t index) const
    {        
        GASSERT(index <= pTable->SizeMask);
        return *(((entry*) (pTable + 1)) + index);
    }


    // Resize the ghash_set table to the given size (Rehash the
    // contents of the current table).  The arg is the number of
    // ghash_set table entries, not the number of elements we should
    // actually contain (which will be less than this).
    void    set_raw_capacity(size_t newSize)    
    {
        if (newSize <= 0)
        {
            // Special case.
            clear();
            return;
        }

        // Minimum size; don't incur rehashing cost when expanding
        // very small tables. Not that we perform this check before 
        // 'log2f' call to avoid fp exception with newSize == 1.
        if (newSize < GFC_HASH_MIN_SIZE)        
            newSize = GFC_HASH_MIN_SIZE;       
        else
        {
            // Force newSize to be a power of two.
            int bits = gfchop( glog2f((float)(newSize-1)) + 1);
            // GASSERT((1 << bits) >= newSize);
            newSize = size_t(1) << bits;
        }

        self  newHash;
        newHash.pTable = (table*) GALLOC(sizeof(table) + sizeof(entry) * newSize);
        // Need to do something on alloc failure!
        GASSERT(newHash.pTable);

        newHash.pTable->EntryCount = 0;
        newHash.pTable->SizeMask = newSize - 1;
        size_t i, n;

        // Mark all entries as empty.
        for (i = 0; i < newSize; i++)
            newHash.E(i).Next_in_chain = -2;

        // Copy stuff to newHash
        if (pTable)
        {            
            for (i = 0, n = pTable->SizeMask; i <= n; i++)
            {
                entry*  e = &E(i);
                if (e->is_empty() == false)
                {
                    // Insert old entry into new ghash_set.
                    newHash.add(e->value);
                    // placement delete of old element
                    e->clear();
                }
            }

            // Delete our old data buffer.
            GFREE(pTable);
        }

        // Steal newHash's data.
        pTable = newHash.pTable;
        newHash.pTable = NULL;
    }

    struct table
    {
        size_t  EntryCount;
        size_t  SizeMask;
        // Entry array follows this structure
        // in memory.
    };
    table*  pTable;
};


// ghash_set with uncached hash code; declared for convenience.
template<class C, class hashf = gfixed_size_hash<C>,
                  class althashf = hashf >
class ghash_set_uncached : public ghash_set<C, hashf, althashf, ghashset_entry<C, hashf> >
{
public:
    
    typedef ghash_set_uncached<C, hashf, althashf>                   self;
    typedef ghash_set<C, hashf, althashf, ghashset_entry<C, hashf> > base_class;

    // Delegated constructors.
    ghash_set_uncached()                                        { }
    ghash_set_uncached(int size_hint) : base_class(size_hint)   { }
    ghash_set_uncached(const self& src) : base_class(src)       { }
    ~ghash_set_uncached()                                       { }
    
    void    operator = (const self& src)
    {
        base_class::operator = (src);
    }
};


// ***** GTL::ghash hash table implementation

// Node for ghash - necessary so that ghash can delegate its implementation
// to ghash_set.
template<class C, class U, class hashf>
struct ghash_node
{
    C   first;
    U   second;

    // node_ref is used to allow passing of elements into ghash_set
    // without using a temporary object.
    struct node_ref
    {
        const C*   pfirst;
        const U*   psecond;

        node_ref(const C& f, const U& s) : pfirst(&f), psecond(&s) { }
        node_ref(const node_ref& src)    : pfirst(src.pfirst), psecond(src.psecond) { }

        // Enable computation of ghash_node_hashf.
        inline size_t get_hash() const          { return hashf()(*pfirst); } 
        // Necessary conversion to allow ghash_node::operator == to work.
        operator const C& () const              { return *pfirst; }
    };

    // Note: No default constructor is necessary.
    ghash_node(const ghash_node& src) : first(src.first), second(src.second)    { }
    ghash_node(const node_ref& src) : first(*src.pfirst), second(*src.psecond)  { }
    void operator = (const node_ref& src)  { first  = *src.pfirst; second = *src.psecond; }

#ifdef GFC_CC_RENESAS
    bool operator == (const node_ref& src) const { return (first == *src.pfirst); }
#endif

    template<class K>
    bool operator == (const K& src) const   { return (first == src); }

    template<class K>
    static size_t calc_hash(const K& data)  { return hashf()(data); }
    inline size_t get_hash() const          { return hashf()(first); }

    // Hash functors used with this node. A separate functor is used for alternative
    // key lookup so that it does not need to access the '.first' element.    
    struct node_hashf    
    {    
        template<class K>
        size_t  operator()(const K& data) const { return data.get_hash(); } 
    };    
    struct node_althashf    
    {
        template<class K>
        size_t  operator()(const K& data) const { return ghash_node<C,U,hashf>::calc_hash(data); }
    };
};



template<class C, class U,
         class hashf = gfixed_size_hash<C>,
         class hash_node = ghash_node<C,U,hashf>,
         class entry = ghashset_cached_entry<C, hashf> >
class ghash 
{
public:
    GFC_MEMORY_REDEFINE_NEW(ghash)

    // Types used for hash_set.
    typedef ghash<C, U, hashf, hash_node, entry>    self;
    typedef typename hash_node::node_hashf          hash_node_f;
    typedef ghash_set<hash_node, hash_node_f,
                      typename hash_node::node_althashf,
                      ghashset_entry<hash_node, hash_node_f> >   container;

    // Actual hash table itself, implemented as hash_set.
    container   Hash;

public:    

    ghash()     {  }
    ghash(int size_hint) : Hash(size_hint)  { }
    ghash(const self& src) : Hash(src.Hash) { }
    ~ghash()    { }

    void    operator = (const self& src)    { Hash = src.Hash; }

    // Remove all entries from the ghash table.
    inline void    clear()                  { Hash.clear(); }
    // Returns true if the ghash is empty.
    inline bool    is_empty() const         { return Hash.is_empty(); }

    // Access (set).
    inline void    set(const C& key, const U& value)
    {
        typename hash_node::node_ref e(key, value);
        Hash.set(e);
    }
    inline void    add(const C& key, const U& value)
    {
        typename hash_node::node_ref e(key, value);
        Hash.add(e);
    }

    // Removes an element by clearing its entry.
    inline void     remove(const C& key)
    {   
        Hash.remove_alt(key);
    }
    template<class K>
    inline void     remove_alt(const K& key)
    {   
        Hash.remove_alt(key);
    }

    // Retrieve the value under the given key.    
    //  - If there's no value under the key, then return false and leave *pvalue alone.
    //  - If there is a value, return true, and set *Pvalue to the entry's value.
    //  - If value == NULL, return true or false according to the presence of the key.    
    bool    get(const C& key, U* pvalue) const   
    {
        const hash_node* p = Hash.get_alt(key);
        if (p)
        {
            if (pvalue)
                *pvalue = p->second;
            return true;
        }
        return false;
    }

    template<class K>
    bool    get_alt(const K& key, U* pvalue) const   
    {
        const hash_node* p = Hash.get_alt(key);
        if (p)
        {
            if (pvalue)
                *pvalue = p->second;
            return true;
        }
        return false;
    }

    // Retrieve the pointer to a value under the given key.    
    //  - If there's no value under the key, then return NULL.    
    //  - If there is a value, return the pointer.    
    inline U*  get(const C& key)
    {
        hash_node* p = Hash.get_alt(key);
        return p ? &p->second : 0;
    }
    inline const U* get(const C& key) const
    {
        const hash_node* p = Hash.get_alt(key);
        return p ? &p->second : 0;
    }

    template<class K>
    inline U*  get_alt(const K& key)
    {
        hash_node* p = Hash.get_alt(key);
        return p ? &p->second : 0;
    }
    template<class K>
    inline const U* get_alt(const K& key) const
    {
        const hash_node* p = Hash.get_alt(key);
        return p ? &p->second : 0;
    }

    // Sizing methods - delegate to ghash.
    inline UPInt    size() const                 { return Hash.size(); }    
    inline void    resize(size_t n)             { Hash.resize(n); }
    inline void    set_capacity(UPInt new_size)  { Hash.set_capacity(new_size); }

    // Iterator API, like STL.
    typedef typename container::const_iterator  const_iterator;
    typedef typename container::iterator        iterator;

    inline iterator        begin()              { return Hash.begin(); }
    inline iterator        end()                { return Hash.end(); }
    inline const_iterator  begin() const        { return Hash.begin(); }
    inline const_iterator  end() const          { return Hash.end();   }

    iterator        find(const C& key)          { return Hash.find_alt(key);  }
    const_iterator  find(const C& key) const    { return Hash.find_alt(key);  }

    template<class K>
    iterator        find_alt(const K& key)      { return Hash.find_alt(key);  }
    template<class K>
    const_iterator  find_alt(const K& key) const { return Hash.find_alt(key);  }
};


// ghash with uncached hash code; declared for convenience.
template<class C, class U, class hashf = gfixed_size_hash<C> >
class ghash_uncached
    : public ghash<C, U, hashf, ghash_node<C,U,hashf>, ghashset_entry<C, hashf> >
{
public:

    typedef ghash_uncached<C, U, hashf>                                          self;
    typedef ghash<C, U, hashf, ghash_node<C,U,hashf>, ghashset_entry<C, hashf> > base_class;

    // Delegated constructors.
    ghash_uncached()                                        { }
    ghash_uncached(int size_hint) : base_class(size_hint)   { }
    ghash_uncached(const self& src) : base_class(src)       { }
    ~ghash_uncached()                                       { }
    
    void    operator = (const self& src)
    {
        base_class::operator = (src);
    }
};


// And identity hash in which keys serve as hash value. Can be uncached,
// since hash computation is assumed cheap.
template<class C, class U, class hashf = gidentity_hash<C> >
class ghash_identity
    : public ghash_uncached<C, U, hashf>
{
public:

    typedef ghash_identity<C, U, hashf> self;
    typedef ghash_uncached<C, U, hashf> base_class;

    // Delegated constructors.
    ghash_identity()                                        { }
    ghash_identity(int size_hint) : base_class(size_hint)   { }
    ghash_identity(const self& src) : base_class(src)       { }
    ~ghash_identity()                                       { }
    
    void    operator = (const self& src)
    {
        base_class::operator = (src);
    }
};



#if defined(GFC_BUILD_DEFINE_NEW) && defined(GFC_DEFINE_NEW)
#define new GFC_DEFINE_NEW
#endif


/////////////////
// 
template <typename T, typename UIntType = UInt8>
class gpaged_heap : public GRefCountBase<gpaged_heap<T, UIntType> >
{
public:
    enum
    {
        Last_Free_Elem = (UIntType)-1,
        Not_Free_Elem = (UIntType)-2
    };

private:
    struct Element
    {
        T           Data;
        UIntType    NextFreeIndex; // -1 - last free element
                                   // -2 - not free element
                                   // otherwise - index of next free element
    };

    struct Page
    {
        UIntType    FirstFreeIndex;
        UIntType    LastFreeIndex;
        UIntType    nUsedElements;
        //UByte*  pPageElements; // use GetPageElements()

        Page(): FirstFreeIndex(Last_Free_Elem), LastFreeIndex(Last_Free_Elem), nUsedElements(0)
        {
            //pPageElements = (UByte*)GALLOC(ElementSize() * nelems_per_page);
        }
        ~Page()
        {
            for (UInt i = 0; i < nUsedElements; ++i)
            {
                Element* pe = GetElementAt(i);
                if (pe->NextFreeIndex == Not_Free_Elem)
                    pe->Data.~T();
            }
        }
        // aligned element's size - align element size to the size of pointer type.
        static UInt ElementSize() { return ((sizeof(Element)+sizeof(void*) - 1)/sizeof(void*)*sizeof(void*)); } 

        // align a ptr to the first elem accordingly to the size of pointer type
        static UInt PageHeaderSize() { return (sizeof(Page)+sizeof(void*)-1)/sizeof(void*)*sizeof(void*); } 

        Element* GetElementAt(UInt n)
        {
            return reinterpret_cast<Element*>(((UByte*)this)+ PageHeaderSize() + ElementSize() * n);
        }

        UInt allocate_element(const T& val, UIntType nelems_per_page)
        {
            Element* pe = NULL;
            UInt peIdx = ~0u;
            if (nUsedElements < nelems_per_page)
            {
                // get elements from the end of the page
                pe = GetElementAt(nUsedElements);
                peIdx = nUsedElements++;
            }
            else
            {
                // look for elements in the list of freed ones.
                if (FirstFreeIndex != Last_Free_Elem)
                {
                    pe = GetElementAt(FirstFreeIndex);
                    peIdx = FirstFreeIndex;
                    FirstFreeIndex = pe->NextFreeIndex;
                    if (FirstFreeIndex == Last_Free_Elem)
                        LastFreeIndex = Last_Free_Elem;
                }
            }
            if (pe)
            {
                pe->NextFreeIndex = Not_Free_Elem;
                GTL::gconstruct_alt<T, T>(&pe->Data, val);
            }
            return peIdx;
        }
    };
    GTL::garray<Page*>  Pages;
    UIntType            NElemsPerPage;
public:
    gpaged_heap(UInt nelems_per_page = 20) //???
    {
        if (nelems_per_page > ((UIntType)-2))
            NElemsPerPage = (UIntType)-2;
        else
            NElemsPerPage = (UIntType)nelems_per_page;
    }
    ~gpaged_heap()
    {
        for(UPInt i = 0, n = Pages.size(); i < n; ++i)
        {
            Pages[i]->~Page();
            GFREE(Pages[i]);
        }
    }

    UInt allocate_element(const T& val = T())
    {
        UInt index = ~0u;
        allocate_elements(1, &index, val);
        return index;
    }
    void allocate_elements(UInt num, UInt* pindices, const T& val = T())
    {
        UInt idxInIndicesArr = 0;
        UInt n = (UInt)Pages.size();
        for(UInt i = 0; i < n && idxInIndicesArr < num; ++i)
        {
            UInt peIdx = Pages[i]->allocate_element(val, NElemsPerPage);
            if (peIdx != ~0u)
                pindices[idxInIndicesArr++] =  i * NElemsPerPage + peIdx;
        }
        if (idxInIndicesArr < num)
        {
            // need to allocate more? Allocate new pages
            UInt remainingElems = num - idxInIndicesArr;
            UInt newPagesNum = (remainingElems + NElemsPerPage - 1) / NElemsPerPage;
            Pages.resize(n + newPagesNum);
            for (UInt np = 0; np < newPagesNum && idxInIndicesArr < num; ++np)
            {
                Page *p = (Page*)GALLOC(Page::PageHeaderSize() + Page::ElementSize() * NElemsPerPage);
#ifdef GFC_BUILD_DEBUG
                memset(p, 0xDD, Page::PageHeaderSize() + Page::ElementSize() * NElemsPerPage);
#endif
                GTL::gconstruct<Page>(p);
                Pages[n + np] = p;
                for (UInt i = 0; i < NElemsPerPage && idxInIndicesArr < num; ++i)
                {
                    UInt peIdx = p->allocate_element(val, NElemsPerPage);
                    if (peIdx != ~0u)
                        pindices[idxInIndicesArr++] =  (n + np) * NElemsPerPage + peIdx;
                }
            }
        }
    }

    void release_element(UInt elemIdx)
    {
        UInt ne = elemIdx / NElemsPerPage;
        if (ne < Pages.size())
        {
            Page* p = Pages[ne];
            if (p)
            {
                ne = elemIdx % NElemsPerPage;
                if (ne < p->nUsedElements)
                {
                    Element* pe = p->GetElementAt(ne);
                    pe->Data.~T();
#ifdef GFC_BUILD_DEBUG
                    memset(&pe->Data, 0xEE, Page::ElementSize());
#endif
                    pe->NextFreeIndex = Last_Free_Elem;

                    // add this element to freed list
                    if (p->LastFreeIndex != Last_Free_Elem)
                    {
                        pe = p->GetElementAt(p->LastFreeIndex);
                        pe->NextFreeIndex = UIntType(ne);
                    }
                    p->LastFreeIndex = UIntType(ne);
                    if (p->FirstFreeIndex == Last_Free_Elem)
                        p->FirstFreeIndex = UIntType(ne);
                }
            }
        }
    }

    T* get_element_at(UInt n) const
    {
        UInt ne = n/NElemsPerPage;
        if (ne < Pages.size())
        {
            Page* p = Pages[ne];
            if (p)
            {
                ne = n % NElemsPerPage;
                if (ne < p->nUsedElements)
                {
                    Element* pe = p->GetElementAt(ne);
                    return &pe->Data;
                }
            }
        }
        return NULL;
    }
};

/////////////////
// 
template <typename T, typename UIntType = UInt8>
class gindexed_paged_array
{
public:
    typedef gpaged_heap<T,UIntType> HeapType;
private:
    GPtr<HeapType>      pHeap;
    GTL::garray<UInt>   Indices;
public:
    gindexed_paged_array() : pHeap(NULL) {}
    gindexed_paged_array(HeapType* pheap) : pHeap(pheap) {}
    gindexed_paged_array(HeapType& heap)  : pHeap(heap) {}
    ~gindexed_paged_array() { if (pHeap) resize(0); }

    const   T& operator[](UPInt n) const
    {
        GASSERT(pHeap);
        GASSERT(n < Indices.size());
        return *pHeap->get_element_at(Indices[n]);
    }
    T& operator[](UPInt n)
    {
        GASSERT(pHeap);
        GASSERT(n < Indices.size());
        return *pHeap->get_element_at(Indices[n]);
    }

    UPInt size() const { return Indices.size(); }

    void push_back(const T& val)
    {
        append(val);
    }
    void pop_back()
    {
        GASSERT(size() > 0);
        remove(size() - 1);
    }
    void remove(UPInt index)
    {
        GASSERT(index < Indices.size());
        GASSERT(pHeap);
        pHeap->release_element(Indices[index]);
        Indices.remove(index);
    }
    void removeMultiple(UPInt index, UPInt num)
    {
        GASSERT(index + num <= Indices.size());
        GASSERT(pHeap);
        for (UInt i = 0; i < num; ++i)
            pHeap->release_element(Indices[index + i]);
        Indices.removeMultiple(index, num);
    }
    void    insert(UPInt index, const T& val = T())
    {
        GASSERT(index <= Indices.size());
        GASSERT(pHeap);
        Indices.insert(index);
        Indices[index] = pHeap->allocate_element(val);
    }

    void    insertMultiple(UInt index, UInt num)
    {
        GASSERT(index <= Indices.size());
        GASSERT(pHeap);
        Indices.insertMultiple(index, num);
        pHeap->allocate_elements(num, &Indices[index]);
    }
    void    append(const T& value)
    {
        GASSERT(pHeap);
        UInt idx = pHeap->allocate_element(value);
        Indices.append(&idx, 1);
    }
    void    resize(UPInt num)
    {
        GASSERT(pHeap);
        UPInt oldSize = Indices.size();
        if (oldSize < num)
        {
            // expand
            Indices.resize(num);
            pHeap->allocate_elements(UInt(num - oldSize), &Indices[oldSize]);
        }
        else
        {
            // shrink
            for (UPInt i = num; i < oldSize; ++i)
                pHeap->release_element(Indices[i]);
            Indices.resize(num);
        }
    }
    void    clear()
    {
        resize(0);
    }
    void    set_heap(HeapType& heap)
    {
        if (size() == 0)
            pHeap = heap;
        else
            GASSERT(0); // can't change heap since elements are already allocated
    }
    void    set_heap(HeapType* pheap)
    {
        if (size() == 0)
            pHeap = pheap;
        else
            GASSERT(0); // can't change heap since elements are already allocated
    }
    void move_indices(UPInt insertionPos, gindexed_paged_array<T,UIntType>* psrcArray, UPInt srcIndex, UPInt num)
    {
        GASSERT(psrcArray);
        GASSERT(pHeap == psrcArray->pHeap);
        GASSERT(srcIndex + num <= psrcArray->Indices.size());
        Indices.insertMultiple(insertionPos, num);
        for (UPInt i = 0; i < num; ++i)
        {
            Indices[i + insertionPos] = psrcArray->Indices[i + srcIndex];
        }
        psrcArray->Indices.removeMultiple(srcIndex, num);
    }

    class iterator
    {
        gindexed_paged_array*   pArray;
        int                     CurIndex;
    public:
        iterator() : pArray(NULL), CurIndex(-1) {}
        iterator(gindexed_paged_array* parr) : pArray(parr), CurIndex(0) {}
        iterator(gindexed_paged_array* parr, int idx) : pArray(parr), CurIndex(idx) {}
        iterator(const iterator& it) : pArray(it.pArray), CurIndex(it.CurIndex) {}

        iterator& operator=(const iterator& it)
        {
            pArray = it.pArray;
            CurIndex = it.CurIndex;
            return *this;
        }

        bool operator==(const iterator& it) const { return pArray == it.pArray && CurIndex == it.CurIndex; }
        bool operator!=(const iterator& it) const { return pArray != it.pArray || CurIndex != it.CurIndex; }

        iterator& operator++()
        {
            if (pArray)
            {
                if (CurIndex < (int)pArray->size())
                    ++CurIndex;
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator it(*this);
            operator++();
            return it;
        }
        iterator& operator--()
        {
            if (pArray)
            {
                if (CurIndex >= 0)
                    --CurIndex;
            }
            return *this;
        }
        iterator operator--(int)
        {
            iterator it(*this);
            operator--();
            return it;
        }
        iterator operator+(int delta) const
        {
            return iterator(pArray, CurIndex + delta);
        }
        iterator operator-(int delta) const
        {
            return iterator(pArray, CurIndex - delta);
        }
        SPInt operator-(const iterator& right) const
        {
            GASSERT(pArray == right.pArray);
            return CurIndex - right.CurIndex;
        }
        T& operator*() const    { GASSERT(pArray); return (*pArray)[CurIndex]; }
        T* operator->() const   { GASSERT(pArray); return &(*pArray)[CurIndex]; }
        T* GetPtr() const       { GASSERT(pArray); return &(*pArray)[CurIndex]; }

        bool IsFinished() const { return !pArray || CurIndex < 0 || CurIndex >= (int)pArray->size(); }

        void remove()
        {
            if (!IsFinished())
                pArray->remove(CurIndex);
        }

        int get_index() const { return CurIndex; }
    };
    typedef iterator Iterator;
    iterator begin() { return iterator(this); }
    iterator end()   { return iterator(this, (int)size()); }
    iterator Begin() { return begin(); }
    iterator End()   { return end(); }
    iterator Last()  { return iterator(this, ((int)size()) - 1); }

    class const_iterator
    {
        const gindexed_paged_array* pArray;
        int                         CurIndex;
    public:
        const_iterator() : pArray(NULL), CurIndex(-1) {}
        const_iterator(const gindexed_paged_array* parr) : pArray(parr), CurIndex(0) {}
        const_iterator(const gindexed_paged_array* parr, int idx) : pArray(parr), CurIndex(idx) {}
        const_iterator(const iterator& it) : pArray(it.pArray), CurIndex(it.CurIndex) {}

        const_iterator& operator=(const const_iterator& it)
        {
            pArray = it.pArray;
            CurIndex = it.CurIndex;
            return *this;
        }

        bool operator==(const const_iterator& it) const { return pArray == it.pArray && CurIndex == it.CurIndex; }
        bool operator!=(const const_iterator& it) const { return pArray != it.pArray || CurIndex != it.CurIndex; }

        const_iterator& operator++()
        {
            if (pArray)
            {
                if (CurIndex < (int)pArray->size())
                    ++CurIndex;
            }
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator it(*this);
            operator++();
            return it;
        }
        const_iterator& operator--()
        {
            if (pArray)
            {
                if (CurIndex >= 0)
                    --CurIndex;
            }
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator it(*this);
            operator--();
            return it;
        }
        const_iterator operator+(int delta) const
        {
            return const_iterator(pArray, CurIndex + delta);
        }
        const_iterator operator-(int delta) const
        {
            return const_iterator(pArray, CurIndex - delta);
        }
        SPInt operator-(const const_iterator& right) const
        {
            GASSERT(pArray == right.pArray);
            return CurIndex - right.CurIndex;
        }
        const T& operator*() const  { GASSERT(pArray); return (*pArray)[CurIndex]; }
        const T* operator->() const { GASSERT(pArray); return &(*pArray)[CurIndex]; }
        const T* GetPtr() const     { GASSERT(pArray); return &(*pArray)[CurIndex]; }

        bool IsFinished() const { return !pArray || CurIndex < 0 || CurIndex >= pArray->size(); }

        int get_index() const { return CurIndex; }
    };
    typedef const_iterator ConstIterator;
    const_iterator begin() const { return const_iterator(this); }
    const_iterator end() const   { return const_iterator(this, (int)size()); }
    const_iterator Begin() const { return begin(); }
    const_iterator End() const   { return end(); }
    const_iterator Last() const  { return const_iterator(this, (int)size() - 1); }
};

template <typename T, typename UIntType = UInt8, int nelems_per_page = 10>
class gindexed_array : public gindexed_paged_array<T,UIntType>
{
public:
    typedef typename gindexed_paged_array<T,UIntType>::HeapType base_HeapType;

    gindexed_array():gindexed_paged_array<T,UIntType>(*new base_HeapType(nelems_per_page)) {}
    gindexed_array(UIntType n_elems_per_page):gindexed_paged_array<T,UIntType>(*new base_HeapType(n_elems_per_page)) {}
    gindexed_array(base_HeapType* pheap) : gindexed_paged_array<T,UIntType>(pheap) {}
    gindexed_array(base_HeapType& heap)  : gindexed_paged_array<T,UIntType>(heap) {}
    ~gindexed_array() { }
};

// *** Algorithms

// Container find
template<class Container, class EqualityComparable>
typename Container::iterator find(Container &c, const EqualityComparable &val)
{   return find(c.begin(),c.end(),val); }

template<class Container, class EqualityComparable>
typename Container::const_iterator const_find(const Container &c, const EqualityComparable &val)
{   return find(c.begin(),c.end(),val); }

// Container find_index
template<class Container, class EqualityComparable>
SPInt    find_index(const Container &c, const EqualityComparable &val)
{
    SPInt size = (SPInt)c.size();
    for (SPInt i=0; i<size; i++)
        if (c[i] == val)
            return i;
    return -1; 
}

template <class Comparator, class Container, class Comparable>
SPInt find_index_in_sorted_primitive_array(const Container &c, UPInt arrSize, const Comparable& e, const Comparator& cmp = Comparator())
{
    // do a binary search
    const SPInt count = (SPInt)arrSize;

    SPInt lower = 0;
    SPInt upper = count - 1;

    while (lower < upper && upper != -1) {
        SPInt middle = (lower + upper)/2;
        int cmpRes = cmp.Compare(c[middle], e);
        if (cmpRes == 0) 
        { // equal
            return middle;
        }
        if (cmpRes < 0) // *mpArray[middle] < *elem
            lower = middle+1;
        else
            upper = middle-1;
    }

    if (lower == upper && cmp.Compare(c[lower], e) == 0)
        return lower;
    return -1;
}

template <class Comparator, class Container, class Comparable>
SPInt find_index_in_sorted_array(const Container &c, const Comparable& e, const Comparator& cmp = Comparator())
{
    return find_index_in_sorted_primitive_array(c, c.size(), e, cmp);
}

template <class Comparator, class Container, class Comparable>
SPInt find_insertion_index_in_sorted_primitive_array(const Container &c, UPInt arrSize, const Comparable& e, const Comparator& cmp = Comparator())
{
    // do a binary search
    const SPInt count = (SPInt)arrSize;

    SPInt lower = 0;
    SPInt upper = count - 1;

    while (lower < upper && upper != -1) {
        SPInt middle = (lower + upper)/2;
        int cmpRes = cmp.Compare(c[middle], e);
        if (cmpRes == 0) 
        { // equal
            return middle;
        }
        if (cmpRes < 0) // *mpArray[middle] < *elem
            lower = middle+1;
        else
            upper = middle-1;
    }
    while(lower <= upper && cmp.Compare(c[lower], e) < 0)
        ++lower;
    return lower;
}

template <class Comparator, class Container, class Comparable>
SPInt find_insertion_index_in_sorted_array(const Container &c, const Comparable& e, const Comparator& cmp = Comparator())
{
    return find_insertion_index_in_sorted_primitive_array(c, c.size(), e, cmp);
}

template <class Comparable, class ElementType>
struct gdefault_comparator
{
    int Compare(const ElementType& e1, const Comparable& e2)
    {
        if (e1 == e2)
            return 0;
        else if (e1 < e2)
            return -1;
        else
            return 1;
    }
};

template <class ElementType, class Container, class Comparable>
SPInt find_index_in_sorted_array_default(const Container &c, const Comparable &val)
{
    gdefault_comparator<Comparable, ElementType> cmp;
    return find_index_in_sorted_array(c, val, cmp);
}

template <class Comparator, class Container, class Comparable>
SPInt find_nearest_index_in_sorted_array(const Container &c, const Comparable &e, const Comparator& cmp = Comparator())
{
    const SPInt count = (SPInt)c.size();
    if (count == 0)
    {
        return -1;
    }

    SPInt lower = 0;
    SPInt upper = count - 1;
    SPInt lowest = 0;

    while (lower < upper && upper != -1) {
        SPInt middle = (lower + upper)/2;
        int cmpRes = cmp.Compare(c[middle], e);
        if (cmpRes == 0) 
        { // equal
            return middle;
        }
        if (cmpRes < 0) // *mpArray[middle] < *elem
        {
            lowest = lower;
            lower = middle+1;
        }
        else
            upper = middle-1;
    }

    if (lower == upper && cmp.Compare(c[lower], e) == 0)
        return lower;
    while(lowest < upper && cmp.Compare(c[lowest + 1], e) < 0)
        ++lowest;
    return lowest;
}

template <class ElementType, class Container, class Comparable>
SPInt find_nearest_index_in_sorted_array_default(const Container &c, const Comparable &val)
{
    gdefault_comparator<Comparable, ElementType> cmp;
    return find_nearest_index_in_sorted_array(c, val, cmp);
}

// Do not change the order of lines below, otherwise VC++ will gen warnings in /Wp64 mode
inline bool IsMax(UPInt v) { return v == GFC_MAX_UPINT; }
#ifdef GFC_64BIT_POINTERS
inline bool IsMax(UInt v) { return v == GFC_MAX_UINT; }
#endif

}; // namespace GTL



#endif
