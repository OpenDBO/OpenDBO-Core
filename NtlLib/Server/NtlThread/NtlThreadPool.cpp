//***********************************************************************************
//
//	File		:	NtlThreadPool.cpp
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Thread Pool Class
//
//***********************************************************************************


#include "StdAfx.h"

#include "NtlBitFlag.h"
#include "NtlBase.h"

#include "NtlThreadPool.h"
#include "NtlThread.h"
#include "NtlThreadException.h"
#include "NtlMutex.h"
#include "NtlMonitor.h"




//---------------------------------------------------------------------------------------
// CThreadPoolImp ( ThreadPool 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CThreadPoolImp
{
public:

	virtual bool			Start() { return true; }

	virtual void			Shutdown() {}

	virtual void			QueueWorkItem(CNtlWorkItem * pWorkItem) = 0;
};


//---------------------------------------------------------------------------------------
// CThreadPoolImp_KernelMode ( ThreadPool 클래스 내부용 )
//---------------------------------------------------------------------------------------
class CThreadPoolImp_KernelMode : public CThreadPoolImp
{
public:

	void					QueueWorkItem(CNtlWorkItem * pWorkItem);
};


//---------------------------------------------------------------------------------------
// CThreadPoolImp_UserMode ( ThreadPool 클래스 내부용 )
// 테스트 필요
//---------------------------------------------------------------------------------------
class CWorkerThread;
class CWorkerThreadMonitor;
class CWorkItemQueueMonitor;
class CThreadPoolImp_UserMode : public CThreadPoolImp
{
friend class CWorkerThread;
friend class CWorkerThreadMonitor;
friend class CWorkItemQueueMonitor;


public:
	
	CThreadPoolImp_UserMode(	int nMaxThreads, int nMinSpareThreads, int nMaxSpareThread,
								int nMonitorTimeout, int nQueueHandlerTimeout);


private:

	CThreadPoolImp_UserMode( const CThreadPoolImp_UserMode &);
	CThreadPoolImp_UserMode & operator=( const CThreadPoolImp_UserMode &);


public:

// override

	bool					Start();

	void					QueueWorkItem(CNtlWorkItem * pWorkItem);

	void					Shutdown();


public:

	void					RunWorkItem(CNtlWorkItem * pWorkItem);


private:

	void					HarvestSpareWorkers();

	void					ReturnWorkerThread(CWorkerThread * pWorkerThread);

	void					AdjustLimits();

	int						OpenThreads(int nOpenThreadCount);

	int						GetThreadMonitorTimeout() { return m_nThreadMonitorTimeout; }

	int						GetWorkItemMonitorTimeout() { return m_nWorkItemMonitorTimeout; }

	CNtlWorkItem *			GetWorkItem();


private:

	CNtlLinkList			m_ndleThreadList;	

	CNtlLinkList			m_workItemList;

	CNtlMutex				m_workItemMutex;

	CWorkerThreadMonitor *	m_pThreadMonitor;

	CWorkItemQueueMonitor *	m_pWorkItemQueueMonitor;

	CNtlMonitor				m_monitor;


	int						m_nMaxThreads;

	int						m_nMaxSpareThreads;

	int						m_nMinSpareThreads;

	int						m_nThreadMonitorTimeout;

	int						m_nWorkItemMonitorTimeout;

	int						m_nTotalThreadCount;

	int						m_nActiveThreadCount;

	bool					m_bShutdown;

};



//---------------------------------------------------------------------------------------
// NtlThread Pool 
//---------------------------------------------------------------------------------------
CNtlThreadPool::CNtlThreadPool(void)
:
m_pThreadPoolImp( 0 )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlThreadPool::~CNtlThreadPool(void)
{
	SAFE_DELETE( m_pThreadPoolImp );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlThreadPool::Create(int createFlag, int nMaxThreads, int nMinSpareThreads, int nMaxSpareThread, int nMonitorTimeout, int nQueueHandlerTimeout)
{
	if( BIT_FLAG_TEST( createFlag, POOL_KERNEL_MODE ) )
	{
		m_pThreadPoolImp = new CThreadPoolImp_KernelMode;
	}
	else if( BIT_FLAG_TEST( createFlag, POOL_USER_MODE ) )
	{
		m_pThreadPoolImp = new CThreadPoolImp_UserMode(nMaxThreads, nMinSpareThreads, nMaxSpareThread, nMonitorTimeout, nQueueHandlerTimeout);
	}
	else
	{
		return NTL_ERR_THREAD_POOL_CREATE_FAIL;
	}


	if (NULL == m_pThreadPoolImp)
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadPool::Start()
{
	if( NULL != m_pThreadPoolImp )
	{
		m_pThreadPoolImp->Start();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadPool::QueueWorkItem(CNtlWorkItem * pWorkItem)
{
	m_pThreadPoolImp->QueueWorkItem( pWorkItem );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlThreadPool::Shutdown()
{
	if( NULL != m_pThreadPoolImp)
	{
		m_pThreadPoolImp->Shutdown();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	: CWorkerThreadMonitor : 주기적으로 쓰레드들을 관리한다 ( ThreadPool 클래스 내부용 )
//-----------------------------------------------------------------------------------
class CWorkerThreadMonitor : public CNtlRunObject
{
friend class CThreadPoolImp_UserMode;

private:

	CWorkerThreadMonitor(CThreadPoolImp_UserMode * pPool)
		:m_pPool( pPool ), m_bShutdown( false )
	{
		CNtlThread * pThread = CNtlThreadFactory::CreateThread(this, "MonitorThread", true);
		pThread->Start();
	}

	void Run()
	{
		bool bLoop = true;
		while( false != bLoop )
		{
			Wait( m_pPool->GetThreadMonitorTimeout() );

			m_mutex.Lock();
			bool bFlag = m_bShutdown;
			m_mutex.Unlock();

			if( bFlag )
			{
				break;
			}

			// 쓰레드 정리
			m_pPool->HarvestSpareWorkers();
		}
	}

	void Shutdown()
	{
		m_mutex.Lock();

		m_bShutdown = true;

		m_mutex.Unlock();
	}



private:

	CThreadPoolImp_UserMode *	m_pPool;

	bool						m_bShutdown;

	CNtlMutex					m_mutex;

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	: WorkItem 큐를 감시하고 워크 쓰레드를 동작시킨다
//-----------------------------------------------------------------------------------
class CWorkItemQueueMonitor : public CNtlRunObject
{
public:

	CWorkItemQueueMonitor(CThreadPoolImp_UserMode * pPool)
		:m_pPool( pPool ), m_bShutdown( false )
	{
		CNtlThread * pThread = CNtlThreadFactory::CreateThread( this, "WorkItemQueueMonitor");
		pThread->Start();
	}


	void Run()
	{
		bool bLoop = true;
		while( false != bLoop )
		{
			CNtlWorkItem * pWorkItem = m_pPool->GetWorkItem();

			if( !pWorkItem )
			{
				m_workItemCond.Lock();
				m_workItemCond.Wait();
				m_workItemCond.Unlock();
			}
			else
			{
				m_pPool->RunWorkItem( pWorkItem );
			}

			m_mutex.Lock();
			bool bShutdown = m_bShutdown;
			m_mutex.Unlock();

			if( bShutdown )
			{
				break;
			}
		}
	}


	void Shutdown()
	{
		m_mutex.Lock();
		m_bShutdown = true;
		m_mutex.Unlock();

		m_workItemCond.Notify(); // 쓰레드가 wait 중일 경우도 있다.
	}

	void NotifyWorkItemReady()
	{
		m_workItemCond.Notify();
	}


private:

	CThreadPoolImp_UserMode *	m_pPool;

	bool						m_bShutdown;

	CNtlMutex					m_mutex;

	CNtlMonitor					m_workItemCond;

};

//---------------------------------------------------------------------------------------
// 실제 WorkItem을 처리하는 WorkThread
//---------------------------------------------------------------------------------------
class CPoolObject;
class CWorkerThread : public CNtlRunObject
{
friend class CPoolObject;
friend class CThreadPoolImp_UserMode;

private:

	CWorkerThread(CThreadPoolImp_UserMode * pPool)
		:m_pPool( pPool ), m_pWorkItem( 0 ), m_bShutdown( false ), m_bRun( false ), m_pPoolObject( 0 )
	{
		static int nThreadNumber = 0;
		CNtlString strTemp;
		strTemp.Format("WorkerThread[%03d]", ++nThreadNumber);
		CNtlThread * pThread = CNtlThreadFactory::CreateThread( this, strTemp.c_str() );
		pThread->Start();
	}

	void Reset()
	{
		SAFE_DELETE( m_pWorkItem );
		m_bShutdown = false;
		m_bRun = false;
	}

	void Run()
	{
		bool bLoop = true;
		while( false != bLoop )
		{
			m_cond.Lock();

			if( !m_bRun && !m_bShutdown )
			{
				m_cond.Wait();
			}

			bool bRun		= m_bRun;
			bool bShutdown	= m_bShutdown;

			m_cond.Unlock();

			if( bShutdown )
			{
				break;
			}

			if( bRun )
			{
				try
				{
					m_pWorkItem->Run();
				}
				catch( CNtlException e )
				{
					e.Dump();
				}
				catch( ... )
				{
				}

				Reset();
				m_pPool->ReturnWorkerThread( this );
			}
		}

		Reset();
	}

	void Schedule(CNtlWorkItem * pWorkItem)
	{
		m_cond.Lock();

		m_bRun = true;

		m_pWorkItem = pWorkItem;

		m_cond.Notify();

		m_cond.Unlock();
	}

	void Shutdown()
	{
		m_cond.Lock();

		m_bShutdown = true;

		m_cond.Notify();

		m_cond.Unlock();
	}

private:

	void SetPoolObject(CPoolObject * pPoolObject) { m_pPoolObject = pPoolObject; }
	CPoolObject * GetPoolObject() const { return m_pPoolObject; }

public:


private:

	CNtlWorkItem *				m_pWorkItem;

	bool						m_bShutdown;

	bool						m_bRun;

	CPoolObject *				m_pPoolObject;

	CThreadPoolImp_UserMode *	m_pPool;

	CNtlMonitor					m_cond;
};


class CPoolObject : public CNtlLinkObject
{
public:

	CPoolObject(CWorkerThread * pWorkerThread)
		:m_pWorkerThread( pWorkerThread )
	{
		m_pWorkerThread->SetPoolObject( this );
	}

	CWorkerThread *				GetWorkerThread() { return m_pWorkerThread; }

private:

	CWorkerThread *				m_pWorkerThread;
};


//---------------------------------------------------------------------------------------
// Work Thread Procedure
//---------------------------------------------------------------------------------------
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	CNtlWorkItem * pWorkItem = (CNtlWorkItem *) lpParam;

	pWorkItem->Run();

	SAFE_DELETE( pWorkItem );

	return 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CThreadPoolImp_KernelMode::QueueWorkItem(CNtlWorkItem * pWorkItem)
{
	// 테스트 필요!!
	::QueueUserWorkItem(ThreadProc, (LPVOID) pWorkItem, 0);
}

	
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CThreadPoolImp_UserMode::CThreadPoolImp_UserMode(	int nMaxThreads, int nMinSpareThreads, int nMaxSpareThread,
							   int nMonitorTimeout, int nQueueHandlerTimeout)								
{
	m_nMaxThreads				= nMaxThreads;
	m_nMaxSpareThreads			= nMaxSpareThread;
	m_nMinSpareThreads			= nMinSpareThreads;
	m_nThreadMonitorTimeout		= nMonitorTimeout;
	m_nWorkItemMonitorTimeout	= nQueueHandlerTimeout;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline bool CThreadPoolImp_UserMode::Start()
{
	m_monitor.Lock();

	m_bShutdown = false;

	m_nTotalThreadCount = 0;

	m_nActiveThreadCount = 0;

	AdjustLimits();


	if( NTL_SUCCESS != OpenThreads( m_nMinSpareThreads ) )
	{
		return false;
	}


	m_pThreadMonitor = new CWorkerThreadMonitor( this );
	if( NULL == m_pThreadMonitor )
	{
		return false;
	}


	m_pWorkItemQueueMonitor = new CWorkItemQueueMonitor( this );
	if( NULL == m_pWorkItemQueueMonitor )
	{
		return false;
	}


	if (NULL == m_pThreadMonitor || NULL == m_pWorkItemQueueMonitor)
	{
		SAFE_DELETE(m_pThreadMonitor);
		SAFE_DELETE(m_pWorkItemQueueMonitor);

		return false;
	}

	m_monitor.Unlock();

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CThreadPoolImp_UserMode::QueueWorkItem(CNtlWorkItem * pWorkItem)
{
	m_monitor.Lock();

	if( 0 == m_nTotalThreadCount || m_bShutdown )
	{
		SAFE_DELETE( pWorkItem );
		m_monitor.Unlock();
		return;
	}

	m_monitor.Unlock();


	m_workItemMutex.Lock();

	m_workItemList.Prepend( pWorkItem );

	m_workItemMutex.Unlock();


	m_pWorkItemQueueMonitor->NotifyWorkItemReady();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CThreadPoolImp_UserMode::RunWorkItem(CNtlWorkItem * pWorkItem)
{
	m_monitor.Lock();

	if( !m_nTotalThreadCount || m_bShutdown )
	{
		m_monitor.Unlock();
		SAFE_DELETE( pWorkItem );
		return;
	}

	if( m_nActiveThreadCount == m_nTotalThreadCount )
	{
		if( m_nTotalThreadCount	< m_nMaxThreads )
		{
			int nOpenThreads = m_nTotalThreadCount + m_nMinSpareThreads;

			OpenThreads( nOpenThreads );
		}
		else
		{
			while( m_nActiveThreadCount == m_nTotalThreadCount )
			{
				m_monitor.Wait();
			}

			if( 0 == m_nTotalThreadCount || m_bShutdown )
			{
				m_monitor.Unlock();
				return;
			}
		}
	}


	CPoolObject * pPoolObject = (CPoolObject*) m_ndleThreadList.Pop();
	CWorkerThread * pWorkerThread = pPoolObject->GetWorkerThread();

	++m_nActiveThreadCount;

	m_monitor.Unlock();

	pWorkerThread->Schedule( pWorkItem );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CThreadPoolImp_UserMode::Shutdown()
{
	m_monitor.Lock();
	if( !m_bShutdown )
	{
		m_bShutdown = true;

		m_pThreadMonitor->Shutdown();
		m_pWorkItemQueueMonitor->Shutdown();

		CPoolObject * pPoolObject = (CPoolObject *) m_ndleThreadList.Pop();
		while( pPoolObject )
		{
			CWorkerThread * pThread = pPoolObject->GetWorkerThread();
			pThread->Shutdown();

			SAFE_DELETE( pPoolObject );
			pPoolObject = (CPoolObject*) m_ndleThreadList.Pop();
		}
	}

	m_monitor.NotifyAll();
	m_monitor.Unlock();

	m_nTotalThreadCount = m_nActiveThreadCount = 0;
	m_pThreadMonitor = 0;
	m_pWorkItemQueueMonitor = 0;

	CNtlWorkItem * pWorkItem = (CNtlWorkItem*) m_workItemList.Pop();
	while( pWorkItem )
	{
		SAFE_DELETE( pWorkItem );
		pWorkItem = (CNtlWorkItem*) m_workItemList.Pop();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CThreadPoolImp_UserMode::HarvestSpareWorkers()
{
	CPoolObject * pPoolObject = 0;
	int nThreadFreeCount = 0;

	m_monitor.Lock();

	if( m_bShutdown )
	{
		m_monitor.Unlock();
		return;
	}

	nThreadFreeCount = m_nTotalThreadCount - m_nActiveThreadCount - m_nMaxSpareThreads;
	if( nThreadFreeCount > 0 )
	{
		for(int i = 0; i < nThreadFreeCount; i++)
		{
			pPoolObject = (CPoolObject*) m_ndleThreadList.Pop();
			CWorkerThread * pWorkerThread = pPoolObject->GetWorkerThread();

			pWorkerThread->Shutdown();

			--m_nTotalThreadCount;
		}
	}

	m_monitor.Unlock();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CThreadPoolImp_UserMode::ReturnWorkerThread(CWorkerThread * pWorkerThread)
{
	m_monitor.Lock();

	if( m_bShutdown || 0 == m_nTotalThreadCount )
	{
		pWorkerThread->Shutdown();
	}
	else
	{
		--m_nActiveThreadCount;

		m_ndleThreadList.Push( pWorkerThread->GetPoolObject() );

		m_monitor.Notify();
	}

	m_monitor.Unlock();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline void CThreadPoolImp_UserMode::AdjustLimits()
{
	if( m_nMaxThreads <= 0  )
	{
		m_nMaxThreads = CNtlThreadPool::MAX_THREADS;
	}

	if( m_nMaxSpareThreads >= m_nMaxThreads )
	{
		m_nMaxSpareThreads = m_nMaxThreads;
	}

	if( m_nMaxSpareThreads <= 0 )
	{
		m_nMaxSpareThreads = 1;
	}

	if( m_nMinSpareThreads > m_nMaxSpareThreads )
	{
		m_nMinSpareThreads = m_nMaxSpareThreads;
	}

	if( m_nMinSpareThreads <= 0 )
	{
		m_nMinSpareThreads = 1;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline int CThreadPoolImp_UserMode::OpenThreads(int nOpenThreadCount)
{
	if( nOpenThreadCount > m_nMaxThreads )
	{
		nOpenThreadCount = m_nMaxThreads;
	}


	for( int i = 0; i < nOpenThreadCount; i++ )
	{
		CWorkerThread * pWorkerThread = new CWorkerThread( this );
		if( NULL == pWorkerThread )
		{
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}


		CPoolObject * pPoolObject = new CPoolObject( pWorkerThread );
		if( NULL == pPoolObject )
		{
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}

		m_ndleThreadList.Push( pPoolObject );
	}

	m_nTotalThreadCount = nOpenThreadCount;

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
inline CNtlWorkItem * CThreadPoolImp_UserMode::GetWorkItem()
{
	CNtlWorkItem * pWorkItem = 0;

	m_workItemMutex.Lock();
	pWorkItem = (CNtlWorkItem *) m_workItemList.Pop();
	m_workItemMutex.Unlock();

	return pWorkItem;
}






/*#include <iostream>
#include <winsock2.h>
#include <map>
void CNtlThreadPool::UnitTest()
{
	typedef std::multimap<DWORD,int> THREADMAP;
	typedef THREADMAP::iterator THREADIT;
	typedef THREADMAP::value_type THREADVAL;

	struct sWORKINFO
	{
		sWORKINFO(int workItem, LONG & workCount, THREADMAP & map, CNtlMutex & mutex)
			:nWorkItem( workItem ), rWorkCount(workCount), rMap(map), rMutex(mutex) {}

		int nWorkItem;
		LONG & rWorkCount;
		THREADMAP & rMap;
		CNtlMutex & rMutex;
	};

	class CTestWorkItem : public CNtlWorkItem
	{
	public:
		CTestWorkItem(sWORKINFO * pWorkInfo)
			:m_pWorkInfo(pWorkInfo) {}

		void Run()
		{	
			DWORD dwThreadID = GetCurrentThreadId();

			m_pWorkInfo->rMutex.Lock();
			m_pWorkInfo->rMap.insert( THREADVAL( dwThreadID, m_pWorkInfo->nWorkItem ) );
			m_pWorkInfo->rMutex.Unlock();

			std::cout << "WorkItem : " << m_pWorkInfo->nWorkItem << " ThreadID : " <<  dwThreadID << std::endl;
			fflush( stdout );

			InterlockedIncrement(&m_pWorkInfo->rWorkCount);

			Sleep( 100 );

			SAFE_DELETE( m_pWorkInfo );
		}

		sWORKINFO * m_pWorkInfo;
	};


	LONG workCount = 0;
	THREADMAP threadMap;
	CNtlMutex mutex;

	CNtlThreadPool pool;	
	//if( !pool.Create( POOL_KERNEL_MODE, 7, 3, 3 ) )
	if( !pool.Create( POOL_USER_MODE, 1, 2, 3 ) )
	{
		std::cout << "pool create fail" << std::endl;
	}

	pool.Start();

	int nCount =0;
	while( workCount != 100 )
	{
		sWORKINFO * pWorkInfo = new sWORKINFO( nCount++, workCount, threadMap, mutex);

		pool.QueueWorkItem( new CTestWorkItem(pWorkInfo) );
		Sleep(10);
	}

	for( THREADIT it = threadMap.begin(); it != threadMap.end(); it++)
	{
		mutex.Lock();
		std::cout << "THREAD ID[" << it->first << "] WORK ID[" << it->second << "]" << std::endl;
		mutex.Unlock();
	}

	int nTotalThread = 0;
	for( THREADIT it = threadMap.begin(); it != threadMap.end(); )
	{
		mutex.Lock();
		std::cout << "THREAD [" << it->first << "]" << " WORK COUNT" << "[" << threadMap.count(it->first) << "]" << std::endl;
		mutex.Unlock();
		it = threadMap.upper_bound(it->first);
		++nTotalThread;
	}

	std::cout << "THREAD USE COUNT[" << nTotalThread << "]" << std::endl;

	pool.Shutdown();
}*/