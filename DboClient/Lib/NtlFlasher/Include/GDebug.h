/**********************************************************************

Filename    :   GDebug.h
Content     :   General purpose debugging support
Created     :   July 18, 2001
Authors     :   Brendan Iribe, Michael Antonov

Copyright   :   (c) 2001-2006 Scaleform Corp. All Rights Reserved.

Notes       :   GFC Debug warning functionality is enabled
                if and only if GFC_BUILD_DEBUG is defined.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GDEBUG_H
#define INC_GDEBUG_H

#include "GTypes.h"
#include <stdarg.h>


// ***** Declared Classes
class GDebug;



// ***** GDebug class

// GDebug class contains debug error/warning output routines with a
// customizable handler.
class GDebug
{
public:

    /*

    Different error messages have separate purposes and should be used appropriately
    as follows:

        Message_Note:       Note messages do not have a prefix and perform output for information
                            purposes only in debug mode.

        Message_Warning:    Warning messages should be emitted when the API is missused or
                            is in such a state that the desired operation will most likely
                            not perform as expected. However, the problem that caused the
                            warning is still handled gracefully by the system and will
                            not cause a crash. The release build of the program will not
                            display a message, but will handle the condition in the same
                            graceful manner.

        Message_Error:      Error messages should be emitted if there is an internal problem
                            that will cause a system or component to malfunction. These messages
                            may be generates when certain critical resources (files, devices,
                            memory) are not available or do not function properly. Error
                            messages can also be generated if the condition is serious enough
                            but will not be checked for in the release build (resource leak,
                            for example). Note that conditions for these messages should not cause
                            an immediate crash even in release build.

        Message_Assert:     Assertion failures should be generated when the program data structure
                            or parameter is in an invalid state that will cause an immediate
                            failure or crash. Assertion failure usually indicates a program logic
                            error or invalid use of an API (assert messages and should NOT be 
                            generated due to invalid user input or device/file states that can
                            be handled more gracefully). Assert conditions will not be checked for in
                            any shape or form in the release build and will most likely
                            cause an immediate crash.


    As an example, suppose a class object is passed to function that then utilizes its methods. The
    function can generate various messages based on the following criteria:

        - If the class pointer is null, or its internal state is invalid due to programmer oversight,
          the using function should generate a:
            - a Warning if the given parameter value/state will be handled gracefully in release build.
            - an Assert if the given conditions will not be checked for and would cause a crash/
              malfunction in release build.

        - If a method is further called on the specified object and then fails, it should generate
            - No message or a Note message if this is a valid user-handled behavior.
            - Warning message if this behavior can be avoided by external checks by the developer.
            - Error message if this behavior was outside of immediate caller's control, but
              will cause a malfunction. (I.E. Graphics device does not support required operation,
              memory was exhausted, or some internal algorithm buffer constraint has overflowed).

    */

    // GFC Error, controls the prefix that will be inserted into the message
    enum MessageType
    {
        Message_Note,       // No prefix
        Message_Warning,    // "GFC Warning:" prefix
        Message_Error,      // "GFC Error:" prefix      
        Message_Assert      // "GFC Assert:" prefix
    };

    // Formatting message handler
    typedef void    (GCDECL *MessageHandlerType)(MessageType msgType, const char *pformat, va_list argList);

    GEXPORT static void GCDECL Message(MessageType msgType, const char *pformat, ...);
    
    // Sets the message handler that will be used for output of debug messages
    // A value of null (0) causes the system-default handler to be used
    GEXPORT static void GCDECL SetMessageHandler(MessageHandlerType handler = 0);
};




// If not in debug build, macros do nothing
#ifndef GFC_BUILD_DEBUG

    #define GFC_DEBUG_BREAK                                                 ((void)0)
    #define GASSERT(p)                                                      ((void)0)
                                                                            
    // Simple output with prefix tag                                        
    #define GFC_DEBUG_OUTPUT(cond, msgtype, str )                           ((void)0)
    #define GFC_DEBUG_OUTPUT1(cond, msgtype, str, p1)                       ((void)0)
    #define GFC_DEBUG_OUTPUT2(cond, msgtype, str, p1, p2)                   ((void)0)
    #define GFC_DEBUG_OUTPUT3(cond, msgtype, str, p1, p2, p3)               ((void)0)
    #define GFC_DEBUG_OUTPUT4(cond, msgtype, str, p1, p2, p3, p4)           ((void)0)
    #define GFC_DEBUG_OUTPUT5(cond, msgtype, str, p1, p2, p3, p4, p5)       ((void)0)
    #define GFC_DEBUG_OUTPUT6(cond, msgtype, str, p1, p2, p3, p4, p5, p6)   ((void)0)

#else 

    // Simple output with prefix tag
    #define GFC_DEBUG_OUTPUT(cond, msgtype, str)                            do { if (cond) GDebug::Message(msgtype, str); } while (0)
    #define GFC_DEBUG_OUTPUT1(cond, msgtype, str, p1)                       do { if (cond) GDebug::Message(msgtype, str, p1); } while (0)
    #define GFC_DEBUG_OUTPUT2(cond, msgtype, str, p1, p2)                   do { if (cond) GDebug::Message(msgtype, str, p1,p2); } while (0)
    #define GFC_DEBUG_OUTPUT3(cond, msgtype, str, p1, p2, p3)               do { if (cond) GDebug::Message(msgtype, str, p1,p2,p3); } while (0)
    #define GFC_DEBUG_OUTPUT4(cond, msgtype, str, p1, p2, p3, p4)           do { if (cond) GDebug::Message(msgtype, str, p1,p2,p3,p4); } while (0)
    #define GFC_DEBUG_OUTPUT5(cond, msgtype, str, p1, p2, p3, p4, p5)       do { if (cond) GDebug::Message(msgtype, str, p1,p2,p3,p4,p5); } while (0)
    #define GFC_DEBUG_OUTPUT6(cond, msgtype, str, p1, p2, p3, p4, p5, p6)   do { if (cond) GDebug::Message(msgtype, str, p1,p2,p3,p4,p5,p6); } while (0)


// Microsoft Win32 specific debugging support
#if defined(GFC_OS_WIN32) || defined(GFC_OS_XBOX)
# ifdef GFC_CPU_X86
#   if defined(GFC_CC_GNU)
        #define GFC_DEBUG_BREAK do { GASM("int $3\n\t"); } while(0)
#   else
        #define GFC_DEBUG_BREAK do { GASM int 3 } while (0)
#   endif
# else
    #define GFC_DEBUG_BREAK     do { __debugbreak(); } while(0)
# endif
// Xbox360 specific debugging
# elif defined(GFC_OS_XBOX360)
#define GFC_DEBUG_BREAK         DebugBreak()
// Unix specific debugging support
# elif defined(GFC_CPU_X86)
    #define GFC_DEBUG_BREAK     do { GASM("int $3\n\t"); } while(0)
#else
    #define GFC_DEBUG_BREAK     do { *((int *) 0) = 1; } while(0)
#endif
    // This will cause compiler breakpoint
    #define GASSERT(p)          do { if (!(p))  { GFC_DEBUG_BREAK; } } while(0)

    // Assertion with messages
    // Unlike GASSERT, these macros display a message before breaking
    #define GFC_DEBUG_ASSERT(cond, str)                                     do { if (!(cond)) GDebug::Message(GDebug::Message_Assert, str); GFC_DEBUG_BREAK; } while (0)
    #define GFC_DEBUG_ASSERT1(cond, str, p1)                                do { if (!(cond)) GDebug::Message(GDebug::Message_Assert, str, p1); GFC_DEBUG_BREAK; } while (0)
    #define GFC_DEBUG_ASSERT2(cond, str, p1, p2)                            do { if (!(cond)) GDebug::Message(GDebug::Message_Assert, str, p1,p2); GFC_DEBUG_BREAK; } while (0)
    #define GFC_DEBUG_ASSERT3(cond, str, p1, p2, p3)                        do { if (!(cond)) GDebug::Message(GDebug::Message_Assert, str, p1,p2,p3); GFC_DEBUG_BREAK; } while (0)
    #define GFC_DEBUG_ASSERT4(cond, str, p1, p2, p3, p4)                    do { if (!(cond)) GDebug::Message(GDebug::Message_Assert, str, p1,p2,p3,p4); GFC_DEBUG_BREAK; } while (0)
    #define GFC_DEBUG_ASSERT5(cond, str, p1, p2, p3, p4, p5)                do { if (!(cond)) GDebug::Message(GDebug::Message_Assert, str, p1,p2,p3,p4,p5); GFC_DEBUG_BREAK; } while (0)
    #define GFC_DEBUG_ASSERT6(cond, str, p1, p2, p3, p4, p5, p6)            do { if (!(cond)) GDebug::Message(GDebug::Message_Assert, str, p1,p2,p3,p4,p5,p6); GFC_DEBUG_BREAK; } while (0)


#endif // GFC_BUILD_DEBUG


// Conditional warnings - "GFC Warning: " prefix
#define GFC_DEBUG_WARNING(cond, str)                            GFC_DEBUG_OUTPUT(cond,  GDebug::Message_Warning, str)
#define GFC_DEBUG_WARNING1(cond, str, p1)                       GFC_DEBUG_OUTPUT1(cond, GDebug::Message_Warning, str,  p1)
#define GFC_DEBUG_WARNING2(cond, str, p1, p2)                   GFC_DEBUG_OUTPUT2(cond, GDebug::Message_Warning, str,  p1,p2)
#define GFC_DEBUG_WARNING3(cond, str, p1, p2, p3)               GFC_DEBUG_OUTPUT3(cond, GDebug::Message_Warning, str,  p1,p2,p3)
#define GFC_DEBUG_WARNING4(cond, str, p1, p2, p3, p4)           GFC_DEBUG_OUTPUT4(cond, GDebug::Message_Warning, str,  p1,p2,p3,p4)
#define GFC_DEBUG_WARNING5(cond, str, p1, p2, p3, p4, p5)       GFC_DEBUG_OUTPUT5(cond, GDebug::Message_Warning, str,  p1,p2,p3,p4,p5)
#define GFC_DEBUG_WARNING6(cond, str, p1, p2, p3, p4, p5, p6)   GFC_DEBUG_OUTPUT6(cond, GDebug::Message_Warning, str,  p1,p2,p3,p4,p5,p6)
// Conditional errors - "GFC Error: " prefix
#define GFC_DEBUG_ERROR(cond, str)                              GFC_DEBUG_OUTPUT(cond,  GDebug::Message_Error, str)
#define GFC_DEBUG_ERROR1(cond, str, p1)                         GFC_DEBUG_OUTPUT1(cond, GDebug::Message_Error, str,  p1)
#define GFC_DEBUG_ERROR2(cond, str, p1, p2)                     GFC_DEBUG_OUTPUT2(cond, GDebug::Message_Error, str,  p1,p2)
#define GFC_DEBUG_ERROR3(cond, str, p1, p2, p3)                 GFC_DEBUG_OUTPUT3(cond, GDebug::Message_Error, str,  p1,p2,p3)
#define GFC_DEBUG_ERROR4(cond, str, p1, p2, p3, p4)             GFC_DEBUG_OUTPUT4(cond, GDebug::Message_Error, str,  p1,p2,p3,p4)
#define GFC_DEBUG_ERROR5(cond, str, p1, p2, p3, p4, p5)         GFC_DEBUG_OUTPUT5(cond, GDebug::Message_Error, str,  p1,p2,p3,p4,p5)
#define GFC_DEBUG_ERROR6(cond, str, p1, p2, p3, p4, p5, p6)     GFC_DEBUG_OUTPUT6(cond, GDebug::Message_Error, str,  p1,p2,p3,p4,p5,p6)
// Conditional messages - no prefix
#define GFC_DEBUG_MESSAGE(cond, str)                            GFC_DEBUG_OUTPUT(cond,  GDebug::Message_Note, str)
#define GFC_DEBUG_MESSAGE1(cond, str, p1)                       GFC_DEBUG_OUTPUT1(cond, GDebug::Message_Note, str,  p1)
#define GFC_DEBUG_MESSAGE2(cond, str, p1, p2)                   GFC_DEBUG_OUTPUT2(cond, GDebug::Message_Note, str,  p1,p2)
#define GFC_DEBUG_MESSAGE3(cond, str, p1, p2, p3)               GFC_DEBUG_OUTPUT3(cond, GDebug::Message_Note, str,  p1,p2,p3)
#define GFC_DEBUG_MESSAGE4(cond, str, p1, p2, p3, p4)           GFC_DEBUG_OUTPUT4(cond, GDebug::Message_Note, str,  p1,p2,p3,p4)
#define GFC_DEBUG_MESSAGE5(cond, str, p1, p2, p3, p4, p5)       GFC_DEBUG_OUTPUT5(cond, GDebug::Message_Note, str,  p1,p2,p3,p4,p5)
#define GFC_DEBUG_MESSAGE6(cond, str, p1, p2, p3, p4, p5, p6)   GFC_DEBUG_OUTPUT6(cond, GDebug::Message_Note, str,  p1,p2,p3,p4,p5,p6)


// Convenient simple type value object output
// - v must be a stack object
#define GFC_DEBUG_SINT(v)                                       GFC_DEBUG_OUTPUT1(1, GDebug::Message_None, #v "(%d)", SInt(v))
#define GFC_DEBUG_UINT(v)                                       GFC_DEBUG_OUTPUT1(1, GDebug::Message_None, #v "(%u)", UInt(v))
#define GFC_DEBUG_HEX(v)                                        GFC_DEBUG_OUTPUT1(1, GDebug::Message_None, #v "(0x%X)", UInt(v))
#define GFC_DEBUG_FLOAT(v)                                      GFC_DEBUG_OUTPUT1(1, GDebug::Message_None, #v "(%f)", Double(v))
#define GFC_DEBUG_DOUBLE(v)                                     GFC_DEBUG_OUTPUT1(1, GDebug::Message_None, #v "(%f)", Double(v))
#define GFC_DEBUG_CHAR(v)                                       GFC_DEBUG_OUTPUT1(1, GDebug::Message_None, #v "(%c)", char(v))
#define GFC_DEBUG_WCHAR(v)                                      GFC_DEBUG_OUTPUT1(1, GDebug::Message_None, #v "(%C)", wchar_t(v))


#endif
