/**********************************************************************

Filename    :   GConfig.h
Content     :   GFx configuration file - contains #ifdefs for
                the optional components of the library
Created     :   June 15, 2005
Authors     :   Michael Antonov

Copyright   :   (c) 2005-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GCONFIG_H
#define INC_GCONFIG_H

// Allows developers to replace the math.h
//#define GFC_MATH_H  <math.h>


//#define  GFC_NO_THREADSUPPORT

// This macro needs to be defined if it is necessary to avoid the use of Double.
// In that case Double in defined as Float and thus extra #ifdef checks on
// overloads need to be done.
// NOTE: By default, PS2 and PSP define this if not manually defined here.
//#define GFC_NO_DOUBLE


// Undefine this macro to disable the need for linking/compiling GFile_sys.
// If this macro is not defined, GFile and GDelegatedFile interfaces will still be
// available, but not GSysFile. Applications removing this flag will need to
// provide their own file opening and I/O logic; in GFxPlayer that can be achieved
// by specifying a FileOpenCallback in GFxLoader.
#define GFC_USE_SYSFILE

// Undefine this macro to disable use of LIBJPEG and make JPEGUtil a no-op stub.
// If disabled, SWF JPEG image loading will stop functioning.
// - saves roughly 60k in release build.
#define GFC_USE_LIBJPEG

// Undefine this macro to disable use of ZLIB and comment out GZLibFile class.
// If disabled, compressed SWF files will no longer load.
// - saves roughly 20k in release build.
#define GFC_USE_ZLIB

// Define this macro to enable use of LIBPNG.
// If disabled, SWF PNG image loading is not functioning.
// Note: beside the uncommenting the line below, it is necessary
// to provide the path and the library name for the linker.
// The name of the library is libpng.lib for all configurations;
// the path is different. For example, VC8, 64-bit, Release Static
// library is located at %(GFX)\3rdParty\libpng\lib\x64\msvc80\release_mt_static
//#define GFC_USE_LIBPNG



// ***** GFx Specific options

// If this macro is defined GFx will not use the stroker to render lines;
// - saves roughly 20K in release build.
//#define GFC_NO_FXPLAYER_STROKER

// If this macro is defined GFx will not include EdgeAA support;
// - saves roughly 30K in release build.
//#define GFC_NO_FXPLAYER_EDGEAA


// Default mouse support enable state for GFx. Enabling mouse is also
// dynamically controlled by GFxMovieView::EnableMouseSupport().
#if defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360) || \
    defined(GFC_OS_PSP) || defined(GFC_OS_PS2) || defined(GFC_OS_PS3) || \
    defined(GFC_OS_GAMECUBE)
#define GFC_MOUSE_SUPPORT_ENABLED   0
#else
#define GFC_MOUSE_SUPPORT_ENABLED   1
#endif


// *** Logging options

// Define this macro to eliminate all support for verbose parsing of input files.
// If this option is set, none of verbose parse options are available, and the
// GFxLoader::SetVerboseParse call will have no effect.
// Game production release builds should probably define this option.
//#define GFC_NO_FXPLAYER_VERBOSE_PARSE

#ifdef GFC_NO_FXPLAYER_VERBOSE_PARSE
    #define GFC_NO_FXPLAYER_VERBOSE_PARSE_ACTION
    #define GFC_NO_FXPLAYER_VERBOSE_PARSE_SHAPE
    #define GFC_NO_FXPLAYER_VERBOSE_PARSE_MORPHSHAPE
#else

    // Define this macro to eliminate all support for verbose parsing of actions
    // (disables support for disassembly during loading).
    //#define GFC_NO_FXPLAYER_VERBOSE_PARSE_ACTION

    // Define this macro to eliminate all support for verbose parsing
    // of shape character structures.
    //#define GFC_NO_FXPLAYER_VERBOSE_PARSE_SHAPE

    // Define this macro to eliminate all support for verbose parsing
    // of morph shape character structures.
    //#define GFC_NO_FXPLAYER_VERBOSE_PARSE_MORPHSHAPE

#endif


// Define this macro to eliminate support for verbose logging of executed ActionScript opcodes.
// If this macro is defined, GFxMovie::SetVerboseAction will have no effect.
// Game production release builds should probably define this option.
//#define GFC_NO_FXPLAYER_VERBOSE_ACTION

// Define this macro to throw assertion if any font texture is generated during
// the runtime.
//#define GFC_ASSERT_ON_FONT_BITMAP_GEN

// Define this macro to throw assertion if any gradient texture is generated during
// the runtime.
//#define GFC_ASSERT_ON_GRADIENT_BITMAP_GEN

// Define this macro to throw assertion if any re-sampling occurred in renderer during
// the runtime.
//#define GFC_ASSERT_ON_RENDERER_RESAMPLING

// Define this macro to throw assertion if any mipmap levels generation occurred in
// renderer during the runtime.
//#define GFC_ASSERT_ON_RENDERER_MIPMAP_GEN

// Define this macro to exclude FontGlyphPacker
//#define GFC_NO_FONT_GLYPH_PACKER

// Define this macro to exclude the dynamic glyph cache
//#define GFC_NO_GLYPH_CACHE

// Define this macro to exclude gradient generation
//#define GFC_NO_GRADIENT_GEN

// Define this macro to exclude sound support
// Due to no GFx sound support, this macro cannot be undefined
#define GFC_NO_SOUND

#endif
