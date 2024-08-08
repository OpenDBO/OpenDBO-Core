// NtlBindDesc.cpp: implementation of the CNtlBindDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NtlBindDesc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNtlBindDesc::CNtlBindDesc()
{
	m_ptr = NULL;
}

CNtlBindDesc::CNtlBindDesc(SQLSMALLINT type, int size)
{
	m_ptr = NULL;
	m_type = type;
	m_size = size;
}

CNtlBindDesc::~CNtlBindDesc()
{
	Free();
}

void CNtlBindDesc::MAlloc()
{
	MAlloc(m_size);
}

void CNtlBindDesc::MAlloc(DWORD cb)
{
	Free();
	m_ptr = malloc(cb);
}

void CNtlBindDesc::Free()
{
	if(m_ptr)	
		free(m_ptr);
	m_ptr = NULL;
}
