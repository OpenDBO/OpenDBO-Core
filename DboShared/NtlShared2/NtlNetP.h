#pragma once

#define MAX_NETP_POINT ( 0xfffffffeui32 )

const DWORD	NETPY_POINTS_TIME_DURATION = 900;
const DWORD NETPY_MAX_SHOPPOINTS = 50000;
// Latest known TW info mentions 4 points every 15 minutes (was 1 every 15 minutes at first).
// Everyone says 14 but that's because UI shows 14 due to seconds being simply trimmed on 14:XX.
// http://www.dbocom.com/forum/showthread.php?p=336784#post336784
// TODO: Also they should be account bound, not character bound.
const DWORD	NETPY_POINTS_TIME_REWARD = 15;
const DWORD NETPY_POINTS_REWARD = 4;
