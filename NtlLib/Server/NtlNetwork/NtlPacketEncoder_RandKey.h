//***********************************************************************************
//
//	File		:	NtlPacketEncoder_RandKey.h
//
//	Begin		:	2007-01-31
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlPacketEncoder.h"

#include "NtlRandomGenerator.h"


class CNtlPacketEncoder_RandKey : public CNtlPacketEncoder
{
public:

	CNtlPacketEncoder_RandKey(bool bSeedSwap = false);

	~CNtlPacketEncoder_RandKey(void);


public:

	int						RxDecrypt(void * pvPacketHeader);

	int						TxEncrypt(void * pvPacketHeader);

	int						RxDecrypt(CNtlPacket& rPacket);

	int						TxEncrypt(CNtlPacket& rPacket);


private:

	CNtlRandomGenerator		m_rxBodyKeyGenerator;

	CNtlRandomGenerator		m_txBodyKeyGenerator;

	CNtlRandomGenerator		m_rxHeaderKeyGenerator;

	CNtlRandomGenerator		m_txHeaderKeyGenerator;
};
