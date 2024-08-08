/**********************************************************************

Filename    :   GRefCount.h
Content     :   Reference counting implementation headers
Created     :   January 14, 1999
Authors     :   Michael Antonov
Notes       :

History     :   

Copyright   :   (c) 1999-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GREFCOUNT_H
#define INC_GREFCOUNT_H

#include "GTypes.h"
#include "GMemory.h"


// ***** Declared classes
template<class C>
class   GRefCountBase;
class   GRefCountBaseImpl;
class   GRefCountImpl;

template<class C>
class   GWeakPtr;
class   GWeakPtrProxy;



// ***** Reference Count Base

// This class can be used whenever reference counting
// with static vs. dynamic allocation determination is necessary


// *** Macros for GRefCountBase

// Key values are used to determine static/dynamic creation
#define GFC_REFCOUNT_KEY1   0x56471E89
#define GFC_REFCOUNT_KEY2   0x9FE1234A

// These macros specify whether reference counting is configured to be
// thread safe or not. By default, reference counting is not thread-safe
// for most classes.
#define GFC_REFCOUNT_NORMAL     0x00000000
#define GFC_REFCOUNT_THREADSAFE 0x00000001


// ***** Implementation For REF-Counting


class GRefCountImpl
{
public:
    void    (GFASTCALL *pAddRef)(GRefCountBaseImpl *pbase);
    void    (GFASTCALL *pRelease)(GRefCountBaseImpl *pbase, UInt flags);
};



class GRefCountBaseImpl
{
public:
    // Local Variables
    volatile SInt   RefCount;
    GRefCountImpl*  pRefCountImpl;

    // TBD: Inherited for SWF support, NTS (Not thread-safe)
    mutable GWeakPtrProxy*  pWeakProxy;


    // Constructor
    // Does some custom processing to initialize the base
    GEXPORT GRefCountBaseImpl();
    GEXPORT GRefCountBaseImpl(GRefCountImpl *pnormalImpl, GRefCountImpl *pnullImpl=0);

    // Need virtual destructor
    // This:    1. Makes sure the right destructor's called
    //          2. Makes us have VTable, necessary if we are going to have format needed by InitNewMem()
    GEXPORT virtual ~GRefCountBaseImpl();

    // Ref-Count Implementation
    GINLINE void    AddRef()                        { pRefCountImpl->pAddRef(this); }
    GINLINE void    Release(UInt flags=0)           { pRefCountImpl->pRelease(this,flags);  }
    GEXPORT bool    SetRefCountMode(UInt mode);

    

    // Create/return create proxy, users must release proxy when no longer needed
    GWeakPtrProxy*  CreateWeakProxy() const;
    // Debug method only, NTS
    SInt            GetRefCount() const { return RefCount;  }


    // If New operator's overridden by the macros
    // it must call this function to initialize new memory before returning it  
    GINLINE static void InitNewRefCountMem(GRefCountBaseImpl *pmem)
    { 
        pmem->RefCount      = GFC_REFCOUNT_KEY1;
        pmem->pRefCountImpl = (GRefCountImpl*)UPInt(GFC_REFCOUNT_KEY2); 
    }   

    // Base class ref-count content should not be copied.
    void operator = (const GRefCountBaseImpl &) { }

    // *** Override New and Delete

// DOM-IGNORE-BEGIN
// Undef new temporarily if it is being redefined
#ifdef GFC_BUILD_DEFINE_NEW
#ifdef GFC_DEFINE_NEW
#undef new
#endif
#endif

    // Custom use initializer on allocation, to distinguish dynamic allocation
    #define GFC_REFCOUNTALLOC_INIT_MEM(class_name, p)   \
        do {if (p) InitNewRefCountMem((class_name*)p); } while(0)
    // Redefine all new & delete operators.
    GFC_MEMORY_REDEFINE_NEW_IMPL(GRefCountBaseImpl, GFC_REFCOUNTALLOC_INIT_MEM)

#ifdef GFC_DEFINE_NEW
#define new GFC_DEFINE_NEW
#endif
// GFC_BUILD_DEFINE_NEW
// DOM-IGNORE-END
};


// *** GInterface RefCount Base

// This class exists so that new can be overriden in a future, even when ::new is redefines
// It keeps interface consistent with GFC Complete
template<class C>
class GRefCountBase : public GRefCountBaseImpl
{
public:

    // Constructor
    // Does some custom processing to initialize the base
    GINLINE GRefCountBase() : GRefCountBaseImpl() 
        { }
    GINLINE GRefCountBase(GRefCountImpl *pnormalImpl, GRefCountImpl *pnullImpl=0) : 
            GRefCountBaseImpl(pnormalImpl, pnullImpl) 
        { }
};



// ***** Ref-Counted template pointer

// Automatically AddRefs and Releases interfaces

template<class C>
class GPtr
{   
protected:
    C   *pObject;

public:

    // Constructors
    GINLINE GPtr() : pObject(0)
    { }
    GINLINE GPtr(C &robj)
    {
        pObject = &robj;
    }
    GINLINE GPtr(C *pobj)
    {
        if (pobj) pobj->AddRef();   
        pObject = pobj;
    }
    GINLINE GPtr(const GPtr<C> &src)
    {
        if (src.pObject) src.pObject->AddRef();     
        pObject = src.pObject;
    }

    template<class R>
    GINLINE GPtr(GPtr<R> &src)
    {
        if (src) src->AddRef();
        pObject = src;
    }

    // Destructor
    GINLINE ~GPtr()
    {
        if (pObject) pObject->Release();        
    }

    // Compares
    GINLINE bool operator == (const GPtr &other) const      { return pObject == other.pObject; }
    GINLINE bool operator != (const GPtr &other) const      { return pObject != other.pObject; }

    GINLINE bool operator == (const C *pother) const        { return pObject == pother; }
    GINLINE bool operator != (const C *pother) const        { return pObject != pother; }
    GINLINE bool operator == (C *pother) const              { return pObject == pother; }
    GINLINE bool operator != (C *pother) const              { return pObject != pother; }


    GINLINE bool operator < (const GPtr &other) const       { return pObject < other.pObject; }

    // Assignment
    template<class R>
    GINLINE const GPtr<C>& operator = (const GPtr<R> &src)
    {
        if (src) src->AddRef();
        if (pObject) pObject->Release();        
        pObject = src;
        return *this;
    }   
    // Specialization
    GINLINE const GPtr<C>& operator = (const GPtr<C> &src)
    {
        if (src) src->AddRef();
        if (pObject) pObject->Release();        
        pObject = src;
        return *this;
    }   
    
    GINLINE const GPtr<C>& operator = (C *psrc)
    {
        if (psrc) psrc->AddRef();
        if (pObject) pObject->Release();        
        pObject = psrc;
        return *this;
    }   
    GINLINE const GPtr<C>& operator = (C &src)
    {       
        if (pObject) pObject->Release();        
        pObject = &src;
        return *this;
    }
    
    // Set Assignment
    template<class R>
    GINLINE GPtr<C>& SetPtr(const GPtr<R> &src)
    {
        if (src) src->AddRef();
        if (pObject) pObject->Release();
        pObject = src;
        return *this;
    }
    // Specialization
    GINLINE GPtr<C>& SetPtr(const GPtr<C> &src)
    {
        if (src) src->AddRef();
        if (pObject) pObject->Release();
        pObject = src;
        return *this;
    }   
    
    GINLINE GPtr<C>& SetPtr(C *psrc)
    {
        if (psrc) psrc->AddRef();
        if (pObject) pObject->Release();
        pObject = psrc;
        return *this;
    }   
    GINLINE GPtr<C>& SetPtr(C &src)
    {       
        if (pObject) pObject->Release();
        pObject = &src;
        return *this;
    }

    // Nulls ref-counted pointer without decrement
    GINLINE void    NullWithoutRelease()    
    { 
        pObject = 0;    
    }

    // Clears the pointer to the object
    GINLINE void    Clear()
    {
        if (pObject) pObject->Release();
        pObject = 0;
    }

    // Obtain pointer reference directly, for D3D interfaces
    GINLINE C*& GetRawRef()                 { return pObject; }

    // Access Operators
    GINLINE C* GetPtr() const               { return pObject; }
    GINLINE C& operator * () const          { return *pObject; }
    GINLINE C* operator -> ()  const        { return pObject; }
    // Conversion                   
    GINLINE operator C* () const            { return pObject; }
};



// *** Weak Pointer Support

// Helper for making objects that can have weak_ptr's.
// TBD: Converted WeakPtr logic is not thread-safe, need to work this out

class GWeakPtrProxy : public GNewOverrideBase
{
public:
    GWeakPtrProxy() :
        RefCount(1), Alive(1)
    { }

    // GWeakPtr calls this to determine if its pointer is valid or not
    GINLINE bool    IsAlive() const
    { return Alive; }

    // Objects call this to inform of their death
    GINLINE void    NotifyObjectDied() { Alive = 0; }

    GINLINE void    AddRef()
    {   RefCount++; }
    GINLINE void    Release()
    {
        RefCount--;
        if (RefCount == 0)
            delete this;                
    }

private:
    // Hidden
    GWeakPtrProxy(const GWeakPtrProxy& w) { GUNUSED(w); }
    void    operator=(const GWeakPtrProxy& w) { GUNUSED(w); }

    SInt    RefCount;
    bool    Alive;
};


// A weak pointer points at an object, but the object may be deleted
// at any time, in which case the weak pointer automatically becomes
// NULL.  The only way to use a weak pointer is by converting it to a
// strong pointer (i.e. for temporary use).
//
// The class pointed to must have a "GWeakPtrProxy* GetWeakPtrProxy()" method.
//
// Usage idiom:
//
// if (GPtr<my_type> ptr = weak_ptr_to_my_type) { ... use ptr->whatever() safely in here ... }


template<class C>
class GWeakPtr
{
public:
    GWeakPtr() : pObject(0)
        { }

    GINLINE GWeakPtr(C* ptr) // : pObject(0)
        { operator=(ptr); }

    GINLINE GWeakPtr(const GPtr<C>& ptr)
        { operator=(ptr.GetPtr()); }

    // Default constructor and assignment from GWeakPtr<C> are OK
    GINLINE void    operator=(C* ptr)
    {
        pObject = ptr;
        if (pObject)
        {
            pProxy = *pObject->CreateWeakProxy();
            //assert(pProxy != NULL);
            //assert(pProxy->is_alive());
        }           
        else            
            pProxy = 0;         
    }

    GINLINE void    operator=(const GPtr<C>& ptr)
        { operator=(ptr.GetPtr()); }

    // Conversion to GPtr
    inline operator GPtr<C>()
    {
        CheckProxy();
        return GPtr<C>(pObject);
    }

    GINLINE bool    operator==(C* ptr)
        { CheckProxy(); return pObject == ptr; }
    GINLINE bool    operator==(const GPtr<C>& ptr)
        { CheckProxy(); return pObject == ptr.GetPtr(); }

private:
    
    // Set pObject to NULL if the object died
    GINLINE void CheckProxy()   
    {
        if (pObject)
        {
            //assert(pProxy != NULL);
            if (!pProxy->IsAlive())
            {
                // Underlying object went away.
                pProxy = 0;
                pObject = 0;
            }
        }
    }

    GPtr<GWeakPtrProxy> pProxy;
    C*                  pObject;
};



#endif
