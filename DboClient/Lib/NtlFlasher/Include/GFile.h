/**********************************************************************

Filename    :   GFile.h
Content     :   Header for all internal file management-
                functions and structures to be inherited by OS
                specific subclasses.
Created     :   July 29, 1998
Authors     :   Michael Antonov, Brendan Iribe, Andrew Reisse

Notes       :   errno may not be preserved across use of GBaseFile member functions
            :   Directories cannot be deleted while files opened from them are in use
                (For the GetFullName function)

History     :   4/04/1999   :   Changed GBaseFile to support directory
                1/20/1999   :   Updated to follow new style
                8/25/2001   :   MA - Restructured file, directory, added streams,
                                    item directories & paths

Copyright   :   (c) 1998-2003 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFILE_H
#define INC_GFILE_H

#include "GRefCount.h"
#include "GFunctions.h"
#include "GTypes.h"
//#include "GTime.h"
//#include "GStream.h"

#include <stdio.h>


// ***** Declared classes
class   GFileConstants;
class   GFile;
class   GDelegatedFile;
class   GBufferedFile;
class   GSysFile;
//class GFileStats;


// ***** Flags for File & Directory accesses

class GFileConstants
{
public:

    // *** File open flags
    enum OpenFlags
    {
        Open_Read       = 1,
        Open_Write      = 2,
        Open_ReadWrite  = 3,

        // Opens file and truncates it to zero length
        // - file must have write permission
        // - when used with Create, it opens an existing 
        //   file and empties it or creates a new file
        Open_Truncate   = 4,

        // Creates and opens new file 
        // - does not erase contents if file already
        //   exists unless combined with Truncate
        Open_Create     = 8,

         // Returns an error value if the file already exists
        Open_CreateOnly = 24,

        Open_NoFollow   = 32,

        // Only follow links that stay under this directory
        Open_NoFollowOut= 64
    };

    // *** File Mode flags
    enum Modes
    {
        Mode_Read       = 0444,
        Mode_Write      = 0222,
        Mode_Execute    = 0111,

        Mode_ReadWrite  = 0666
    };

    // *** Seek operations
    enum SeekOps
    {
        Seek_Set        = 0,
        Seek_Cur        = 1,
        Seek_End        = 2
    };

    // *** Errors
    enum Errors
    {
        Error_FileNotFound  = 0x1001,
        Error_Access        = 0x1002,
        Error_IOError       = 0x1003,
        Error_DiskFull      = 0x1004
    };
};


// ***** File Class


// The pure virtual base random-access file
// This is a base class to all files

class GFile : public GRefCountBase<GFile>, public GFileConstants
{   
public:
    // ** Location Information

    // Returns a file name path relative to the 'reference' directory                                           
    // This is often a path that was used to create a file                                                      
    // (this is not a global path, global path can be obtained with help of directory)                          
    virtual const char* GetFilePath()                                                       = 0;    
                                                                                        

    // ** File Information

    // Return 1 if file's usable (open)
    virtual bool        IsValid()                                                           = 0;
    // Return 1 if file's writable, otherwise 0                                         
    virtual bool        IsWritable()                                                        = 0;
    // Return 1 if file supports CloseCancel()                                          
    //virtual bool      IsRecoverable()                                                     = 0;

                                                                                        
    // Return position
    virtual SInt        Tell ()                                                             = 0;
    virtual SInt64      LTell ()                                                            = 0;
    
    // File size                                                                        
    virtual SInt        GetLength ()                                                        = 0;
    virtual SInt64      LGetLength ()                                                       = 0;                                                            
                                                                                        
    // Returns file stats                                                               
    // 0 for failure                                                                    
    //virtual bool      Stat(GFileStats *pfs)                                               = 0;
                                                                                        
    // Return errno-based error code                                                    
    // Useful if any other function failed                                              
    virtual SInt        GetErrorCode()                                                      = 0;
                                                                                        
                                                                                        
    // ** GFxStream implementation & I/O

    // Blocking write, will write in the given number of bytes to the stream
    // Returns : -1 for error
    //           Otherwise number of bytes read 
    virtual SInt        Write(const UByte *pbufer, SInt numBytes)                           = 0;    
    // Blocking read, will read in the given number of bytes or less from the stream
    // Returns : -1 for error
    //           Otherwise number of bytes read,
    //           if 0 or < numBytes, no more bytes available; end of file or the other side of stream is closed
    virtual SInt        Read(UByte *pbufer, SInt numBytes)                                  = 0;

    // Skips (ignores) a given # of bytes
    // Same return values as Read
    virtual SInt        SkipBytes(SInt numBytes)                                            = 0;
        
    // Returns the number of bytes available to read from a stream without blocking
    // For a file, this should generally be number of bytes to the end
    virtual SInt        BytesAvailable()                                                    = 0;

    // Causes any implementation's buffered data to be delivered to destination
    // Return 0 for error
    virtual bool        Flush()                                                             = 0;
                                                                                            

    // Need to provide a more optimized implementation that doe snot necessarily involve a lot of seeking
    GINLINE bool        IsEOF()
        { return !BytesAvailable(); }
    

    // Seeking                                                                              
    // Returns new position, -1 for error                                                   
    virtual SInt        Seek(SInt offset, SInt origin=Seek_Set)                             = 0;
    virtual SInt64      LSeek(SInt64 offset, SInt origin=Seek_Set)                          = 0;
    // Seek simplification
    SInt                SeekToBegin()           {return Seek(0); }
    SInt                SeekToEnd()             {return Seek(0,Seek_End); }
    SInt                Skip(SInt numBytes)     {return Seek(numBytes,Seek_Cur); }
                        
    // Resizing the file                                    
    // Return 0 for failure
    virtual bool        ChangeSize(SInt newSize)                                            = 0;

    // Appends other file data from a stream
    // Return -1 for error, else # of bytes written
    virtual SInt        CopyFromStream(GFile *pstream, SInt byteSize)                       = 0;

    // Closes the file
    // After close, file cannot be accessed 
    virtual bool        Close()                                                             = 0;
    // Closes the file & recovers it to initial state, if supported
    //virtual bool      CloseCancel()                                                       = 0;


    // ***** Inlines for convenient primitive type serialization

    // Read/Write helpers
private:
    UInt64  PRead64()           { UInt64 v = 0; Read((UByte*)&v, 8); return v; }
    UInt32  PRead32()           { UInt32 v = 0; Read((UByte*)&v, 4); return v; }
    UInt16  PRead16()           { UInt16 v = 0; Read((UByte*)&v, 2); return v; }
    UInt8   PRead8()            { UInt8  v = 0; Read((UByte*)&v, 1); return v; }
    void    PWrite64(UInt64 v)  { Write((UByte*)&v, 8); }
    void    PWrite32(UInt32 v)  { Write((UByte*)&v, 4); }
    void    PWrite16(UInt16 v)  { Write((UByte*)&v, 2); }
    void    PWrite8(UInt8 v)    { Write((UByte*)&v, 1); }

public:

    // Writing primitive types - Little Endian
    GINLINE void    WriteUByte(UByte v)         { PWrite8((UInt8)GByteUtil::SystemToLE(v));     }
    GINLINE void    WriteSByte(SByte v)         { PWrite8((UInt8)GByteUtil::SystemToLE(v));     }
    GINLINE void    WriteUInt8(UInt8 v)         { PWrite8((UInt8)GByteUtil::SystemToLE(v));     }
    GINLINE void    WriteSInt8(SInt8 v)         { PWrite8((UInt8)GByteUtil::SystemToLE(v));     }
    GINLINE void    WriteUInt16(UInt16 v)       { PWrite16((UInt16)GByteUtil::SystemToLE(v));   }
    GINLINE void    WriteSInt16(SInt16 v)       { PWrite16((UInt16)GByteUtil::SystemToLE(v));   }
    GINLINE void    WriteUInt32(UInt32 v)       { PWrite32((UInt32)GByteUtil::SystemToLE(v));   }
    GINLINE void    WriteSInt32(SInt32 v)       { PWrite32((UInt32)GByteUtil::SystemToLE(v));   }
    GINLINE void    WriteUInt64(UInt64 v)       { PWrite64((UInt64)GByteUtil::SystemToLE(v));   }
    GINLINE void    WriteSInt64(SInt64 v)       { PWrite64((UInt64)GByteUtil::SystemToLE(v));   }
    GINLINE void    WriteFloat(Float v)         { v = GByteUtil::SystemToLE(v); Write((UByte*)&v, 4); } 
    GINLINE void    WriteDouble(Double v)       { v = GByteUtil::SystemToLE(v); Write((UByte*)&v, 8); }
    // Writing primitive types - Big Endian
    GINLINE void    WriteUByteBE(UByte v)       { PWrite8((UInt8)GByteUtil::SystemToBE(v));     }
    GINLINE void    WriteSByteBE(SByte v)       { PWrite8((UInt8)GByteUtil::SystemToBE(v));     }
    GINLINE void    WriteUInt8BE(UInt16 v)      { PWrite8((UInt8)GByteUtil::SystemToBE(v));     }
    GINLINE void    WriteSInt8BE(SInt16 v)      { PWrite8((UInt8)GByteUtil::SystemToBE(v));     }
    GINLINE void    WriteUInt16BE(UInt16 v)     { PWrite16((UInt16)GByteUtil::SystemToBE(v));   }
    GINLINE void    WriteSInt16BE(UInt16 v)     { PWrite16((UInt16)GByteUtil::SystemToBE(v));   }
    GINLINE void    WriteUInt32BE(UInt32 v)     { PWrite32((UInt32)GByteUtil::SystemToBE(v));   }
    GINLINE void    WriteSInt32BE(UInt32 v)     { PWrite32((UInt32)GByteUtil::SystemToBE(v));   }
    GINLINE void    WriteUInt64BE(UInt64 v)     { PWrite64((UInt64)GByteUtil::SystemToBE(v));   }
    GINLINE void    WriteSInt64BE(UInt64 v)     { PWrite64((UInt64)GByteUtil::SystemToBE(v));   }
    GINLINE void    WriteFloatBE(Float v)       { v = GByteUtil::SystemToBE(v); Write((UByte*)&v, 4); }
    GINLINE void    WriteDoubleBE(Double v)     { v = GByteUtil::SystemToBE(v); Write((UByte*)&v, 8); }
        
    // Reading primitive types - Little Endian
    GINLINE UByte   ReadUByte()                 { return (UByte)GByteUtil::LEToSystem(PRead8());    }
    GINLINE SByte   ReadSByte()                 { return (SByte)GByteUtil::LEToSystem(PRead8());    }
    GINLINE UInt8   ReadUInt8()                 { return (UInt8)GByteUtil::LEToSystem(PRead8());    }
    GINLINE SInt8   ReadSInt8()                 { return (SInt8)GByteUtil::LEToSystem(PRead8());    }
    GINLINE UInt16  ReadUInt16()                { return (UInt16)GByteUtil::LEToSystem(PRead16());  }
    GINLINE SInt16  ReadSInt16()                { return (SInt16)GByteUtil::LEToSystem(PRead16());  }
    GINLINE UInt32  ReadUInt32()                { return (UInt32)GByteUtil::LEToSystem(PRead32());  }
    GINLINE SInt32  ReadSInt32()                { return (SInt32)GByteUtil::LEToSystem(PRead32());  }
    GINLINE UInt64  ReadUInt64()                { return (UInt64)GByteUtil::LEToSystem(PRead64());  }
    GINLINE SInt64  ReadSInt64()                { return (SInt64)GByteUtil::LEToSystem(PRead64());  }
    GINLINE Float   ReadFloat()                 { Float v = 0.0f; Read((UByte*)&v, 4); return GByteUtil::LEToSystem(v); }
    GINLINE Double  ReadDouble()                { Double v = 0.0; Read((UByte*)&v, 8); return GByteUtil::LEToSystem(v); }
    // Reading primitive types - Big Endian
    GINLINE UByte   ReadUByteBE()               { return (UByte)GByteUtil::BEToSystem(PRead8());    }
    GINLINE SByte   ReadSByteBE()               { return (SByte)GByteUtil::BEToSystem(PRead8());    }
    GINLINE UInt8   ReadUInt8BE()               { return (UInt8)GByteUtil::BEToSystem(PRead8());    }
    GINLINE SInt8   ReadSInt8BE()               { return (SInt8)GByteUtil::BEToSystem(PRead8());    }
    GINLINE UInt16  ReadUInt16BE()              { return (UInt16)GByteUtil::BEToSystem(PRead16());  }
    GINLINE SInt16  ReadSInt16BE()              { return (SInt16)GByteUtil::BEToSystem(PRead16());  }
    GINLINE UInt32  ReadUInt32BE()              { return (UInt32)GByteUtil::BEToSystem(PRead32());  }
    GINLINE SInt32  ReadSInt32BE()              { return (SInt32)GByteUtil::BEToSystem(PRead32());  }
    GINLINE UInt64  ReadUInt64BE()              { return (UInt64)GByteUtil::BEToSystem(PRead64());  }
    GINLINE SInt64  ReadSInt64BE()              { return (SInt64)GByteUtil::BEToSystem(PRead64());  }
    GINLINE Float   ReadFloatBE()               { Float v = 0.0f; Read((UByte*)&v, 4); return GByteUtil::BEToSystem(v); }
    GINLINE Double  ReadDoubleBE()              { Double v = 0.0; Read((UByte*)&v, 8); return GByteUtil::BEToSystem(v); }
};


// *** Delegated File

class GDelegatedFile : public GFile
{
protected:
    // Delegating file pointer
    GPtr<GFile>     pFile;

    // Hidden default constructor
    GDelegatedFile()                                { pFile=0; }    
    GDelegatedFile(const GDelegatedFile &source) : GFile()    { GUNUSED(source); }
public:
    // Constructors
    GDelegatedFile(GFile *pfile) : pFile(pfile)     { }

    // ** Location Information  
    virtual const char* GetFilePath()                                           { return pFile->GetFilePath(); }    

    // ** File Information                                                      
    virtual bool        IsValid()                                               { return pFile && pFile->IsValid(); }   
    virtual bool        IsWritable()                                            { return pFile->IsWritable(); }
//  virtual bool        IsRecoverable()                                         { return pFile->IsRecoverable(); }          
                                                                                
    virtual SInt        Tell()                                                  { return pFile->Tell(); }
    virtual SInt64      LTell()                                                 { return pFile->LTell(); }
    
    virtual SInt        GetLength()                                             { return pFile->GetLength(); }
    virtual SInt64      LGetLength()                                            { return pFile->LGetLength(); }
    
    //virtual bool      Stat(GFileStats *pfs)                                   { return pFile->Stat(pfs); }
    
    virtual SInt        GetErrorCode()                                          { return pFile->GetErrorCode(); }
    
    // ** GFxStream implementation & I/O
    virtual SInt        Write(const UByte *pbuffer, SInt numBytes)              { return pFile->Write(pbuffer,numBytes); }  
    virtual SInt        Read(UByte *pbuffer, SInt numBytes)                     { return pFile->Read(pbuffer,numBytes); }   
    
    virtual SInt        SkipBytes(SInt numBytes)                                { return pFile->SkipBytes(numBytes); }      
    
    virtual SInt        BytesAvailable()                                        { return pFile->BytesAvailable(); } 
    
    virtual bool        Flush()                                                 { return pFile->Flush(); }
                                                                                
    // Seeking                                                                  
    virtual SInt        Seek(SInt offset, SInt origin=Seek_Set)                 { return pFile->Seek(offset,origin); }
    virtual SInt64      LSeek(SInt64 offset, SInt origin=Seek_Set)              { return pFile->LSeek(offset,origin); }

    // Resizing the file            
    virtual bool        ChangeSize(SInt newSize)                                { return pFile->ChangeSize(newSize); }  
    virtual SInt        CopyFromStream(GFile *pstream, SInt byteSize)           { return pFile->CopyFromStream(pstream,byteSize); }
                        
    // Closing the file 
    virtual bool        Close()                                                 { return pFile->Close(); }  
    //virtual bool      CloseCancel()                                           { return pFile->CloseCancel(); }
};


// *** Buffered File

// This file class adds buffering to an existing file
// Buffered file never fails by itself; if there's not
// enough memory for buffer, no buffer's used

class GBufferedFile : public GDelegatedFile
{   
protected:  
    enum BufferModeType
    {
        NoBuffer,
        ReadBuffer,
        WriteBuffer
    };

    // Buffer & the mode it's in
    UByte*          pBuffer;
    BufferModeType  BufferMode;
    // Position in buffer
    UInt            Pos;
    // Data in buffer if reading
    UInt            DataSize;
    // Underlying file position 
    UInt64          FilePos;

    // Initializes buffering to a certain mode
    GEXPORT bool    SetBufferMode(BufferModeType mode);
    // Flushes buffer
    // WriteBuffer - write data to disk, ReadBuffer - reset buffer & fix file position  
    GEXPORT void    FlushBuffer();
    // Loads data into ReadBuffer
    // WARNING: Right now LoadBuffer() assumes the buffer's empty
    GEXPORT void    LoadBuffer();

    // Hidden constructor
    GEXPORT GBufferedFile();
    GINLINE GBufferedFile(const GBufferedFile &source) : GDelegatedFile() { GUNUSED(source); }
public:

    // Constructor
    // - takes another file as source
    GEXPORT GBufferedFile(GFile *pfile);
    GEXPORT ~GBufferedFile();
    
    
    // ** Overridden functions

    // We override all the functions that can possibly
    // require buffer mode switch, flush, or extra calculations
    GEXPORT virtual SInt        Tell();
    GEXPORT virtual SInt64      LTell();

    GEXPORT virtual SInt        GetLength();
    GEXPORT virtual SInt64      LGetLength();

//  GEXPORT virtual bool        Stat(GFileStats *pfs);  

    GEXPORT virtual SInt        Write(const UByte *pbufer, SInt numBytes);
    GEXPORT virtual SInt        Read(UByte *pbufer, SInt numBytes);

    GEXPORT virtual SInt        SkipBytes(SInt numBytes);

    GEXPORT virtual SInt        BytesAvailable();

    GEXPORT virtual bool        Flush();

    GEXPORT virtual SInt        Seek(SInt offset, SInt origin=Seek_Set);
    GEXPORT virtual SInt64      LSeek(SInt64 offset, SInt origin=Seek_Set);

    GEXPORT virtual bool        ChangeSize(SInt newSize);
    GEXPORT virtual SInt        CopyFromStream(GFile *pstream, SInt byteSize);
    
    GEXPORT virtual bool        Close();
    //GEXPORT   virtual bool        CloseCancel();
};                          



// *** File Statistics

// This class contents are similar to _stat, providing
// creation, modify and other information about the file.
struct GFileStat
{
    SInt64  CreateTime;
    SInt64  ModifyTime;
    SInt64  AccessTime;
    SInt64  FileSize;

    bool operator== (const GFileStat& stat) const
    {
        return ( (CreateTime == stat.CreateTime) &&
                 (ModifyTime == stat.ModifyTime) &&
                 (AccessTime == stat.AccessTime) &&
                 (FileSize == stat.FileSize) );
    }
};


// *** System File

#ifdef GFC_USE_SYSFILE

// System file is created to access objects on file system directly
// This file can refer directly to path.
// System file can be open & closed several times; however, such use is not recommended

class GSysFile : public GDelegatedFile
{
protected:
  GSysFile(const GSysFile &source) : GDelegatedFile () { GUNUSED(source); }
public:

    // ** Constructor
    GEXPORT GSysFile();
    // Opens a file
    GEXPORT GSysFile(const char *ppath, SInt flags =Open_Read, SInt mode = Mode_ReadWrite); 

    // ** Open & management 
    // Will fail if file's already open
    GEXPORT bool    Open(const char *ppath, SInt flags =Open_Read, SInt mode = Mode_ReadWrite);
        
    GINLINE bool    Create(const char *ppath, SInt mode = Mode_ReadWrite)
        {   return Open(ppath, Open_ReadWrite|Open_Create, mode); }

    // Helper function: obtain file statistics information. In GFx, this is used to detect file changes.
    // Return 0 if function failed, most likely because the file doesn't exist.
    GEXPORT static bool    GetFileStat(GFileStat* pfileStats, const char* ppath);
    
    // ** Overrides
    // Overridden to provide re-open support
    GEXPORT virtual SInt    GetErrorCode();

    GEXPORT virtual bool    IsValid();

    GEXPORT virtual bool    Close();
    //GEXPORT   virtual bool    CloseCancel();
};

#endif // GFC_USE_SYSFILE

#ifdef GFC_OS_WII
GFile *GFileWiiDvdOpen(const char *ppath, SInt flags=GFile::Open_Read);
#elif defined(GFC_OS_PS2)
GFile *GFilePS2Open(const char *ppath, SInt flags=GFile::Open_Read);
#elif defined(GFC_OS_PSP)
GFile *GFilePSPOpen(const char *ppath, SInt flags=GFile::Open_Read);
#endif


// *** Memory File

class GMemoryFile : public GFile
{
public:

    const char *GetFilePath()       { return FilePath; }

    bool        IsValid()           { return Valid; }
    bool        IsWritable()        { return false; }

    bool        Flush()             { return true; }
    SInt        GetErrorCode()      { return 0; }

    SInt        Tell()              { return FileIndex; }
    SInt64      LTell()             { return (SInt64) FileIndex; }

    SInt        GetLength()         { return FileSize; }
    SInt64      LGetLength()        { return (SInt64) FileSize; }

    bool        Close()
    {
        Valid = false;
        return false;
    }

    SInt        CopyFromStream(GFile *pstream, SInt byteSize)
    {   GUNUSED2(pstream, byteSize);
        return 0;
    }

    SInt        Write(const UByte *pbuffer, SInt numBytes)
    {   GUNUSED2(pbuffer, numBytes);
        return 0;
    }

    SInt        Read(UByte *pbufer, SInt numBytes)
    {
        if (FileIndex + numBytes > FileSize)
        {
            numBytes = FileSize - FileIndex;
        }

        if (numBytes > 0)
        {
            ::memcpy (pbufer, &FileData [FileIndex], numBytes);

            FileIndex += numBytes;
        }

        return numBytes;
    }

    SInt        SkipBytes(SInt numBytes)
    {
        if (FileIndex + numBytes > FileSize)
        {
            numBytes = FileSize - FileIndex;
        }

        FileIndex += numBytes;

        return numBytes;
    }

    SInt        BytesAvailable()
    {
        return (FileSize - FileIndex);
    }

    SInt        Seek(SInt offset, SInt origin = Seek_Set)
    {
        switch (origin)
        {
        case Seek_Set : FileIndex  = offset;               break;
        case Seek_Cur : FileIndex += offset;               break;
        case Seek_End : FileIndex  = FileSize - offset;  break;
        }

        return FileIndex;
    }

    SInt64      LSeek(SInt64 offset, SInt origin = Seek_Set)
    {
        return (SInt64) Seek((SInt) offset, origin);
    }

    bool        ChangeSize(SInt newSize)
    {
        FileSize = newSize;
        return true;
    }

public:

   GMemoryFile (const char *pFileName, const UByte *pBuffer, SInt buffSize)
    {
        FilePath  = pFileName;
        FileData  = pBuffer;
        FileSize  = buffSize;
        FileIndex = 0;
        Valid     = (pFileName && pBuffer && buffSize > 0) ? true : false;
    }

private:

    const char  *FilePath;
    const UByte *FileData;
    SInt         FileSize;
    SInt         FileIndex;
    bool         Valid;
};

#endif
