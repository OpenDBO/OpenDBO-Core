//***********************************************************************************
//
//	File		:	NtlThreadPool.h
//
//	Begin		:	2005-11-30
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Thread Pool Class
//
//***********************************************************************************

#ifndef __NTLTHREADPOOL_H__
#define __NTLTHREADPOOL_H__

#include "NtlLinkList.h"

class CNtlWorkItem : public CNtlLinkObject
{
public:
	virtual ~CNtlWorkItem() {}

	virtual void Run() = 0;
};

class CThreadPoolImp; // ThreadPool ³»ºÎ¿ë
class CNtlThreadPool
{
public:

	enum THREAD_POOL_OPTION
	{
		POOL_KERNEL_MODE		= 0x01,
		POOL_USER_MODE			= 0x02,

		MAX_THREADS				= 25,
		MIN_SPARE_THREADS		= 1,
		MAX_SPARE_THREADS		= 5,

		WORK_WAIT_TIMEOUT		= 60,
		QUEUE_HANDLER_TIMEOUT	= 30,

		MAX_THREAD_POOL_OPTION
	};

public:

	CNtlThreadPool(void);

public:
	virtual ~CNtlThreadPool(void);


private:

	CNtlThreadPool(const CNtlThreadPool&);
	CNtlThreadPool& operator=(const CNtlThreadPool&);


public:

	int						Create(	int createFlag,
									int nMaxThreads = MAX_THREADS,
									int nMinSpareThreads = MIN_SPARE_THREADS,
									int nMaxSpareThread = MAX_SPARE_THREADS,
									int nMonitorTimeout = WORK_WAIT_TIMEOUT,
									int nQueueHandlerTimeout = QUEUE_HANDLER_TIMEOUT);

	void					Start();

	void					QueueWorkItem(CNtlWorkItem * pWorkItem);

	void					Shutdown();


	static void				UnitTest();


private:

	CThreadPoolImp *		m_pThreadPoolImp;
};


#endif // __NTLTHREADPOOL_H__
