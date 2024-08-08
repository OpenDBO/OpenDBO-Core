/**********************************************************************

Filename    :   GFxFontProviderWin32.h
Content     :   Win32 API Font provider (GetGlyphOutline)
Created     :   6/21/2007
Authors     :   Maxim Shemanarev

Copyright   :   (c) 2001-2007 Scaleform Corp. All Rights Reserved.

Notes       :   

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

----------------------------------------------------------------------
The code of these classes was taken from the Anti-Grain Geometry
Project and modified for the use by Scaleform. 
Permission to use without restrictions is hereby granted to 
Scaleform Corporation by the author of Anti-Grain Geometry Project.
See http://antigtain.com for details.
**********************************************************************/

#ifndef INC_GFxFontProviderWin32_H
#define INC_GFxFontProviderWin32_H

#include <windows.h>
#include "GFxFont.h"
#include "GFxLoader.h"


//------------------------------------------------------------------------
struct GFxFontSysDataWin32
{
    enum { BufSizeInc = 1024-8 };

    GTL::garray<UByte> GlyphBuffer;
    HDC                WinHDC;

    GFxFontSysDataWin32(HDC dc) : WinHDC(dc) {}
};



//------------------------------------------------------------------------
class GFxShapeCache;
class GFxShapeCacheNode;
class GFxFontProviderWin32;

class GFxExternalFontWin32 : public GFxFont
{
    enum { FontHeight = 1024, ShapePageSize = 256-2 - 8-4 };
public:
    GFxExternalFontWin32(GFxFontProviderWin32 *pprovider,
                         GFxFontSysDataWin32* pSysData, 
                         const char* name, UInt fontFlags);
    virtual ~GFxExternalFontWin32();

    bool    IsValid() const { return WinHFont != 0; }
    
    virtual GFxTextureGlyphData*    GetTextureGlyphData() const { return 0; }

    virtual int                     GetGlyphIndex(UInt16 code);
    virtual GFxShapeCharacterDef*   GetGlyphShape(UInt glyphIndex);
    virtual Float                   GetAdvance(UInt glyphIndex) const;
    virtual Float                   GetKerningAdjustment(UInt lastCode, UInt thisCode) const;

    virtual Float                   GetGlyphWidth(UInt glyphIndex) const;
    virtual Float                   GetGlyphHeight(UInt glyphIndex) const;
    virtual GRectF&                 GetGlyphBounds(UInt glyphIndex, GRectF* prect) const;

    virtual const char*             GetName() const;

private:
    void    decomposeGlyphOutline(const UByte* data, UInt size, GFxShapeCharacterDef* shape);
    void    loadKerningPairs();

    struct GlyphType
    {
        UInt                Code;
        Float               Advance;
        GRectF              Bounds;
        GFxShapeCacheNode*  pShapeNode;
        UInt                Stamp;
    };

    struct KerningPairType
    {           
        UInt16  Char0, Char1;
        bool    operator==(const KerningPairType& k) const
            { return Char0 == k.Char0 && Char1 == k.Char1; }
    };

    // AddRef for font provider since it contains our cache.
    GPtr<GFxFontProviderWin32>          pFontProvider;    
    GFxFontSysDataWin32*                pSysData;
    GTL::garray<char>                   Name; 
    HFONT                               WinHFont;
    GTL::garray<GlyphType>              Glyphs;
    GTL::ghash_identity<UInt16, UInt>   CodeTable;
    GTL::ghash<KerningPairType, Float>  KerningPairs;
};



//------------------------------------------------------------------------
class GFxFontProviderWin32 : public GFxFontProvider
{
public:
    GFxFontProviderWin32(HDC dc, UInt maxNumShapes=4000);
    virtual ~GFxFontProviderWin32();
    
    virtual GFxFont*    CreateFont(const char* name, UInt fontFlags);    

    GFxShapeCache*      GetShapeCache() const { return pCache; }

private:
    GFxFontSysDataWin32                      SysData;
    GFxShapeCache*                           pCache;    
};



#endif
