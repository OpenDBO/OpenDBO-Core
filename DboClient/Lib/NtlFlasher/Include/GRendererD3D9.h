/**********************************************************************

Filename    :   GRendererD3D9.h
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


#ifndef INC_GRENDERERD3D9_H
#define INC_GRENDERERD3D9_H

#include "GRendererD3D9Common.h"


// Base class for Direct3D9 Renderer implementation.
// This class will be replaced with a new shape-centric version in the near future.

class GRendererD3D9 : public GRenderer
{
public:

    // Creates a new renderer object
    static GRendererD3D9* GSTDCALL CreateRenderer();

    // Returns created objects with a refCount of 1, must be user-released.
    virtual GTextureD3D9* CreateTexture()                                                               = 0;    
    

    // *** Implement Dependent Video Mode configuration

    enum VMConfigFlags
    {
        // Prevents BeginScene/EndScene from being called inside BeginDisplay/EndDisplay
        // - assumes that Direct3D is already in scene by the time BeginDisplay/EndDisplay are called
        // - allows user to manage their own begin/end scene calls 
        VMConfig_NoSceneCalls = 0x00000001,
        VMConfig_CombinePrims = 0x00000002,

        // Use D3DUSAGE_DYNAMIC updateable textures for compatibility with some broken drivers
        VMConfig_UseDynamicTex = 0x00000004,
    };

    virtual bool                SetDependentVideoMode(
                                        LPDIRECT3DDEVICE9 pd3dDevice,
                                        D3DPRESENT_PARAMETERS* ppresentParams,
                                        UInt32 vmConfigFlags = 0,
                                        HWND hwnd = 0)                                                  = 0;

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


    

    // *** Direct3D9 Access

    // Return various Dirext3D related information
    virtual LPDIRECT3D9         GetDirect3D() const                                                     = 0;
    virtual LPDIRECT3DDEVICE9   GetDirect3DDevice() const                                               = 0;
    virtual bool                GetDirect3DPresentParameters(D3DPRESENT_PARAMETERS* pparams) const      = 0;

};
    


#endif 
