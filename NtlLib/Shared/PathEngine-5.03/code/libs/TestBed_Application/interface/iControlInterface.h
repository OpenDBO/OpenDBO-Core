//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class controlinterface
{
public:
	virtual long __stdcall getversion(long min, long max) = 0;
	
	virtual long __stdcall getmousex() = 0;
	virtual long __stdcall getmousey() = 0;
	virtual long __stdcall getlmbdown() = 0;
	virtual long __stdcall getmmbdown() = 0;
	virtual long __stdcall getrmbdown() = 0;
	virtual long __stdcall getismousevalid() = 0;
	virtual void __stdcall reserved_mouse0() = 0;
	virtual void __stdcall reserved_mouse1() = 0;
	virtual void __stdcall reserved_mouse2() = 0;
	virtual void __stdcall reserved_mouse3() = 0;
	virtual void __stdcall reserved_mouse4() = 0;
	virtual void __stdcall reserved_mouse5() = 0;
	virtual void __stdcall reserved_mouse6() = 0;
	virtual void __stdcall reserved_mouse7() = 0;
	virtual void __stdcall reserved_mouse8() = 0;
	virtual void __stdcall reserved_mouse9() = 0;

	virtual long __stdcall getkeystate(const char *keystring) = 0;
	virtual char* __stdcall getkeymessage() = 0;
	virtual void __stdcall reserved_kbd0() = 0;
	virtual void __stdcall reserved_kbd1() = 0;
	virtual void __stdcall reserved_kbd2() = 0;
	virtual void __stdcall reserved_kbd3() = 0;
	virtual void __stdcall reserved_kbd4() = 0;
	virtual void __stdcall reserved_kbd5() = 0;
	virtual void __stdcall reserved_kbd6() = 0;
	virtual void __stdcall reserved_kbd7() = 0;
	virtual void __stdcall reserved_kbd8() = 0;
	virtual void __stdcall reserved_kbd9() = 0;

// extra calls in version 2 interface
	virtual long __stdcall getMouseDX() = 0;
	virtual long __stdcall getMouseDY() = 0;
};
