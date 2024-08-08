// NtlBindDesc.h: interface for the CNtlBindDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTLBINDDESC_H__AF6491AE_835A_4748_B26A_1527798F8ADE__INCLUDED_)
#define AFX_NTLBINDDESC_H__AF6491AE_835A_4748_B26A_1527798F8ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NtlBase.h"
#include <SQL.h>

class CNtlBindDesc 
{
public:
	SQLSMALLINT m_type;
	int			m_size;

	LPVOID		m_ptr;

public:
	CNtlBindDesc();
	CNtlBindDesc(SQLSMALLINT type, int size);
	virtual ~CNtlBindDesc();

	void Free();
	void MAlloc();
	void MAlloc(DWORD cb);
};

#endif // !defined(AFX_NTLBINDDESC_H__AF6491AE_835A_4748_B26A_1527798F8ADE__INCLUDED_)
