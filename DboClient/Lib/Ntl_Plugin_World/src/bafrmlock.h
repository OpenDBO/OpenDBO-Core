/*
 * bafrmlock.h
 *
 * Lock around the dirty frame list, see bafrmlock.c
 */

#ifndef BAFRMLOCK_H
#define BAFRMLOCK_H

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

extern void _rwFrameDirtyListLockOpen(void);
extern void _rwFrameDirtyListLockClose(void);
extern void _rwFrameDirtyListLock(void);
extern void _rwFrameDirtyListUnlock(void);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* BAFRMLOCK_H */
