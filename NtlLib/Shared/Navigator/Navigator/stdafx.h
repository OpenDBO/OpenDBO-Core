#ifndef ___STD_AFX_H___
#define ___STD_AFX_H___

#pragma once

#pragma warning( disable:4099 )		// Ignore pdb missing warning
#pragma warning( disable:4204 )		// Ignore pdb missing warning

// Turn-off this Warning for BoundsChecker

#pragma warning( disable:4651 )
#pragma warning( disable:4652 )
#pragma warning( disable:4653 )
#pragma warning( disable:4748 )


/*
#ifndef WINVER						// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501				// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT				// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501			// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS				// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410		// Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE					// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600			// Change this to the appropriate value to target other versions of IE.
#endif
*/

#define WIN32_LEAN_AND_MEAN			// Exclude rarely-used stuff from Windows headers


#include <tchar.h>
#include <windows.h>

/*
#ifndef WIN64
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)
#pragma warning(disable : 4996)
#pragma warning(disable : 4389)
#pragma warning(disable : 4100)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4127)
#pragma warning(disable : 4189)
#pragma warning(disable : 4239)
#pragma warning(disable : 4245)
#pragma warning(disable : 4701)
#pragma warning(disable : 4702)
#pragma warning(disable : 4706)
#pragma warning(disable : 4740)
#pragma warning(disable : 4748)
#include "DebugEx.h"
#pragma warning(default : 4244)
#pragma warning(default : 4018)
#pragma warning(default : 4996)
#pragma warning(default : 4389)
#pragma warning(default : 4100)
#pragma warning(default : 4311)
#pragma warning(default : 4312)
#pragma warning(default : 4127)
#pragma warning(default : 4189)
#pragma warning(default : 4239)
#pragma warning(default : 4245)
#pragma warning(default : 4701)
#pragma warning(default : 4702)
#pragma warning(default : 4706)
#pragma warning(default : 4740)
#pragma warning(default : 4748)
#endif //WIN64
*/

#endif //___STD_AFX_H___