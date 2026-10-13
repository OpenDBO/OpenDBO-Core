#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptDBScramble.h"

// simulation
#include "NtlSLLogic.h"


CNtlWorldConceptDBScramble::CNtlWorldConceptDBScramble()
{
}

CNtlWorldConceptDBScramble::~CNtlWorldConceptDBScramble()
{
}

void CNtlWorldConceptDBScramble::Update(RwReal fElapsedTime)
{
	if (m_iWorldState == WORLD_STATE_ENTER)
		ChangeState(WORLD_STATE_IDLE);
}

void CNtlWorldConceptDBScramble::ChangeState(RwInt32 iState)
{
	CNtlWorldConceptController::ChangeState(iState);

	// leave at once so the concept can be added again right away (relog, server change)
	if (iState == WORLD_STATE_EXIT)
		CNtlWorldConceptController::ChangeState(WORLD_STATE_NONE);
}

RwBool CNtlWorldConceptDBScramble::IsAttackableEnemy(CNtlSob* pSobObj)
{
	if (m_iWorldState != WORLD_STATE_IDLE)
		return FALSE;

	return Logic_IsDBScrambleEnemy(pSobObj);
}

