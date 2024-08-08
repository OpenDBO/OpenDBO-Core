/**********************************************************************

Filename    :   GColorMacros.h
Content     :   Basic color structures and macros
Created     :   March 26, 1999
Authors     :   Michael Antonov & Brendan Iribe

Copyright   :   (c) 1999-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GCOLORMACROS_H
#define INC_GCOLORMACROS_H

#include "GTypes.h"


// ***** Color conversion macros

// 12 bit ARGB - 4444 color format (12 bit RGB, 4 bit alpha)

#define GFC_RGB12_RED_BITS              4
#define GFC_RGB12_GREEN_BITS            4
#define GFC_RGB12_BLUE_BITS             4
#define GFC_RGB12_ALPHA_BITS            4

#define GFC_RGB12_RED_MASK              0x0F00
#define GFC_RGB12_GREEN_MASK            0x00F0
#define GFC_RGB12_BLUE_MASK             0x000F
#define GFC_RGB12_ALPHA_MASK            0xF000
#define GFC_RGB12_RGB_MASK              (GFC_RGB12_RED_MASK|GFC_RGB12_GREEN_MASK|GFC_RGB12_BLUE_MASK)
                                    
#define GFC_RGB12_RED(color)            (UByte)(((color)&GFC_RGB12_RED_MASK)>>8)
#define GFC_RGB12_GREEN(color)          (UByte)(((color)&GFC_RGB12_GREEN_MASK)>>4)
#define GFC_RGB12_BLUE(color)           (UByte)((color)&GFC_RGB12_BLUE_MASK)
#define GFC_RGB12_ALPHA(color)          (UByte)((color)>>12)
                                    
#define GFC_RGB12_MAKE(r,g,b)           (UInt16)((((r)&0xF0)<<8)|(((g)&0xF0)<<4)|(((b)>>4))
#define GFC_RGBA12_MAKE(r,g,b,a)        (UInt16)(GFC_RGB12_MAKE(r,g,b)|(((a)&0xF0)<<12))

#define GFC_RGB12_SET_RED(color, r)     (UInt16)(((color)&~GFC_RGB12_RED_MASK)|(((r)&0xF0)<<8))
#define GFC_RGB12_SET_GREEN(color, g)   (UInt16)(((color)&~GFC_RGB12_GREEN_MASK)|(((g)&0xF0)<<4))
#define GFC_RGB12_SET_BLUE(color, b)    (UInt16)(((color)&~GFC_RGB12_BLUE_MASK)|((b)>>4))
#define GFC_RGB12_SET_ALPHA(color, a)   (UInt16)(((color)&~GFC_RGB12_ALPHA_MASK)|(((a)&0xF0)<<12))

                                    
// Conversion                                   
// Converting up does not rescale the values
#define GFC_RGB12_TO_RGB15(color)       (UInt16)((((color)&GFC_RGB12_RED_MASK)<<3)   |      \
                                                 (((color)&GFC_RGB12_GREEN_MASK)<<2) |      \
                                                 (((color)&GFC_RGB12_BLUE_MASK)<<1))

#define GFC_RGB12_TO_RGBA15(color)      (UInt16)((((color)&GFC_RGB12_RED_MASK)<<3)    |     \
                                                 (((color)&GFC_RGB12_GREEN_MASK)<<2)  |     \
                                                 (((color)&GFC_RGB12_BLUE_MASK)<<1)   |     \
                                                 (((color)&GFC_RGB15_ALPHA_MASK)) )

#define GFC_RGB12_TO_RGB16(color)       (UInt16)((((color)&GFC_RGB12_RED_MASK)<<4)    |     \
                                                 (((color)&GFC_RGB12_GREEN_MASK)<<3)  |     \
                                                 (((color)&GFC_RGB12_BLUE_MASK)<<1))

#define GFC_RGB12_TO_RGB32(color)       (UInt32)(((UInt32)((color)&GFC_RGB12_RED_MASK)<<12)  |      \
                                                 ((UInt32)((color)&GFC_RGB12_GREEN_MASK)<<8) |      \
                                                 ((UInt32)((color)&GFC_RGB12_BLUE_MASK)<<4) )

#define GFC_RGB12_TO_RGBA32(color)      (UInt32)(((UInt32)((color)&GFC_RGB12_RED_MASK)<<12)  |      \
                                                 ((UInt32)((color)&GFC_RGB12_GREEN_MASK)<<8) |      \
                                                 ((UInt32)((color)&GFC_RGB12_BLUE_MASK)<<4)  |      \
                                                 ((UInt32)((color)&GFC_RGB12_ALPHA_MASK)<<16) )

#define GFC_RGB12_TO_RGB64(color)       (UInt64) (((UInt64)GFC_RGB12_RED(color)  <<8)<<32)|         \
                                                 (((UInt64)GFC_RGB12_GREEN(color)<<8)<<16)|         \
                                                 (((UInt64)GFC_RGB12_BLUE(color) <<8))
#define GFC_RGB12_TO_RGBA64(color)      (UInt64) (((UInt64)GFC_RGB12_RED(color)  <<8)<<32)|         \
                                                 (((UInt64)GFC_RGB12_GREEN(color)<<8)<<16)|         \
                                                 (((UInt64)GFC_RGB12_BLUE(color) <<8))|             \
                                                 (((UInt64)GFC_RGB12_ALPHA(color)<<8)<<48)


// 15 bit ARGB - 1555 color format (15 bit RGB, 1 bit alpha)

#define GFC_RGB15_RED_BITS              5
#define GFC_RGB15_GREEN_BITS            5
#define GFC_RGB15_BLUE_BITS             5
#define GFC_RGB15_ALPHA_BITS            1

#define GFC_RGB15_RED_MASK              0x7C00
#define GFC_RGB15_GREEN_MASK            0x03E0
#define GFC_RGB15_BLUE_MASK             0x001F
#define GFC_RGB15_ALPHA_MASK            0x8000
#define GFC_RGB15_RGB_MASK              (GFC_RGB15_RED_MASK|GFC_RGB15_GREEN_MASK|GFC_RGB15_BLUE_MASK)                               
                                    
#define GFC_RGB15_RED(color)            (UByte)(((color)&GFC_RGB15_RED_MASK)>>6)
#define GFC_RGB15_GREEN(color)          (UByte)(((color)&GFC_RGB15_GREEN_MASK)>>2)
#define GFC_RGB15_BLUE(color)           (UByte)(((color)&GFC_RGB15_BLUE_MASK)<<3)
#define GFC_RGB15_ALPHA(color)          (UByte)(((color)&GFC_RGB15_ALPHA_MASK)>>12)
                                    
#define GFC_RGB15_MAKE(r,g,b)           (UInt16)((((r)&0xF8)<<6)|(((g)&0xF8)<<2)|(((b)>>3))
#define GFC_RGBA15_MAKE(r,g,b,a)        (UInt16)(GFC_RGB15_MAKE(r,g,b)|(((a)>0)<<15))

#define GFC_RGB15_SET_RED(color, r)     (UInt16)(((color)&~GFC_RGB15_RED_MASK)|(((r)&0xF8)<<6))
#define GFC_RGB15_SET_GREEN(color, g)   (UInt16)(((color)&~GFC_RGB15_GREEN_MASK)|(((g)&0xF8)<<2))
#define GFC_RGB15_SET_BLUE(color, b)    (UInt16)(((color)&~GFC_RGB15_BLUE_MASK)|((b)>>3))
#define GFC_RGB15_SET_ALPHA(color, a)   (UInt16)(((color)&~GFC_RGB15_ALPHA_MASK)|(((a)>0)<<15))

// Conversion                       
#define GFC_RGB15_TO_RGB16(color)       (UInt16)((((color)&GFC_RGB15_RED_MASK)<<1)  |   \
                                                 (((color)&GFC_RGB15_GREEN_MASK)<<1)|   \
                                                  ((color)&GFC_RGB15_BLUE_MASK))
                                    
#define GFC_RGB15_TO_RGB12(color)       (UInt16)((((color)&0x7800)>>3)|(((color)&0x03C0)>>2)|(((color)&GFC_RGB15_BLUE_MASK)>>1))
#define GFC_RGB15_TO_RGBA12(color)      (UInt16)((((color)&0x7800)>>3)|(((color)&0x03C0)>>2)|(((color)&GFC_RGB15_BLUE_MASK)>>1)|((color)&GFC_RGB15_ALPHA_MASK))

#define GFC_RGB15_TO_RGB32(color)       (UInt32)((((color)<<3)&0xF8)|(((color)<<6)&0xF800)|(((color)<<9)&0xF80000))
#define GFC_RGB15_TO_RGBA32(color)      (UInt32)((((color)<<3)&0xF8)|(((color)<<6)&0xF800)|(((color)<<9)&0xF80000)|(((color)&GFC_RGB15_ALPHA_MASK)<<16))
                                    

#define GFC_RGB15_TO_RGB64(color)       (UInt64) (((UInt64)GFC_RGB15_RED(color)  <<8)<<32)  |   \
                                                 (((UInt64)GFC_RGB15_GREEN(color)<<8)<<16)  |   \
                                                 (((UInt64)GFC_RGB15_BLUE(color) <<8))
#define GFC_RGB15_TO_RGBA64(color)      (UInt64) (((UInt64)GFC_RGB15_RED(color)  <<8)<<32)  |   \
                                                 (((UInt64)GFC_RGB15_GREEN(color)<<8)<<16)  |   \
                                                 (((UInt64)GFC_RGB15_BLUE(color) <<8))      |   \
                                                 (((UInt64)GFC_RGB15_ALPHA(color)<<8)<<48)
                
// 16 bit RGB - 565 color format        

#define GFC_RGB16_RED_BITS              5
#define GFC_RGB16_GREEN_BITS            6
#define GFC_RGB16_BLUE_BITS             5
#define GFC_RGB16_ALPHA_BITS            0
    
#define GFC_RGB16_RED_MASK              0xF800
#define GFC_RGB16_GREEN_MASK            0x07E0
#define GFC_RGB16_BLUE_MASK             0x001F
#define GFC_RGB16_RGB_MASK              (GFC_RGB16_RED_MASK|GFC_RGB16_GREEN_MASK|GFC_RGB16_BLUE_MASK)
                                    
#define GFC_RGB16_RED(color)            (UByte)(((color)&GFC_RGB16_RED_MASK)>>7)
#define GFC_RGB16_GREEN(color)          (UByte)(((color)&GFC_RGB16_GREEN_MASK)>>3)
#define GFC_RGB16_BLUE(color)           (UByte)(((color)&GFC_RGB16_BLUE_MASK)<<3)
                                    
#define GFC_RGB16_MAKE(r,g,b)           (UInt16)((((r)&0xF8)<<7)|(((g)&0xFC)<<3)|(((b)>>3))

#define GFC_RGB16_SET_RED(color, r)     (UInt16)(((color)&~GFC_RGB16_RED_MASK)|(((r)&0xF8)<<7))
#define GFC_RGB16_SET_GREEN(color, g)   (UInt16)(((color)&~GFC_RGB16_GREEN_MASK)|(((g)&0xFC)<<3))
#define GFC_RGB16_SET_BLUE(color, b)    (UInt16)(((color)&~GFC_RGB16_BLUE_MASK)|((b)>>3))
                                    
// Conversion                       
#define GFC_RGB16_TO_RGB15(color)       (UInt16)((((color)&GFC_RGB16_RED_MASK)>>1)   |          \
                                                 (((color)>>1)&GFC_RGB15_GREEN_MASK) |          \
                                                 ((color)&GFC_RGB16_BLUE_MASK) )

#define GFC_RGB16_TO_RGB12(color)       (UInt16)((((color)&0xF000)>>4)|(((color)&0x07C0)>>3)|(((color)&GFC_RGB16_BLUE_MASK)>>1))
                                    
// Converting up does not rescale the values
#define GFC_RGB16_TO_RGB32(color)       (UInt32)(((UInt32)((color)&GFC_RGB16_RED_MASK)  <<8)   |    \
                                                 ((UInt32)((color)&GFC_RGB16_GREEN_MASK)<<5)   |    \
                                                 ((UInt32)((color)&GFC_RGB16_BLUE_MASK) <<3))
// Converting up to 64 bit
#define GFC_RGB16_TO_RGB64(color)       (UInt64) (((UInt64)GFC_RGB16_RED(color)  <<8)<<32)  |   \
                                                 (((UInt64)GFC_RGB16_GREEN(color)<<8)<<16)  |   \
                                                 (((UInt64)GFC_RGB16_BLUE(color) <<8))




// *** Default 32 bit color functions

#define GFC_RGB32_RED_BITS              8
#define GFC_RGB32_GREEN_BITS            8
#define GFC_RGB32_BLUE_BITS             8
#define GFC_RGB32_ALPHA_BITS            8

#define GFC_RGB32_RED_MASK              0x00FF0000
#define GFC_RGB32_GREEN_MASK            0x0000FF00
#define GFC_RGB32_BLUE_MASK             0x000000FF
#define GFC_RGB32_ALPHA_MASK            0xFF000000
#define GFC_RGB32_RGB_MASK              (GFC_RGB32_RED_MASK|GFC_RGB32_GREEN_MASK|GFC_RGB32_BLUE_MASK)

#define GFC_RGB32_RED(color)            (UByte)(((color)&GFC_RGB32_RED_MASK)>>16)
#define GFC_RGB32_GREEN(color)          (UByte)(((color)&GFC_RGB32_GREEN_MASK)>>8)
#define GFC_RGB32_BLUE(color)           (UByte)((color)&GFC_RGB32_BLUE_MASK)
#define GFC_RGB32_ALPHA(color)          (UByte)((color)>>24)

#define GFC_RGB32_SET_RED(color, r)     (UInt32)(((color)&~GFC_RGB32_RED_MASK)|((r)<<16))
#define GFC_RGB32_SET_GREEN(color, g)   (UInt32)(((color)&~GFC_RGB32_GREEN_MASK)|((g)<<8))
#define GFC_RGB32_SET_BLUE(color, b)    (UInt32)(((color)&~GFC_RGB32_BLUE_MASK)|(b))
#define GFC_RGB32_SET_ALPHA(color, a)   (UInt32)(((color)&~GFC_RGB32_ALPHA_MASK)|((a)<<24))

#define GFC_RGB32_MAKE(r,g,b)           (UInt32((b)|((g)<<8)|((r)<<16)))
#define GFC_RGBA32_MAKE(r,g,b,a)        (UInt32(GFC_RGB32_MAKE(r,g,b)|((a)<<24)))

// Simple color format conversion macros
#define GFC_RGB32_TO_RGB12(color)       (UInt16)((((color)&0xF0)>>4)|(((color)&0xF000)>>8)|(((color)&0xF00000)>>12))
#define GFC_RGB32_TO_RGBA12(color)      (UInt16)((((color)&0xF0)>>4)|(((color)&0xF000)>>8)|(((color)&0xF00000)>>12)|(((color)&0xF0000000)>>16))
#define GFC_RGB32_TO_RGB15(color)       (UInt16)((((color)&0xF8)>>3)|(((color)&0xF800)>>6)|(((color)&0xF80000)>>9))
#define GFC_RGB32_TO_RGBA15(color)      (UInt16)((((color)&0xF8)>>3)|(((color)&0xF800)>>6)|(((color)&0xF80000)>>9)|(((color)&0x80000000)>>16))
#define GFC_RGB32_TO_RGB16(color)       (UInt16)((((color)&0xF8)>>3)|(((color)&0xFC00)>>5)|(((color)&0xF80000)>>8))
// Converting up to 64 bit
#define GFC_RGB32_TO_RGB64(color)       (UInt64) (((UInt64)GFC_RGB32_RED(color)  <<8)<<32)  |   \
                                                 (((UInt64)GFC_RGB32_GREEN(color)<<8)<<16)  |   \
                                                 (((UInt64)GFC_RGB32_BLUE(color) <<8))
#define GFC_RGB32_TO_RGBA64(color)      (UInt64) (((UInt64)GFC_RGB32_RED(color)  <<8)<<32)  |   \
                                                 (((UInt64)GFC_RGB32_GREEN(color)<<8)<<16)  |   \
                                                 (((UInt64)GFC_RGB32_BLUE(color) <<8))      |   \
                                                 (((UInt64)GFC_RGB32_ALPHA(color)<<8)<<48)


// *** Short 32bit color defines
#define GRGB(r,g,b)                     GFC_RGB32_MAKE(r,g,b)
#define GRGBA(r,g,b,a)                  GFC_RGBA32_MAKE(r,g,b,a)



#endif
