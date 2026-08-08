/******************************************************************************
* File			: NtlWorldConceptScramble.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 15
* Abstract		: 
*****************************************************************************
* Desc			: 도장 쟁탈전 월드 컨셉
*****************************************************************************/

#pragma once

// shared
#include "NtlDojo.h"

// simulation
#include "NtlWorldConceptController.h"

class CNtlWorldConceptScramble : public CNtlWorldConceptController
{
public:
	struct sSCRAMBLE_TEAM
	{
		GUILDID			guildID;
	};

public:
	CNtlWorldConceptScramble(void);
	~CNtlWorldConceptScramble(void);

	virtual RwBool		IsEnableAction(EAvatarAction eAction);
	virtual void		ChangeState(RwInt32 iState);

	CNtlSobActor*		GetNearAttackableEnemy( CNtlSobActor* pActor );

	RwBool				IsEnemy(const WCHAR* pwcAvatarGuildName, const WCHAR* pwcOtherGuildName);
	RwBool				IsAttackableEnemy(RwUInt32 uiHandle);

protected:	
};
