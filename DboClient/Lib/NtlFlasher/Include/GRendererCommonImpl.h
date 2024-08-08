/**********************************************************************

Filename    :   GRendererCommonImpl.h
Content     :   Renderer classes shared by all back ends
Created     :   June 29, 2005
Authors     :   Michael Antonov

Notes       :   
History     :   

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GRENDERERCOMMONIMPL_H
#define INC_GRENDERERCOMMONIMPL_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GRenderer.h"


// ***** Declared Classes
class GRendererNode;
class GTextureImplNode;
class CacheNode;


// Define this macro to enable CacheProvider testing/sample logic.
// CacheProvider allows for allocation of renderer-specific data structures
// that will be associated with DrawPrimitive() data inside of the renderer.
//#define   GFC_RENDERER_TEST_CACHEDATA


// ***** GRendererNode Implementation

// A simple doubly-linked list node, used to track textures and other resources.
class GRendererNode
{
public:
    union   {
        GRendererNode *pPrev;       
        GRendererNode *pLast;
    };
    union   {
        GRendererNode *pNext;
        GRendererNode *pFirst;      
    };
    
    // Default constructor, creates root node (empty list).
    GRendererNode()
        { pNext = pPrev = this; }
    // Inserting constructor, inserts this node at the head of the list.
    GRendererNode(GRendererNode *proot)
    {
        pNext = proot->pFirst;
        pPrev = proot;
        proot->pFirst->pPrev = this;
        proot->pFirst = this;
    }
    
    // Remove
    void    RemoveNode()
    {
        pPrev->pNext = pNext;
        pNext->pPrev = pPrev;
        // Null our pointers
        pNext = pPrev = 0;
    }
};


// ***** GTextureImplNode

// Base class for texture implementations, provides support for 
// adding/removing handlers and tracking textures in a list.

class GTextureImplNode : public GTexture, public GRendererNode
{
public:
    // User data handle.
    Handle              UserHandle;
    // Change handlers installed, if any.
    bool                HandlerArrayFlag;
    union {
        ChangeHandler*                  pHandler;
        GTL::garray<ChangeHandler*> *   pHandlerArray;  
    };

    // List-inserting constructor.
    GTextureImplNode(GRendererNode *plistRoot);
    ~GTextureImplNode();

    // User handle access.
    virtual Handle      GetUserData() const         { return UserHandle; }
    virtual void        SetUserData(Handle hdata)   { UserHandle = hdata; }
    
    // Add/Remove notification
    virtual void        AddChangeHandler(ChangeHandler *phandler);
    virtual void        RemoveChangeHandler(ChangeHandler *phandler);
    // Routine to call handlers
    void                CallHandlers(ChangeHandler::EventType event);
    bool                CallRecreate();
};


class GArrayNode : public GRendererNode, public GNewOverrideBase
{
    GRenderer::CachedData*  pCache;
    const void*             pData;
    UInt                    Size;

public:
    GArrayNode () { pCache = 0; pData = 0; Size = 0; }
    GArrayNode(GRenderer* prenderer, GRendererNode* proot, const void* pdata, UInt size, GRenderer::CachedData* pcache);

    const void* GetData() const { return pData; }

    GArrayNode* AddNode(GRenderer* prenderer, GRenderer::CachedDataType type, const void* pdata, UInt size, GRenderer::CacheProvider* pcache);
    void        ReleaseData(GRenderer::CachedData* pcache);
    void        ReleaseRenderer();
};


class GRendererEventHandlerImpl
{
    bool                HandlerArrayFlag;
    union {
        GRenderer::EventHandler*                  pHandler;
        GTL::garray<GRenderer::EventHandler*> *   pHandlerArray;  
    };

public:
    GRendererEventHandlerImpl() { HandlerArrayFlag = 0; pHandler = 0; }
    ~GRendererEventHandlerImpl();

    bool        AddHandler(GRenderer::EventHandler *phandler);
    void        RemoveHandler(GRenderer::EventHandler *phandler);

    // Routine to call handlers
    void        CallHandlers(GRenderer *prenderer, GRenderer::EventHandler::EventType event);
};

// ***** CacheProvider Testing Implementation

#ifdef GFC_RENDERER_TEST_CACHEDATA

// CacheNode helps implement CacheProvider testing/sample logic, by
// representing a node associated with DrawPrimitive data. To support caching
// this node can hold pointers to static vertex buffers, etc.
class CacheNode : public GRendererNode, public GNewOverrideBase
{
public:
    enum BufferType
    {
        Buffer_None,
        Buffer_Vertex = GRenderer::Cached_Vertex,
        Buffer_Index = GRenderer::Cached_Index,
        Buffer_BitmapList = GRenderer::Cached_BitmapList
    };

private:
    // Store pointer to GFxPlayer-stored CacheData, we will need this
    // if we are ever going to release this node.
    GRenderer::CachedData*  pCacheData;
    // Describes the kind of buffer this cached data is for.
    BufferType              BuffType;
    
    // Values associated with a primitive.
    // These could be pointers to/offsets into static/dynamic buffers.
    // For our testing purposes we just verify data buffer matches out cached expectations.
    int                     Check1, Check2;

public:

    CacheNode() : GRendererNode()
    {
        pCacheData      = 0;
        BuffType        = Buffer_None;
        Check1 = Check2 = 0;
    }
    CacheNode(CacheNode *proot) : GRendererNode(proot)
    {
        pCacheData      = 0;
        BuffType        = Buffer_None;
        Check1 = Check2 = 0;
    }

    // Verifies functionality of the caching support subsystem by creating cached nodes and
    // matching cached canaries with incoming buffer data.
    void        VerifyCachedData(GRenderer *prenderer, GRenderer::CacheProvider *pcache, BufferType buffType,
                                 bool verifyFlag, int check1, int check2);  

    // Releases list, assuming that this is a Root node. Used by ~GRenderer.
    void        ReleaseList();
    
    // Implements release of cached data that was allocated from the cache provider.
    static void GSTDCALL ReleaseCachedData(GRenderer::CachedData *pdata, GRenderer::CachedDataType type);
};

#else

// CacheNode for when cache testing is disabled.
class CacheNode : public GNewOverrideBase
{   
public:
    enum BufferType
    {
        Buffer_None,
        Buffer_Vertex = GRenderer::Cached_Vertex,
        Buffer_Index = GRenderer::Cached_Index,
        Buffer_BitmapList = GRenderer::Cached_BitmapList
    };

    CacheNode() 
    { }
    CacheNode(CacheNode *proot) 
    { GUNUSED(proot); }

    // No verification - do nothing.
    void        VerifyCachedData(GRenderer *prenderer, GRenderer::CacheProvider *pcache, BufferType buffType,
                                 bool verifyFlag, int check1, int check2) 
    { GUNUSED3(prenderer,pcache,buffType); GUNUSED3(verifyFlag,check1,check2); }
    
    void        ReleaseList() 
    { }
    
    static void GSTDCALL ReleaseCachedData(GRenderer::CachedData *pdata, GRenderer::CachedDataType type) 
    { GUNUSED2(pdata, type); }
};

#endif // GFC_RENDERER_TEST_CACHEDATA

#ifdef GFC_ASSERT_ON_RENDERER_RESAMPLING
#define GASSERT_ON_RENDERER_RESAMPLING GASSERT(0)
#else
#define GASSERT_ON_RENDERER_RESAMPLING ((void)0)
#endif //GFC_ASSERT_ON_RENDERER_RESAMPLING

#ifdef GFC_ASSERT_ON_RENDERER_MIPMAP_GEN
#define GASSERT_ON_RENDERER_MIPMAP_GEN GASSERT(0)
#else
#define GASSERT_ON_RENDERER_MIPMAP_GEN ((void)0)
#endif //GFC_ASSERT_ON_RENDERER_MIPMAP_GEN

#endif 
