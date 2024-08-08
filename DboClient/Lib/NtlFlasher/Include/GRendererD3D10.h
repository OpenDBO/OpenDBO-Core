/**********************************************************************

Filename    :   GRendererD3D10.h
Content     :   D3D10 Renderer
Created     :   Oct 1, 2007
Authors     :   

Notes       :   
History     :   

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/


#ifndef INC_GRENDERERD3D10_H
#define INC_GRENDERERD3D10_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GRenderer.h"
#include "GRendererCommonImpl.h"
#include <D3D10.h>


class GTextureD3D10 : public GTextureImplNode
{
public:
    GTextureD3D10 (GRendererNode *plistRoot) : GTextureImplNode(plistRoot) { }

    virtual bool    InitTexture(GImageBase* pim, int targetWidth = 0, int targetHeight = 0)          = 0; 
    virtual bool    InitTexture(ID3D10Texture2D *ptex, SInt width, SInt height)                      = 0;

    // does not addref
    virtual ID3D10Texture2D*  GetD3DTexture() const                                                  = 0;
};

// Base class for Direct3D Renderer implementation.
// This class will be replaced with a new shape-centric version in the near future.

class GRendererD3D10 : public GRenderer
{
public:

    // Creates a new renderer object
    static GRendererD3D10* GSTDCALL CreateRenderer();

    // Returns created objects with a refCount of 1, must be user-released.
    virtual GTextureD3D10* CreateTexture()                                                               = 0;    


    // *** Implement Dependent Video Mode configuration

    enum VMConfigFlags
    {
        VMConfig_CombinePrims = 0x00000002,
        VMConfig_Multisample  = 0x00000004,
    };

    // A new GRenderer instance must be created to use a different D3D device.
    virtual bool                SetDependentVideoMode(
        ID3D10Device*           pd3dDevice,
        UInt32                  vmConfigFlags = 0)                                                      = 0;

    // Returns back to original mode (cleanup)                                                      
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




    // *** Direct3D Access

    // Return various Direct3D related information
    virtual ID3D10Device*       GetDirect3DDevice() const                                               = 0;

};

#endif
