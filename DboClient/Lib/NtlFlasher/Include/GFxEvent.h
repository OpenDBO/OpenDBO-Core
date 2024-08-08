/**********************************************************************

Filename    :   GFxEvent.h
Content     :   Public interface for SWF Movie input events
Created     :   June 21, 2005
Authors     :   Michael Antonov

Notes       :   

Copyright   :   (c) 2005-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFXEVENT_H
#define INC_GFXEVENT_H

#include "GTypes.h"
#include "GRefCount.h"
#include "GTLTypes.h"

#include <stdlib.h>
#include <string.h>

// ***** Declared Classes
class GFxKey;
class GFxEvent;
class GFxMouseEvent;
class GFxKeyEvent;
class GFxCharEvent;
class GFxKeyboardState;
class GFxSceneEvent;

// Forward declarations.
class GASStringContext;

// Key code wrapper class. This class contains enumeration
// for keyboard constants.

class GFxKey
{
public:
    // These key codes match Flash-defined values exactly.
    enum Code
    {
        VoidSymbol      = 0,

        // A through Z and numbers 0 through 9.
        A               = 65,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Num0            = 48,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        // Numeric keypad.
        KP_0            = 96,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_Multiply,
        KP_Add,
        KP_Enter,
        KP_Subtract,
        KP_Decimal,
        KP_Divide,

        // Function keys.
        F1              = 112,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,

        // Other keys.
        Backspace       = 8,
        Tab,
        Clear           = 12,
        Return,
        Shift           = 16,
        Control,
        Alt,
        CapsLock        = 20, // Toggle
        Escape          = 27,
        Space           = 32,
        PageUp,
        PageDown,
        End             = 35,
        Home,
        Left,
        Up,
        Right,
        Down,
        Insert          = 45,
        Delete,
        Help,
        NumLock         = 144, // Toggle
        ScrollLock      = 145, // Toggle

        Semicolon       = 186,
        Equal           = 187,
        Comma           = 188, // Platform specific?
        Minus           = 189,
        Period          = 190, // Platform specific?
        Slash           = 191,
        Bar             = 192,
        BracketLeft     = 219,
        Backslash       = 220,
        BracketRight    = 221,
        Quote           = 222,

        // Total number of keys.
        KeyCount
    };
};

// ***** Debug check support

// This expression will only be avaluated in debug mode.
#ifdef GFC_BUILD_DEBUG
    #define GFC_DEBUG_EXPR(arg) arg
#else
    #define GFC_DEBUG_EXPR(arg)
#endif


// ***** Event classes


// GFxEvent is a base class for input event notification. Objects of this type
// can be passed to GFxMovie::HandleEvent to inform the movie of various input
// actions. Events that have specific subclasses, such as GFxMouseEvent, must
// use those subclasses and NOT the base GFxEvent class.

class GFxEvent
{
public:
    enum    EventType
    {
        None,       
        // Informative events sent to the player.
        MouseMove,
        MouseDown,
        MouseUp,
        MouseWheel,
        KeyDown,
        KeyUp,
        SceneResize,
        SetFocus,
        KillFocus,

        // Action events, to be handled by user.
        DoShowMouse,
        DoHideMouse,
        DoSetMouseCursor,

        CharEvent
    };

    // What kind of event this is.
    EventType   Type;
    // Size of class, used in debug build to verify that
    // appropriate classes are used for messages.
    GFC_DEBUG_EXPR(UInt EventClassSize);


    GFxEvent(EventType eventType = None)
    {
        Type = eventType;
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxEvent));
    }
};


class   GFxMouseEvent : public GFxEvent
{
public:
    SInt    x, y;
    Float   ScrollDelta;
    UInt    Button;

    GFxMouseEvent() : GFxEvent()
    {
        Button = 0; x = 0; y = 0; ScrollDelta = 0.0f;
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxMouseEvent));
    }
    // User event constructor - x, y must always be filled in.
    GFxMouseEvent(EventType eventType, UInt button,
                  SInt xpos, SInt ypos, Float scrollVal = 0.0f)
        : GFxEvent(eventType)
    {
        Button = button; x = xpos; y = ypos; ScrollDelta = scrollVal;
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxMouseEvent));
    }
};

class   GFxMouseCursorEvent : public GFxEvent
{
public:
    enum CursorShapeType
    {
        ARROW = 0,
        HAND  = 1,
        IBEAM = 2
    };
    UInt    CursorShape;

    GFxMouseCursorEvent() : GFxEvent(GFxEvent::DoSetMouseCursor), CursorShape(HAND)
    {
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxMouseCursorEvent));
    }
    GFxMouseCursorEvent(UInt cursorShape)
        : GFxEvent(GFxEvent::DoSetMouseCursor), CursorShape(cursorShape)
    {
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxMouseCursorEvent));
    }
};

struct GFxSpecialKeysState 
{
    enum
    {
        Key_ShiftPressed    = 0x01,
        Key_CtrlPressed     = 0x02,
        Key_AltPressed      = 0x04,
        Key_CapsToggled     = 0x08,
        Key_NumToggled      = 0x10,
        Key_ScrollToggled   = 0x20
    };
    UInt8   States;

    GFxSpecialKeysState():States(0) {}
    GFxSpecialKeysState(int):States(0) {}

    void Reset() { States = 0; }

    bool IsShiftPressed() const { return (States & Key_ShiftPressed) != 0; }
    bool IsCtrlPressed() const  { return (States & Key_CtrlPressed) != 0; }
    bool IsAltPressed() const   { return (States & Key_AltPressed) != 0; }
    bool IsCapsToggled() const  { return (States & Key_CapsToggled) != 0; }
    bool IsNumToggled() const   { return (States & Key_NumToggled) != 0; }
    bool IsScrollToggled() const{ return (States & Key_ScrollToggled) != 0; }

    void SetShiftPressed(bool v = true)  { (v) ? States |= Key_ShiftPressed : States &= ~Key_ShiftPressed; }
    void SetCtrlPressed(bool v = true)   { (v) ? States |= Key_CtrlPressed  : States &= ~Key_CtrlPressed; }
    void SetAltPressed(bool v = true)    { (v) ? States |= Key_AltPressed   : States &= ~Key_AltPressed; }
    void SetCapsToggled(bool v = true)   { (v) ? States |= Key_CapsToggled  : States &= ~Key_CapsToggled; }
    void SetNumToggled(bool v = true)    { (v) ? States |= Key_NumToggled   : States &= ~Key_NumToggled; }
    void SetScrollToggled(bool v = true) { (v) ? States |= Key_ScrollToggled: States &= ~Key_ScrollToggled; }
};

class GFxKeyEvent : public GFxEvent
{
public:
    // Key, and whether it was pressed up or down
    GFxKey::Code        KeyCode;
    // State of special keys
    GFxSpecialKeysState SpecialKeysState;
    // ASCII code. 0, if unavailable.
    UByte               AsciiCode; // left for compatibility reasons only
    UInt                WcharCode; // left for compatibility reasons only

    GFxKeyEvent(EventType eventType = None, GFxKey::Code code = GFxKey::VoidSymbol, UByte asciiCode = 0, UInt32 wcharCode = 0)
        : GFxEvent(eventType), KeyCode (code), AsciiCode (asciiCode), WcharCode(wcharCode)
    { 
        SpecialKeysState.Reset(); 
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxKeyEvent));
    }
};

class GFxCharEvent : public GFxEvent
{
public:
    UInt32              WcharCode;
    // State of special keys
    //GFxSpecialKeysState SpecialKeysState;

    GFxCharEvent(UInt32 wcharCode)
        : GFxEvent(CharEvent), WcharCode(wcharCode)
    { 
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxCharEvent));
    }
};

// Keyboard state and queue class. 
class GFxKeyboardState : public GRefCountBase<GFxKeyboardState>
{
public:
    class IListener : public GRefCountBase<IListener> 
    {
    public:
        virtual void OnKeyDown(GASStringContext *psc, int code, UByte ascii, UInt32 wcharCode)
            { GUNUSED4(psc, code, ascii, wcharCode); }
        virtual void OnKeyUp(GASStringContext *psc, int code, UByte ascii, UInt32 wcharCode)
            { GUNUSED4(psc, code, ascii, wcharCode); }
    };

protected:
    
    class KeyQueue 
    {
    private:
        enum { KeyQueueSize = 100 };
        struct KeyRecord 
        {
            UInt32                          wcharCode;
            short                           code;
            GFxEvent::EventType             event;
            UByte                           ascii;
            GFxSpecialKeysState             specialKeysState;
        }               Buffer[KeyQueueSize];
        unsigned        PutIdx;
        unsigned        GetIdx;
        unsigned        Count;
    public:
        KeyQueue ();

        void Put(short code, UByte ascii, UInt32 wcharCode, GFxEvent::EventType event, GFxSpecialKeysState specialKeysState = 0);
        bool Get(short* code, UByte* ascii, UInt32* wcharCode, GFxEvent::EventType* event, GFxSpecialKeysState* specialKeysState = 0);

        GINLINE bool IsEmpty () const { return Count == 0; }

        void ResetState();

    } KeyQueue;

    UByte               Keymap[GFxKey::KeyCount / 8 + 1];   // bit-GTL::garray
    bool                Toggled[3];
    GTL::garray<GWeakPtr<IListener> >   Listeners;
public:

    GFxKeyboardState();

    bool    IsKeyDown(int code) const;
    bool    IsKeyToggled(int code) const;
    void    SetKeyToggled(int code, bool toggle);
    void    SetKeyDown(int code, UByte ascii, GFxSpecialKeysState specialKeysState = 0);
    void    SetKeyUp(int code, UByte ascii, GFxSpecialKeysState specialKeysState = 0);
    void    SetChar(UInt32 wcharCode);

    bool    IsQueueEmpty() const { return KeyQueue.IsEmpty(); }
    bool    GetQueueEntry(short* code, UByte* ascii, UInt32* wcharCode, GFxEvent::EventType* event, GFxSpecialKeysState* specialKeysState = 0);

    void    CleanupListeners();
    void    AddListener(IListener* listener);
    void    RemoveListener(IListener* listener);

    void    NotifyListeners(GASStringContext *psc, short code, UByte ascii, UInt32 wcharCode, GFxEvent::EventType event);

    void    ResetState();
};

class GFxSceneEvent : public GFxEvent
{
public:
    // New size of the scene.
    SInt    Width, Height;


    GFxSceneEvent()
        : GFxEvent()
    {
        Width = Height = 0;
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxSceneEvent));
    }
    GFxSceneEvent(EventType eventType, SInt w, SInt h)
        : GFxEvent(eventType)
    {
        Width = w; Height = h;
        GFC_DEBUG_EXPR(EventClassSize = sizeof(GFxSceneEvent));
    }
};



#endif // ! INC_GFXEVENT_H
