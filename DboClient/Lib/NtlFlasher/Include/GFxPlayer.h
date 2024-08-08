/**********************************************************************

Filename    :   GFxPlayer.h
Content     :   Public interface to SWF File playback
Created     :   June 21, 2005
Authors     :   Michael Antonov

Notes       :

Copyright   :   (c) 2005-2007 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFXPLAYER_H
#define INC_GFXPLAYER_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GFxRenderConfig.h"
#include "GRenderer.h"
#include "GFxEvent.h"
#include "GFxLoader.h"

#include <ctype.h>  // for wchar_t
#include <stdarg.h> // for va_list args


// ***** Declared Classes
class GFxMovieDef;
class GFxMovie;
class GFxMovieView;

// ***** External Classes
class GFile;
class GColor;
class GImageInfoBase;
class GFxLog;


// ***** Movie Definition and Instance interfaces

// GFxMovieDef represents loaded shared data for an SWF movie file. These
// objects are normally created by GFxLoader::CreateMovie and are shared
// by all movie instances. If the movie definition is registered in the
// library during load time (see GFxLoader::LoadUseLibrary flag), loading
// the same file again will return a pointer to the same cached GFxMovieDef.
//
// Individual movie instances can be created by GFxMovieDef::CreateInstance.
// Once the instance is created, it is normally initialized with a
// renderer and used during playback.

class GFxMovieDef : public GFxResource, public GFxSharedState
{
public:

    // Query SWF file version, dimensions, and other stats.
    virtual UInt        GetVersion() const              = 0;
    virtual Float       GetWidth() const                = 0;
    virtual Float       GetHeight() const               = 0;
    virtual UInt        GetFrameCount() const           = 0;
    virtual Float       GetFrameRate() const            = 0;
    // Returns frame rectangle, in pixels
    virtual GRectF      GetFrameRect() const            = 0;
    // Returns SWF file flags defined by GFxMovieInfo::SWFFlagConstants.
    virtual UInt        GetSWFFlags() const             = 0;
    // Returns the file path/URL from which this SWF was loaded.
    // This will normally be the path passed to GFxLoader::CreateMovie.
    virtual const char* GetFileURL() const              = 0;

    // SWF 8 File attributes, returned by GetFileAttributes
    enum FileAttrFlags
    {
        FileAttr_UseNetwork     = 0x0001,
        FileAttr_HasMetadata    = 0x0010
    };

    // Returns file attributes for SWF file version 8+. Will
    // return 0 for earlier SWF files.
    virtual UInt        GetFileAttributes() const                       = 0;
    // Retrieves meta-data that can be embedded into SWF-8 files. Returns
    // the number of bytes written, up to buffSize. Specify 0 for pbuff
    // to return the buffer size required.
    virtual UInt        GetMetadata(char *pbuff, UInt buffSize) const   = 0;


    // Exporter/strip tool information - returns data about image extraction
    // arguments passed to 'gfxexport' tool when generating this '.gfx' file.
    // Null will be returned if this movie def came from a '.swf'.
    virtual const GFxExporterInfo*  GetExporterInfo() const             = 0;


    // Creates a movie view instance, that is used for playback. Each movie
    // instance will have its own timeline, animation and Action-Script state.
    //
    // The returned created movie is AddRef-ed, so it can be stored in
    // a smart pointer and released when no longer necessary. Null pointer
    // will be returned in case of failure (low memory, etc.).
    // Specify initFirstFrame = 1 to initialize objects of frame 1, populating 
    // display list. Otherwise, display list will be empty.
    // Ex:  GPtr<GFxMovieView> pmovie = *pdef->CreateInstance();
    virtual GFxMovieView*   CreateInstance(bool initFirstFrame = 1)     = 0;


    // ***  Import interface

    // Import support interface. SWF files can import dictionary symbols from
    // other SWF files. This is done automatically if GFxLoader::LoadImports flag
    // is specified when loading a movie (default for GFxLoader::CreateMovie);
    // however, if it is not specified this interface can be used to manually
    // enumerate and resolve imports.

    // Visitor interface, used for movie enumeration through VisitImportedMovies.
    struct ImportVisitor
    {
        virtual ~ImportVisitor () { }

        // Users must override Visit.
        //  - pimportedMovieFilename is the partial filename being used for import;
        //    to get the full path use pimportDef->GetFileURL().
        virtual void    Visit(GFxMovieDef* pparentDef, GFxMovieDef* pimportDef,
                              const char* pimportedMovieFilename) = 0;
    };

    // Enumerates a list of names of external SWF files imported into this movie.
    // Calls pvistor->Visit() method for each import movie file name.
    virtual void    VisitImportedMovies(ImportVisitor* pvisitor)            = 0;



    // *** Resource enumeration

    // Resource enumeration is used primarily by the GFxExport tool to collect
    // all of the resources within the file.

    // Visitor interface, used for movie enumeration through VisitImportedMovies.
    struct ResourceVisitor : public GFxFileConstants
    {
        virtual ~ResourceVisitor () { }       

        virtual void    Visit(GFxMovieDef* pmovieDef, GFxResource* presource,
                              GFxResourceId rid, const char* pexportName) = 0;
    };

    enum VisitResourceMask
    {
        ResVisit_NestedMovies   = 0x8000,

        // Types of resources to visit:
        ResVisit_Fonts          = 0x01,
        ResVisit_Bitmaps        = 0x02,
        ResVisit_GradientImages = 0x04,
        ResVisit_EditTextFields = 0x08,
        
        // Combined flags.
        ResVisit_AllLocalImages = (ResVisit_Bitmaps | ResVisit_GradientImages),
        ResVisit_AllImages      = (ResVisit_Bitmaps | ResVisit_GradientImages | ResVisit_NestedMovies)
    };
    
    // Enumerates all resources.
    // Calls pvistor->Visit() method for each resource in the SWF/GFX file.
    virtual void    VisitResources(ResourceVisitor* pvisitor, UInt visitMask = ResVisit_AllImages) = 0;


    /*
    // Fills in extracted image file info for a given characterId. Character
    // ids are passed to ImageVisitor::Visit method. Returns 0 if such
    // information is not available.
    virtual bool    GetImageFileInfo(GFxImageFileInfo *pinfo, int characterId)      = 0;
    */

    // Retrieves a resource pointer based on an export name. Export name is specified in
    // the flash studio by right-clicking on the bitmap in the library and
    // modifying its Linkage identifier attribute. The symbol must also be
    // exported for runtime or ActionScript sharing.
    virtual GFxResource*     GetResource(const char *pexportName) const         = 0;

};

// ***** GFxImportVisitor

// Default import visitor - used to enumerate imports. This visitor is called 
// during loading (import binding stage) after a each import is resolved.
class   GFxImportVisitor : public GFxState, public GFxMovieDef::ImportVisitor, public GFxFileConstants
{
public:    
    GFxImportVisitor() : GFxState(State_ImportVisitor) { }       
    virtual ~GFxImportVisitor () { }       

    // Users must override Visit.
    //  - pimportedMovieFilename is the partial filename being used for import;
    //    to get the full path use pimportDef->GetFileURL().
    virtual void    Visit(GFxMovieDef* pparentDef, GFxMovieDef* pimportDef,
                          const char* pimportedMovieFilename) = 0;
};

// Sets a default visitor that is used when after files are loaded.
inline  void GFxSharedState::SetImportVisitor(GFxImportVisitor *ptr)
{
    SetState(GFxState::State_ImportVisitor, ptr);
}
inline  GPtr<GFxImportVisitor> GFxSharedState::GetImportVisitor() const
{
    return *(GFxImportVisitor*) GetStateAddRef(GFxState::State_ImportVisitor);
}




// ***** GFxValue - used to pass/recieve values from ActionScript.

// GFxValue holds a simple type and the value.
// No buffer is allocated for strings, so string data must be managed externally.
// In case of GetVariable() and Invoke(), strings are available until the next
// call to a similar function in GFxMovieView. If the movie view dies, those
// values also become invalid.

class GFxValue
{
public:

    // The type of value stored in GFxValue.
    enum ValueType
    {
        VT_Undefined,
        VT_Null,
        VT_Boolean,
        VT_Number,
        VT_String,

        // StringW can be passed as an argument type, but it will only be returned 
        // if VT_ConvertStringW was specified, as it is not a native type.
        VT_StringW,  // wchar_t* string
        
        // Specify this type to request SetVariable/Invoke result to be converted 
        // to the specified type. After return of the function, the specified type
        // will *ALWAYS* be stored in the value.
        VT_ConvertBit       = 0x10,
        VT_ConvertBoolean   = VT_ConvertBit | VT_Boolean,
        VT_ConvertNumber    = VT_ConvertBit | VT_Number,
        VT_ConvertString    = VT_ConvertBit | VT_String,
        VT_ConvertStringW   = VT_ConvertBit | VT_StringW,
    };

    // Constructors.
    GFxValue()                      : Type(VT_Undefined)  { }
    GFxValue(ValueType type)        : Type(type)          { Value.pString = 0; }
    GFxValue(Double v)              : Type(VT_Number)     { Value.NValue = v; }
    GFxValue(const char* ps)        : Type(VT_String)     { Value.pString = ps; }
    GFxValue(const wchar_t* ps)     : Type(VT_StringW)    { Value.pStringW = ps; }
    GFxValue(const GFxValue& src)   : Type(src.Type)      { Value = src.Value; }

    const GFxValue& operator = (const GFxValue& src) { Type = src.Type; Value = src.Value; return *this; }

    // Get type based on which you can interpret the value.
    inline ValueType   GetType() const         { return Type; }

    // Get values for each type.
    inline bool        GetBool() const         { GASSERT(Type == VT_Boolean); return Value.BValue; }
    inline Double      GetNumber() const       { GASSERT(Type == VT_Number); return Value.NValue;  }
    inline const char* GetString() const       { GASSERT(Type == VT_String); return Value.pString; }
    inline const wchar_t* GetStringW() const   { GASSERT(Type == VT_StringW); return Value.pStringW; }

    // Set types and values.
    inline void        SetUndefined()           { Type = VT_Undefined; }
    inline void        SetNull()                { Type = VT_Null; }
    inline void        SetBoolean(bool v)       { Type = VT_Boolean; Value.BValue = v; }
    inline void        SetNumber(Double v)      { Type = VT_Number; Value.NValue = v; }
    inline void        SetString(const char* p) { Type = VT_String; Value.pString = p; }
    inline void        SetStringW(const wchar_t* p) { Type = VT_StringW; Value.pStringW = p; }

    inline void        SetConvertBoolean()      { Type = VT_ConvertBoolean; }
    inline void        SetConvertNumber()       { Type = VT_ConvertNumber; }
    inline void        SetConvertString()       { Type = VT_ConvertString; }
    inline void        SetConvertStringW()      { Type = VT_ConvertStringW; }

protected:
    union ValueUnion
    {
        Double      NValue;
        bool        BValue;
        const char* pString;
        const wchar_t* pStringW;
    };

    ValueType   Type;
    ValueUnion  Value;
};




//
// GFxMovieView and GFxMovie classes provide the client program's interface to
// an instance of a movie (i.e. an independent movie with a separate timeline
// animation state).
//
// GFxMovie interface is separated from GFxMovieView because it can also
// represent nested movie clips (not exposed in this version). Developers
// can use methods in both classes to manipulate the movie.


class GFxMovie : public GRefCountBase<GFxMovie>
{
public:

    // Obtain the movie definition that created us.
    virtual GFxMovieDef*    GetMovieDef() const                     = 0;

    // Delegated for convenience.
    GINLINE UInt    GetFrameCount() const   { return GetMovieDef()->GetFrameCount(); }
    GINLINE Float   GetFrameRate() const    { return GetMovieDef()->GetFrameRate(); }

    // Frame counts in this API are 0-Based (unlike ActionScript)
    virtual UInt    GetCurrentFrame() const                         = 0;
    virtual bool    HasLooped() const                               = 0;


    // Moves a playhead to a specified frame. Note that calling this method
    // may cause ActionScript tags attached to frames to be bypassed.
    virtual void    GotoFrame(UInt frameNumber)                     = 0;
    // Returns true if labeled frame is found.
    virtual bool    GotoLabeledFrame(const char* plabel, SInt offset = 0) = 0;

    enum PlayState
    {
        Playing,
        Stopped
    };
    // Changes playback state, allowing animation to be paused
    // and resumed. Setting the state to Stopped will prevent Advance from
    // advancing movie frames.
    virtual void        SetPlayState(PlayState s)                   = 0;
    virtual PlayState   GetPlayState() const                        = 0;


    // Visibility control.
    // Make the movie visible/invisible.  An invisible movie does not
    // advance and does not render.
    virtual void    SetVisible(bool visible)                        = 0;
    virtual bool    GetVisible() const                              = 0;



    // *** Action Script Interface
    
    // Checks for availability of a field/method/nested clip. This can be used to
    // determine if a certain variable can be accessed, or an Invoke can be called
    // without an unexpected failure warning.
    virtual bool    IsAvailable(const char* ppathToVar) const       = 0;


    // Variable Access

    // Set an ActionScript variable within this movie. This can be used to
    // to set the value of text fields, variables, and properties of
    // named nested characters within the movie.

    enum SetVarType
    {
        SV_Normal,      // Sets variable only if target clip is found.
        SV_Sticky,      // Sets variable if target clip is found, otherwise
                        // queues a set until the clip is created at path.
                        // When the target clip, value will be lost.
        SV_Permanent    // Sets variable applied to this and all future
                        // clips at given path.
    };

    // Set a variable identified by a path to a new value.
    // Specifying "sticky" flag will cause the value assignment to be pending until its target 
    // object is created. This allows initialization of objects created in later key-frames.
    // SetVariable will fail if path is invalid.
    virtual bool    SetVariable(const char* ppathToVar, const GFxValue& value, SetVarType setType = SV_Sticky)     = 0;
    
    // Obtain a variable. If *pval has a VT_Convert type, the result will always be filled in with that type,
    // even if GetVariable fails. Otherwise, the value is untouched on failure.
    virtual bool    GetVariable(GFxValue *pval, const char* ppathToVar) const                                      = 0;
    
    // Convenience inline methods for variable access.
  
    // Set a variable identified by a path to a new value, in UTF8.
    inline bool    SetVariable(const char* ppathToVar, const char* pvalue, SetVarType setType = SV_Sticky);
    // Unicode string version of SetVariable for convenience.
    inline bool    SetVariable(const char* ppathToVar, const wchar_t* pvalue, SetVarType setType = SV_Sticky);
    // Sets a variable to a Double value.
    inline bool    SetVariableDouble(const char* ppathToVar, Double value, SetVarType setType = SV_Sticky);
       
    // Get the value of an ActionScript variable. Stores a value in a temporary GFxMovie buffer
    // in UTF8 format. The buffer is overwritten by the next call.
    inline const char* GetVariable(const char* ppathToVar) const;
    inline const wchar_t* GetVariableStringW(const char* ppathToVar) const;
    // Gets a variable as a Double; return 0.0 if path was not found.
    inline Double  GetVariableDouble(const char* ppathToVar) const;
    

    // Variable Array Access

    // Type of array being set.
    enum SetArrayType
    {
        SA_Int,     // Array of 'SInt'
        SA_Double,  // Array of 'Double' in memory
        SA_Float,   // Array of 'Float' in memory
        SA_String,  // Array of 'const char*'.
        SA_StringW, // Array of 'const wchar_t*'.
        SA_Value    // Array of GFxValue.
    };

    // Sets array elements in specified range to data items of specified type.
    // If array does not exist, it is created. If an array already exists but
    // does not contain enough items, it is resized appropriately. Under some
    // conditions index > 0 can cause new elements to be inserted into the
    // array. These elements are initialized to 0 for numeric types, empty
    // string "" for strings, and 'undefined' for values. SetVariableArray 
    // can fail if there is not enough memory or if the path is invalid.
    virtual bool    SetVariableArray(SetArrayType type, const char* ppathToVar,
                                     UInt index, const void* pdata, UInt count, SetVarType setType = SV_Sticky) = 0;

    // Sets array's size. If array doesn't exists it will allocate array and resize it. 
    // No data will be assigned to its elements.
    virtual bool    SetVariableArraySize(const char* ppathToVar, UInt count, SetVarType setType = SV_Sticky) = 0;

    // Returns the size of array buffer necessary for GetVariableArray.
    virtual UInt    GetVariableArraySize(const char* ppathToVar)                                                = 0;
    
    // Populates a buffer with results from an array. The pdata pointer must
    // contain enough space for count items. If a variable path is not found
    // or is not an array, 0 will be returned an no data written to buffer.
    // The buffers allocated for string content are temporary within the
    // GFxMovieView, both when SA_String and SA_Value types are used. These
    // buffers may be overwritten after the next call to GetVariableArray;
    // they will also be lost if GFxMovieView dies. This means that users
    // should copy any string data immediately after the call, as access of 
    // it later may cause a crash.
    virtual bool    GetVariableArray(SetArrayType type, const char* ppathToVar,
                                     UInt index, void* pdata, UInt count)                                       = 0;

    // Convenience methods for GFxValue.
    inline  bool    SetVariableArray(const char* ppathToVar, UInt index, const GFxValue* pdata, UInt count,
                                     SetVarType setType = SV_Sticky);
    inline  bool    GetVariableArray(const char* ppathToVar, UInt index, GFxValue* pdata, UInt count);    


    // ActionScript Invoke

    // ActionScript method invoke interface. There are two different
    // approaches to passing data into Invoke:
    //  1) Using an array of GFxValue objects (slightly more efficient).
    //  2) Using a format string followed by a variable argument list.
    //
    // Return values are stored in GFxValue object, with potential conversion
    // to UTF8 strings or other types.  If a string is returned, it is stored
    // within a temporary  buffer in GFxMovieView and should be copied
    // immediately (if required by user). The value in a temporary buffer may
    // be overwritten by the next call to GetVariable or Invoke.
    //
    // Use case (1):
    //
    //   GFxValue args[3], result;
    //   args[0].SetNumber(i);
    //   args[1].SetString("test");
    //   args[2].SetNumber(3.5);
    //
    //   pMovie->Invoke("path.to.methodName", &result, args, 3);
    //
    // Use case (2):
    //
    //   pmovie->Invoke("path.to.methodName", "%d, %s, %f", i, "test", 3.5);
    //
    // The printf style format string describes arguments that follow in the
    // same  order; it can include commas and whitespace characters that are
    // ignored. Valid format arguments are:
    //
    //  %u      - undefined value, no argument required
    //  %d      - integer argument
    //  %s      - null-terminated char* string argument
    //  %ls     - null-terminated wchar_t* string argument
    //  %f      - double argument
    //  %hf     - float argument, only in InvokeArgs
    //
    // pmethodName is a name of the method to call, with a possible namespace
    // syntax to access methods in the nested objects and movie clips.
    //
   
    // *** Invokes:

    // presult can be null if no return value is desired.
    virtual bool       Invoke(const char* pmethodName, GFxValue *presult, const GFxValue* pargs, UInt numArgs)     = 0;
    virtual bool       Invoke(const char* pmethodName, GFxValue *presult, const char* pargFmt, ...)                = 0;
    virtual bool       InvokeArgs(const char* pmethodName, GFxValue *presult, const char* pargFmt, va_list args)   = 0;
   
    // String - returning versions of Invoke.
    // These versions of Invoke return 0 if Invoke failed and string-converted value otherwise.
    inline const char* Invoke(const char* pmethodName, const GFxValue* pargs, UInt numArgs);
    inline const char* Invoke(const char* pmethodName, const char* pargFmt, ...);
    inline const char* InvokeArgs(const char* pmethodName, const char* pargFmt, va_list args);

    static inline Float  GetRenderPixelScale() { return 20.f; }
};


// *** Inline Implementation - GFxMovie

inline bool    GFxMovie::SetVariable(const char* ppathToVar, const char* pvalue, SetVarType setType)
{
    GFxValue v(pvalue);
    return SetVariable(ppathToVar, v, setType);
}
inline bool    GFxMovie::SetVariable(const char* ppathToVar, const wchar_t* pvalue, SetVarType setType)
{
    GFxValue v(pvalue);
    return SetVariable(ppathToVar, v, setType);
}
inline bool    GFxMovie::SetVariableDouble(const char* ppathToVar, Double value, SetVarType setType)
{
    GFxValue v(value);
    return SetVariable(ppathToVar, v, setType);
}
inline const char* GFxMovie::GetVariable(const char* ppathToVar) const
{
    GFxValue v(GFxValue::VT_ConvertString);
    GetVariable(&v, ppathToVar);
    if (v.GetType() == GFxValue::VT_String)
        return v.GetString();
    return NULL;
}
inline const wchar_t* GFxMovie::GetVariableStringW(const char* ppathToVar) const
{
    GFxValue v(GFxValue::VT_ConvertStringW);
    GetVariable(&v, ppathToVar);
    if (v.GetType() == GFxValue::VT_StringW)
        return v.GetStringW();
    return NULL;
}
inline Double   GFxMovie::GetVariableDouble(const char* ppathToVar) const
{
    GFxValue v(GFxValue::VT_ConvertNumber);
    GetVariable(&v, ppathToVar);
    if (v.GetType() == GFxValue::VT_Number)
        return v.GetNumber();
    return 0.0;    
}

// Convenience methods for GFxValue.
inline  bool    GFxMovie::SetVariableArray(const char* ppathToVar,
                                 UInt index, const GFxValue* pdata, UInt count, SetVarType setType)
{
    return SetVariableArray(SA_Value, ppathToVar, index, pdata, count, setType);
}
inline  bool    GFxMovie::GetVariableArray(const char* ppathToVar, UInt index, GFxValue* pdata, UInt count)
{
    return GetVariableArray(SA_Value, ppathToVar, index, pdata, count);
}

// String-returning Invokes.
inline const char* GFxMovie::Invoke(const char* pmethodName, const GFxValue* pargs, UInt numArgs)
{
    GFxValue v(GFxValue::VT_ConvertString);
    return Invoke(pmethodName, &v, pargs, numArgs) ? v.GetString() : 0;
}
inline const char* GFxMovie::Invoke(const char* pmethodName, const char* pargFmt, ...)
{
    GFxValue v(GFxValue::VT_ConvertString);
    va_list args;
    va_start(args, pargFmt);
    bool result = InvokeArgs(pmethodName, &v, pargFmt, args);
    va_end(args);
    return result ? v.GetString() : 0;
}
inline const char* GFxMovie::InvokeArgs(const char* pmethodName, const char* pargFmt, va_list args)
{
    GFxValue v(GFxValue::VT_ConvertString);
    return InvokeArgs(pmethodName, &v, pargFmt, args) ? v.GetString() : 0;
}

// *** End Inline Implementation - GFxMovie


// *** GFxExternalInterface State

class GFxExternalInterface : public GFxState
{
public:
    GFxExternalInterface() : GFxState(State_ExternalInterface) { }

    virtual ~GFxExternalInterface() {}

    // A callback for ExternalInterface.call. 'methodName' may be NULL, if ExternalInterface is
    // called without parameter. 'args' may be NULL, if argCount is 0. The value, returned 
    // by this callback will be returned by 'ExternalInterface.call' method.
    // To return a value, use pmovieView->SetExternalInterfaceRetVal method.
    virtual void Callback(GFxMovieView* pmovieView, const char* methodName, const GFxValue* args, UInt argCount) = 0;
};


// Sets the external interface used.
inline void GFxSharedState::SetExternalInterface(GFxExternalInterface* p)
{
    SetState(GFxState::State_ExternalInterface, p);
}
inline GPtr<GFxExternalInterface> GFxSharedState::GetExternalInterface() const
{
    return *(GFxExternalInterface*) GetStateAddRef(GFxState::State_ExternalInterface);
}




// GFxMovieView is created by the GFxMovieDef::CreateInstance function and is a
// primary user interface to a movie instance. After being created,
// GFxMovieView is normally configured by setting the renderer, viewport, and
// fscommand callback, if necessary.
//
// After that the movie is ready for playback. Its animation state can be
// advanced by calling Advance() and it can be rendered at by calling Display().
// Both of those methods are inherited from GFxMovie.

class GFxMovieView : public GFxMovie, public GFxSharedState
{
protected:    
    friend class GFxCharacter;
public:

    // Renderer configuration is now in GFxSharedState.

    // *** Viewport and Scaling configuration

    // Sets the render-target surface viewport to which the movie is scaled;
    // coordinates are specified in pixels. If not specified, the default
    // viewport is (0,0, MovieWidth, MovieHeight).
    virtual void        SetViewport(const GViewport& viewDesc)               = 0;
    virtual void        GetViewport(GViewport *pviewDesc) const              = 0;

    // Viewport inline helper; mostly provided for compatibility.
    GINLINE void        SetViewport(SInt bufw, SInt bufh, SInt left, SInt top, SInt w, SInt h, UInt flags = 0)
    {
        GViewport desc(bufw, bufh, left, top, w, h, flags);
        SetViewport(desc);
    }
    // Scale mode
    enum ScaleModeType
    {
        SM_NoScale,
        SM_ShowAll,
        SM_ExactFit,
        SM_NoBorder
    };
    virtual void            SetViewScaleMode(ScaleModeType) = 0;
    virtual ScaleModeType   GetViewScaleMode() const        = 0;
    enum AlignType
    {
        Align_Center,
        Align_TopCenter,
        Align_BottomCenter,
        Align_CenterLeft,
        Align_CenterRight,
        Align_TopLeft,
        Align_TopRight,
        Align_BottomLeft,
        Align_BottomRight
    };
    virtual void            SetViewAlignment(AlignType) = 0;
    virtual AlignType       GetViewAlignment() const    = 0;

    // Returns currently visible frame rectangle, in pixels
    virtual GRectF          GetVisibleFrameRect() const = 0;

    // *** Execution State

    virtual void        Restart()                                       = 0;

    // Advance a movie based on the time that has passed since th last
    // advance call, in seconds. In general, the time interval should
    // be <= 1 / movie FrameRate. If it is not, multiple frames will be
    // advanced only of frameCatchUp == 1.
    // Returns time remaining till next advance must be called, in seconds.
    virtual Float       Advance(Float deltaT, UInt frameCatchUpCount = 2) = 0;

    // Outputs the movie view to its renderer.
    virtual void        Display()                                       = 0;


    // *** Background color interface
    
    virtual void    SetBackgroundColor(const GColor BgColor)        = 0;

    // Sets the background color alpha applied to the movie clip.
    // Set to 0.0f if you do not want the movie to render its background
    // (ex. if you are displaying a hud on top of the game scene).
    // The default value is 1.0f, indication full opacity.
    virtual void    SetBackgroundAlpha(Float alpha)                 = 0;
    virtual Float   GetBackgroundAlpha() const                      = 0;



    // *** Input

    // Event notification interface.
    // Users can call this handler to inform GFxPlayer of mouse and keyboard
    // input, as well as stage and other events.
    virtual void        HandleEvent(const GFxEvent &event)          = 0;

    // State interface. This interface can be used to inform about the
    // current mouse/keyboard state. For simple cases, it can be used
    // instead of the event interface; changes in state will automatically
    // generate internal events.

    // Notifies of mouse state; should be called at regular intervals.
    // The mouse coordinates need to be in the coordinate system of the
    // viewport set by SetViewport.
    virtual void        NotifyMouseState(int x, int y, int buttons) = 0;

    enum HitTestType
    {
        HitTest_Bounds = 0,
        HitTest_Shapes = 1,
        HitTest_ButtonEvents = 2
    };
    // Performs a hit test on the movie view and returns whether or not
    // the passed point is inside the valid area
    virtual bool        HitTest(int x, int y, HitTestType testCond = HitTest_Shapes) = 0;

    // Enable/Disable mouse support for the movie view. Supporting mouse
    // incurs extra processing overhead during Advance due to the execution
    // of extra hit-testing logic when objects move; disabling it will
    // cause mouse events and NotifyMouseState to be ignored.
    // The default state is controlled by the GFC_MOUSE_SUPPORT_ENABLED
    // configuration option and is disabled on consoles and enabled on PCs.
    // Returns the old mouse support state.
    virtual bool        EnableMouseSupport(bool mouseEnabled)       = 0;


    // *** ExternalInterface / User value handling

	// Used to set the return value from GFxExternalInterface::Callback function.
    virtual void        SetExternalInterfaceRetVal(const GFxValue&) = 0;

    // Set and get user data handle; useful for the FsCommand handler.
    virtual Handle      GetUserData() const                         = 0;
    virtual void        SetUserData(Handle)                         = 0;


    // Installs display callbacks that can be used for client rendering. The callback
    // is called after rendering the object it's attached to.
    // Attach NULL to disable the callback. Returns 1 for success, 0 for failure
    //  (object path not found or not of the right type).
    virtual bool        AttachDisplayCallback(const char* ppathToObject,
                            void (GCDECL *callback)(void* puser), void* puser) = 0;

};


#endif // ! INC_GFXPLAYER_H
