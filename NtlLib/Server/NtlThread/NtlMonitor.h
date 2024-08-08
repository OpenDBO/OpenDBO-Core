//***********************************************************************************
//
//	File		:	NtlMonitor.h
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Mutex(mutual exclusion)와 Event (signal and wait) 특징 갖고 동기화 객체 획득에 대한 순서 보장을 한다.
//
//***********************************************************************************

#ifndef __NTLMONITOR_H__
#define __NTLMONITOR_H__

#include "NtlLinkList.h"
#include "NtlMutex.h"

class CNtlThread;
class CNtlMonitor
{
public:
	CNtlMonitor(void);

	virtual ~CNtlMonitor(void);


private:

	CNtlMonitor(const CNtlMonitor &);

	CNtlMonitor & operator=(const CNtlMonitor &);


	void					Reset();


public:


	void					Lock();

	void					Unlock();

	bool					TryLock();

	void					Wait();

	bool					Wait(unsigned int millisecs);

	void					Notify();

	void					NotifyAll();


protected:

	void					Grant();


private:

	CNtlMutex				m_mutex;

	CNtlThread *			m_pOwner;

	int						m_nLockCount;

	CNtlLinkList			m_waitList; // 현재 wait 중인 쓰레드 리스트

	CNtlLinkList			m_readyList; // 현재 소유권 획득을 위해 대기하는 쓰레드 리스트

};

#endif // __NTLMONITOR_H__
