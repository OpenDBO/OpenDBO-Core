/**********************************************************************

Filename    :   GFunctions.h
Content     :   General purpose bit access functions
Created     :   December 28, 1998
Authors     :   Michael Antonov

History     :   1/16/1999 MA    Revised Bit operation parameters

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFUNCTIONS_H
#define INC_GFUNCTIONS_H

#include "GTypes.h"

#include <string.h>



// ******* Special (optimized) memory routines
// Note: null (bad) pointer is not tested
class GMemUtil
{
public:
    
    // Memory copy
    GINLINE static void*    Copy  (void* pdest, const void* psrc, UPInt byteCount)      { return memcpy(pdest, psrc, byteCount); }
    GINLINE static void*    Copy16(void* pdest, const void* psrc, UPInt int16Count)     { return memcpy(pdest, psrc, int16Count*2); }
    GINLINE static void*    Copy32(void* pdest, const void* psrc, UPInt int32Count)     { return memcpy(pdest, psrc, int32Count*4); }
    GINLINE static void*    Copy64(void* pdest, const void* psrc, UPInt int64Count)     { return memcpy(pdest, psrc, int64Count*8); }
                                            
    // Memory move
    GINLINE static void*    Move  (void* pdest, const void* psrc, UPInt byteCount)      { return memmove(pdest, psrc, byteCount); }
    GINLINE static void*    Move16(void* pdest, const void* psrc, UPInt int16Count)     { return memmove(pdest, psrc, int16Count*2); }
    GINLINE static void*    Move32(void* pdest, const void* psrc, UPInt int32Count)     { return memmove(pdest, psrc, int32Count*4); }
    GINLINE static void*    Move64(void* pdest, const void* psrc, UPInt int64Count)     { return memmove(pdest, psrc, int64Count*8); }
                                    
    // Memory compare
    GINLINE static SInt     Cmp  (const void* p1, const void* p2, UPInt byteCount)      { return memcmp(p1, p2, byteCount); }
    GINLINE static SInt     Cmp16(const void* p1, const void* p2, UPInt int16Count);
    GINLINE static SInt     Cmp32(const void* p1, const void* p2, UPInt int32Count);
    GINLINE static SInt     Cmp64(const void* p1, const void* p2, UPInt int64Count);

    // Memory set
    GINLINE static void     Set  (void* pdest, SInt   value, UPInt byteCount)           { memset(pdest, value, byteCount); }
//  GEXPORT static void     GSTDCALL Set16(void* pdest, SInt16 value, UPInt int16Count);
//  GEXPORT static void     GSTDCALL Set32(void* pdest, SInt32 value, UPInt int32Count);
//  GEXPORT static void     GSTDCALL Set64(void* pdest, SInt64 value, UPInt int64Count);
};

// ** Inline Implementation

GINLINE SInt    GMemUtil::Cmp16(const void* p1, const void* p2, UPInt int16Count)
{
    SInt16* pa  = (SInt16*)p1; 
    SInt16* pb  = (SInt16*)p2;
    UInt    ic  = 0;
    if (int16Count == 0)
        return 0;
    while (pa[ic] == pb[ic])
        if (++ic==int16Count)
            return 0;
    return pa[ic] > pb[ic] ? 1 : -1;
}
GINLINE SInt    GMemUtil::Cmp32(const void* p1, const void* p2, UPInt int32Count)
{
    SInt32* pa  = (SInt32*)p1;
    SInt32* pb  = (SInt32*)p2;
    UInt    ic  = 0;
    if (int32Count == 0)
        return 0;
    while (pa[ic] == pb[ic])
        if (++ic==int32Count)
            return 0;
    return pa[ic] > pb[ic] ? 1 : -1;
}
GINLINE SInt    GMemUtil::Cmp64(const void* p1, const void* p2, UPInt int64Count)
{
    SInt64* pa  = (SInt64*)p1;
    SInt64* pb  = (SInt64*)p2;
    UInt    ic  = 0;
    if (int64Count == 0)
        return 0;
    while (pa[ic] == pb[ic])
        if (++ic==int64Count)
            return 0;
    return pa[ic] > pb[ic] ? 1 : -1;
}

// ** End Inline Implementation



// ******* Byte Order Conversions
class GByteUtil
{
public:

    // *** Swap Byte Order

    // Swap the byte order of a byte array
    GINLINE static void     SwapOrder(void* pv, SInt size)
    {
        UByte*  pb = (UByte*)pv;
        UByte   temp;
        for (SInt i = 0; i < size>>1; i++)
        { 
            temp            = pb[size-1-i];
            pb[size-1-i]    = pb[i];
            pb[i]           = temp; 
        }
    }

    // Swap the byte order of primitive types
    GINLINE static UInt8    SwapOrder(UInt8 v)      { return v; }
    GINLINE static SInt8    SwapOrder(SInt8 v)      { return v; }
    GINLINE static UInt16   SwapOrder(UInt16 v)     { return UInt16(v>>8)|UInt16(v<<8); }
    GINLINE static SInt16   SwapOrder(SInt16 v)     { return SInt16((UInt16(v)>>8)|(v<<8)); }
    GINLINE static UInt32   SwapOrder(UInt32 v)     { return (v>>24)|((v&0x00FF0000)>>8)|((v&0x0000FF00)<<8)|(v<<24); }
    GINLINE static SInt32   SwapOrder(SInt32 p)     { return (SInt32)SwapOrder(UInt32(p)); }
    GINLINE static UInt64   SwapOrder(UInt64 v)
    { 
        return   (v>>56) |
                 ((v&GUINT64(0x00FF000000000000))>>40) |
                 ((v&GUINT64(0x0000FF0000000000))>>24) |
                 ((v&GUINT64(0x000000FF00000000))>>8)  |
                 ((v&GUINT64(0x00000000FF000000))<<8)  |
                 ((v&GUINT64(0x0000000000FF0000))<<24) |
                 ((v&GUINT64(0x000000000000FF00))<<40) |
                 (v<<56); 
    }
    GINLINE static SInt64   SwapOrder(SInt64 v)     { return (SInt64)SwapOrder(UInt64(v)); }
    GINLINE static Float    SwapOrder(Float p)      
    { 
        UInt32 &v = *((UInt32*)&p);
        v = SwapOrder(v);
        return p; 
    }

#ifndef GFC_NO_DOUBLE
    GINLINE static Double   SwapOrder(Double p)
    { 
        UInt64 &v = *((UInt64*)&p);
        v = SwapOrder(v);
        return p; 
    }
#endif
    
    // *** Byte-order conversion

#if (GFC_BYTE_ORDER == GFC_LITTLE_ENDIAN)
    // Little Endian to System (LE)
    GINLINE static UInt8    LEToSystem(UInt8  v)    { return v; }
    GINLINE static SInt8    LEToSystem(SInt8  v)    { return v; }
    GINLINE static UInt16   LEToSystem(UInt16 v)    { return v; }
    GINLINE static SInt16   LEToSystem(SInt16 v)    { return v; }
    GINLINE static UInt32   LEToSystem(UInt32 v)    { return v; }
    GINLINE static SInt32   LEToSystem(SInt32 v)    { return v; }
    GINLINE static UInt64   LEToSystem(UInt64 v)    { return v; }
    GINLINE static SInt64   LEToSystem(SInt64 v)    { return v; }
    GINLINE static Float    LEToSystem(Float  v)    { return v; }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   LEToSystem(Double v)    { return v; }
#endif
    // Big Endian to System (LE)
    GINLINE static UInt8    BEToSystem(UInt8  v)    { return SwapOrder(v); }
    GINLINE static SInt8    BEToSystem(SInt8  v)    { return SwapOrder(v); }
    GINLINE static UInt16   BEToSystem(UInt16 v)    { return SwapOrder(v); }
    GINLINE static SInt16   BEToSystem(SInt16 v)    { return SwapOrder(v); }
    GINLINE static UInt32   BEToSystem(UInt32 v)    { return SwapOrder(v); }
    GINLINE static SInt32   BEToSystem(SInt32 v)    { return SwapOrder(v); }
    GINLINE static UInt64   BEToSystem(UInt64 v)    { return SwapOrder(v); }
    GINLINE static SInt64   BEToSystem(SInt64 v)    { return SwapOrder(v); }
    GINLINE static Float    BEToSystem(Float  v)    { return SwapOrder(v); }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   BEToSystem(Double v)    { return SwapOrder(v); }
#endif
    // System (LE) to Little Endian
    GINLINE static UInt8    SystemToLE(UInt8  v)    { return v; }
    GINLINE static SInt8    SystemToLE(SInt8  v)    { return v; }
    GINLINE static UInt16   SystemToLE(UInt16 v)    { return v; }
    GINLINE static SInt16   SystemToLE(SInt16 v)    { return v; }
    GINLINE static UInt32   SystemToLE(UInt32 v)    { return v; }
    GINLINE static SInt32   SystemToLE(SInt32 v)    { return v; }
    GINLINE static UInt64   SystemToLE(UInt64 v)    { return v; }
    GINLINE static SInt64   SystemToLE(SInt64 v)    { return v; }
    GINLINE static Float    SystemToLE(Float  v)    { return v; }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   SystemToLE(Double v)    { return v; }   
#endif
    // System (LE) to Big Endian
    GINLINE static UInt8    SystemToBE(UInt8  v)    { return SwapOrder(v); }
    GINLINE static SInt8    SystemToBE(SInt8  v)    { return SwapOrder(v); }
    GINLINE static UInt16   SystemToBE(UInt16 v)    { return SwapOrder(v); }
    GINLINE static SInt16   SystemToBE(SInt16 v)    { return SwapOrder(v); }
    GINLINE static UInt32   SystemToBE(UInt32 v)    { return SwapOrder(v); }
    GINLINE static SInt32   SystemToBE(SInt32 v)    { return SwapOrder(v); }
    GINLINE static UInt64   SystemToBE(UInt64 v)    { return SwapOrder(v); }
    GINLINE static SInt64   SystemToBE(SInt64 v)    { return SwapOrder(v); }
    GINLINE static Float    SystemToBE(Float  v)    { return SwapOrder(v); }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   SystemToBE(Double v)    { return SwapOrder(v); }
#endif

#elif (GFC_BYTE_ORDER == GFC_BIG_ENDIAN)
    // Little Endian to System (BE)
    GINLINE static UInt8    LEToSystem(UInt8  v)    { return SwapOrder(v); }
    GINLINE static SInt8    LEToSystem(SInt8  v)    { return SwapOrder(v); }
    GINLINE static UInt16   LEToSystem(UInt16 v)    { return SwapOrder(v); }
    GINLINE static SInt16   LEToSystem(SInt16 v)    { return SwapOrder(v); }
    GINLINE static UInt32   LEToSystem(UInt32 v)    { return SwapOrder(v); }
    GINLINE static SInt32   LEToSystem(SInt32 v)    { return SwapOrder(v); }
    GINLINE static UInt64   LEToSystem(UInt64 v)    { return SwapOrder(v); }
    GINLINE static SInt64   LEToSystem(SInt64 v)    { return SwapOrder(v); }
    GINLINE static Float    LEToSystem(Float  v)    { return SwapOrder(v); }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   LEToSystem(Double v)    { return SwapOrder(v); }
#endif
    // Big Endian to System (BE)
    GINLINE static UInt8    BEToSystem(UInt8  v)    { return v; }
    GINLINE static SInt8    BEToSystem(SInt8  v)    { return v; }
    GINLINE static UInt16   BEToSystem(UInt16 v)    { return v; }
    GINLINE static SInt16   BEToSystem(SInt16 v)    { return v; }
    GINLINE static UInt32   BEToSystem(UInt32 v)    { return v; }
    GINLINE static SInt32   BEToSystem(SInt32 v)    { return v; }
    GINLINE static UInt64   BEToSystem(UInt64 v)    { return v; }
    GINLINE static SInt64   BEToSystem(SInt64 v)    { return v; }
    GINLINE static Float    BEToSystem(Float  v)    { return v; }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   BEToSystem(Double v)    { return v; }
#endif
    // System (BE) to Little Endian
    GINLINE static UInt8    SystemToLE(UInt8  v)    { return SwapOrder(v); }
    GINLINE static SInt8    SystemToLE(SInt8  v)    { return SwapOrder(v); }
    GINLINE static UInt16   SystemToLE(UInt16 v)    { return SwapOrder(v); }
    GINLINE static SInt16   SystemToLE(SInt16 v)    { return SwapOrder(v); }
    GINLINE static UInt32   SystemToLE(UInt32 v)    { return SwapOrder(v); }
    GINLINE static SInt32   SystemToLE(SInt32 v)    { return SwapOrder(v); }
    GINLINE static UInt64   SystemToLE(UInt64 v)    { return SwapOrder(v); }
    GINLINE static SInt64   SystemToLE(SInt64 v)    { return SwapOrder(v); }
    GINLINE static Float    SystemToLE(Float  v)    { return SwapOrder(v); }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   SystemToLE(Double v)    { return SwapOrder(v); }
#endif
    // System (BE) to Big Endian
    GINLINE static UInt8    SystemToBE(UInt8  v)    { return v; }
    GINLINE static SInt8    SystemToBE(SInt8  v)    { return v; }
    GINLINE static UInt16   SystemToBE(UInt16 v)    { return v; }
    GINLINE static SInt16   SystemToBE(SInt16 v)    { return v; }
    GINLINE static UInt32   SystemToBE(UInt32 v)    { return v; }
    GINLINE static SInt32   SystemToBE(SInt32 v)    { return v; }
    GINLINE static UInt64   SystemToBE(UInt64 v)    { return v; }
    GINLINE static SInt64   SystemToBE(SInt64 v)    { return v; }
    GINLINE static Float    SystemToBE(Float  v)    { return v; }
#ifndef GFC_NO_DOUBLE
    GINLINE static Double   SystemToBE(Double v)    { return v; }
#endif

#else
    #error "GFC_BYTE_ORDER must be defined to GFC_LITTLE_ENDIAN or GFC_BIG_ENDIAN"
#endif

};


#endif
