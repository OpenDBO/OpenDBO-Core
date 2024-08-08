/**********************************************************************

Filename    :   GRendererOGL.h
Content     :   Vector graphics 2D renderer implementation for 
Created     :   June 29, 2005
Authors     :   

Notes       :   
History     :   

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/


#ifndef INC_GRENDEREROGL_H
#define INC_GRENDEREROGL_H

#include "GTypes.h"
#include "GRefCount.h"

#include "GRenderer.h"
#include "GRendererCommonImpl.h"


class GTextureOGL : public GTextureImplNode
{
public:
    GTextureOGL (GRendererNode *plistRoot) : GTextureImplNode(plistRoot) { }

    virtual bool    InitTexture(GImageBase* pim, int targetWidth = 0, int targetHeight = 0)   = 0; 
    virtual bool    InitTexture(UInt texID, SInt width, SInt height, bool deleteTexture = 1)  = 0;
    virtual bool    InitTexture(int width, int height, GImage::ImageFormat format, int mipmaps,
                                int targetWidth , int targetHeight)                           = 0;
    virtual UInt    GetGLTexture() const                                                      = 0;
};


// Base class for OpenGL Renderer implementation.
// This class will be replaced with a new shape-centric version in the near future.

class GRendererOGL : public GRenderer
{
public:

    // Creates a new renderer object
    static GRendererOGL* GSTDCALL CreateRenderer();

    // Returns created objects with a refCount of 1, must be user-released.
    virtual GTextureOGL* CreateTexture()                                                                = 0;    


    // *** Implement Dependent Video Mode configuration
    
    // Call SetDependentVideoMode with the desired gl context (HGLRC, GLXContext, etc.) current.
    // This context must be current for all further calls to this GRendererOGL instance.
    // If the context is deleted or reset externally, a new GRendererOGL instance must be created.
    virtual bool                SetDependentVideoMode()                                                 = 0;
                                        

    // No effect - Video mode is managed by the application
    virtual bool                ResetVideoMode()                                                        = 0;

    
    // Query display status
    enum DisplayStatus
    {
        DisplayStatus_Ok            = 0,
        DisplayStatus_NoModeSet     = 1,    // Video mode 
        DisplayStatus_Unavailable   = 2,    // Display is unavailable for rendering; check status again later
        DisplayStatus_NeedsReset    = 3     // May be returned in Dependent mode to indicate external action being required
    };

    virtual DisplayStatus       CheckDisplayStatus() const                                              = 0;


};
    


#endif 
