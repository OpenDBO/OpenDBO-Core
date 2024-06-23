//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "interface/iControlInterface.h"

class cControlInterface : public controlinterface
{
    long _mouseX, _mouseY;
    bool _leftMouseDown;
    bool _middleMouseDown;
    bool _rightMouseDown;
    long _previousMouseX, _previousMouseY;
    long _mouseDeltaX, _mouseDeltaY;

    cControlInterface()
    {
        _mouseX = 0;
        _mouseY = 0;
        _leftMouseDown = false;
        _middleMouseDown = false;
        _rightMouseDown = false;
        _previousMouseX = 0;
        _previousMouseY = 0;
        _mouseDeltaX = 0;
        _mouseDeltaY = 0;
    }

public:

    static cControlInterface& refInstance();

    void updateMouseDeltas();
    void setMouseDeltasTo(long deltaX, long deltaY);

    void setMousePosition(long x, long y)
    {
        _mouseX = x;
        _mouseY = y;
    }
    void leftMouseDown()
    {
        _leftMouseDown = true;
    }
    void leftMouseUp()
    {
        _leftMouseDown = false;
    }
    void middleMouseDown()
    {
        _middleMouseDown = true;
    }
    void middleMouseUp()
    {
        _middleMouseDown = false;
    }
    void rightMouseDown()
    {
        _rightMouseDown = true;
    }
    void rightMouseUp()
    {
        _rightMouseDown = false;
    }

// interface

	long __stdcall getversion(long min, long max);
	
	long __stdcall getmousex();
	long __stdcall getmousey();
	long __stdcall getlmbdown();
	long __stdcall getmmbdown();
	long __stdcall getrmbdown();
	long __stdcall getismousevalid();
	void __stdcall reserved_mouse0();
	void __stdcall reserved_mouse1();
	void __stdcall reserved_mouse2();
	void __stdcall reserved_mouse3();
	void __stdcall reserved_mouse4();
	void __stdcall reserved_mouse5();
	void __stdcall reserved_mouse6();
	void __stdcall reserved_mouse7();
	void __stdcall reserved_mouse8();
	void __stdcall reserved_mouse9();

	long __stdcall getkeystate(const char* keystring);
	char* __stdcall getkeymessage();
	void __stdcall reserved_kbd0();
	void __stdcall reserved_kbd1();
	void __stdcall reserved_kbd2();
	void __stdcall reserved_kbd3();
	void __stdcall reserved_kbd4();
	void __stdcall reserved_kbd5();
	void __stdcall reserved_kbd6();
	void __stdcall reserved_kbd7();
	void __stdcall reserved_kbd8();
	void __stdcall reserved_kbd9();

// extra calls in version 2 interface
	long __stdcall getMouseDX();
	long __stdcall getMouseDY();
};
