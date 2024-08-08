//***********************************************************************************
//
//	File		:	NtlNetworkProcessor.h
//
//	Begin		:	2007-01-16
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "NtlThread.h"


//---------------------------------------------------------------------------------------
// 네트워크에서 일어나는 Accept/Connect/Receive/Close Event에 대한 순차적 처리
//---------------------------------------------------------------------------------------
class CNtlNetwork;
class CNtlSession;

class CNtlNetworkProcessor : public CNtlRunObject
{
public:

	CNtlNetworkProcessor(CNtlNetwork *  pNetwork);

	~CNtlNetworkProcessor();


public:

	int							Create();

	void						Destroy();

public:

	int							PostNetEvent(WPARAM wParam, LPARAM lParam);

	void						Run();

	void						Close();

private:

	HANDLE						m_hEventIOCP;
};
//-----------------------------------------------------------------------------------
