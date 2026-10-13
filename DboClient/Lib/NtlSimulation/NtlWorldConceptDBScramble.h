/*****************************************************************************
* File			: NtlWorldConceptDBScramble.h
* Abstract		: Dragonball scramble world concept
*****************************************************************************
* Desc			: Active while the avatar takes part in the dragonball scramble.
*				  Other participants outside of towns become enemies, except party members.
*****************************************************************************/

#pragma once

#include "NtlWorldConceptController.h"

class CNtlSob;

class CNtlWorldConceptDBScramble : public CNtlWorldConceptController
{
public:

	CNtlWorldConceptDBScramble();
	~CNtlWorldConceptDBScramble();

	virtual void	Update(RwReal fElapsedTime);
	virtual void	ChangeState(RwInt32 iState);

public:

	RwBool			IsAttackableEnemy(CNtlSob* pSobObj);
};
