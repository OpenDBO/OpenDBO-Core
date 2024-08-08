/**********************************************************************

Filename    :   GImageInfo.h
Content     :   Defines GImageInfo family of classes, which associate
                a logical image with a texture.
Created     :   January 30, 2007
Authors     :   Michael Antonov

Notes       :   

Copyright   :   (c) 2005-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GIMAGEINFO_H
#define INC_GIMAGEINFO_H

#include "GRenderer.h"

// ***** Classes Declared

class GImageInfoBase;
class GImageInfo;

// Forward declarations:
class GImage;


// ***** GImageInfo Related classes.

// GImageInfoBase - a base class that provides a way to obtain a texture
// corresponding to an image. Image resources will store pointers to this class.

class GImageInfoBase : public GRefCountBase<GImageInfoBase>, public GTexture::ChangeHandler
{
public:

    enum ImageInfoType
    {        
        IIT_ImageInfo,      // GImageInfo        
        IIT_Other
    };
    
    // Obtains logical dimensions of images. These are usually the dimensions of the 
    // image that should be displayed even even if the image itself is rounded up.
    virtual UInt        GetWidth() const                    = 0;
    virtual UInt        GetHeight() const                   = 0;

    // Obtains the texture pointer from the data,
    // for a given renderer. Optionally create the texture
    virtual GTexture*   GetTexture(GRenderer* prenderer)    = 0;
    
    // RTTI for GImageInfoBase.
    virtual UInt        GetImageInfoType() const   { return IIT_Other; }
};


// GImageInfoBaseImpl - partial implementation of GImageInfoBase that stores a
// cached texture, relying on virtual Recreate function to initialize its data.
// Note that Recreate is inherited from GTexture::ChangeHandler in base; user
// is responsible for overriding Recreate in order to initialize the texture.
class GImageInfoBaseImpl : public GImageInfoBase
{
protected:
    // Texture, in the future could be a hash from renderer to texture.
    GPtr<GTexture>  pTexture;

    // Protected constructors, intended for derived classes only.
    GImageInfoBaseImpl() { }
    GImageInfoBaseImpl(GTexture *ptexture);
public:

    ~GImageInfoBaseImpl();
        
    // Relies of Recreate() to initialize / re-initaliase ptexture.
    virtual GTexture*   GetTexture(GRenderer* prenderer);
    // Partial GTexture::ChangeHandler implementation.
    virtual void        OnChange(GRenderer* prenderer, EventType changeType);    
};




// GImageInfo - Wraps a texture optionally backed by GImage.
//  - If GImage constructor is used, in-memory image is uses to create a texture
//   and re-initialize it as necessary.
//  - If GTexture constructor is used, texture is served directly and can not
//    be recreated unless Recreate is overridden in the derived class. In an event
//    of data loss, texture pointer is cleared and must be re-initialized by users.
// 

class GImageInfo : public GImageInfoBaseImpl
{
protected:
    // Source image data for the texture. If this is null, texture has no backup
    // data and recreate will fail.
    GPtr<GImage>    pImage;
    // Logical dimensions passed during texture creation to ensure
    // that renderer performs correct scaling.
    UInt            TargetWidth, TargetHeight;   
public:

    // *** GImageInfo constructors

    // Initialized GImageInfo with an image that will later be used for
    // texture creation. Passing an image has an advantage of allowing the
    // data to be re-created if the texture is lost. Storing image has no
    // benefit, however, on platforms that do not incur texture loss. 
    GImageInfo(GImage *pimage = 0);
    GImageInfo(GImage *pimage, UInt targetWidth, UInt targetHeight);

    // Initializes GImageInfo with a texture. Since no image is provided,
    // texture will be cleared out in the event of data loss.
    GImageInfo(GTexture *ptexture, UInt targetWidth, UInt targetHeight);

    virtual ~GImageInfo();


    // *** Data Assignment

    // SetImage and SetTexture APIs have the same behavior as constructors;
    // they re-initalize the GImageInfo object.

    // Set the image, keeping original target size. This will re-initialize the texture.
    void                SetImage(GImage* pimage);
    // Set the image, assigning the new target dimensions.
    void                SetImage(GImage* pimage, UInt targetWidth, UInt targetHeight);

    // Set image into to a texture, keeping original target size. If a reference to
    // an image was maintained, it will be released.
    void                SetTexture(GTexture *ptexture);
    // Sets the texture and potential target dimensions.
    void                SetTexture(GTexture *ptexture, UInt targetWidth, UInt targetHeight);

    // GImage query, if one exists.
    inline GImage*      GetImage() const { return pImage; }

    
    // *** GImageInfoBase Implementation

    virtual UInt        GetWidth() const;
    virtual UInt        GetHeight() const;
    virtual UInt        GetImageInfoType() const   { return IIT_ImageInfo; }
    virtual GTexture*   GetTexture(GRenderer* prenderer);
    virtual void		OnChange(GRenderer* prenderer, EventType changeType);    
    virtual bool        Recreate(GRenderer* prenderer);
};




/*
// GImageInfoBase version which loads textures directly from file.
class GImageFileInfo : public GImageInfoBase
{
public:
    char*               pFileName;
    GPtr<GTexture>      pTexture;
    // Target sizes describe the desired size of the image, they can be 
    // smaller then the texture size in case the texture was rounded
    // up to the next power of two.
    UInt                TargetWidth, TargetHeight;

    GImageFileInfo(UInt targetWidth, UInt targetHeight)
        : pFileName(0), TargetWidth(targetWidth), TargetHeight(targetHeight) 
    {
    }
    ~GImageFileInfo();

    void        SetFileName(const char* name);
    // Returned size can be incorrect (zero) if not specified.
    UInt        GetWidth() const  { return TargetWidth;  }
    UInt        GetHeight() const { return TargetHeight; }
    GTexture*   GetTexture(class GRenderer* prenderer);

    // GTexture::ChangeHandler implementation           
    virtual void    OnChange(GRenderer* prenderer, EventType changeType);
    virtual bool    Recreate(GRenderer* prenderer);         
};
*/

#endif
