/**********************************************************************

Filename    :   GTimer.h
Content     :   Provides static functions for precise timing
Created     :   June 28, 2005
Authors     :   
Notes       :   

Copyright   :   (c) 1998-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GTIMER_H
#define INC_GTIMER_H

#include "GTypes.h"


// ***** Declared Classes
class GTimer;



// Hi-res timer for CPU profiling.
class GTimer
{
public:

    // General-purpose wall-clock timer.
    // May not be hi-res enough for profiling.               
    static UInt64   GSTDCALL GetTicks();
    static Double   GSTDCALL TicksToSeconds(UInt64 ticks);

    // Return a hi-res timer value.  Time 0 is arbitrary, so
    // generally you want to call this at the start and end of an
    // operation, and pass the difference to
    // ProfileTicksToSeconds() to find out how long the operation took. 
    static UInt64   GSTDCALL GetProfileTicks();

    // Convert a hi-res ticks value into seconds
    static Double   GSTDCALL ProfileTicksToSeconds(UInt64 profileTicks);

#ifdef GFC_OS_PS3
private:
    static double s_fFreqInv;
    static bool s_bInitialized;
#endif // GFC_OS_PS3
};


#endif // INC_GTIMER_H
