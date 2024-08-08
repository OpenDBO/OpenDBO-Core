//***********************************************************************************
//
//	File		:	NtlSingleton.h
//
//	Begin		:	2006-01-05
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#ifndef __NTLSINGLETON_H__
#define __NTLSINGLETON_H__

#include "NtlDebug.h"


template <class TYPE> class CNtlSingleton
{
public:

	CNtlSingleton()
	{
		NTL_ASSERT( NULL == m_pInstance );

		m_pInstance = static_cast<TYPE*>(this);
	}

	virtual ~CNtlSingleton()
	{
		NTL_ASSERT( NULL != m_pInstance);
		m_pInstance = NULL;
	}


	static TYPE* GetInstance(void)
	{
		return m_pInstance;
	}


private:

	static TYPE * m_pInstance;

};

template <class TYPE> TYPE* CNtlSingleton<TYPE>::m_pInstance = NULL;



#endif // __NTLSINGLETON_H__
