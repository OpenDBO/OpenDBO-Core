/**********************************************************************

Filename    :   GFxString.h
Content     :   GFxString UTF8 string implementation with copy-on
                write semantics (thread-safe for assignment but not
                modification).
Created     :   April 27, 2007
Authors     :   Ankur

Notes       :
History     :

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFxString_H
#define INC_GFxString_H

#include "GTypes.h"
#include "GTLTypes.h"
#include "GUTF8Util.h"
#include "GAtomic.h"
#include <string.h>
#include "GStd.h"

// ***** GFxString Class

class GFxString
{
private:
	// Internal structure to hold string data
	struct DataDesc
	{
		// Number of bytes. Will be the same as the number of chars if the characters
		// are ascii, may not be equal to number of chars in case string data is UTF8.
		UPInt   Size;
		UPInt   BuffSize;
        volatile SInt32	RefCount;
		char	Data[1];

		void	AddRef()
		{
			GAtomicOps<SInt32>::ExchangeAdd_NoSync(&RefCount, 1);
		}
		// Decrement ref count. This needs to be thread-safe, since
		// a different thread could have also decremented the ref count.
		// For example, if u start off with a ref count = 2. Now if u
		// decrement the ref count and check against 0 in different
		// statements, a different thread can also decrement the ref count
		// in between our decrement and checking against 0 and will find
		// the ref count = 0 and delete the object. This will lead to a crash
		// when context switches to our thread and we'll be trying to delete
		// an already deleted object. Hence decrementing the ref count and
		// checking against 0 needs to made an atomic operation.
		void	Release()
		{
			if ((GAtomicOps<SInt32>::ExchangeAdd_NoSync(&RefCount, -1) - 1) == 0)
				GFREE(this);
		}

		void	SetSize(UPInt size, bool flag);
	};

	DataDesc* pData;

public:
	// Constructors
	GFxString(const char* data);
	GFxString(const char* data, UPInt buflen);
    explicit GFxString(const wchar_t* data);
	GFxString();

	// Declaration of NullString
	static DataDesc NullData;

	enum FlagConstants
	{
		//Flag_GetLength      = 0x7FFFFFFF,
		// This flag is set if GetLength() == GetSize() for a string.
		// Avoid extra scanning is Substring and indexing logic.
		Flag_LengthIsSizeShift   = (sizeof(UPInt)*8 - 1)
	};

	// copy constructor
	GFxString(const GFxString& src)
	{
		pData = src.pData;
		pData->AddRef();
	}
	// Destructor
	~GFxString()
	{
		pData->Release();
	}

	// *** General Functions

	void        Clear();

	// For casting to a pointer to Char
	operator const char*() const
	{
		return pData->Data;
	}
	// Pointer to raw buffer.
	const char* ToCStr() const          { return pData->Data; }

	// Returns number of bytes
	UPInt       GetSize() const         { return pData->Size & ~(UPInt(1) << Flag_LengthIsSizeShift) ; }

	// Tells whether or not the string is empty
	bool        IsEmpty() const         { return GetSize() == 0; }

	// Returns  number of characters
	UPInt       GetLength() const;

	// Returns  character at the specified index
	UInt32      GetCharAt(UPInt index) const;

	// Appends a character
	void		AppendChar(UInt32 ch);

    // Append a string
    void        AppendString(const wchar_t* pstr, SPInt len = -1);
    void        AppendString(const char* putf8str, SPInt utf8StrSz = -1);

	//  Resize the string to the new size
	void		Resize(UPInt _size);

	// Removes the character at posAt
	void        Remove(UPInt posAt, SPInt len = 1);

	// Returns a GFxString that's a substring of this.
	//  -start is the index of the first UTF8 character you want to include.
	//  -end is the index one past the last UTF8 character you want to include.
	GFxString   Substring(UPInt start, UPInt end);

	// Case-conversion
	GFxString   ToUpper() const;
	GFxString   ToLower() const;

	// Inserts substr at posAt
	GFxString&	Insert (const char* substr, UPInt posAt, SPInt len = -1);

	// Inserts character at posAt
	UPInt		InsertCharAt(UInt32 c, UPInt posAt);

	// Inserts substr at posAt
	GFxString&	Insert(const UInt32* substr, UPInt posAt, SPInt len = -1);

	// Get Byte index of the character at position = index
	UPInt       GetByteIndex(UPInt index) const { return (UPInt)GUTF8Util::GetByteIndex(index, pData->Data); }

	// Utility: case-insensitive string compare.  stricmp() & strnicmp() are not
	// ANSI or POSIX, do not seem to appear in Linux.
    static int  GSTDCALL	CompareNoCase(const char* a, const char* b);
    static int  GSTDCALL	CompareNoCase(const char* a, const char* b, SPInt len);

	// Hash function, case-insensitive
	static size_t GSTDCALL	BernsteinHashFunctionCIS(const void* pdataIn, size_t size, size_t seed = 5381);

	// Hash function, case-sensitive
	static size_t GSTDCALL	BernsteinHashFunction(const void* pdataIn, size_t size, size_t seed = 5381);

    // Encode/decode special html chars
    static void EscapeSpecialHTML(const char* psrc, UPInt length, GFxString* pescapedStr);
    static void UnescapeSpecialHTML(const char* psrc, UPInt length, GFxString* punescapedStr);

	// Operators
	// Assignment
	void		operator =	(const char* str);
	void		operator =	(const wchar_t* str);
	void		operator =	(const GFxString& src);

	// Addition
	void		operator += (const GFxString& src);
	void		operator += (const char* src);
	void		operator += (const wchar_t* src);
	void		operator += (char str);
	GFxString   operator +	(const char* str) const ;
	GFxString   operator +	(const GFxString& src)  const ;

	// Comparison
	bool		operator ==	(const GFxString& str) const
	{
		return (gfc_strcmp(pData->Data, str.pData->Data)== 0);
	}

	bool		operator != (const GFxString& str) const
	{
		return pData != str.pData;
	}

	bool		operator == (const char* str) const
	{
		return gfc_strcmp(pData->Data, str) == 0;
	}

	bool		operator != (const char* str) const
	{
		return !operator == (str);
	}

	bool		operator <	(const char* pstr) const
	{
		return gfc_strcmp(pData->Data, pstr) < 0;
	}

	bool		operator <	(const GFxString& str) const
	{
		return *this < str.pData->Data;
	}

	bool		operator >	(const char* pstr) const
	{
		return gfc_strcmp(pData->Data, pstr) > 0;
	}

	bool		operator >	(const GFxString& str) const
	{
		return *this > str.pData->Data;
	}

    int CompareNoCase(const char* pstr) const
    {
        return CompareNoCase(pData->Data, pstr);
    }
    int CompareNoCase(const GFxString& str) const
    {
        return CompareNoCase(pData->Data, str.ToCStr());
    }

	// Accesses raw bytes
	char&		operator [] (int index) const
	{
		GASSERT(index >= 0 && (UPInt)index < GetSize());
		return pData->Data[index];
	}
    char&		operator [] (UPInt index) const
    {
        GASSERT(index < GetSize());
        return pData->Data[index];
    }


	// Case insensitive keys are used to look up insensitive string in hash tables
	// for SWF files with version before SWF 7.
	struct NoCaseKey
	{	
		const GFxString* pStr;
		NoCaseKey(const GFxString &str) : pStr(&str){};
	};

	bool    operator == (const NoCaseKey& strKey) const
	{
		return (CompareNoCase(ToCStr(), strKey.pStr->ToCStr()) == 0);
	}
	bool    operator != (const NoCaseKey& strKey) const
	{
		return !(CompareNoCase(ToCStr(), strKey.pStr->ToCStr()) == 0);
	}

    // Hash functor used for strings.
    struct HashFunctor
    {    
        size_t  operator()(const GFxString& data) const
        {
            size_t  size = data.GetSize();
            return GFxString::BernsteinHashFunction((const char*)data, size);
        }        
    };
    // Case-insensitive hash functor used for strings. Supports additional
    // lookup based on NoCaseKey.
    struct NoCaseHashFunctor
    {    
        size_t  operator()(const GFxString& data) const
        {
            size_t  size = data.GetSize();
            return GFxString::BernsteinHashFunctionCIS((const char*)data, size);
        }
        size_t  operator()(const NoCaseKey& data) const
        {		
            size_t  size = data.pStr->GetSize();
            return GFxString::BernsteinHashFunctionCIS((const char*)data.pStr->ToCStr(), size);
        }
    };


private:

	DataDesc*	AllocNewNode(UPInt newSizeRounded, int refCount, UPInt newSize, bool flag = false);

	//using memcpy is better than using strcpy because strcpy does extra checking
	void		copy(char* dest, const char* src, UPInt size){
		memcpy(dest, src, size);
		dest[size] = 0; //append null character
	}

	void        ResizeInternal(UPInt size, bool flag);

	void        InternalRemove(UPInt posAt, SPInt len = 1);
};



// ***** GFxWStringBuffer Class

// A class used to store a string buffer of wchar_t. This is used by GFxTranslator
// interface, or when a variable size Unicode character buffer is necessary.

class GFxWStringBuffer
{
public:

    // Reservation header, keeps size and reservation pointer. This structure allows
    // us to not have the rest of the class be a template.
    struct ReserveHeader
    {
        wchar_t* pBuffer;
        UPInt    Size;

        ReserveHeader() : pBuffer(0), Size(0) { }
        ReserveHeader(wchar_t* pbuffer, UPInt size) : pBuffer(pbuffer), Size(size) { }
        ReserveHeader(const ReserveHeader &other) : pBuffer(other.pBuffer), Size(other.Size) { }
    };
    // Buffer data reservation object. Creates a buffer of fixed size,
    // must be passed on constructor.
    template<unsigned int size>
    struct Reserve : public ReserveHeader
    {
        wchar_t  Buffer[size];
        Reserve() : ReserveHeader(Buffer, size) { }
    };

private:
    wchar_t*      pText;
    UPInt         Length;
    ReserveHeader Reserved;

public:

    GFxWStringBuffer()
        : pText(0), Length(0), Reserved(0,0) { }
    GFxWStringBuffer(const ReserveHeader& reserve)
        : pText(reserve.pBuffer), Length(0), Reserved(reserve) { }
    GFxWStringBuffer(const GFxWStringBuffer& other);

    ~GFxWStringBuffer();

    // Resize the buffer to desired size.
    bool     Resize(UPInt size);
    // Resets buffer contents to empty.
    void     Clear() { Resize(0); }

    inline const wchar_t* ToWStr() const { return (pText != 0) ? pText : L""; }
    inline UPInt    GetLength() const    { return Length; }
    inline wchar_t* GetBuffer() const    { return pText; }

    // Character access and mutation.
    inline wchar_t operator [] (UPInt index) const { GASSERT(pText && (index <= Length)); return pText[index]; }
    inline wchar_t& operator [] (UPInt index)            { GASSERT(pText && (index <= Length)); return pText[index]; }

    // Assign buffer data.
    GFxWStringBuffer& operator = (const GFxWStringBuffer& buff);
    GFxWStringBuffer& operator = (const GFxString& str);
    GFxWStringBuffer& operator = (const wchar_t *pstr);
    GFxWStringBuffer& operator = (const char* putf8str);

    void ResizeInternal(UPInt _size, bool flag = false);

    void SetString(const wchar_t* pstr, UPInt length = GFC_MAX_UPINT);
};


#endif
