/**********************************************************************

Filename    :   GFxLog.h
Content     :   Logging functionality
Created     :   June 23, 2005
Authors     :   Michael Antonov

Copyright   :   (c) 2001-2006 Scaleform Corp. All Rights Reserved.

Notes       :   

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFXLOG_H
#define INC_GFXLOG_H

#include "GRefCount.h"
// Include Loader because GFxLog derives from GFxSharedState.
#include "GFxLoader.h"

// ***** Declared Classes
class GFxLog;
class GFxLogConstants;
template<class Derived> 
class GFxLogBase;


// GFXLOG_MESSAGE_FUNC macro, generates convenience printf-style inline functions for message types
#ifdef __GNUC__
    #define GFXLOG_VAARG_ATTRIBUTE __attribute__((format (printf, 2, 3)))
#else
    #define GFXLOG_VAARG_ATTRIBUTE
#endif

#ifdef GFC_OS_PS2
#define GFXLOG_INLINE 
#else
#define GFXLOG_INLINE inline
#endif

#define GFXLOG_MESSAGE_FUNC_DECL(name, message_type) GFXLOG_INLINE void   name(const char* pfmt, ...) const GFXLOG_VAARG_ATTRIBUTE;

#define GFXLOG_MESSAGE_FUNC(name, message_type)                                                                 \
    template <class Derived>                                                                                    \
    GFXLOG_INLINE void GFxLogBase<Derived>::name(const char* pfmt, ...) const                                         \
        { va_list argList; va_start(argList, pfmt);                                                             \
          Derived* p = (Derived*)this;                                                                          \
          GFxLog *plog = p->GetLog();                                                                           \
          if (p->IsLogNotNull() || plog) plog->LogMessageVarg(message_type, pfmt, argList);                     \
          va_end(argList); }

#define GFXLOG_MESSAGE_FUNC_COND(name, cond_func, message_type)                                                 \
    template <class Derived>                                                                                    \
    GFXLOG_INLINE void GFxLogBase<Derived>::name(const char* pfmt, ...) const                                         \
        { va_list argList; va_start(argList, pfmt);                                                             \
          Derived* p = (Derived*)this;                                                                          \
          if (p->P_##cond_func()) {                                                                             \
              GFxLog *plog = p->GetLog();                                                                       \
              if (p->IsLogNotNull() || plog) plog->LogMessageVarg(message_type, pfmt, argList); }               \
          va_end(argList); }



// ***** General convenience solution for logging

// Log Constants are separated into a separate class base so that LogMessageType
// does not cause compiler errors in templates.
class GFxLogConstants
{
public:
    
    // *** Log Message Types

    enum LogMessageType
    {
        // *** Channels

        // The idea of message channels is that they correspond to separate
        // logs that can be displayed individually. Several message sub-types
        // can, however, be generated within the same channel. End users may
        // choose to ignore the recommended channel structure and either
        // combine all of the messages (default output), or separate them
        // further into individual logs.

        Log_Channel_General         = 0x10, // Used for end-user messages about I/O, errors, etc.
        Log_Channel_Script          = 0x20, // Used for script-related messages (bad function parameters, etc.)
        Log_Channel_Parse           = 0x30, // Used for parse log generated during loading
        Log_Channel_Action          = 0x40, // Used for action-script instruction execution log
        Log_Channel_Debug           = 0x50, // Reserved flag for mapping GDebug messages into single log object.
        Log_Channel_Mask            = 0xF0,
        // Standard channel sub-types
        Log_MessageType_Error       = 0,
        Log_MessageType_Warning     = 1,
        Log_MessageType_Message     = 2,

        
        // *** Combined Log Message types

        // General I/O errors and warnings
        Log_Error                   = Log_Channel_General | Log_MessageType_Error,
        Log_Warning                 = Log_Channel_General | Log_MessageType_Warning,
        Log_Message                 = Log_Channel_General | Log_MessageType_Message,
        // Script errors and warnings
        Log_ScriptError             = Log_Channel_Script | Log_MessageType_Error,
        Log_ScriptWarning           = Log_Channel_Script | Log_MessageType_Warning,
        Log_ScriptMessage           = Log_Channel_Script | Log_MessageType_Message,
        // Parse log messages (generated based on verbosity settings during loading)
        Log_Parse                   = Log_Channel_Parse | 0,
        Log_ParseShape              = Log_Channel_Parse | 1,
        Log_ParseMorphShape         = Log_Channel_Parse | 2,
        Log_ParseAction             = Log_Channel_Parse | 3,
        // Action
        Log_Action                  = Log_Channel_Action | 0,

    };
};

// This class is used as a mix-in base to classes that provide GetLog() function.
// It adds convenience inlines to the class that can be used for logging.

template<class Derived>
class GFxLogBase : public GFxLogConstants
{
public:
    virtual ~GFxLogBase () { }

    // *** Parse Configuration

    // These definitions control whether a certain type of logging is enabled
    // or disabled. The functions should be overriden in Derived class if they
    // need to return a programmer-specified value.
    // One can think of these functions as "template-virtual".

//  bool    IsVerboseParse() const              { return 0; }
//  bool    IsVerboseParseShape() const         { return 0; }
//  bool    IsVerboseParseMorphShape() const    { return 0; }
//  bool    IsVerboseParseAction() const        { return 0; }
//  bool    IsVerboseAction() const             { return 0; }


#ifdef GFC_DOM
    // Declarations for DOM only (not compiled by C++)
    // Need to put them here since DOM does not handle declarations below correctly.
    virtual bool    IsVerboseParse() const              { return 0; }
    virtual bool    IsVerboseParseShape() const         { return 0; }
    virtual bool    IsVerboseParseMorphShape() const    { return 0; }
    virtual bool    IsVerboseParseAction() const        { return 0; }
    virtual bool    IsVerboseAction() const             { return 0; }

    // Convenient printf-style functions.
    inline  void    LogError(const char* pfmt, ...);
    inline  void    LogWarning(const char* pfmt, ...);
    inline  void    LogMessage(const char* pfmt, ...);
    // Dynamic conditional printf-style functions.
    // Parsing - controls by VerboseParse flags in loader
    inline  void    LogParse(const char* pfmt, ...);
    inline  void    LogParseShape(const char* pfmt, ...);
    inline  void    LogParseMorphShape(const char* pfmt, ...);
    inline  void    LogParseAction(const char* pfmt, ...);
    // Action - controller by Verbose action flags in log
    inline  void    LogAction(const char* pfmt, ...);
    inline  void    LogScriptError(const char* pfmt, ...);
    inline  void    LogScriptWarning(const char* pfmt, ...);
    inline  void    LogScriptMessage(const char* pfmt, ...);
#endif

    // Tell DOM to ignore declarations below:
    // DOM-IGNORE-BEGIN

protected:
#ifdef  GFC_NO_FXPLAYER_VERBOSE_PARSE
    bool    P_IsVerboseParse() const            { return 0; }
    bool    P_IsVerboseParseShape() const       { return 0; }
    bool    P_IsVerboseParseMorphShape() const  { return 0; }
    bool    P_IsVerboseParseAction() const      { return 0; }
#else

    bool    P_IsVerboseParse() const            { return ((Derived*)this)->IsVerboseParse(); }

#ifdef  GFC_NO_FXPLAYER_VERBOSE_PARSE_SHAPE
    bool    P_IsVerboseParseShape() const       { return 0; }
#else
    bool    P_IsVerboseParseShape() const       { return ((Derived*)this)->IsVerboseParseShape(); }
#endif

#ifdef  GFC_NO_FXPLAYER_VERBOSE_PARSE_MORPHSHAPE
    bool    P_IsVerboseParseMorphShape() const  { return 0; }
#else
    bool    P_IsVerboseParseMorphShape() const  { return ((Derived*)this)->IsVerboseParseMorphShape(); }
#endif

#ifdef  GFC_NO_FXPLAYER_VERBOSE_PARSE_ACTION
    bool    P_IsVerboseParseAction() const      { return 0; }
#else
    bool    P_IsVerboseParseAction() const      { return ((Derived*)this)->IsVerboseParseAction(); }
#endif

#endif

#ifdef      GFC_NO_FXPLAYER_VERBOSE_ACTION
    bool    P_IsVerboseAction() const           { return 0; }
#else
    bool    P_IsVerboseAction() const           { return ((Derived*)this)->IsVerboseAction(); }
#endif

#ifdef      GFC_NO_FXPLAYER_VERBOSE_ACTION
    bool    P_IsVerboseActionErrors() const     { return 1; }
#else
    bool    P_IsVerboseActionErrors() const     { return ((Derived*)this)->IsVerboseActionErrors(); }
#endif
// DOM-IGNORE-END
    virtual bool    IsVerboseActionErrors() const   { return true; } //??

public:

    // Inline used to short-circuit "if (pLog)" conditional in log functions.
    // Implement it in base class to return 1 if GetLog is always 
    // guaranteed to return a valid pointer.
    bool    IsLogNotNull() const                { return 0; }
    

    // *** Non-Conditional (always working) logging lines
// DOM-IGNORE-BEGIN
    // Convenient printf-style functions.
    GFXLOG_MESSAGE_FUNC_DECL(LogError,          Log_Error)
    GFXLOG_MESSAGE_FUNC_DECL(LogWarning,        Log_Warning)
    GFXLOG_MESSAGE_FUNC_DECL(LogMessage,        Log_Message)

    // Dynamic conditional printf-style functions.
    // Parsing - controls by VerboseParse flags in loader
    GFXLOG_MESSAGE_FUNC_DECL(LogParse,  Log_Parse)
    GFXLOG_MESSAGE_FUNC_DECL(LogParseShape, Log_ParseShape)
    GFXLOG_MESSAGE_FUNC_DECL(LogParseMorphShape,  Log_ParseMorphShape)
    GFXLOG_MESSAGE_FUNC_DECL(LogParseAction, Log_ParseAction)
    // Action - controller by Verbose action flags in log
    GFXLOG_MESSAGE_FUNC_DECL(LogAction, Log_Action)
    GFXLOG_MESSAGE_FUNC_DECL(LogScriptError,    Log_ScriptError)
    GFXLOG_MESSAGE_FUNC_DECL(LogScriptWarning,  Log_ScriptWarning)
    GFXLOG_MESSAGE_FUNC_DECL(LogScriptMessage,  Log_ScriptMessage)
// DOM-IGNORE-END
};



class GFxLog : public GFxState, public GFxLogBase<GFxLog>
{
public: 
    GFxLog() : GFxState(State_Log) { }
    virtual ~GFxLog () { }

    // This virtual function receives all the messages,
    // developers should override this function in order to do custom logging
    virtual void    LogMessageVarg(LogMessageType messageType, const char* pfmt, va_list argList);

    // GFxLogBase implementation (use const_cast for compatibility with all other derived::GetLog declarations)
    GFxLog* GetLog() const          { return const_cast<GFxLog*>(this); }
    bool    IsLogNotNull() const    { return 1; }
};

GFXLOG_MESSAGE_FUNC(LogError,           Log_Error)
GFXLOG_MESSAGE_FUNC(LogWarning,         Log_Warning)
GFXLOG_MESSAGE_FUNC(LogMessage,         Log_Message)

GFXLOG_MESSAGE_FUNC_COND(LogScriptError,    IsVerboseActionErrors, Log_ScriptError)
GFXLOG_MESSAGE_FUNC_COND(LogScriptWarning,  IsVerboseActionErrors, Log_ScriptWarning)
GFXLOG_MESSAGE_FUNC(LogScriptMessage,   Log_ScriptMessage)

GFXLOG_MESSAGE_FUNC_COND(LogParse,  IsVerboseParse, Log_Parse)
GFXLOG_MESSAGE_FUNC_COND(LogParseShape, IsVerboseParseShape, Log_ParseShape)
GFXLOG_MESSAGE_FUNC_COND(LogParseMorphShape, IsVerboseParseMorphShape, Log_ParseMorphShape)
GFXLOG_MESSAGE_FUNC_COND(LogParseAction, IsVerboseParseAction, Log_ParseAction)
GFXLOG_MESSAGE_FUNC_COND(LogAction, IsVerboseAction, Log_Action)


// *** Inline implementation for GFxSharedState (GFxLoader.h) - must come after GFxLog.

inline  void    GFxSharedState::SetLog(GFxLog *plog)
{
    SetState(GFxState::State_Log, plog);
}
inline  GPtr<GFxLog> GFxSharedState::GetLog() const
{
    return *(GFxLog*)GetStateAddRef(GFxState::State_Log);
}



#endif // INC_GFXLOG_H

