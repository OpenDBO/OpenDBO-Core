/**********************************************************************

Filename    :   GRendererD3D9Common.h
Content     :   D3D9 Common support
Created     :   Dec 8, 2006
Authors     :   Andrew Reisse

Notes       :   
History     :   

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/


#ifndef INC_GRENDERERD3D9COMMON_H
#define INC_GRENDERERD3D9COMMON_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GRenderer.h"
#include "GRendererCommonImpl.h"
#include <d3d9.h>


class GTextureD3D9 : public GTextureImplNode
{
public:
    GTextureD3D9 (GRendererNode *plistRoot) : GTextureImplNode(plistRoot) { }

    virtual bool    InitTexture(GImageBase* pim, int targetWidth = 0, int targetHeight = 0)          = 0; 
    virtual bool    InitTexture(IDirect3DTexture9 *ptex, SInt width, SInt height)                    = 0;

    // does not addref
    virtual IDirect3DTexture9*  GetD3DTexture() const                                                = 0;
};

#endif
