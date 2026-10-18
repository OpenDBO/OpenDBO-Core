/*
 * bafrmlock.c
 *
 * The dirty frame list (RWSRCGLOBAL(dirtyFrameList)) is shared by every thread that moves frames.
 * The game loads the world on a worker thread (CGameLoadingThread) while the main thread keeps
 * rendering, and both change the list. Without a lock an insert could be lost, leaving a frame
 * flagged dirty but not linked; destroying that frame later (logout) relinked the list through stale
 * pointers and the next insert made a self linked node, so _rwFrameSyncDirty never ended.
 *
 * Kept in its own file so the RenderWare sources do not need windows.h.
 */

#include <windows.h>

#include "bafrmlock.h"

static CRITICAL_SECTION frameDirtyListLock;
static int              frameDirtyListLockReady = 0;

void
_rwFrameDirtyListLockOpen(void)
{
    if (!frameDirtyListLockReady)
    {
        InitializeCriticalSection(&frameDirtyListLock);
        frameDirtyListLockReady = 1;
    }
}

void
_rwFrameDirtyListLockClose(void)
{
    if (frameDirtyListLockReady)
    {
        frameDirtyListLockReady = 0;
        DeleteCriticalSection(&frameDirtyListLock);
    }
}

/* recursive: frame syncs run object callbacks that may dirty frames again */
void
_rwFrameDirtyListLock(void)
{
    if (frameDirtyListLockReady)
    {
        EnterCriticalSection(&frameDirtyListLock);
    }
}

void
_rwFrameDirtyListUnlock(void)
{
    if (frameDirtyListLockReady)
    {
        LeaveCriticalSection(&frameDirtyListLock);
    }
}
