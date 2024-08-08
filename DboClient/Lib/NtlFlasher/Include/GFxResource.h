/**********************************************************************

Filename    :   GFxResource.h
Content     :   Resource and Resource Library definition for GFx
Created     :   January 11, 2007
Authors     :   Michael Antonov

Notes       :   

Copyright   :   (c) 2005-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFXRESOURCE_H
#define INC_GFXRESOURCE_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GTLTypes.h"

#include "GThreads.h"

#include "GFxString.h"
#include "GTLTypes.h"

// ***** Classes Declared

class GFxResourceId;
class GFxResourceKey;
class GFxResource;

class GFxResourceLib;
class GFxResourceWeakLib;



/* *** GFxResourceId

GFxResourceId describes an id used to identify a resource in an SWF/GFX file.
ResourceId is used for indexing the resource in the resource table loaded
into MovieDataDef.

Most of the resource ids are assigned by the Flash Studio; however, extra
ids can also be generated in GFx either during loading (IdType_Static) or
when exporting dynamic resources with GFxExport (IdType_Export). Such ids
have their corresponding bits set, differentiating them from the original
SWF file resource ids.

In GFxExport, ids are used primarily to generate unique file names for
exported resources. 
*/


// Character Id wrapper class.
class GFxResourceId
{
    UInt32    Id;

public:
    enum IdTypeConstants
    {
        IdType_Bit_IndexMask    = 0x0000FFFFu,
        IdType_Bit_TypeMask     = 0x0FFF0000u,

        // 2 bits - Describe Id generation source.
        IdType_Bit_SWF          = 0x00000,   // Id comes from SWF File.
        IdType_Bit_Static       = 0x10000,   // Id assigned uniquely during loading.
        IdType_Bit_Export       = 0x20000,   // Id assigned uniquely during export.
        IdType_Bit_GenMask      = 0x30000,

        // The amount to shift type code so that id does not
        // conflict with index and generation source.
        IdType_Bit_TypeShift    = 18,

        // Indicates an explicitly assigned invalid id.
        InvalidId               = (1 << IdType_Bit_TypeShift) | IdType_Bit_SWF
    };

    //  NOTE: Only the lower bits of resourceId (0xFFFF mask) can be used to identify
    //        characters instantiated on stage, due to the way depth is combined
    //        with id in display list searches. If high bits need to be used that
    //        logic (mostly in PlayerImpl and Sprite) would need to be changed.

    // IdType can be one of the following values describing what the id represents.
    enum IdType
    {
        IdType_None             = 0,
        IdType_InternalConstant = (0 << IdType_Bit_TypeShift) | IdType_Bit_Static,
        IdType_GradientImage    = (1 << IdType_Bit_TypeShift) | IdType_Bit_Static,
        IdType_DynFontImage     = (2 << IdType_Bit_TypeShift) | IdType_Bit_Static,
        IdType_FontImage        = (1 << IdType_Bit_TypeShift) | IdType_Bit_Export,
    };

    inline explicit GFxResourceId()                              { Id = InvalidId; }
    inline explicit GFxResourceId(UInt32 resId)                  { Id = resId; }
    inline GFxResourceId(const GFxResourceId &src)               { Id = src.Id; }
    inline GFxResourceId operator = (const GFxResourceId &src)   { Id = src.Id; return *this; }

    bool operator == (const GFxResourceId &other) const          { return Id == other.Id; }
    bool operator != (const GFxResourceId &other) const          { return !operator == (other); }
    // Numeric compare, so that we can compare with InvalidId, etc.
    bool operator == (UInt32 idVal) const                        { return Id == idVal; }
    bool operator != (UInt32 idVal) const                        { return !operator == (idVal); }


    // Returns a Id integer code that uniquely identifies the character.
    inline  UInt32          GetIdValue() const  { return Id; }

    // Returns index and 
    inline  UInt            GetIdIndex() const  { return Id & IdType_Bit_IndexMask;  }
    inline  IdType          GetIdType() const   { return (IdType) (Id & IdType_Bit_TypeMask); }


    // Generate Resource string, up to 8 characters - must pass a buffer of 9.
    UInt                    GenerateIdString(char* pbuffer, size_t bufferSize =9) const;

    // Generate nest Id for the IdType_Bit_Static and IdType_Bit_Export types.
    // The Id to use is returned,
    inline  GFxResourceId   GenerateNextId()
    {        
        GASSERT(Id & IdType_Bit_GenMask);        
        GASSERT((Id & IdType_Bit_IndexMask) != IdType_Bit_IndexMask); // Detect overflows.
        GFxResourceId result = *this;
        Id++;
        return result;
    }


    // Hash Op for when ResourceIds are used in Hash table
    class HashOp
    {
    public:
        size_t operator () (const GFxResourceId& idref)
        {
            return (idref.Id ^ (idref.Id >> (16 - 8)));
        }
    };
};




// ***** File information for file formats from which images can come

class GFxFileConstants
{
public:

    // All File formats supported and/or used by GFx.
    enum FileFormatType
    {        
        File_Unopened,
        File_Unknown,

        // SWF file formats.
        File_SWF,
        File_GFX,

        // Image formats supported by Flash.        
        File_JPEG   = 10,  // Format codes are serialized by 'gfxexport'. They should not change.
        File_PNG    = 11,
        File_GIF    = 12,
        // Other image formats.
        File_TGA    = 13,
        File_DDS    = 14,        
        File_HDR    = 15,
        File_BMP    = 16,
        File_DIB    = 17,
        File_PFM    = 18,
        File_TIFF   = 19,

        File_NextAvail = 20,
        File_Original  = 65535
    };
};


// ***** GFxResourceKey - serves as a general key used in resource matching.


struct GFxResourceFileInfo : public GRefCountBase<GFxResourceFileInfo>
{
    GFxFileConstants::FileFormatType    Format;
    const struct GFxExporterInfo*       pExporterInfo;    
    GFxString                           FileName;

    GFxResourceFileInfo()
    {
        Format = GFxFileConstants::File_Unknown;
        pExporterInfo = 0;
    }
    GFxResourceFileInfo(const GFxResourceFileInfo &src)
        : FileName(src.FileName)
    {
        Format = src.Format;
        pExporterInfo = src.pExporterInfo;
    }

    size_t  GetHashCode() const
    {        
        size_t hashCode = GFxString::BernsteinHashFunction(FileName.ToCStr(), FileName.GetSize());
        return hashCode ^ Format;
    }
};


// General resource keys. Highlights:
//  - Wraps and holds key data, utilizing AddRef semantics.
//  - Can be copied by value.
class GFxResourceKey
{
public:

    enum KeyType
    {
        Key_None,
        Key_Unique,
        Key_File,       // hdata = GFxResourceFileInfo*
        Key_Gradient
    };

    typedef void*   KeyHandle;

    // KeyInterface provides functionality for key value / hash matching and
    // for data lifetime management. Separating interface and its data allows
    // for a more flexible storage of the key, potentially as a part of a resource.
    class KeyInterface
    {
    public:
        virtual ~KeyInterface() { }

        // Reference counting on the data object, if necessary.
        virtual void    AddRef(KeyHandle hdata)     = 0;
        virtual void    Release(KeyHandle hdata)    = 0;

        // Key/Hash code implementation.
        virtual KeyType GetKeyType(KeyHandle hdata) const                           = 0;
        virtual size_t  GetHashCode(KeyHandle hdata) const                          = 0;
        virtual bool    KeyEquals(KeyHandle hdata, const GFxResourceKey& other)     = 0;

        // Get file info about the resource, potentially available with Key_File.
        
        // MA: FileInfo can not be available here, since FileFormat, etc may
        // not yet be available when we create a key. In other words, creating a key
        // should not require information that can only be obatined by opening a file.
        //virtual const GFxResourceFileInfo* GetFileInfo(KeyHandle) const    { return 0; }

        virtual const char* GetFileURL(KeyHandle) const    { return 0; }
    };

protected:
    // Key data.
    KeyInterface* pKeyInterface;
    KeyHandle     hKeyData;

public:

    GFxResourceKey()
    {
        pKeyInterface = 0;
        hKeyData      = 0;
    }

    GFxResourceKey(KeyInterface* pki, KeyHandle hk)
    {
        if (pki)
            pki->AddRef(hk);
        pKeyInterface = pki;
        hKeyData      = hk;
    }

    GFxResourceKey(const GFxResourceKey &src)
    {
        if (src.pKeyInterface)
            src.pKeyInterface->AddRef(src.hKeyData);

        pKeyInterface = src.pKeyInterface;
        hKeyData      = src.hKeyData;
    }

    GFxResourceKey& operator = (const GFxResourceKey &src)
    {
        if (src.pKeyInterface)
            src.pKeyInterface->AddRef(src.hKeyData);
        if (pKeyInterface)
            pKeyInterface->Release(hKeyData);
        pKeyInterface = src.pKeyInterface;
        hKeyData      = src.hKeyData;
        return *this;
    }

    ~GFxResourceKey()
    {
        if (pKeyInterface)
            pKeyInterface->Release(hKeyData);
    }

    KeyType GetKeyType() const
    {
        return pKeyInterface ? pKeyInterface->GetKeyType(hKeyData) : Key_None;
    }

    /*
    // TBD: Get file for Key_File only.
    const GFxResourceFileInfo* GetFileInfo() const
    {
        return pKeyInterface ? pKeyInterface->GetFileInfo(hKeyData) : 0;
    }
    */

    const char* GetFileURL() const
    {
        return pKeyInterface ? pKeyInterface->GetFileURL(hKeyData) : 0;
    }


    // Equality Comparison: compares keys using KeyEquals.
    // Since both keys can be implemented bu different interfaces,
    // implementation is responsible for performing a check. It 
    // can do so based on two criteria:
    //    1) Interface pointer comparison (requires a singleton interface object).
    //    2) Doing KeyType comparison to a known type through pother->GetKeyType().
    bool operator == (const GFxResourceKey& other) const
    {
        if (pKeyInterface && other.pKeyInterface)
            // A check such as below above is the responsibility
            // of the KeyInterface implementor.
            // && pKeyInterface->GetKeyType() == other.pKeyInterface->GetKeyType()
            return pKeyInterface->KeyEquals(hKeyData, other);

        return 0;
    }


    // Accessors, useful for KeyInterface implementation.
    KeyInterface* GetKeyInterface() const   { return pKeyInterface; }
    KeyHandle     GetKeyData() const        { return hKeyData; }

    // Hash function used when matching keys.
    class HashOp
    {
    public:
        size_t operator () (const GFxResourceKey& key)
        {
            return key.pKeyInterface ? key.pKeyInterface->GetHashCode(key.hKeyData) : 0;
        }
    };
};



// ***** GFxResource

// Resource library managing a certain type of resource
class GFxResourceLibBase : public GRefCountBase<GFxResourceLibBase>
{
public:
    virtual void RemoveResourceOnRelease(GFxResource *pres) = 0;

    // Pin/Unpin.
    virtual void PinResource(GFxResource* pres)     = 0;
    virtual void UnpinResource(GFxResource* pres)   = 0;
};


// Resources can live in the resource library and have following characteristics:
//  - Thread-Safe RefCounting
//  - Back-link to update library on death, i.e. when refCount == 0
//  - Locking on library update
//  - Pin/Unpin methods that allow users to pin them with a ref in strong library.
//  - A unique key that can be used to locate/match them.

class GFxResource : public GNewOverrideBase
{
    friend class GFxResourceWeakLib;    

private:
    GAtomicInt<SInt32>  RefCount;
    // Library to which we belong, if any.
    GFxResourceLibBase* pLib;

public:

    // Thread-Safe ref-count implementation.
    void        AddRef();  
    void        Release();
    bool        AddRef_NotZero();
    SInt32      GetRefCount() const;
    

    GFxResource()
    {
        RefCount.Value = 1; // Non-Atomic write ok on construction. 
        pLib           = 0;
    }
    virtual ~GFxResource()
    {
    }

    // Pin/Unpin delegate to the owner library, if any.
    void    PinResource()       { if (pLib) pLib->PinResource(this); }
    void    UnpinResource()     { if (pLib) pLib->UnpinResource(this);  }



    // Assigns owner library for resource.
    void        SetOwnerResourceLib(GFxResourceLibBase *plib)
    {   // Can not reassign the library of the resource.
        GASSERT((pLib == 0) || (plib == 0));
        pLib = plib;
    }


    // *** Resource Type and Use

    enum ResourceType
    {
        // Set this bit if CharacterDef
        RT_CharacterDef_Bit = 0x0080,

        // Non-character types.
        RT_None             = 0,
        RT_Image,
        RT_Font,
        // Internal.
        RT_MovieDef,
        RT_SoundSample,
        
        // GFxCharacterDef derived types.
        RT_MovieDataDef     = RT_CharacterDef_Bit | 0,
        RT_ButtonDef,
        RT_TextDef,
        RT_EditTextDef,
        RT_SpriteDef,
        RT_ShapeDef,

        // Mask and shift applied to store ResourceType in type code.
        RT_TypeCode_Mask    = 0xFF00,
        RT_TypeCode_Shift   = 8
    };

    // Note: Uses correspond to a type,
    // but they don't have to be treated differently.
    enum ResourceUse
    {
        Use_None        = 0,
        Use_Bitmap      = 1,
        Use_Gradient    = 2,
        Use_FontTexture = 3,
        // Use mask within TypeCode.
        Use_TypeCode_Mask = 0xFF,
    };

    
    // Get the resource key that can be used for library matching.
    virtual GFxResourceKey  GetKey()                        { GASSERT(0); return GFxResourceKey(); }
    // Query Resource type code, which is a combination of ResourceType and ResourceUse.
    virtual UInt            GetResourceTypeCode() const     { return MakeTypeCode(RT_None); }
    

    // Simple Resource Type and Use access.
    inline ResourceType    GetResourceType() const
    {
        return (ResourceType) 
            ((GetResourceTypeCode() & RT_TypeCode_Mask) >> RT_TypeCode_Shift);
    }
    inline ResourceUse    GetResourceUse() const
    {
        return (ResourceUse) (GetResourceTypeCode() & Use_TypeCode_Mask);
    }

    static inline UInt      MakeTypeCode(ResourceType rt, ResourceUse ru = Use_None)
    {
        return (rt << RT_TypeCode_Shift) | ru;
    }
};




// ***** GFxResourceLib implementation.

/*  GFxResourceLib Architecture

    GFxResourceLib is a thread-safe library used for sharing resources based on
    their keys. Different types of resources can exist in the library simultaneously,
    including both user-created external resources as well as resources created
    internally by the flash file. If two GFxLoader objects have the same library,
    they will be able to share resources.

    
    Strong and Weak Libraries    

    To facilitate automated sharing of resources without reference counting problems 
    the library is separated into a strong and weak library classes, as follows:
        - GFxResourceLib is a strong library, owned by loader
        - GFxResourceWeakLib is a weak library, owned by GFxResourceLib and also
          owned/shared by the movies. GFxResourceWeakLib can exist without the strong
          lib, but not the other way around.

    Any sharable resource loaded into memory always exists in a weak library; however,
    the weak library does not AddRef to the resource. If the resource is released because
    it reference count is decremented, it will also update GFxResourceWeakLib in a thread
    safe manner. Movie resources will get released if the user releases all of their
    smart pointer references to the movie instances and definitions. No extra work needs
    to be done if the developer is content with automated sharing.
    
    If the developer would like the resource to remain alive, they can do so by either
    keeping a smart pointer to it or 'pinning' it in memory. Pinning a resource creates
    a smart pointer to it in the strong library; it is achieved by calling PinResource().
    Once a resource is pinned it will remain alive and sharable until a strong library
    itself is released (this normally happens when GFxLoader is destroyed).


    Adding Resources to Library

    Since GFxResourceLib is meant to be thread safe, it synchronizes access to its
    data structures. However, simple data locking is not enough to ensure that
    loading threads cooperate. Consider a traditional approach for creation and 
    library sharing of resources, which may include the following algorithm:

        presource = Library.Get(resourceKey);
        if (presource == NULL)
        {
            presource = LoadResource(resourceKey);
            Library.Add(resourceKey, presource);
        }

    This logic can malfunction if several threads try to load the resource at the
    same time. Specifically, if thread B attempts to load a resource while thread A is
    already in the process of doing so, the logic above will end up loading it twice,
    wasting both memory and processing resources.

    To avoid this problem GFxResourceLib the makes use of bind handles. The actual
    insertion of a resource therefore relies on the following logic:

        GFxResourceLib::BindHandle bh;        

        if (Library.BindResourceKey(&bh, resourceKey) == GFxResourceLib::RS_NeedsResolve)
        {
            presource = *LoadResource(resourceKey);
            if (presource)
                bh.ResolveResource(presource);
            else
            {
                bh.CancelResolve("error message);
                HandleError();
            }
        }
        else
        {
            presource = *bh.WaitForResolve();
            if (!presource)            
                HandleError();
        }
    
    Here, the initial thread requesting the resource creates a bind handle and
    proceeds to resolve it if the bind status is RS_NeedsResolve. Once finished
    with loading, it resolves it with ResolveResource (or cancels loading with
    CancelResolve). Overall, BindResourceKey and ResolveResource calls are similar
    to the Get and Add calls in the earlier example. There major difference,
    however, occurs if a second thread comes in to load the resource while the
    first one is in the process of doing so. In this case the bind status returned
    by the BindResourceKey function will be RS_WaitingResolve, allowing the second
    thread to wait for resource loading completion. Once the first thread is done
    loading and resolves the resource, WaitForResolve will return the resource
    with its content appropriately shared.

*/


// Strong resource library
//  - Contains an array of strong pointers.
//  - Delegates to GFxResourceWeakLib for all lookup purposes.
//  - GFxResourceWeakLib always 'lives longer'.

class GFxResourceLib : public GRefCountBase<GFxResourceLib>
{
    friend class GFxResourceWeakLib;    
    
    struct GFxResourcePtrHashFunc
    {    
        typedef GFxResource* C;
        // Hash code is computed based on a pointer.
        size_t  operator() (const C& data) const                     { return (((size_t)data) >> 6) ^ ((size_t)data); }
        // Hash update and value access.
        static size_t   get_cached_hash(const C& data)               { return (((size_t)data) >> 6) ^ ((size_t)data); }
        static void     set_cached_hash(C& data, size_t hashValue)   { GUNUSED2(data, hashValue); }
        static C&       get_value(C& data)                           { return data; }
        static const C& get_value(const C& data)                     { return data; }
    };

    typedef GTL::ghash_set_uncached<GFxResource*, GFxResourcePtrHashFunc> PinHashSet;

    // Strong pointer to the WeakLib.
    GFxResourceWeakLib*      pWeakLib;
    // 'Pin' hash. Every object is only pinned once. AddRef'ed manually.
    // Updates performed only during weak library lock.
    PinHashSet               PinSet;

public:

    GFxResourceLib();
    ~GFxResourceLib();

    enum ResolveState
    {
        RS_Unbound,
        RS_Available,
        RS_WaitingResolve,
        RS_NeedsResolve,
        RS_Error
    };

    
   // GFxResource Slots: Allocated in a separate list during a lock
    // 

    class ResourceSlot : public GRefCountBase<ResourceSlot>
    {
    public:
        enum ResolveState
        {
            Resolve_InProgress,
            Resolve_Success,
            Resolve_Fail
        };

    private:
        GPtr<GFxResourceWeakLib> pLib;
        volatile ResolveState   State;
        GFxResource* volatile   pResource;
        GFxResourceKey          Key;
        GFxString               ErrorMessage;

#ifndef GFC_NO_THREADSUPPORT
        GEvent                  ResolveComplete;
#endif        

    public:

        ResourceSlot(GFxResourceWeakLib* plib, const GFxResourceKey& key);
        ~ResourceSlot();
        
        const GFxResourceKey& GetKey() const { return Key; }

        // *** Interface for Waiter
        // If we are not responsible, wait to receive resource.
        // The returned resource will be AddRefed.
        GFxResource*    WaitForResolve();
        // Check availability.
        bool            IsResolved() const;
        const char*     GetError() const;
                
        // *** Interface for resolver
        // If we are responsible call one of these two:
        void            Resolve(GFxResource* pres);       
        void            CancelResolve(const char* perrorMessage);
    };


    class BindHandle
    {
    public:

        ResolveState    State;
        union
        {
            GFxResource*    pResource;  // Valid only if State == RS_Available
            ResourceSlot*   pSlot;      // Valid only if State >= RS_WaitingResolve
        };

        BindHandle()
        {
            State       = RS_Unbound;
            pResource   = 0;
        }

        ~BindHandle()
        {
            if (State == RS_Available)
                pResource->Release();
            else if (State >= RS_WaitingResolve)
                pSlot->Release();
        }

        // Clears the bind handle so that it can be reused.
        void                    Clear()
        {
            if (State == RS_Available)
                pResource->Release();
            else if (State >= RS_WaitingResolve)
            {
                /*
                // If this hits, that means we didn't close the handle properly by calling 
                // ResolveResource or CancelResolve. Doing so is an error, since it could
                // leave other threads waiting indefinitely.
                GASSERT(State != RS_NeedsResolve);
                */

                pSlot->Release();
            }
            // Reset values.
            State       = RS_Unbound;
            pResource   = 0;
        }


        // Obtain state; based on it you can call different functions.
        inline  ResolveState    GetState() const        { return State; }
        inline  bool            IsAvailable() const     { return State == RS_Available; }

        // Determine if we need resolve. If so,
        // we can all either Resolve() or CancelResolve().        
        // Note that release of all bind handles is treated as cancel.
        // - problem: what happens to other bind handles? They report error. 
        inline  bool            NeedsResolve() const    { return State == RS_NeedsResolve; }

        // If we are not responsible for resolving, wait to receive resource.
        // The result returned from WaitToResolve will be AddRef'ed, so the
        // user is responsible for releasing it.
        GFxResource*            WaitForResolve();

        // Get resource pointer; only if GetState() == RS_Available.
        inline  GFxResource*    GetResource() const
        {
            GASSERT(State == RS_Available);
            return pResource;
        }       

        // Get error in case WaitForResolve returned 0.
        inline const char*     GetResolveError() const
        {
            return (State >= RS_WaitingResolve) ? pSlot->GetError() : "";
        }

        // *** Interface for resolver

        // If we are responsible call one of these two:
        inline void    ResolveResource(GFxResource* pres)
        {
            GASSERT(State == RS_NeedsResolve);
            pSlot->Resolve(pres);
        }

        inline void    CancelResolve(const char* perrorMessage)
        {
            GASSERT(State == RS_NeedsResolve);
            pSlot->CancelResolve(perrorMessage);
        }
    };


    // Returns AddRefed GFxResourceWeakLib ptr.
    inline GFxResourceWeakLib*    GetWeakLib() const  { return pWeakLib; }

    // Resource access - Delegates to WeakLib.
    inline GFxResource*     GetResource(const GFxResourceKey &k);    
    inline ResolveState     BindResourceKey(BindHandle *phandle, const GFxResourceKey &k);

    inline void             UnpinAll();
};


class GFxResourceWeakLib : public GFxResourceLibBase
{
    friend class GFxResourceLib;
    friend class GFxResourceLib::ResourceSlot;

    typedef GFxResourceLib::ResourceSlot    ResourceSlot;    

    // Node that needs to be stored in case resource has not been resolved yet.
    struct ResourceNode
    {
        enum NodeType
        {
            Node_Resource,
            Node_Resolver
        };

        NodeType        Type;

        union
        {
            ResourceSlot*   pResolver;
            GFxResource*    pResource;
        };

        bool    IsResource() const { return Type == Node_Resource; }

        // Comparison to key, required for hashtable.
        bool    operator == (const GFxResourceKey& k) const
        {
            GASSERT(pResource != 0);
            if (Type == Node_Resource)
                return pResource->GetKey() == k;
            return pResolver->GetKey() == k;
        }
        bool    operator != (const GFxResourceKey& k) const
        {
            return operator != (k);
        }

        // Hash function used for nodes.
        struct HashOp
        {                
            // Hash code is computed based on a pointer.
            size_t  operator() (const ResourceNode& node) const
            {
                GASSERT(node.pResource != 0);
                GFxResourceKey k = (node.Type == Node_Resource) ?
                                    node.pResource->GetKey() : node.pResolver->GetKey();
                return GFxResourceKey::HashOp()(k);                
            }

            size_t  operator() (const GFxResourceKey& k) const
            {
                return GFxResourceKey::HashOp()(k);
            }
        };
    };

    // Weak pointer to the strong library container.
    GFxResourceLib *                pStrongLib;
    GLock                           ResourceLock;
    // Collection of weak references to resources.
    GTL::ghash_set<ResourceNode, ResourceNode::HashOp>    Resources;

public:

    GFxResourceWeakLib(GFxResourceLib *pstrongLib);
    ~GFxResourceWeakLib();

    typedef GFxResourceLib::BindHandle      BindHandle;
    typedef GFxResourceLib::ResolveState    ResolveState;    
    
    // Lookup resource, but only if resolved.
    // All resources are considered AddRefed.
    GFxResource*    GetResource(const GFxResourceKey &k);    
    // Lookup resource and insert its slot. We get back a BindHandle.    
    ResolveState    BindResourceKey(BindHandle *phandle, const GFxResourceKey &k);

    // Pin-management: for strong links.
    // Returns 1 if resource is pinned in strong library.
    bool            IsPinned(GFxResource* pres);
    void            PinResource(GFxResource* pres);
    void            UnpinResource(GFxResource* pres);
    void            UnpinAll();    

    // Virtual function called when resource RefCount has reached 0
    // and the resource is about to die.
    virtual void    RemoveResourceOnRelease(GFxResource *pres);    

    inline GFxResourceLib*  GetResourceLib() const { return pStrongLib; }
};

// GFxResourceLib inlines.
inline GFxResource* GFxResourceLib::GetResource(const GFxResourceKey &k)
{
    return pWeakLib->GetResource(k);
}
inline GFxResourceLib::ResolveState GFxResourceLib::BindResourceKey(
                GFxResourceLib::BindHandle *phandle, const GFxResourceKey &k)
{
    return pWeakLib->BindResourceKey(phandle, k);
}
inline void GFxResourceLib::UnpinAll()
{
    return pWeakLib->UnpinAll();
}        


#endif

