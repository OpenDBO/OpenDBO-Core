/**********************************************************************

Filename    :   GImage.h
Content     :   Image storage / manipulation classes
Created     :   July 29, 1998
Authors     :   Michael Antonov, Brendan Iribe

Notes       :   
History     :   

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/


#ifndef INC_GIMAGE_H
#define INC_GIMAGE_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GTLTypes.h"
#include "GColor.h"


// ***** Declared Classes
class GImageBase;
class GImage;

// ***** External Classes
class GFile;
class GJPEGInput;


// ***** Image Base class

class GImageBase
{

public:
    enum ImageFormat
    {
        Image_None      = 0,
        Image_ARGB_8888 = 1,
        Image_RGB_888   = 2,
    //  Image_RGB_565   = 3,
    //  Image_ARGB_4444 = 4,
        Image_L_8       = 8,
        Image_A_8       = 9,
        Image_DXT1      = 10,
        Image_DXT3      = 11,
        Image_DXT5      = 12,

        Image_P_8       = 100 // Palette8, just 256-color TGA is supported; 
                              // being converted to RGB_888 during loading by default.
    };

    ImageFormat Format;
    UInt32      Width, Height;
    UInt32      Pitch;

    // Pointer to data
    UByte*      pData;
    UInt        DataSize;

    // MipMap count (for DDS textures)
    UInt        MipMapCount;

    GTL::garray<GColor> ColorMap;


    // Scanline access
    UByte*          GetScanline(UInt y)
        { GASSERT(Pitch * y < DataSize); return pData + Pitch * y; }
    const UByte*    GetScanline(UInt y) const
        { GASSERT(Pitch * y < DataSize); return pData + Pitch * y; }

    static UInt GSTDCALL GetBytesPerPixel(ImageFormat fmt);
    UInt            GetBytesPerPixel() const
        { return GetBytesPerPixel(Format);  }

    static UInt GSTDCALL GetPitch(ImageFormat fmt, UInt width);
    UInt            GetPitch() const
        { return Pitch; }

    // Base constructor
    GImageBase()
        { }
    GImageBase(ImageFormat format, UInt32 width, UInt32 height, UInt32 pitch)
        { Format = format; Width = width; Height = height; Pitch = pitch; MipMapCount = 1; pData = 0; DataSize = 0; }

    void    ClearImageBase()
    {
        Format      = Image_None;
        Width       = Height = Pitch = 0;
        pData       = 0;
        DataSize    = 0;
        MipMapCount = 1;
        ColorMap.clear();
    }

    // Set pixel, sets only the appropriate channels
    void    SetPixelAlpha(SInt x, SInt y, UByte alpha);
    void    SetPixelLum(SInt x, SInt y, UByte lum);
    void    SetPixelRGBA(SInt x, SInt y, UInt32 color);
    void    SetPixelRGBA(SInt x, SInt y, UByte r, UByte g, UByte b, UByte a)
        { SetPixelRGBA(x,y, ((UInt32)r) | (((UInt32)g)<<8) | (((UInt32)b)<<16) | (((UInt32)a)<<24) ); }

    // Compute a hash code based on image contents.  Can be useful
    // for comparing images. Will return 0 if pData is null.
    UPInt   ComputeHash() const;

    inline bool           IsDataCompressed() const { return Format >= Image_DXT1 && Format <= Image_DXT5; }
    // level = 0 for main texture
    // if pwidth and pheight are not NULL they will contain dimensions of mipmap level.
    UByte*                GetMipMapLevelData(UInt level, UInt* pwidth, UInt* pheight, UInt* ppitch = 0);
    static UInt GSTDCALL  GetMipMapLevelSize(ImageFormat format, UInt w, UInt h);
};


// ***** Image class

class GImage : public GRefCountBase<GImage>, public GImageBase
{
public:
    
    // Constructors
    // End users should rely on the create function instead
    GEXPORT GImage();
    GEXPORT GImage(const GImageBase &src);
    GEXPORT GImage(ImageFormat format, UInt32 width, UInt32 height);

    GEXPORT ~GImage();

    // An image is considered non-empty if it has allocated data
    bool                IsEmpty() const
        { GASSERT(pData != 0 || DataSize == 0); return pData == 0; }
    // Deletes image data
    void                Clear();

    // Raw comparison of data
    bool    operator == (const GImage &src) const;


    // Create an image (return 0 if allocation failed)
    static  GImage*     GSTDCALL CreateImage(ImageFormat format, UInt32 width, UInt32 height);

    
    // *** JPEG I/O
    
    // Returns created images, need to *
    static GImage*      GSTDCALL ReadJpeg(const char* pfilename);
    static GImage*      GSTDCALL ReadJpeg(GFile* in);    

    // For reading SWF JPEG2-style image Data (slight variation on ordinary JPEG).  
    static GImage*      GSTDCALL ReadSwfJpeg2(GFile* pin);

    // For reading SWF JPEG2-style image data, using pre-loaded
    // headers stored in the given jpeg::input object.
    static GImage*      GSTDCALL ReadSwfJpeg2WithTables(GJPEGInput* ploader);

    // For reading SWF JPEG3-style image data, like ordinary JPEG, 
    // but stores the data in rgba format.
    static GImage*      GSTDCALL ReadSwfJpeg3(GFile* pin);

    bool                WriteJpeg(GFile* pout, int quality);    


    // *** TGA I/O

    // Loads TGA, only 32/24 bit formats supported.
    static GImage*      GSTDCALL ReadTga(GFile* in, ImageFormat destFormat = Image_None);

    bool                WriteTga(GFile* pout);


    // *** DDS Format loading

    // These routines do not rely on D3DX API and can therefore be used for compressed
    // textures with OpenGL as well as on other platforms.

    // Loads DDS from file creating GImage.
    static GImage*      GSTDCALL ReadDDS(GFile* in);

    // Loads DDS from a chunk of memory, data is copied.
    static GImage*      GSTDCALL ReadDDSFromMemory(const UByte* ddsData, UPInt dataSize);


    // *** PNG format loading
    static GImage*      GSTDCALL ReadPng(const char* pfilename);
    static GImage*      GSTDCALL ReadPng(GFile* in);    

    GImage*             ConvertImage(ImageFormat destFormat);
};


#endif 
