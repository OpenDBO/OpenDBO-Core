//***********************************************************************************
//
//	File		:	NtlPacketEncoder_XOR.h
//
//	Begin		:	2007-01-29
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	YOSHIKI
//
//	Desc		:	
//
//***********************************************************************************


#pragma once

#include "NtlPacketEncoder.h"

class CNtlPacketEncoder_XOR : public CNtlPacketEncoder
{
public:

	CNtlPacketEncoder_XOR(void);

	~CNtlPacketEncoder_XOR(void);


public:

	int						RxDecrypt(CNtlPacket& rPacket);

	int						TxEncrypt(CNtlPacket& rPacket);
};
