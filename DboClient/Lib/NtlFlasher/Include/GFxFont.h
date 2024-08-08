/**********************************************************************

Filename    :   GFxFont.h
Content     :   Abstract base class for system and built-in fonts.
Created     :   6/18/2007
Authors     :   Maxim Shemanarev, Michael Antonov

Copyright   :   (c) 2001-2007 Scaleform Corp. All Rights Reserved.

Notes       :   

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFxFont_H
#define INC_GFxFont_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GTypes2DF.h"
#include "GFxString.h"

class GFxTextureGlyphData;
class GFxShapeCharacterDef;
class GFxFontProvider;

class GFxFont : public GRefCountBase<GFxFont>
{
public:
    GFxFont(UInt fontFlags = 0)
        : Ascent(0), Descent(0), Leading(0), Flags(fontFlags) {}
    virtual ~GFxFont() {}


    enum FontFlags
    {
        // General font style flags.
        FF_Italic               = 0x0001,
        FF_Bold                 = 0x0002,
        FF_BoldItalic           = FF_Bold | FF_Italic,

        // These flag bits are used for matching a font. This mask is used
        // for both font searching and hashing.
        FF_Style_Mask          = FF_BoldItalic,

        // This flag is set if the font is being requested from a system device;
        // this can be set by the text field if it prefers system glyphs over
        // the SWF-embedded glyphs.
        FF_DeviceFont           = 0x0010,
        
        // Code page bits. Fonts supported right now are all Unicode,
        // but SWFs before version 6 allowed code pages.
        FF_CodePage_Mask        = 0x0300,
        FF_CodePage_Unicode     = 0x0000,
        FF_CodePage_Ansi        = 0x0100,
        FF_CodePage_ShiftJis    = 0x0200,

        // Mask passed to GFxFontProvider::CreateFont. 
        FF_CreateFont_Mask     = FF_Style_Mask | FF_CodePage_Mask,
       
        // Flash - specific attributes.
        FF_GlyphShapesStripped  = 0x1000,
        FF_HasLayout            = 0x2000,
        FF_WideCodes            = 0x8000
    };


    // *** Font attribute

    virtual GFxTextureGlyphData*    GetTextureGlyphData() const = 0;
    virtual int                     GetGlyphIndex(UInt16 code) = 0;
    virtual GFxShapeCharacterDef*   GetGlyphShape(UInt glyphIndex) = 0;
    virtual Float                   GetAdvance(UInt glyphIndex) const = 0;
    virtual Float                   GetKerningAdjustment(UInt lastCode, UInt thisCode) const = 0;
    virtual Float                   GetGlyphWidth(UInt glyphIndex) const = 0;
    virtual Float                   GetGlyphHeight(UInt glyphIndex) const = 0;
    virtual GRectF&                 GetGlyphBounds(UInt glyphIndex, GRectF* prect) const = 0;

    // Returns the number of glyph shapes, mostly used for font packing.
    // Can return 0 for system fonts, where the # of shapes is not known ahead of time.
    virtual UInt                    GetGlyphShapeCount() const { return 0; }

    inline  Float                   GetLeading() const  { return Leading; }
    inline  Float                   GetDescent() const  { return Descent; }
    inline  Float                   GetAscent()  const  { return Ascent;  }
    
    virtual const char*             GetName() const = 0;
    inline  UInt                    GetFontFlags() const { return Flags; }
    inline  UInt                    GetFontStyleFlags() const { return Flags & FF_Style_Mask; }
    inline  UInt                    GetCreateFontFlags() const { return Flags & FF_CreateFont_Mask; }
    // Flag query.
    inline  bool        IsItalic() const            { return (Flags & FF_Italic) != 0; } 
    inline  bool        IsBold() const              { return (Flags & FF_Bold) != 0; } 
    inline  bool        IsDeviceFont() const        { return (Flags & FF_DeviceFont) != 0; } 


    inline  FontFlags   GetCodePage() const         { return (FontFlags)(Flags & FF_CodePage_Mask); }
    inline  bool        AreUnicodeChars() const     { return GetCodePage() == FF_CodePage_Unicode; }    
    inline  bool        GlyphShapesStripped() const { return (Flags & FF_GlyphShapesStripped) != 0; }
    inline  bool        HasLayout() const           { return (Flags & FF_HasLayout) != 0; }    
    inline  bool        AreWideCodes() const        { return (Flags & FF_WideCodes) != 0; }


    // MatchFont includes a particular heuristic for search based on a font name, helpful
    // for handling Flash DeviceFont flag in text fields:
    //  - If FF_DeviceFont is NOT specified all searches are done based on FF_StyleFont_Mask,
    //    and both regular and device fonts can be returned.
    //  - If FF_DeviceFont IS specified, only device fonts will be matched.
    // The same rule applies to code page - it is considered only if specified as non-unicode.
    static bool         MatchFontFlags_Static(UInt fontFlags, UInt matchFlags)
    {   
        UInt matchMask= (matchFlags & FF_DeviceFont) | ((matchFlags & FF_CodePage_Mask) ? FF_CodePage_Mask : 0);
        UInt ourFlags = fontFlags & (FF_Style_Mask | matchMask);
        UInt flags    = matchFlags & (FF_Style_Mask | FF_DeviceFont | FF_CodePage_Mask);
        return (ourFlags == flags);
    }

    bool                MatchFontFlags(UInt matchFlags) const
    {   
        return MatchFontFlags_Static(GetFontFlags(), matchFlags);
    }

    bool                MatchFont(const char* name, UInt matchFlags) const
    {           
        return (MatchFontFlags(matchFlags) && !GFxString::CompareNoCase(GetName(), name));
    }       

    virtual bool        HasVectorOrRasterGlyphs() const { return true; }    

protected:

    void SetFontMetrics(Float leading, Float ascent, Float descent)
    {
        Leading = leading;
        Ascent  = ascent;
        Descent = descent;
    }

    void    SetItalic(bool v = true)        { (v) ? Flags |= FF_Italic : Flags &= ~FF_Italic; }
    void    SetBold(bool v = true)          { (v) ? Flags |= FF_Bold : Flags &= ~FF_Bold; }
    void    SetCodePage(FontFlags codePage) { Flags = (Flags & ~FF_CodePage_Mask) | (codePage & FF_CodePage_Mask); }
    void    SetGlyphShapesStripped()        { Flags |= FF_GlyphShapesStripped; }
    void    SetHasLayout(bool v = true)     { (v) ? Flags |= FF_HasLayout : Flags &= ~FF_HasLayout; }
    void    SetWideCodes(bool v = true)     { (v) ? Flags |= FF_WideCodes : Flags &= ~FF_WideCodes; }

    // Default sizes reported for non-existing glyph indices.
    static inline Float GetDefaultGlyphWidth() { return 512.0f; }
    static inline Float GetDefaultGlyphHeight() { return 1024.0f; }

    Float   Ascent;
    Float   Descent;
    Float   Leading;    
    UInt    Flags;   // Described by FontFlags enum.  
};


#endif

