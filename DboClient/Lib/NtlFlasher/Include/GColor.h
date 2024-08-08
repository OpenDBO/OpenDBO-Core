/**********************************************************************

Filename    :   GColor.h
Content     :   RGBA Color class
Created     :   March 26, 1999
Authors     :   Michael Antonov, Brendan Iribe

History     :   3/26/1999   :   Converted from previous version

Notes       :   This is a GFC Complete modified version of GColor.h,
                ColorCombine, IO and string support is removed.

Copyright   :   (c) 1999-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GCOLOR_H
#define INC_GCOLOR_H

#include "GTypes.h"
#include "GColorMacros.h"


// ***** Declared Classes
class   GColor;


// GFx has no need for complex color models by default
#define GFC_NO_COLOR_MODEL_CONVERSION
#define GFC_NO_COLOR_COMPLEX_CONVERSION

// The luminance vector values for red, green, and blue
// - to convert a colors into black and white, the balance 
//   coefficient of 0.3086, 0.6094, and 0.0820 is used. 
// - the standard NTSC weights of 0.299, 0.587, and 0.114 are
//   not used, as they are only applicable to RGB colors in a 
//   gamma 2.2 color space. 
#define GFC_COLOR_LUM_RED       0.3086
#define GFC_COLOR_LUM_GREEN     0.6094
#define GFC_COLOR_LUM_BLUE      0.0820
// Fixed point version must be divided by 32768
// - NTSC weights of 9798, 19235, 3735 are not used
#define GFC_COLOR_ILUM_RED      10112L
#define GFC_COLOR_ILUM_GREEN    19969L
#define GFC_COLOR_ILUM_BLUE     2687L


// ***** Color class

class GColor
{   
public:

    // Common RGB color values
    // * indicates the color name has a different value
    //   than the GColorValues version of the same color name
    enum StandardColors 
    {
        // Gray scale
        Black                   =GRGB(  0,  0,  0),     // 0x00000000
        White                   =GRGB(255,255,255),     // 0x00FFFFFF
        VeryLightGray           =GRGB(224,224,224),     // 0x00E0E0E0
        LightGray               =GRGB(192,192,192),     // 0x00C0C0C0   *
        Gray                    =GRGB(128,128,128),     // 0x00808080   *
        DarkGray                =GRGB( 64, 64, 64),     // 0x00404040   *
        VeryDarkGray            =GRGB( 32, 32, 32),     // 0x00202020
                                                            
        // RGB                                              
        Red                     =GRGB(255,  0,  0),     // 0x00FF0000
        LightRed                =GRGB(255,128,128),     // 0x00FF8080
        DarkRed                 =GRGB(128,  0,  0),     // 0x00800000   *
        VeryDarkRed             =GRGB( 64,  0,  0),     // 0x00400000
        Green                   =GRGB(  0,255,  0),     // 0x0000FF00
        LightGreen              =GRGB(128,255,128),     // 0x0080FF80   *
        DarkGreen               =GRGB(  0,128,  0),     // 0x00008000   *
        VeryDarkGreen           =GRGB(  0, 64,  0),     // 0x00004000
        Blue                    =GRGB(  0,  0,255),     // 0x000000FF
        LightBlue               =GRGB(128,128,255),     // 0x008080FF   *
        DarkBlue                =GRGB(  0,  0,128),     // 0x00000080   *
        VeryDarkBlue            =GRGB(  0,  0, 64),     // 0x00000040
        
        // CMY                  
        Cyan                    =GRGB(  0,255,255),     // 0x0000FFFF
        LightCyan               =GRGB(128,255,255),     // 0x0080FFFF   *
        DarkCyan                =GRGB(  0,128,128),     // 0x00008080   *
        Magenta                 =GRGB(255,  0,255),     // 0x00FF00FF
        LightMagenta            =GRGB(255,128,255),     // 0x00FF80FF
        DarkMagenta             =GRGB(128,  0,128),     // 0x00800080   *
        Yellow                  =GRGB(255,255,  0),     // 0x00FFFF00
        LightYellow             =GRGB(255,255,128),     // 0x00FFFF80   *
        DarkYellow              =GRGB(128,128,  0),     // 0x00808000
                                                            
        // Extended RGB color values                        
        Purple                  =GRGB(255,  0,255),     // 0x00FF00FF   *
        DarkPurple              =GRGB(128,  0,128),     // 0x00800080
        Pink                    =GRGB(255,192,192),     // 0x00FFC0C0   *
        DarkPink                =GRGB(192,128,128),     // 0x00C08080
        Beige                   =GRGB(255,192,128),     // 0x00FFC080   *
        LightBeige              =GRGB(255,224,192),     // 0x00FFE0C0
        DarkBeige               =GRGB(192,128, 64),     // 0x00C08040
        Orange                  =GRGB(255,128,  0),     // 0x0080FF00   *
        Brown                   =GRGB(128, 64,  0),     // 0x00804000   *
        LightBrown              =GRGB(192, 96,  0),     // 0x00C06000
        DarkBrown               =GRGB( 64, 32,  0),     // 0x00402000
    };
    
    // Common Alpha color values
    enum StandardAlphas 
    {
        Alpha0                  =GRGBA(0,0,0,  0),
        Alpha25                 =GRGBA(0,0,0, 64),
        Alpha50                 =GRGBA(0,0,0,127),
        Alpha75                 =GRGBA(0,0,0,191),
        Alpha100                =GRGBA(0,0,0,255),
    };

    // 24/32bit Color Structures
    struct Rgb24
    {
    #if GFC_BYTE_ORDER==GFC_LITTLE_ENDIAN
        UByte   Blue;
        UByte   Green;
        UByte   Red;
    #else // GFC_BYTE_ORDER=GFC_=BIG_ENDIAN
        UByte   Red;
        UByte   Green;
        UByte   Blue;
    #endif
    };

    struct Rgb32
    {
    #if GFC_BYTE_ORDER==GFC_LITTLE_ENDIAN
        UByte   Blue;
        UByte   Green;
        UByte   Red;
        UByte   Alpha;
    #else // GFC_BYTE_ORDER=GFC_=BIG_ENDIAN
        UByte   Alpha;
        UByte   Red;
        UByte   Green;
        UByte   Blue;
    #endif
    };

    // 32bit Color Data 
    union
    {
        Rgb32   Channels;
        UInt32  Raw;
    };
    
    // Constructors
    GINLINE GColor()                                                            { }
    GINLINE GColor(const GColor &c)                                             { SetColor(c); }
    GINLINE GColor(const GColor &c, UByte a8)                                   { SetColor(c, a8); }
    GINLINE GColor(const GColor &c, GColor ac)                                  { SetColor(c, ac); }
    GINLINE GColor(UInt32 raw32)                                                { SetColor(raw32); }        
    GINLINE GColor(UInt32 raw32, UByte a8)                                      { SetColor(raw32, a8); }        
    GINLINE GColor(UByte red, UByte green, UByte blue, UByte a8=0)              { SetColor(red, green, blue, a8); } 

    
    // Add formatting to support GFx debugging output
    // Buffer must be at least 32 characters in size
    GEXPORT void Format (char *pbuffer) const;

    
#ifndef GFC_NO_COLOR_MODEL_CONVERSION

    // Color model
    // - Xyz, Lab, Luv and Yiq all require floating point conversion
    enum Model  { Rgb, Hsv, Hsi
#ifndef GFC_NO_COLOR_COMPLEX_CONVERSION
        , Cmy, Xyz, Lab, Luv, Yiq, Yuv
#endif
        };
    
    // When initializing the color with integer values and 
    // the following models: Xyz, Lab, Luv or Yiq the RGB range is (0-100)
    // - the floating point methods should be used with those models
    // - the alpha value for integer methods is always (0-255)
    GEXPORT GColor(SInt x, SInt y, SInt z, Model model);
    GEXPORT GColor(SInt x, SInt y, SInt z, SInt a8, Model model);
    // All floating point values must be between (0.0-1.0)
    GEXPORT GColor(Double x, Double y, Double z, Model model);
    GEXPORT GColor(Double x, Double y, Double z, Double aD, Model model);

#endif // GFC_NO_COLOR_MODEL_CONVERSION

    
    // Initialize the color values

    GINLINE void            SetColor(const GColor &c)                           { Raw = c.Raw; }
    GINLINE void            SetColor(const GColor &c, UByte a8)                 { SetColor(c.Raw,a8); }
    GINLINE void            SetColor(const GColor &c, GColor ac)                { SetColor(c.Raw,ac.GetAlpha()); }
    GINLINE void            SetColor(UInt32 raw)                                { Raw = raw; }
    GINLINE void            SetColor(UInt32 raw, UByte a8)                      { Raw = raw; SetAlpha(a8); }
    GINLINE void            SetColor(UByte r, UByte g, UByte b, UByte a=0)      { SetRGBA(r, g, b, a); }

    GINLINE void            SetColorRGB  (const GColor &c)                      { Raw = UInt32((Raw&GFC_RGB32_ALPHA_MASK)|(c.Raw&GFC_RGB32_RGB_MASK)); }
    GINLINE void            SetColorAlpha(const GColor &c)                      { SetAlpha(c.GetAlpha()); }

    // Return colors
    GINLINE GColor          GetColorRGB     () const                { return GColor(Raw&GFC_RGB32_RGB_MASK);   }
    GINLINE GColor          GetColorAlpha   () const                { return GColor(Raw&GFC_RGB32_ALPHA_MASK); }
    GINLINE GColor          GetColorGray    () const;
    
    // *** Conversion
    GINLINE UInt16          ToColor12       () const                { UInt16 c = (UInt16)GFC_RGB32_TO_RGB12(Raw);  return c; }
    GINLINE UInt16          ToColor12A      () const                { UInt16 c = (UInt16)GFC_RGB32_TO_RGBA12(Raw);  return c; }
    GINLINE UInt16          ToColor15       () const                { UInt16 c = (UInt16)GFC_RGB32_TO_RGB15(Raw);  return c; }
    GINLINE UInt16          ToColor15A      () const                { UInt16 c = (UInt16)GFC_RGB32_TO_RGBA15(Raw); return c; }
    GINLINE UInt16          ToColor16       () const                { UInt16 c = (UInt16)GFC_RGB32_TO_RGB16(Raw);  return c; }
    GINLINE Rgb24           ToColor24       () const                { Rgb24  c; c.Red=GetRed(); c.Green=GetGreen(); c.Blue=GetBlue(); return c; }
    GINLINE UInt32          ToColor32       () const                { return Raw; }

    GINLINE static GColor   FromColor12     (const UInt16 c)        { return GColor((UInt32)GFC_RGB12_TO_RGB32(c),0); }
    GINLINE static GColor   FromColor12A    (const UInt16 c)        { return GColor((UInt32)GFC_RGB12_TO_RGB32(c)); }
    GINLINE static GColor   FromColor15     (const UInt16 c)        { return GColor((UInt32)GFC_RGB15_TO_RGB32(c),0); }
    GINLINE static GColor   FromColor15A    (const UInt16 c)        { return GColor((UInt32)GFC_RGB15_TO_RGB32(c)); }
    GINLINE static GColor   FromColor16     (const UInt16 c)        { return GColor((UInt32)GFC_RGB16_TO_RGB32(c)); }   
    GINLINE static GColor   FromColor24     (const Rgb24  c)        { return GColor(c.Red,c.Green,c.Blue); }
    GINLINE static GColor   FromColor32     (const UInt32 c)        { return GColor(c); }
                                                        
    GINLINE static GColor   FromRed         (UByte r)               { return GColor(r,0,0,0);   }
    GINLINE static GColor   FromGreen       (UByte g)               { return GColor(0,g,0,0);   }
    GINLINE static GColor   FromBlue        (UByte b)               { return GColor(0,0,b,0);   }
    GINLINE static GColor   FromAlpha       (UByte a)               { return GColor(0,0,0,a);   }

    // RGB Color
    GINLINE void            SetRGB (UByte r, UByte g, UByte b);
    GINLINE void            GetRGB (UByte *pr, UByte *pg, UByte *pb) const;
    GINLINE void            SetRGBA(UByte r, UByte g, UByte b, UByte a);                                
    GINLINE void            GetRGBA(UByte *pr, UByte *pg, UByte *pb, UByte *pa) const;

    // RGB Color - floating point
    GINLINE void            SetRGBFloat  (Float r, Float g, Float b);
    GINLINE void            GetRGBFloat  (Float *pr, Float *pg, Float *pb) const;
    GINLINE void            SetRGBAFloat (Float r, Float g, Float b, Float a);  
    GINLINE void            GetRGBAFloat (Float *pr, Float *pg, Float *pb, Float *pa) const;
    GINLINE void            SetAlphaFloat(Float a);
    GINLINE void            GetAlphaFloat(Float *pa) const;

    // Access RGB channels
    GINLINE UByte           GetAlpha() const        { return Channels.Alpha; }
    GINLINE UByte           GetRed  () const        { return Channels.Red;  }
    GINLINE UByte           GetGreen() const        { return Channels.Green; }
    GINLINE UByte           GetBlue () const        { return Channels.Blue; }
    // Initialize RGB channels
    GINLINE void            SetAlpha(UByte a)       { Channels.Alpha = a; }
    GINLINE void            SetRed  (UByte r)       { Channels.Red   = r; }
    GINLINE void            SetGreen(UByte g)       { Channels.Green = g; }
    GINLINE void            SetBlue (UByte b)       { Channels.Blue  = b; }

    // Grayscale 
    GINLINE UByte           GetGray () const;
    GINLINE void            SetGray (UByte g)       { SetRGB(g,g,g); }


    // *** Color operations
    // - Standard functions operate on both RGB and Alpha
    // - RGB functions only deal with the RGB channels and return null alpha value
    // - Alpha functions only deal with the Alpha channel and returns null RGB values
    
    // ** Binary ops
    
    // XOr - (c1 ^ c2)
    GINLINE static UByte    XOrCh               (UInt c1, UInt c2);
    GINLINE static GColor   XOr                 (GColor c1, GColor c2);
    GINLINE static GColor   XOrRGB              (GColor c1, GColor c2);
    GINLINE static GColor   XOrAlpha            (GColor c1, GColor c2);
    GINLINE GColor&         XOr                 (GColor c)                      { SetColor(XOr(*this, c)); return *this; }
    GINLINE GColor&         XOrRGB              (GColor c)                      { SetColor(XOrRGB(*this, c)); return *this; }   
    GINLINE GColor&         XOrAlpha            (GColor c)                      { SetColor(XOrAlpha(*this, c)); return *this; }
                                                                                
    // Or - (c1 | c2)                                                           
    GINLINE static UByte    OrCh                (UInt c1, UInt c2);
    GINLINE static GColor   Or                  (GColor c1, GColor c2);         
    GINLINE static GColor   OrRGB               (GColor c1, GColor c2);         
    GINLINE static GColor   OrAlpha             (GColor c1, GColor c2);         
    GINLINE GColor&         Or                  (GColor c)                      { SetColor(Or(*this, c)); return *this; }
    GINLINE GColor&         OrRGB               (GColor c)                      { SetColor(OrRGB(*this, c)); return *this; }    
    GINLINE GColor&         OrAlpha             (GColor c)                      { SetColor(OrAlpha(*this, c)); return *this; }
                                                                                
    // And - (c1 & c2)                          
    GINLINE static UByte    AndCh               (UInt c1, UInt c2);
    GINLINE static GColor   And                 (GColor c1, GColor c2);         
    GINLINE static GColor   AndRGB              (GColor c1, GColor c2);         
    GINLINE static GColor   AndAlpha            (GColor c1, GColor c2);         
    GINLINE GColor&         And                 (GColor c)                      { SetColor(And(*this, c)); return *this; }
    GINLINE GColor&         AndRGB              (GColor c)                      { SetColor(AndRGB(*this, c)); return *this; }   
    GINLINE GColor&         AndAlpha            (GColor c)                      { SetColor(AndAlpha(*this, c)); return *this; }
                                                                                
    // NotAnd - (~c1) & c2                      
    GINLINE static UByte    NotAndCh            (UInt c1, UInt c2);
    GINLINE static GColor   NotAnd              (GColor c1, GColor c2);         
    GINLINE static GColor   NotAndRGB           (GColor c1, GColor c2);         
    GINLINE static GColor   NotAndAlpha         (GColor c1, GColor c2);         
    GINLINE GColor&         NotAnd              (GColor c)                      { SetColor(NotAnd(*this, c)); return *this; }
    GINLINE GColor&         NotAndRGB           (GColor c)                      { SetColor(NotAndRGB(*this, c)); return *this; }    
    GINLINE GColor&         NotAndAlpha         (GColor c)                      { SetColor(NotAndAlpha(*this, c)); return *this; }
                                                                                
    // AndNot - c1 & (~c2)                      
    GINLINE static UByte    AndNotCh            (UInt c1, UInt c2);
    GINLINE static GColor   AndNot              (GColor c1, GColor c2);         
    GINLINE static GColor   AndNotRGB           (GColor c1, GColor c2);         
    GINLINE static GColor   AndNotAlpha         (GColor c1, GColor c2);         
    GINLINE GColor&         AndNot              (GColor c)                      { SetColor(AndNot(*this, c)); return *this; }
    GINLINE GColor&         AndNotRGB           (GColor c)                      { SetColor(AndNotRGB(*this, c)); return *this; }    
    GINLINE GColor&         AndNotAlpha         (GColor c)                      { SetColor(AndNotAlpha(*this, c)); return *this; }
                                                                                
                                                                                
    // ** Color blending ops                                                    
                                                                                
    // Modulate - (c1 * c2)
    // - equation: ((c1+1)*c2)>>8
    GINLINE static UByte    ModulateCh          (UInt c1, UInt c2);
    GINLINE static GColor   Modulate            (GColor c1, GColor c2);         
    GINLINE static GColor   ModulateRGB         (GColor c1, GColor c2);         
    GINLINE static GColor   ModulateAlpha       (GColor c1, GColor c2);         
    GINLINE GColor&         Modulate            (GColor c)                      { SetColor(Modulate(*this, c)); return *this; }
    GINLINE GColor&         ModulateRGB         (GColor c)                      { SetColor(ModulateRGB(*this, c)); return *this; }  
    GINLINE GColor&         ModulateAlpha       (GColor c)                      { SetColor(ModulateAlpha(*this, c)); return *this; }
                                                                            
    // ModulateFactor - (c1 * factor)
    // - equation: min(c1*(f+1), 255)
    GINLINE static UByte    ModulateFactorCh    (UInt c1,  UByte factor);
    GINLINE static GColor   ModulateFactor      (GColor c1, UByte factor);
    GINLINE static GColor   ModulateFactorRGB   (GColor c1, UByte factor);
    GINLINE static GColor   ModulateFactorAlpha (GColor c1, UByte factor);
    GINLINE GColor&         ModulateFactor      (UByte factor)                  { SetColor(ModulateFactor(*this, factor)); return *this; }
    GINLINE GColor&         ModulateFactorRGB   (UByte factor)                  { SetColor(ModulateFactorRGB(*this, factor)); return *this; }   
    GINLINE GColor&         ModulateFactorAlpha (UByte factor)                  { SetColor(ModulateFactorAlpha(*this, factor)); return *this; }
                                                                            
    // Add - (c1 + c2)
    // - equation: min(c1+c2, 255)
    GINLINE static UByte    AddCh               (UInt c1, UInt c2);
    GINLINE static GColor   Add                 (GColor c1, GColor c2);         
    GINLINE static GColor   AddRGB              (GColor c1, GColor c2);         
    GINLINE static GColor   AddAlpha            (GColor c1, GColor c2);         
    GINLINE GColor&         Add                 (GColor c)                      { SetColor(Add(*this, c)); return *this; }
    GINLINE GColor&         AddRGB              (GColor c)                      { SetColor(AddRGB(*this, c)); return *this; }   
    GINLINE GColor&         AddAlpha            (GColor c)                      { SetColor(AddAlpha(*this, c)); return *this; }
                                                                                
    // Add Smooth - (c1 + c2 * (1-c1))
    // - equation: min(c1+((c2*(256-c1))>>8), 255)
    GINLINE static UByte    AddSmoothCh         (UInt c1, UInt c2);
    GINLINE static GColor   AddSmooth           (GColor c1, GColor c2);         
    GINLINE static GColor   AddSmoothRGB        (GColor c1, GColor c2);         
    GINLINE static GColor   AddSmoothAlpha      (GColor c1, GColor c2);         
    GINLINE GColor&         AddSmooth           (GColor c)                      { SetColor(AddSmooth(*this, c)); return *this; }
    GINLINE GColor&         AddSmoothRGB        (GColor c)                      { SetColor(AddSmoothRGB(*this, c)); return *this; } 
    GINLINE GColor&         AddSmoothAlpha      (GColor c)                      { SetColor(AddSmoothAlpha(*this, c)); return *this; }
                                                                                
    // Add Signed - (c1 + c2 - 0.5) 
    // - equation: c=(c1+c2-128), min(255,max(c,0))
    GINLINE static UByte    AddSignedCh         (UInt c1, UInt c2); 
    GINLINE static GColor   AddSigned           (GColor c1, GColor c2);         
    GINLINE static GColor   AddSignedRGB        (GColor c1, GColor c2);         
    GINLINE static GColor   AddSignedAlpha      (GColor c1, GColor c2);         
    GINLINE GColor&         AddSigned           (GColor c)                      { SetColor(AddSigned(*this, c)); return *this; }
    GINLINE GColor&         AddSignedRGB        (GColor c)                      { SetColor(AddSignedRGB(*this, c)); return *this; } 
    GINLINE GColor&         AddSignedAlpha      (GColor c)                      { SetColor(AddSignedAlpha(*this, c)); return *this; }
                                                                                
    // Subtract - (c1 - c2)
    // - equation: max(c1-c2, 0)
    GINLINE static UByte    SubtractCh          (UInt c1, UInt c2);
    GINLINE static GColor   Subtract            (GColor c1, GColor c2);         
    GINLINE static GColor   SubtractRGB         (GColor c1, GColor c2);         
    GINLINE static GColor   SubtractAlpha       (GColor c1, GColor c2);         
    GINLINE GColor&         Subtract            (GColor c)                      { SetColor(Subtract(*this, c)); return *this; }
    GINLINE GColor&         SubtractRGB         (GColor c)                      { SetColor(SubtractRGB(*this, c)); return *this; }  
    GINLINE GColor&         SubtractAlpha       (GColor c)                      { SetColor(SubtractAlpha(*this, c)); return *this; }
                                                                                
    // Reverse Subtract - (c2 - c1)
    // - equation: max(c2-c1, 0)
    GINLINE static UByte    RevSubtractCh       (UInt c1, UInt c2);
    GINLINE static GColor   RevSubtract         (GColor c1, GColor c2);         
    GINLINE static GColor   RevSubtractRGB      (GColor c1, GColor c2);         
    GINLINE static GColor   RevSubtractAlpha    (GColor c1, GColor c2);         
    GINLINE GColor&         RevSubtract         (GColor c)                      { SetColor(RevSubtract(*this, c)); return *this; }
    GINLINE GColor&         RevSubtractRGB      (GColor c)                      { SetColor(RevSubtractRGB(*this, c)); return *this; }   
    GINLINE GColor&         RevSubtractAlpha    (GColor c)                      { SetColor(RevSubtractAlpha(*this, c)); return *this; }
                                                                            
    // Blend Source -  (c1 * (1-c2a) + c2 * c2a)
    // - equation: see Blend
    GINLINE static GColor   BlendSource         (GColor c1, GColor c2);         
    GINLINE static GColor   BlendSourceRGB      (GColor c1, GColor c2);         
    GINLINE static GColor   BlendSourceAlpha    (GColor c1, GColor c2);         
    GINLINE GColor&         BlendSource         (GColor c)                      { SetColor(BlendSource(*this, c)); return *this; }
    GINLINE GColor&         BlendSourceRGB      (GColor c)                      { SetColor(BlendSourceRGB(*this, c)); return *this; }   
    GINLINE GColor&         BlendSourceAlpha    (GColor c)                      { SetColor(BlendSourceAlpha(*this, c)); return *this; }
                                                                            
    // Blend Destination - (c1 * (1-c1a) + c2 * c1a)
    // - equation: see Blend
    GINLINE static GColor   BlendDest           (GColor c1, GColor c2);         
    GINLINE static GColor   BlendDestRGB        (GColor c1, GColor c2);         
    GINLINE static GColor   BlendDestAlpha      (GColor c1, GColor c2);         
    GINLINE GColor&         BlendDest           (GColor c)                      { SetColor(BlendDest(*this, c)); return *this; }
    GINLINE GColor&         BlendDestRGB        (GColor c)                      { SetColor(BlendDestRGB(*this, c)); return *this; } 
    GINLINE GColor&         BlendDestAlpha      (GColor c)                      { SetColor(BlendDestAlpha(*this, c)); return *this; }
                                                                                
    // Blend - (c2 * (1-f) + c1 * f)
    // - equation: (((c1+1)*f)>>8)+(((c2+1)*(255-f))>>8)
    // - treats the second parameter as dest
    GINLINE static UByte    BlendCh             (UInt c1,  UInt c2,  UByte factor);
    GINLINE static GColor   Blend               (GColor c1, GColor c2, UByte factor);
    GINLINE static GColor   BlendRGB            (GColor c1, GColor c2, UByte factor);
    GINLINE static GColor   BlendAlpha          (GColor c1, GColor c2, UByte factor);
    GINLINE GColor&         Blend               (GColor c, UByte factor)        { SetColor(Blend(*this, c, factor)); return *this; }
    GINLINE GColor&         BlendRGB            (GColor c, UByte factor)        { SetColor(BlendRGB(*this, c, factor)); return *this; } 
    GINLINE GColor&         BlendAlpha          (GColor c, UByte factor)        { SetColor(BlendAlpha(*this, c, factor)); return *this; }
    // Special blend version with floating-point factor
    GEXPORT static GColor GSTDCALL Blend        (GColor c1, GColor c2, Float factor);
    
    // Inverse (1-c)
    // - equation: (255-c)
    GINLINE static UByte    InverseCh           ( UInt c );
    GINLINE static GColor   Inverse             ( GColor c );
    GINLINE static GColor   InverseRGB          ( GColor c );
    GINLINE static GColor   InverseAlpha        ( GColor c );
    GINLINE GColor&         Inverse             (  )                            { SetColor(Inverse(*this)); return *this; }
    GINLINE GColor&         InverseRGB          (  )                            { SetColor(InverseRGB(*this)); return *this; }
    GINLINE GColor&         InverseAlpha        (  )                            { SetColor(InverseAlpha(*this)); return *this; }
    

    // *** Operator overrides

    // Equality
    GINLINE bool            operator == (GColor c) const        { return Raw == c.Raw; }
    GINLINE bool            operator != (GColor c) const        { return Raw != c.Raw; }


    GINLINE GColor          operator ~  () const                { return GColor(~Raw); }

    // Assignment
    GINLINE const GColor&   operator =  (const GColor &c)       { Raw = c.Raw; return *this; }

    // Binary operations
    GINLINE const GColor&   operator ^= (GColor c)              { return XOr(c); }
    GINLINE const GColor&   operator |= (GColor c)              { return Or (c); }
    GINLINE const GColor&   operator &= (GColor c)              { return And(c); }

    // Advanced operations                          
    GINLINE const GColor&   operator += (GColor c)              { return Add(c); }
    GINLINE const GColor&   operator -= (GColor c)              { return Subtract(c); }
    GINLINE const GColor&   operator *= (GColor c)              { return Modulate(c); }
    GINLINE const GColor&   operator *= (UByte factor)          { return Modulate(factor); }
    
    // Operator overrides
    // - call color operations
    GINLINE friend const GColor operator ^  (GColor c1, GColor c2);
    GINLINE friend const GColor operator |  (GColor c1, GColor c2);
    GINLINE friend const GColor operator &  (GColor c1, GColor c2);

    GINLINE friend const GColor operator +  (GColor c1, GColor c2);
    GINLINE friend const GColor operator -  (GColor c1, GColor c2);
    GINLINE friend const GColor operator *  (GColor c1, GColor c2);
    GINLINE friend const GColor operator *  (GColor c1, UByte factor);


    // ** Additional color information

    // Calculate the distance between the local color and the passed color
    GINLINE SInt            DistanceSquared(GColor c) const;
    
    // Calculate the intensity of the color
    GINLINE UInt            IIntensity() const;
    GINLINE Float           FIntensity() const;

    
    // ** HSV color conversions

#ifndef GFC_NO_COLOR_HSV_CONVERSION

    // HSV - Hue, Saturation and Value
    // SInt parameter ranges are h (0-360) s(0-255) v(0-255)
    // Float parameter ranges are all (0.0-1.0)
    GEXPORT void            SetHSV(SInt h, SInt s, SInt v);
    GEXPORT void            GetHSV(SInt *ph, SInt *ps, SInt *pv) const;
    GEXPORT void            SetHSV(Float h, Float s, Float v);
    GEXPORT void            GetHSV(Float *ph, Float *ps, Float *pv) const;
    // HSI - Hue, Saturation and Intensity
    // SInt parameter ranges are h (0-360) s(0-255) i(0-255)
    // Float parameter ranges are all (0.0-1.0)
    GEXPORT void            SetHSI(SInt h, SInt s, SInt i);
    GEXPORT void            GetHSI(SInt *ph, SInt *ps, SInt *pi) const;
    GEXPORT void            SetHSI(Float h, Float s, Float i);
    GEXPORT void            GetHSI(Float *ph, Float *ps, Float *pi) const;

    // Precise HSI - RGB conversion using trig functions
    GEXPORT static void GSTDCALL    ConvertHSIToRGB(Double h, Double s, Double i,
                                            Double *pr, Double *pg, Double *pb);
    GEXPORT static void GSTDCALL    ConvertRGBToHSI(Double r, Double g, Double b,
                                            Double *ph, Double *ps, Double *pi);

#endif // GFC_NO_COLOR_HSV_CONVERSION
    

    // ** Additional color conversions

#ifndef GFC_NO_COLOR_COMPLEX_CONVERSION

    // CMYK - Cyan, Magenta, Yellow and Black
    GEXPORT void            SetCMYK(SInt c, SInt m, SInt y, SInt k);
    GEXPORT void            GetCMYK(SInt *pc, SInt *pm, SInt *py, SInt *pk) const;
    // CMY - Cyan, Magenta, Yellow
    GEXPORT void            SetCMY(SInt c, SInt m, SInt y);
    GEXPORT void            GetCMY(SInt *pc, SInt *pm, SInt *py) const;

    // Reduces the C, M and Y components and boosts the K component 
    // to achieve the same color but with a higher black component
    GEXPORT static void GSTDCALL CorrectCMYK(SInt *pc, SInt *pm, SInt *py, SInt *pk);

    // YIQ
    GEXPORT void            SetYIQ(Float y, Float i, Float q);
    GEXPORT void            GetYIQ(Float *py, Float *pi, Float *pq) const;
    // YUV
    GEXPORT void            SetYUV(Float y, Float u, Float v);
    GEXPORT void            GetYUV(Float *py, Float *pu, Float *pv) const;

    // CIE XYZ
    GEXPORT void            SetXYZ(Float x, Float y, Float z);
    GEXPORT void            GetXYZ(Float *px, Float *py, Float *pz) const;
    // CIE Lab
    GEXPORT void            SetLab(Float l, Float a, Float b);
    GEXPORT void            GetLab(Float *pl, Float *pa, Float *pb) const;
    // CIE Luv
    GEXPORT void            SetLuv(Float l, Float u, Float v);
    GEXPORT void            GetLuv(Float *pl, Float *pu, Float *pv) const;

    // Lab - XYZ conversion
    GEXPORT static void GSTDCALL ConvertLabToXYZ(Double l, Double a, Double b,
                                            Double *px, Double *py, Double *pz);
    GEXPORT static void GSTDCALL ConvertXYZToLab(Double x, Double y, Double z,
                                            Double *pl, Double *pa, Double *pb);
    // Luv - XYZ conversion
    GEXPORT static void GSTDCALL ConvertLuvToXYZ(Double l, Double u, Double v,
                                            Double *px, Double *py, Double *pz);
    GEXPORT static void GSTDCALL ConvertXYZToLuv(Double x, Double y, Double z,
                                            Double *pl, Double *pu, Double *pv);

#endif  // GFC_NO_COLOR_COMPLEX_CONVERSION


};

// ** Inline Implementation

// RGB Color                
GINLINE void        GColor::SetRGB (UByte r, UByte g, UByte b)
{
    SetRed(r);
    SetGreen(g);
    SetBlue(b);
}

GINLINE void        GColor::SetRGBA(UByte r, UByte g, UByte b, UByte a)
{
    SetRGB(r, g, b);
    SetAlpha(a);
}
                        

GINLINE void        GColor::GetRGB (UByte *pr, UByte *pg, UByte *pb) const
{ 
    *pr = GetRed(); 
    *pg = GetGreen(); 
    *pb = GetBlue(); 
}

GINLINE void        GColor::GetRGBA(UByte *pr, UByte *pg, UByte *pb, UByte *pa) const
{ 
    GetRGB(pr, pg, pb);
    *pa = GetAlpha();
}

// RGB Color - floating point
GINLINE void        GColor::SetRGBAFloat(Float r, Float g, Float b, Float a)
{
    SetRGBFloat(r,g,b);
    SetAlphaFloat(a);
}

GINLINE void        GColor::SetRGBFloat (Float r, Float g, Float b)
{
    SetRed  ( UByte(r*255) );
    SetGreen( UByte(g*255) );
    SetBlue ( UByte(b*255) );
}

GINLINE void        GColor::SetAlphaFloat(Float a)
{
    SetAlpha( UByte(a*255) );
}

GINLINE void        GColor::GetRGBAFloat(Float *pr, Float *pg, Float *pb, Float *pa) const
{
    GetRGBFloat(pr, pg, pb);
    GetAlphaFloat(pa);
}

GINLINE void        GColor::GetRGBFloat (Float *pr, Float *pg, Float *pb) const
{
    *pr = GetRed()==0   ? 0 : GetRed()   / Float(255);
    *pg = GetGreen()==0 ? 0 : GetGreen() / Float(255);
    *pb = GetBlue()==0  ? 0 : GetBlue()  / Float(255);
}

GINLINE void        GColor::GetAlphaFloat(Float *pa) const
{
    *pa = GetAlpha()==0 ? 0 : GetAlpha() / Float(255);
}

// Grayscale 
GINLINE UByte       GColor::GetGray () const
{ 
    return UByte( (UInt(GetRed())*11 + UInt(GetGreen())*16 + UInt(GetBlue())*5)/32 ); 
}

GINLINE GColor      GColor::GetColorGray() const
{
    UByte gray = GetGray();
    return GColor(gray, gray, gray);
}

GINLINE UInt        GColor::IIntensity() const
{
    return (UInt)((GFC_COLOR_ILUM_RED*GetRed()+GFC_COLOR_ILUM_GREEN*GetGreen()+GFC_COLOR_ILUM_BLUE*GetBlue())/32768L);
}

GINLINE Float       GColor::FIntensity() const
{
    return (Float)(GFC_COLOR_LUM_RED*GetRed() + GFC_COLOR_LUM_GREEN*GetGreen() + GFC_COLOR_LUM_BLUE*GetBlue());
}

GINLINE SInt        GColor::DistanceSquared(GColor c) const
{
    return ( ((c.GetRed()   - SInt(GetRed()))   * (c.GetRed()   - SInt(GetRed())))   + 
             ((c.GetGreen() - SInt(GetGreen())) * (c.GetGreen() - SInt(GetGreen()))) + 
             ((c.GetBlue()  - SInt(GetBlue()))  * (c.GetBlue()  - SInt(GetBlue()))) );
}

// *** Binary ops

// XOr - (c1 ^ c2)
GINLINE UByte       GColor::XOrCh           (UInt c1, UInt c2)
    { return UByte( c1 ^ c2 ); }
GINLINE GColor      GColor::XOr             (GColor c1, GColor c2)
    { return GColor( c1.Raw ^ c2.Raw ); }
GINLINE GColor      GColor::XOrRGB          (GColor c1, GColor c2)
    { return GColor( c1.Raw ^ c2.Raw, c1.GetAlpha() ); }
GINLINE GColor      GColor::XOrAlpha        (GColor c1, GColor c2)
    { return GColor( c1, c1.GetAlpha() ^ c2.GetAlpha() ); }

// Or - (c1 | c2)
GINLINE UByte       GColor::OrCh            (UInt c1, UInt c2)
    { return UByte( c1 | c2 ); }
GINLINE GColor      GColor::Or              (GColor c1, GColor c2)
    { return GColor( c1.Raw | c2.Raw ); }
GINLINE GColor      GColor::OrRGB           (GColor c1, GColor c2)
    { return GColor( c1.Raw | c2.Raw, c1.GetAlpha() ); }
GINLINE GColor      GColor::OrAlpha         (GColor c1, GColor c2)
    { return GColor( c1, c1.GetAlpha() | c2.GetAlpha() ); }

// And - (c1 & c2)
GINLINE UByte       GColor::AndCh           (UInt c1, UInt c2)
    { return UByte( c1 & c2 ); }
GINLINE GColor      GColor::And             (GColor c1, GColor c2)
    { return GColor( c1.Raw & c2.Raw ); }
GINLINE GColor      GColor::AndRGB          (GColor c1, GColor c2)
    { return GColor( c1.Raw & c2.Raw, c1.GetAlpha() ); }
GINLINE GColor      GColor::AndAlpha        (GColor c1, GColor c2)
    { return GColor( c1, c1.GetAlpha() & c2.GetAlpha() ); }

// NotAnd - (~c1) & c2
GINLINE UByte       GColor::NotAndCh        (UInt c1, UInt c2)
    { return UByte( ~c1 & c2 ); }
GINLINE GColor      GColor::NotAnd          (GColor c1, GColor c2)
    { return GColor( (~c1.Raw) & c2.Raw ); }
GINLINE GColor      GColor::NotAndRGB       (GColor c1, GColor c2)
    { return GColor( (~c1.Raw) & c2.Raw, c1.GetAlpha() ); }
GINLINE GColor      GColor::NotAndAlpha     (GColor c1, GColor c2)
    { return GColor( c1, (~c1.GetAlpha()) & c2.GetAlpha() ); }

// AndNot - c1 & (~c2)
GINLINE UByte       GColor::AndNotCh        (UInt c1, UInt c2)
    { return UByte( c1 & ~c2 ); }
GINLINE GColor      GColor::AndNot          (GColor c1, GColor c2)
    { return GColor( c1.Raw & (~c2.Raw) ); }
GINLINE GColor      GColor::AndNotRGB       (GColor c1, GColor c2)
    { return GColor( c1.Raw & (~c2.Raw), c1.GetAlpha() ); }
GINLINE GColor      GColor::AndNotAlpha     (GColor c1, GColor c2)
    { return GColor( c1, c1.GetAlpha() & (~c2.GetAlpha()) ); }


// *** Color blending ops
 
// Modulate - (c1 * c2)
GINLINE UByte       GColor::ModulateCh      (UInt c1, UInt c2)
    { return UByte( ((UInt(c1)+1)*c2)>>8 ); }
GINLINE GColor      GColor::Modulate        (GColor c1, GColor c2)
{ 
    return GColor( ModulateCh(c1.GetRed(),      c2.GetRed()),
                   ModulateCh(c1.GetGreen(),    c2.GetGreen()),
                   ModulateCh(c1.GetBlue(),     c2.GetBlue()),
                   ModulateCh(c1.GetAlpha(),    c2.GetAlpha()) );
}

GINLINE GColor      GColor::ModulateRGB     (GColor c1, GColor c2)
{
    return GColor( ModulateCh(c1.GetRed(),      c2.GetRed()),
                   ModulateCh(c1.GetGreen(),    c2.GetGreen()),
                   ModulateCh(c1.GetBlue(),     c2.GetBlue()),
                   c1.GetAlpha() );
}

GINLINE GColor      GColor::ModulateAlpha   (GColor c1, GColor c2)
{
    return GColor( c1, ModulateCh(c1.GetAlpha(), c2.GetAlpha()) );
}

// Modulate - (c1 * factor)
GINLINE UByte       GColor::ModulateFactorCh        (UInt c1, UByte f)
{ 
    return UByte( GTL::gmin<UInt>(UInt(c1*(UInt(f)+1)), 255) ); 
}
GINLINE GColor      GColor::ModulateFactor          (GColor c1, UByte f)
{
    return GColor( ModulateFactorCh(c1.GetRed(),    f),
                   ModulateFactorCh(c1.GetGreen(),  f),
                   ModulateFactorCh(c1.GetBlue(),   f),
                   ModulateFactorCh(c1.GetAlpha(),  f)  );
}

GINLINE GColor      GColor::ModulateFactorRGB       (GColor c1, UByte f)
{
    return GColor( ModulateFactorCh(c1.GetRed(),    f),
                   ModulateFactorCh(c1.GetGreen(),  f),
                   ModulateFactorCh(c1.GetBlue(),   f),
                   c1.GetAlpha() );
}

GINLINE GColor      GColor::ModulateFactorAlpha     (GColor c1, UByte f)
{
    return GColor( c1, ModulateFactorCh(c1.GetAlpha(), f) );
}



// Add - (c1 + c2)
GINLINE UByte       GColor::AddCh           (UInt c1, UInt c2)
{ 
    return UByte( GTL::gmin<UInt>(UInt(c1)+c2, 255) ); 
}
GINLINE GColor      GColor::Add             (GColor c1, GColor c2)
{
    return GColor( AddCh(c1.GetRed(),   c2.GetRed()),
                   AddCh(c1.GetGreen(), c2.GetGreen()),
                   AddCh(c1.GetBlue(),  c2.GetBlue()),
                   AddCh(c1.GetAlpha(), c2.GetAlpha()) );
}

GINLINE GColor      GColor::AddRGB          (GColor c1, GColor c2)
{
    return GColor( AddCh(c1.GetRed(),   c2.GetRed()),
                   AddCh(c1.GetGreen(), c2.GetGreen()),
                   AddCh(c1.GetBlue(),  c2.GetBlue()),
                   c1.GetAlpha() );
}

GINLINE GColor      GColor::AddAlpha        (GColor c1, GColor c2)
{
    return GColor( c1, AddCh(c1.GetAlpha(), c2.GetAlpha()) );
}


// Add Smooth - (c1 + c2*(1-c1))
GINLINE UByte       GColor::AddSmoothCh     (UInt c1, UInt c2)
{ 
    return UByte( GTL::gmin(UInt(c1 + ((c2 * UInt(256-c1))>>8)), UInt(255)) ); 
}
GINLINE GColor      GColor::AddSmooth       (GColor c1, GColor c2)
{
    return GColor( AddSmoothCh(c1.GetRed(),     c2.GetRed()),
                   AddSmoothCh(c1.GetGreen(),   c2.GetGreen()),
                   AddSmoothCh(c1.GetBlue(),    c2.GetBlue()),
                   AddSmoothCh(c1.GetAlpha(),   c2.GetAlpha()) );
}

GINLINE GColor      GColor::AddSmoothRGB    (GColor c1, GColor c2)
{
    return GColor( AddSmoothCh(c1.GetRed(),     c2.GetRed()),
                   AddSmoothCh(c1.GetGreen(),   c2.GetGreen()),
                   AddSmoothCh(c1.GetBlue(),    c2.GetBlue()),
                   c1.GetAlpha() );
}
GINLINE GColor      GColor::AddSmoothAlpha  (GColor c1, GColor c2)
{
    return GColor( c1, AddSmoothCh(c1.GetAlpha(), c2.GetAlpha()) );
}

// Add Signed - (c1 + c2 - 0.5)
GINLINE UByte       GColor::AddSignedCh     (UInt c1, UInt c2)
{ 
    SInt c = c1 + SInt(c2) - 128;
    return UByte( c>255 ? 255 : (c<0 ? 0 : c) ); 
}
GINLINE GColor      GColor::AddSigned       (GColor c1, GColor c2)
{
    return GColor( AddSignedCh(c1.GetRed(),     c2.GetRed()),
                   AddSignedCh(c1.GetGreen(),   c2.GetGreen()),
                   AddSignedCh(c1.GetBlue(),    c2.GetBlue()),
                   AddSignedCh(c1.GetAlpha(),   c2.GetAlpha()) );
}

GINLINE GColor      GColor::AddSignedRGB    (GColor c1, GColor c2)
{
    return GColor( AddSignedCh(c1.GetRed(),     c2.GetRed()),
                   AddSignedCh(c1.GetGreen(),   c2.GetGreen()),
                   AddSignedCh(c1.GetBlue(),    c2.GetBlue()),
                   c1.GetAlpha() );
}

GINLINE GColor      GColor::AddSignedAlpha  (GColor c1, GColor c2)
{
    return GColor( c1, AddSignedCh(c1.GetAlpha(), c2.GetAlpha()) );
}


// Subtract - (c1 - c2)
GINLINE UByte       GColor::SubtractCh      (UInt c1, UInt c2)
{ 
    return UByte( GTL::gmax<SInt>(SInt(c1)-c2, 0) ); 
}
GINLINE GColor      GColor::Subtract        (GColor c1, GColor c2)
{
    return GColor( SubtractCh(c1.GetRed(),      c2.GetRed()),
                   SubtractCh(c1.GetGreen(),    c2.GetGreen()),
                   SubtractCh(c1.GetBlue(),     c2.GetBlue()),
                   SubtractCh(c1.GetAlpha(),    c2.GetAlpha()) );
}

GINLINE GColor      GColor::SubtractRGB     (GColor c1, GColor c2)
{
    return GColor( SubtractCh(c1.GetRed(),      c2.GetRed()),
                   SubtractCh(c1.GetGreen(),    c2.GetGreen()),
                   SubtractCh(c1.GetBlue(),     c2.GetBlue()),
                   c1.GetAlpha() );
}

GINLINE GColor      GColor::SubtractAlpha   (GColor c1, GColor c2)
{
    return GColor( c1, SubtractCh(c1.GetAlpha(),    c2.GetAlpha()) );
}

// Reverse Subtract - (c2 - c1)
GINLINE UByte       GColor::RevSubtractCh   (UInt c1, UInt c2)
{ 
    return UByte( GTL::gmax<SInt>(SInt(c2)-c1, 0) ); 
}
GINLINE GColor      GColor::RevSubtract     (GColor c1, GColor c2)
{
    return GColor( RevSubtractCh(c1.GetRed(),   c2.GetRed()),
                   RevSubtractCh(c1.GetGreen(), c2.GetGreen()),
                   RevSubtractCh(c1.GetBlue(),  c2.GetBlue()),
                   RevSubtractCh(c1.GetAlpha(), c2.GetAlpha()) );
}

GINLINE GColor      GColor::RevSubtractRGB  (GColor c1, GColor c2)
{
    return GColor( RevSubtractCh(c1.GetRed(),   c2.GetRed()),
                   RevSubtractCh(c1.GetGreen(), c2.GetGreen()),
                   RevSubtractCh(c1.GetBlue(),  c2.GetBlue()),
                   c1.GetAlpha() );
}

GINLINE GColor      GColor::RevSubtractAlpha(GColor c1, GColor c2)
{
    return GColor( c1, RevSubtractCh(c1.GetAlpha(), c2.GetAlpha()));
}


// Blend Source -    c1 * (1-c2a) + c2 * c2a                                
GINLINE GColor  GColor::BlendSource     (GColor c1, GColor c2)          
    { return Blend(c1, c2, c2.GetAlpha()); }

GINLINE GColor  GColor::BlendSourceRGB  (GColor c1, GColor c2)
    { return BlendRGB(c1, c2, c2.GetAlpha()); }

GINLINE GColor  GColor::BlendSourceAlpha(GColor c1, GColor c2)
    { return BlendAlpha(c1, c2, c2.GetAlpha()); }
                                                                        
// Blend Destination - c1 * (1-c1a) + c2 * c1a                          
GINLINE GColor  GColor::BlendDest       (GColor c1, GColor c2)      
    { return Blend(c1, c2, c1.GetAlpha()); }

GINLINE GColor  GColor::BlendDestRGB    (GColor c1, GColor c2)          
    { return BlendRGB(c1, c2, c1.GetAlpha()); }

GINLINE GColor  GColor::BlendDestAlpha  (GColor c1, GColor c2)          
    { return BlendAlpha(c1, c2, c1.GetAlpha()); }


// Blend 
GINLINE UByte       GColor::BlendCh     (UInt c1, UInt c2, UByte f)
{ 
    return UByte( (((UInt(c1)+1)*f)>>8) + (((UInt(c2)+1)*(255-f))>>8) ); 
}

GINLINE GColor      GColor::Blend       (GColor c1, GColor c2, UByte f)
{
    return GColor(  BlendCh(c1.GetRed(),    c2.GetRed(), f),
                    BlendCh(c1.GetGreen(),  c2.GetGreen(), f),
                    BlendCh(c1.GetBlue(),   c2.GetBlue(), f),
                    BlendCh(c1.GetAlpha(),  c2.GetAlpha(), f) );
}

GINLINE GColor      GColor::BlendRGB    (GColor c1, GColor c2, UByte f)
{
    return GColor(  BlendCh(c1.GetRed(),    c2.GetRed(), f),
                    BlendCh(c1.GetGreen(),  c2.GetGreen(), f),
                    BlendCh(c1.GetBlue(),   c2.GetBlue(), f),
                    c1.GetAlpha() );
}

GINLINE GColor      GColor::BlendAlpha  (GColor c1, GColor c2, UByte f)
{
    return GColor( c1, BlendCh(c1.GetAlpha(), c2.GetAlpha(), f) );
}


GINLINE UByte       GColor::InverseCh           ( UInt c )
{ 
    return UByte(255-c); 
}

GINLINE GColor      GColor::Inverse             ( GColor c )
{
    return GColor(  InverseCh(c.GetRed()),
                    InverseCh(c.GetGreen()),
                    InverseCh(c.GetBlue()),
                    InverseCh(c.GetAlpha()) );
}

GINLINE GColor      GColor::InverseRGB          ( GColor c )
{
    return GColor(  InverseCh(c.GetRed()),
                    InverseCh(c.GetGreen()),
                    InverseCh(c.GetBlue()),
                    c.GetAlpha() );
}

GINLINE GColor      GColor::InverseAlpha        ( GColor c )
    { return GColor( c, InverseCh(c.GetAlpha()) ); }


GINLINE const GColor    operator ^  (GColor c1, GColor c2)
    { return GColor::XOr(c1, c2); }
GINLINE const GColor    operator |  (GColor c1, GColor c2)
    { return GColor::Or(c1, c2); }
GINLINE const GColor    operator &  (GColor c1, GColor c2)
    { return GColor::And(c1, c2); }

GINLINE const GColor    operator -  (GColor c1, GColor c2)
    { return GColor::Subtract(c1, c2); }
GINLINE const GColor    operator +  (GColor c1, GColor c2)
    { return GColor::Add(c1, c2); }
GINLINE const GColor    operator *  (GColor c1, GColor c2)
    { return GColor::Modulate(c1, c2); }
GINLINE const GColor    operator *  (GColor c1, UByte factor)
    { return GColor::Modulate(c1, factor); }


// ** End Inline Implementation

#endif
