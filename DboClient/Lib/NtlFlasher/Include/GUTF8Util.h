/**********************************************************************

Filename    :   GUTF8Util.h
Content     :   UTF8 Unicode character encoding/decoding support
Created     :   June 27, 2005
Authors     :   
Notes       :   

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GUTF8UTIL_H
#define INC_GUTF8UTIL_H

#include "GTypes.h"


// ***** Declared Classes
class GUTF8Util;


// ***** UTF8 Support utility classes

class GUTF8Util
{
public:

    // *** UTF8 string length and indexing.

    // Determines the length of UTF8 string in characters.
    static SPInt    GSTDCALL GetLength(const char* putf8str, SPInt length = -1);

    // Gets a decoded UTF8 character at index; you can access up to the index returned
    // by GetLength. 0 will be returned for out of bounds access.
    static UInt32   GSTDCALL GetCharAt(SPInt index, const char* putf8str, SPInt length = -1);

    // Converts UTF8 character index into byte offset.
    // -1 is returned if index was out of bounds.
    static SPInt    GSTDCALL GetByteIndex(SPInt index, const char* putf8str, SPInt length = -1);


    // *** 16-bit Unicode string Encoding/Decoding routines.

    // Determines the number of bytes necessary to encode a string.
    // Does not count the terminating 0 (null) character.
    static SPInt    GSTDCALL GetEncodeStringSize(const wchar_t* pchar, SPInt length = -1);

    // Encodes a unicode (UCS-2 only) string into a buffer. The size of buffer must be at
    // least GetEncodeStringSize() + 1.
    static void     GSTDCALL EncodeString(char *pbuff, const wchar_t* pchar, SPInt length = -1);

    // Decode UTF8 into a wchar_t buffer. Must have GetLength()+1 characters available.
    // Characters over 0xFFFF are replaced with 0xFFFD.
    static void     GSTDCALL DecodeString(wchar_t *pbuff, const char* putf8str, SPInt length = -1);


    // *** Individual character Encoding/Decoding.

    // Determined the number of bytes necessary to encode a UCS character.
    static int      GSTDCALL GetEncodeCharSize(UInt32 ucsCharacter);

    // Encodes the given UCS character into the given UTF-8 buffer.
    // Writes the data starting at buffer[offset], and 
    // increments offset by the number of bytes written.
    // May write up to 6 bytes, so make sure there's room in the buffer
    static void     GSTDCALL EncodeChar(char* pbuffer, SPInt* poffset, UInt32 ucsCharacter);

    // Return the next Unicode character in the UTF-8 encoded buffer.
    // Invalid UTF-8 sequences produce a U+FFFD character as output.
    // Advances *utf8_buffer past the character returned, unless 
    // the returned character is '\0', in which case the buffer does not advance.
    static UInt32   GSTDCALL DecodeNextChar(const char** putf8Buffer);
};


#endif

