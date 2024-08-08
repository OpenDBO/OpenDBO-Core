/**********************************************************************

Filename    :   GFxLoader.h
Content     :   Public SWF loading interface for GFxPlayer
Created     :   June 21, 2005
Authors     :   Michael Antonov

Notes       :   Redesigned to use inherited state objects for GFx 2.0.

Copyright   :   (c) 2005-2007 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFXLOADER_H
#define INC_GFXLOADER_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GTLTypes.h"
#include "GFxResource.h"

// ***** Declared Classes
struct  GFxMovieInfo;
struct  GFxTagInfo;
class   GFxSharedState;
class   GFxLoader;
class   GFxTranslator;
class   GFxGradientParams;

struct  GFxExporterInfo;
struct  GFxImageFileInfo;
class   GFxImageCreateInfo;

// ***** External Classes
class   GFile;
class   GFxMovieDef;
class   GFxMovieDataDef;
class   GImage;
class   GImageInfoBase;
class   GRenderer;

// Externally declared states:
class   GFxRenderConfig;
class   GFxRenderStats;
class   GFxLog;
class   GFxExternalInterface;
class   GFxImportVisitor;
class   GFxTaskManager;
class   GFxFontLib;


// ***** GFxLoader States

// GFxState - a base class for all Loader/Movie states in GFx. These states are
// accessible through the  GShadedState::SetState/GetStateAddRef functions.

class GFxState :  public GRefCountBase<GFxState>
{
public:
    enum StateType
    {
        State_None,

        // *** Instance - related states.
        State_RenderConfig,
        State_RenderStats,
        State_Translator,
        State_Log,
        State_ImageLoader,
        State_ActionControl,    // Verbose Action
        State_UserEventHandler,
        State_FSCommandHandler,
        State_ExternalInterface,

        // *** Loading - related states.
        State_FileOpener,
        State_URLBuilder,
        State_ImageCreator,
        State_ParseControl,     // Verbose Parse
        State_ProgressHandler,
        State_ImportVisitor,
//        State_ImageVisitor,
        State_FontPackParams,
        State_FontCacheManager,
        State_FontLib,
        State_FontProvider,
        State_FontMap,
        State_GradientParams,
        State_TaskManager
    };

protected:        
    StateType       SType;
public:

    // State Ref-Counting and interaction needs to be thread-safe.
    GFxState(StateType st = State_None) : SType(st)
    { SetRefCountMode(GFC_REFCOUNT_THREADSAFE); }
    virtual ~GFxState() { }

    // Use inline to make state lookups more efficient.
    inline StateType   GetStateType() const { return SType;  }
};



// ***** GFxTranslator

// GFxTranslator class is used for international language translation of dynamic
// text fields. Translation is performed through the Translate virtual function
// which is expected to fill in a buffer with a translated string.

class GFxTranslator : public GFxState
{
public:
    enum TranslateCaps
    {
        // Specifies that Translate key can include Flash-HTML tags. If not specified
        // translate will only receive stripped text content (default).
        Cap_ReceiveHtml = 0x1,
        // Specifies that Translate can store HTML into a buffer; off by default.
        Cap_ReturnHtml  = 0x2,
        // Forces to strip all trailing new-line symbols before the text is passed
        // to Translate.
        Cap_StripTrailingNewLines = 0x4
    };

    GFxTranslator() : GFxState(State_Translator) { }
  
    // Override this function to specify whether Translate can receive and/or return HTML.
    virtual UInt GetCaps() const         { return 0; }
    inline  bool CanReceiveHtml() const  { return (GetCaps() & Cap_ReceiveHtml) != 0; }
    inline  bool CanReturnHtml() const   { return (GetCaps() & Cap_ReturnHtml) != 0; }
    inline  bool NeedStripNewLines() const { return (GetCaps() & Cap_StripTrailingNewLines) != 0; }

    // Performs lookup of pkey string for language translation, filling in the destination 
    // string buffer. Buffer is guaranteed to be not null and can be assigned directly,
    // as in {*pbuffer = L"translated_text"; }. User must return 'true' and fill in the
    // buffer if translation took place; return false if translation is not necessary.
    virtual bool Translate(GFxWStringBuffer* pbuffer, const wchar_t *pkey) { GUNUSED2(pbuffer, pkey); return false; }
};



// ***** GFxMovieImageLoader

// Callback used by loadMovie() with user-image protocols, such as 'img://' (bilinear image) 
// or 'imgps://' (point sampled image) is called. The user should populate an image
// and return a pointer to it; the image will be displayed inside of the target movie clip.

class GFxImageLoader : public GFxState
{
public:
    GFxImageLoader() : GFxState(State_ImageLoader) { }

    // Image loading virtual function.
    virtual GImageInfoBase*     LoadImage(const char *purl) = 0;
};


// ***** GFxActionControl

// Controls verbosity and runtime behavior of ActionScript.
class GFxActionControl : public GFxState
{
protected:
    UInt            ActionFlags;

public:

    // ActionScript execution control flags.
    enum ActionControlFlags
    {
        Action_Verbose          = 0x01,
        Action_ErrorSuppress    = 0x02
    };

    GFxActionControl(UInt actionFlags = 0)
        : GFxState(State_ActionControl),  ActionFlags(actionFlags)
    { }       
    
    inline void    SetActionFlags(UInt actionFlags)   { ActionFlags = actionFlags; }
    inline UInt    GetActionFlags() const             { return ActionFlags; }

    // Enables verbose output to log when executing ActionScript. Useful
    // for debugging, 0 by default. Not available if GFx is compiled
    // with the GFC_NO_FXPLAYER_VERBOSE_ACTION option defined.
    inline void    SetVerboseAction(bool verboseAction)
    {
        ActionFlags = (ActionFlags & ~(Action_Verbose)) | (verboseAction ? Action_Verbose : 0);
    }
    // Turn off/on log for ActionScript errors..
    inline void    SetActionErrorSuppress(bool suppressErrors)
    {
        ActionFlags = (ActionFlags & ~(Action_ErrorSuppress)) | (suppressErrors ? Action_ErrorSuppress : 0);
    }  
};



// ***** GFxUserEventHandler

// User-insallable interface to handle events fired FROM the player.
// These events include requests to show/hide mouse, etc.

class GFxUserEventHandler : public GFxState
{
public:
    
    GFxUserEventHandler() : GFxState(State_UserEventHandler) { }

    virtual void HandleEvent(class GFxMovieView* pmovie, const class GFxEvent& event) = 0;
};



// ***** GFxFSCommandHandler

// ActionScript embedded in a movie can use the built-in fscommand() method
// to send data back to the host application. This method can be used to
// inform the application about button presses, menu selections and other
// UI events. To receive this data developer need to register a handler
// with SetFSCommandCallback.

// If specified, this handler gets called when ActionScript executes
// the fscommand() statement. The handler receives the GFxMovieView*
// interface for the movie that the script is embedded in and two string
// arguments passed by the script to fscommand().


class GFxFSCommandHandler : public GFxState
{
public:
    GFxFSCommandHandler() : GFxState(State_FSCommandHandler) { }

    virtual void Callback(GFxMovieView* pmovie, const char* pcommand, const char* parg) = 0;
};


// ***** GFxFileOpener

// A callback interface that is used for opening files.
// Must return either an open file or 0 value for failure.
// The caller of the function is responsible for Releasing
// the GFile interface.

class GFxFileOpener : public GFxState
{
public:

    GFxFileOpener();

    // Override to opens a file using user-defined function and/or GFile class.
    // The default implementation uses buffer-wrapped GSysFile, but only
    // if GFC_USE_SYSFILE is defined.
    virtual GFile*      OpenFile(const char *purl);

    // Returns last modified date/time required for file change detection.
    // Can be implemented to return 0 if no change detection is desired.
    // Default implementation checks file time if GFC_USE_SYSFILE is defined.
    virtual SInt64      GetFileModifyTime(const char *purl);

    // Open file with customizable log, by relying on OpenFile.
    // If not null, log will receive error messages on failure.
    GFile*              OpenFileEx(const char *purl, class GFxLog *plog);    
};


// ***** GFxURLBuilder

// URLBuilder class is responsible for building a filename path
// used for loading objects. If can also be overridden if filename
// or extension substitution is desired.

class GFxURLBuilder : public GFxState
{
public:
     
    GFxURLBuilder() : GFxState(State_URLBuilder) { } 

    enum FileUse
    {
        File_Regular,
        File_Import,
        File_ImageImport,
        File_LoadMovie,
        File_LoadVars
    };

    struct LocationInfo
    {
        FileUse         Use;
        GFxString       FileName;
        GFxString       ParentPath;

        LocationInfo(FileUse use, const GFxString& filename)
            : Use(use), FileName(filename) { }
        LocationInfo(FileUse use, const char* pfilename)
            : Use(use), FileName(pfilename) { }
        LocationInfo(FileUse use, const GFxString& filename, const GFxString& path)
            : Use(use), FileName(filename), ParentPath(path) { }
        LocationInfo(FileUse use, const char* pfilename, const char* ppath)
            : Use(use), FileName(pfilename), ParentPath(ppath) { }
    };

    // Builds a new filename based on the provided filename and a parent path.
    // Uses IsPathAbsolute internally do determine whether the FileName is absolute.
    virtual void        BuildURL(GFxString *ppath, const LocationInfo& loc);

    // Static helper function used to determine if the path is absolute.
    // Relative paths should be concatenated in TranslateFilename,
    // while absolute ones are used as-is.
    static bool         IsPathAbsolute(const char *putf8str);
    // Modifies path to not include the filename, leaves trailing '/'.
    static bool         ExtractFilePath(GFxString *ppath);

    // TBD: IN the future, could handle 'http://', 'file://' prefixes and the like.

private:
    // Default implementation used by BuildURL.
    friend class GFxLoadStates;    
    static void DefaultBuildURL(GFxString *ppath, const LocationInfo& loc); 
};


// ***** GFxImageCreator

// Interface used to create GImageInfoBase objects applied during rendering.
// Passed pimage argument can be either valid or 0; if it is valid, then
// the filled in image should be used. If it is 0, and empty image must be created.

class GFxImageCreator : public GFxState
{
    // Read-Only flag that instructs the player to keep GImage data used to create
    // GImageInfoBase in memory. This must remain read-only after creation since
    // changing it after use would cause MovieData to be bound incorrectly.
    bool    KeepImageBindData;
public:

    GFxImageCreator(bool keepImageBindData = 0)
        : GFxState(State_ImageCreator), KeepImageBindData(keepImageBindData)
    { }
    

    // If false (default), loaded movies would be bound to the image creator and the
    // associated GImage data is subsequently released. The down side of this is that
    // using a different creator will force file re-load. Returning true, however, 
    // will enable delayed image binding, and thus allow binding to different creators
    // by keeping original data copy in memory.
    // GFxLoader::LoadKeepBindData takes precedence over this flag.
    inline bool            IsKeepingImageData() const   { return KeepImageBindData; }


    // Default implementation reads images from DDS/TGA files
    // creating GImageInfo objects. This implementation can rely on
    // FileOpener / RendererConfig states passed in GFxImageCreateInfo.    
    virtual GImageInfoBase* CreateImage(const GFxImageCreateInfo &info);
    
    // Loads a GImage from an open file, assuming the specified file format.
    // Can be used as a helper when implementing CreateImage.
    static GImage*          LoadBuiltinImage(GFile* pfile,
                                             GFxFileConstants::FileFormatType format,
                                             GFxResource::ResourceUse use);
};


// ***** GFxParseControl

class GFxParseControl : public GFxState
{
protected:
    UInt            ParseFlags;
public:    

    // Verbose constants, determine what gets displayed during parsing
    enum VerboseParseConstants
    {
        VerboseParseNone        = 0x00,
        VerboseParse            = 0x01,
        VerboseParseAction      = 0x02,
        VerboseParseShape       = 0x10,
        VerboseParseMorphShape  = 0x20,
        VerboseParseAllShapes   = VerboseParseShape | VerboseParseMorphShape,
        VerboseParseAll         = VerboseParse | VerboseParseAction | VerboseParseAllShapes
    };

    GFxParseControl(UInt parseFlags = VerboseParseNone)
        : GFxState(State_ParseControl), ParseFlags(parseFlags)
    { }

    inline void    SetParseFlags(UInt parseFlags)   { ParseFlags = parseFlags; }
    inline UInt    GetParseFlags() const            { return ParseFlags; }

    bool    IsVerboseParse() const              { return (ParseFlags & VerboseParse) != 0; }
    bool    IsVerboseParseShape() const         { return (ParseFlags & VerboseParseShape) != 0; }
    bool    IsVerboseParseMorphShape() const    { return (ParseFlags & VerboseParseMorphShape) != 0; }
    bool    IsVerboseParseAction() const        { return (ParseFlags & VerboseParseAction) != 0; }

};


// ***** GFxLoadProgressHandler

class GFxProgressHandler : public GFxState
{
public:    
    GFxProgressHandler() : GFxState(State_ProgressHandler) { }

    // TBD: Begin/End loading - need a thread-independednt state object?
    virtual void    ProgressUpdate(const GFxString& fileURL, const GFxTagInfo& tagInfo,
                                   UInt bytesLoaded, UInt totalBytes) = 0;   
};



// ***** GFxGradientParams

class GFxGradientParams : public GFxState
{
    // Grad size in pixels. 0 - means default size (see GFX_GRADIENT_SIZE_DEFAULT)
    UInt                RadialGradientImageSize; 
	bool				AdaptiveGradients;

public:
    GFxGradientParams()
        : GFxState(State_GradientParams)
    {
        RadialGradientImageSize = 0;
        AdaptiveGradients       = true;
    }

    GFxGradientParams(UInt radialSize)
        : GFxState(State_GradientParams)
    {
        RadialGradientImageSize = radialSize;
        AdaptiveGradients       = false;
    }

	inline bool     IsAdaptive() const              { return AdaptiveGradients; }
    inline void     SetAdaptive(bool adaptive)      { AdaptiveGradients = adaptive; }

    inline UInt     GetRadialImageSize() const      { return RadialGradientImageSize; }
    inline void     SetRadialImageSize(UInt rsz)    { RadialGradientImageSize = rsz; }   
};



// Forward Declarations
class GFxFontResource;

// ***** GFxFontPackParams

// GFxFontPackParams contains the font rendering configuration options 
// necessary to generate glyph textures. By setting TextureConfig, users can
// control the size of glyphs and textures which are generated.
class GFxFontPackParams : public GFxState
{
public:
    struct TextureConfig
    {
        // The nominal size of the final anti-aliased glyphs stored in
        // the texture.  This parameter controls how large the very
        // largest glyphs will be in the texture; most glyphs will be
        // considerably smaller.  This is also the parameter that
        // controls the trade off between texture RAM usage and
        // sharpness of large text. Default is 48.
        int             NominalSize;
     
        // How much space to leave around the individual glyph image.
        // This should be at least 1.  The bigger it is, the smoother
        // the boundaries of minified text will be, but the more texture
        // space is wasted.
        int             PadPixels;
        
        // The dimensions of the textures that the glyphs get packed into.
        // Default size is 1024.
        int             TextureWidth, TextureHeight;

        TextureConfig() : 
            NominalSize(48), 
            PadPixels(3), 
            TextureWidth(1024), 
            TextureHeight(1024)
        {}
    };

public:
    GFxFontPackParams() : 
        GFxState(State_FontPackParams),
        SeparateTextures(false),
        GlyphCountLimit(0)
    {}

    // Size (in TWIPS) of the box that the glyph should stay within.
    // this *should* be 1024, but some glyphs in some fonts exceed it!
    static const Float          GlyphBoundBox;

    // Obtain/modify variables that control how glyphs are packed in textures.
    void            GetTextureConfig(TextureConfig* pconfig) { *pconfig = PackTextureConfig; }
    // Set does some bound checking to it is implemented in the source file.
    void            SetTextureConfig(const TextureConfig& config);

    bool            GetUseSeparateTextures() const    { return SeparateTextures; }
    void            SetUseSeparateTextures(bool flag) { SeparateTextures = flag; }

    int             GetGlyphCountLimit() const        { return GlyphCountLimit; }
    void            SetGlyphCountLimit(int lim)       { GlyphCountLimit = lim;  }

    Float           GetDrawGlyphScale(int nominalGlyphHeight);    

    // Return the pixel height of text, such that the
    // texture glyphs are sampled 1-to-1 texels-to-pixels.
    // I.E. the height of the glyph box, in texels.
    static Float    GSTDCALL GetTextureGlyphMaxHeight(const GFxFontResource* f);

    // State type
    virtual StateType   GetStateType() const { return State_FontPackParams; }

private:
    // Variables that control how glyphs are packed in textures.
    TextureConfig   PackTextureConfig;

    // Use separate textures for each font. Default is False.
    bool            SeparateTextures;

    // The limit of the number of glyphs in a font. 0 means "no limit".
    // If the limit is exceeded the glyphs are not packed. In this 
    // case the dynamic cache may be used if enabled. Default is 0.
    int             GlyphCountLimit;
};



// ***** GFxFontCacheManager

class GFxFontCacheManager : public GFxState
{
public:
    struct TextureConfig
    {
        UInt TextureWidth;
        UInt TextureHeight; 
        UInt MaxNumTextures; 
        UInt MaxSlotHeight; 
        UInt SlotPadding;
        UInt TexUpdWidth;
        UInt TexUpdHeight;

        TextureConfig():
            TextureWidth(1024),
            TextureHeight(1024),
            MaxNumTextures(1),
            MaxSlotHeight(48),
            SlotPadding(2),
            TexUpdWidth(256),
            TexUpdHeight(512)
        {}
    };

    GFxFontCacheManager(bool enableDynamicCache=true);
    GFxFontCacheManager(const TextureConfig& config, bool enableDynamicCache=true);
   ~GFxFontCacheManager();

    void  GetTextureConfig(TextureConfig* config) { *config = CacheTextureConfig; }
    void  SetTextureConfig(const TextureConfig& config);

    void  EnableDynamicCache(bool v=true) { DynamicCacheEnabled = v; }
    bool  IsDynamicCacheEnabled() const   { return DynamicCacheEnabled; }

    void  SetMaxRasterScale(Float s) { MaxRasterScale = s;    }
    Float GetMaxRasterScale() const  { return MaxRasterScale; }

    void  SetSteadyCount(UInt n) { SteadyCount = n; }
    UInt  GetSteadyCount() const { return SteadyCount; }

    void  SetMaxVectorCacheSize(UInt n);
    UInt  GetMaxVectorCacheSize() const { return MaxVectorCacheSize; }

    void  SetFauxItalicAngle(Float a);
    Float GetFauxItalicAngle() const  { return FauxItalicAngle; }

    void  SetFauxBoldRatio(Float r);
    Float GetFauxBoldRatio() const  { return FauxBoldRatio; }

    class GFxFontCacheManagerImpl* GetImplementation() { return pCache; }

private:
    TextureConfig                  CacheTextureConfig;
    bool                           DynamicCacheEnabled;
    Float                          MaxRasterScale;
    UInt                           SteadyCount;
    UInt                           MaxVectorCacheSize;
    Float                          FauxItalicAngle;
    Float                          FauxBoldRatio;
    class GFxFontCacheManagerImpl* pCache;
};


// ***** GFxFontProvider

// An external interface to handle device/system fonts

class GFxFont;
class GFxFontProvider : public GFxState
{
public:
    GFxFontProvider() : GFxState(State_FontProvider) {}
    virtual ~GFxFontProvider() {}
    
    // fontFlags style as described by GFxFont::FontFlags
    virtual GFxFont*    CreateFont(const char* name, UInt fontFlags) = 0;
    //void EnumerateFonts interface...
};


// ***** GFxFontMap

// Font map defines a set of font name substitutions that apply fonts
// looked up/create from GFxFontLib and GFxFontProvider. By installing a
// map you can change fonts used by SWF/GFX file to refer to alternative
// fonts. For example, if the file makes use of the "Helvetica" font while
// your system only provides "Arial", you can install it as a substitution
// by doing the following:
//    GPtr<GFxFontMap> pfontMap = *new GFxFontMap;
//    pfontMap->AddFontMap("Helvetica", "Arial");
//    loader.SetFontMap(Map);

class GFxFontMap : public GFxState
{
    class GFxFontMapImpl *pImpl;
public:
    GFxFontMap();
    virtual ~GFxFontMap();

    enum MapFontFlags
    {
        // Default state,
        MFF_Original    = 0x0004,
        // Other font flag combinations, indicate that a specific font type
        // will be looked up. The must match up with constants in GFxFont.
        MFF_Normal      = 0x0000,
        MFF_Italic      = 0x0001,
        MFF_Bold        = 0x0002,
        MFF_BoldItalic  = MFF_Italic | MFF_Bold,
    };

    class MapEntry
    {
    public:
        GFxString       Name;
        Float           ScaleFactor;
        MapFontFlags    Flags;

        MapEntry():ScaleFactor(1.f) { Flags = MFF_Original; }
        MapEntry(const GFxString& name, MapFontFlags flags, Float scaleFactor = 1.0f) : 
            Name(name), ScaleFactor(scaleFactor), Flags(flags) { }
        MapEntry(const MapEntry &src) : Name(src.Name), ScaleFactor(src.ScaleFactor), Flags(src.Flags) { }        
        MapEntry& operator = (const MapEntry &src)
        { Name = src.Name; ScaleFactor = src.ScaleFactor; Flags = src.Flags; return *this; }

        // Updates font flags generating new ones based on local MapFontFlags.
        UInt            UpdateFontFlags(UInt originalFlags)
        {
            return (Flags == MFF_Original) ? originalFlags :
                   ((originalFlags & ~MFF_BoldItalic) | Flags);
        }
    };

    // Adds a font mapping; font names expressed in UTF8.
    // A function will fail if a mapping for this font already exists,
    // overwriting a map is not allowed.
    bool            MapFont(const char* pfontName, const char* pnewFontName,
                            MapFontFlags mff = MFF_Original, Float scaleFactor = 1.0f);
    // Adds a font mapping; font names expressed in Unicode.
    bool            MapFont(const wchar_t* pfontName, const wchar_t* pnewFontName,
                            MapFontFlags mff = MFF_Original, Float scaleFactor = 1.0f);
    
    // Obtains a font mapping, filling in MapEntry if succeeds.
    bool            GetFontMapping(MapEntry* pentry, const char *pfontName);
};



// ***** GFxSharedState

// GFxSharedState collects accessor methods for properties that can be
// inherited from GFxLoader to GFxMovieDef, and then to GFxMovieView.
// These properties can be overwritten in any one of above mentioned instances.

class GFxSharedState : public GFxFileConstants
{
protected:
    virtual GFxSharedState* GetSharedImpl() const { return 0; }

public:
    virtual ~GFxSharedState () { }
    
    // General state access. We need to pass state type to SetState so that we know
    // which state is being cleared when null is passed.    
    virtual void        SetState(GFxState::StateType state, GFxState* pstate)
    {
        GFxSharedState* p = GetSharedImpl();    
        GASSERT((pstate == 0) ? 1 : (pstate->GetStateType() == state));
        if (p) p->SetState(state, pstate);
    }
    // We always need to return AddRef'ed state value from GetState to remain
    // thread safe, since otherwise another thread might overwrite the state
    // killing it prematurely and causing Get to return a bad pointer.
    virtual GFxState*   GetStateAddRef(GFxState::StateType state) const
    {
        GFxSharedState* p = GetSharedImpl();
        return p ? p->GetStateAddRef(state) : 0;
    }

    // Fills in a set of states with one call.
    // Initial array pstates must contain null pointers.
    virtual void        GetStatesAddRef(GFxState** pstateList,
                                        const GFxState::StateType *pstates, UInt count) const
    {
        GFxSharedState* p = GetSharedImpl();
        if (p) p->GetStatesAddRef(pstateList, pstates, count);
    }


    // *** Inlines for convenient state access.
 
    // Sets rendering configuration - implemented in GFxRenderConfig.h.
    inline void                 SetRenderConfig(GFxRenderConfig* pri);
    inline GPtr<GFxRenderConfig> GetRenderConfig() const;

    inline void                 SetRenderConfig(GRenderer *prenderer, Float maxError = 1.0f, UInt rendererFlags = 0);
    inline GPtr<GRenderer>      GetRenderer() const;

    inline void                 SetRenderStats(GFxRenderStats* prs);
    inline GPtr<GFxRenderStats> GetRenderStats() const;

       
    // Sets an interface that will be used for logging - implemented in GFxLog.h.
    inline void                 SetLog(GFxLog *plog);
    inline GPtr<GFxLog>         GetLog() const;

    // Set an interface that will be used to translate dynamic text field for international languages.
    inline void                 SetTranslator(GFxTranslator *ptr)           { SetState(GFxState::State_Translator, ptr); }
    inline GPtr<GFxTranslator>  GetTranslator() const                       { return *(GFxTranslator*)GetStateAddRef(GFxState::State_Translator); }

    // Sets loadMovie interface used for images.
    inline void                 SetImageLoader(GFxImageLoader* pmil)        { SetState(GFxState::State_ImageLoader, pmil); }
    inline GPtr<GFxImageLoader> GetImageLoader() const                      { return *(GFxImageLoader*)GetStateAddRef(GFxState::State_ImageLoader); }

    // Loads an image based on installed LoadMovieImageCallback.
    inline GImageInfoBase*      LoadMovieImage(const char *purl)
    { GFxImageLoader* p = GetImageLoader(); return p ? p->LoadImage(purl) : 0; }


    // Set ActionScript control states.
    inline void                 SetActionControl(GFxActionControl* pac)     { SetState(GFxState::State_ActionControl, pac); }
    inline GPtr<GFxActionControl> GetActionControl() const                  { return *(GFxActionControl*)GetStateAddRef(GFxState::State_ActionControl); }

    // UserEventHander - used to control events in 
    inline void                 SetUserEventHandler(GFxUserEventHandler* pri) { SetState(GFxState::State_UserEventHandler, pri); }
    inline GPtr<GFxUserEventHandler> GetUserEventHandler() const            { return *(GFxUserEventHandler*)GetStateAddRef(GFxState::State_UserEventHandler); }

    // Sets loadMovie interface used for images.
    inline void                 SetFSCommandHandler(GFxFSCommandHandler* ps){ SetState(GFxState::State_FSCommandHandler, ps); }
    inline GPtr<GFxFSCommandHandler> GetFSCommandHandler() const            { return *(GFxFSCommandHandler*)GetStateAddRef(GFxState::State_FSCommandHandler); }

    // Sets the external interface used - implemented in GFxPlayer.h.
    inline void                 SetExternalInterface(GFxExternalInterface* p);
    inline GPtr<GFxExternalInterface> GetExternalInterface() const;


    // Installs a callback function that is always used by GFxLoader
    // for opening various files based on a path or url string.
    // If not specified, the system file input will be used,
    // but only if GFC_USE_SYSFILE is defined.
    inline  void                SetFileOpener(GFxFileOpener *ptr)           { SetState(GFxState::State_FileOpener, ptr); }
    inline  GPtr<GFxFileOpener> GetFileOpener() const                       { return *(GFxFileOpener*)GetStateAddRef(GFxState::State_FileOpener); }

    // Open file with customizable log. If not null, log will receive error messages on failure.
    GFile*                      OpenFileEx(const char *pfilename, class GFxLog *plog);
    // Opens a file using the specified callback, same as above but uses local log.
    GFile*                      OpenFile(const char *pfilename);
    
    // File name translation.
    inline void                 SetURLBuilder(GFxURLBuilder *ptr)           { SetState(GFxState::State_URLBuilder, ptr); }
    inline GPtr<GFxURLBuilder>  GetURLBuilder() const                       { return *(GFxURLBuilder*)GetStateAddRef(GFxState::State_URLBuilder); }

    
    // Sets the callback function that will be used to create/initalize images
    // during loading. If no function is set, GImageInfo will be used.
    inline void                 SetImageCreator(GFxImageCreator *ptr)       { SetState(GFxState::State_ImageCreator, ptr); }
    inline GPtr<GFxImageCreator> GetImageCreator() const                    { return *(GFxImageCreator*)GetStateAddRef(GFxState::State_ImageCreator); }

    // Creates an image based on a user-defined creator; if not available uses default logic.
    GImageInfoBase*             CreateImageInfo(const GFxImageCreateInfo& info);   


    // Sets the callback function that will be used to create/initalize images
    // during loading. If no function is set, GImageInfo will be used.
    inline void                 SetParseControl(GFxParseControl *ptr)       { SetState(GFxState::State_ParseControl, ptr); }
    inline GPtr<GFxParseControl> GetParseControl() const                    { return *(GFxParseControl*)GetStateAddRef(GFxState::State_ParseControl); }

    inline void                 SetProgressHandler(GFxProgressHandler *ptr) { SetState(GFxState::State_ProgressHandler, ptr); }
    inline GPtr<GFxProgressHandler> GetProgressHandler() const              { return *(GFxProgressHandler*)GetStateAddRef(GFxState::State_ProgressHandler); }

    // Sets a default visitor that is used when after files are loaded.
    inline void                 SetImportVisitor(GFxImportVisitor *ptr);
    inline GPtr<GFxImportVisitor> GetImportVisitor() const;

    // GFxSharedState access for FontFontPackParams
    inline void                 SetFontPackParams(GFxFontPackParams *ptr)   { SetState(GFxState::State_FontPackParams, ptr); }
    inline GPtr<GFxFontPackParams> GetFontPackParams() const                { return *(GFxFontPackParams*) GetStateAddRef(GFxState::State_FontPackParams); }

    inline void                 SetFontCacheManager(GFxFontCacheManager *ptr) { SetState(GFxState::State_FontCacheManager, ptr); }
    inline GPtr<GFxFontCacheManager> GetFontCacheManager() const            { return *(GFxFontCacheManager*) GetStateAddRef(GFxState::State_FontCacheManager); }

    inline void                 SetFontLib(GFxFontLib* pfl);
    inline GPtr<GFxFontLib>     GetFontLib() const;

    inline void                 SetFontProvider(GFxFontProvider *ptr)       { SetState(GFxState::State_FontProvider, ptr); }
    inline GPtr<GFxFontProvider> GetFontProvider() const                    { return *(GFxFontProvider*) GetStateAddRef(GFxState::State_FontProvider); }

    inline void                 SetFontMap(GFxFontMap *ptr)                 { SetState(GFxState::State_FontMap, ptr); }
    inline GPtr<GFxFontMap>     GetFontMap() const                          { return *(GFxFontMap*) GetStateAddRef(GFxState::State_FontMap); }

    // Sets an interface to Gradient
    inline void                 SetGradientParams(GFxGradientParams *ptr)   { SetState(GFxState::State_GradientParams, ptr); }
    inline GPtr<GFxGradientParams> GetGradientParams() const                { return *(GFxGradientParams*) GetStateAddRef(GFxState::State_GradientParams); }

    // Sets a task manager - used to issue I/O and general processing tasks on other threads.
    inline void                 SetTaskManager(GFxTaskManager *ptr);
    inline GPtr<GFxTaskManager> GetTaskManager() const;
}; 




// ***** GFxLoader

//  This is the main public interface that loads SWF Files,
//  and maintains the loader state.

struct GFxMovieInfo
{
    // Flag definitions for Flags field.
    enum SWFFlagConstants
    {
        SWF_Compressed  = 0x0001,
        SWF_Stripped    = 0x0010,
    };

    UInt    Version;
    UInt    Flags;
    SInt    Width, Height;
    Float   FPS;
    UInt    FrameCount;
    UInt    TagCount;   

    // Extra exporter info
    UInt16  ExporterVersion; // Example: Version 1.10 will be encoded as 0x10A.
    UInt32  ExporterFlags;   // Same content as GFxExporterInfo::Flags.

    void    Clear()
    {
        Version = 0;
        Flags   = 0;
        Width = Height = 0;
        FrameCount = TagCount = 0;
        FPS = 0.0f;   
        ExporterVersion = 0;
        ExporterFlags   = 0;
    }

    bool    IsStripped() const      { return (Flags & SWF_Stripped) != 0;  }
    bool    IsCompressed() const    { return (Flags & SWF_Compressed) != 0;  }
};


// Information provided to the stripper tool when converting '.swf' file to '.gfx'.
struct GFxExporterInfo
{
    GFxFileConstants::FileFormatType Format;    // Format used for images.
    const char*     pPrefix;                    // Prefix assigned to extracted image files.
    const char*     pSWFName;                   // Original SWF file name from which '.gfx' was created.
    UInt16          Version;                    // Export tool version.

    // Export flag bits.
    enum ExportFlagConstants
    {
        EXF_GlyphTexturesExported       = 0x01,
        EXF_GradientTexturesExported    = 0x02,
        EXF_GlyphsStripped              = 0x10,
    };
    UInt            ExportFlags;                // Export flags described by ExportFlagConstants.
};


// GFxLoader provider the following functionality:
//   - collection of states describing loading / execution characteristics
//   - a resource library referenced during loading
//   - a task manager used for threaded loading, if any
//   - An API used to load movies based on the local states

class GFxLoader : public GFxSharedState
{
    // Hidden loader implementation and context pointer.
    class GFxLoaderImpl*    pImpl;
    GFxResourceLib*         pStrongResourceLib;

protected:
    virtual GFxSharedState* GetSharedImpl() const;

    virtual bool CheckTagLoader(int tagType) const;

public:

    GFxLoader(GFxFileOpener* pfileOpener = 0,
              UByte verboseParseFlags = GFxParseControl::VerboseParseNone);

    // Create a new loader, copying it's library and states.
    GFxLoader(const GFxLoader& src);

    GEXPORT virtual ~GFxLoader();

    
    // *** Task Manager

    // Specify task manager used for threaded loading, if any.


    // *** Library management

    // GFxResourceLib is a single heterogeneous library, containing image loaded movies,
    // images, gradients and other resources identified by a key. A default library is
    // created by GFxLoader constructor; however, users can change it and/or force library 
    // sharing across loaders by using these methods.
    void               SetResourceLib(GFxResourceLib *plib);
    GFxResourceLib*    GetResourceLib() const;


    // *** SWF File Loading

    enum LoadConstants
    {
        // Default.
        LoadAll             = 0x00000000,
        
        // Wait until all of the loading has completed before returning GFxMovieDef.
        LoadWaitCompletion  = 0x00000001,
        LoadWaitFrame1      = 0x00000002,

        // Specified ordered binding, where resources are loaded after main data file.
        // If not specified, resource loading will be interleaved with resource and import binding.
        LoadOrdered         = 0x00000010,
        // Perform MovieData to Resource/Import binding on a separate thread.
        LoadThreadedBinding = 0x00000020,

        // Set to preserve Creator bind data.
        // If set image/resource data used for creation will be kept so that
        // it can be re-bound with a different creator instance if necessary.
        // Takes precedence over GFxImageLoader::IsKeepingImageData
        LoadKeepBindData    = 0x00000080,

        // Set this flag to allow images to be loaded into root MovieDef;
        // file formats will be detected automatically.
        LoadImageFiles      = 0x00010000,
        // Set to disable loading of SWF files; only GFX content is allowed
        LoadDisableSWF      = 0x00080000,

        // Disable loading of imports. Files loaded without imports may not
        // be functional. This is primarily used for 'gfxexport' tool.
        LoadDisableImports  = 0x00100000,        

    };

    // Obtains information about SWF file and checks for its availability.
    // Return 1 if the info was obtained successfully (or was null, but SWF file existed),
    // or 0 if it did not exist. Pass LoadCheckLibrary if the library should be checked before loading the file.
    // Specifying LoadFromLibraryOnly can be used to check for presence of the file in the library.
    bool            GetMovieInfo(const char *pfilename, GFxMovieInfo *pinfo, bool getTagCount = 0,
                                 UInt loadLibConstants = LoadAll );

    // Loads a movie definition, user must release the movie definition.
    // If LoadImports, LoadBitmaps or LoadFontShapes was not specified,
    // user may need to resolve those components manually.
    GFxMovieDef*    CreateMovie(const char *pfilename, UInt loadConstants = LoadAll );    
    

    // Unpins all resources held in the library.
    void            UnpinAll();
};

#ifdef GFC_BUILD_DEBUG
extern "C" int GFx_Compile_without_GFC_BUILD_DEBUG;
#else
extern "C" int GFx_Compile_with_GFC_BUILD_DEBUG;
#endif

GINLINE GFxFileOpener::GFxFileOpener() : GFxState(State_FileOpener)
{
#ifdef GFC_BUILD_DEBUG
    GFx_Compile_without_GFC_BUILD_DEBUG = 0;
#else
    GFx_Compile_with_GFC_BUILD_DEBUG = 0;
#endif
}

#endif
