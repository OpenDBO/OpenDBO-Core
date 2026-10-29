#include "precomp_ntlsimulation.h"
#include "NtlWorldConceptDWC.h"

// simulation
#include "NtlSLGlobal.h"


CNtlWorldConceptDWC::CNtlWorldConceptDWC()
{
}

CNtlWorldConceptDWC::~CNtlWorldConceptDWC()
{
}

void CNtlWorldConceptDWC::ChangeState(RwInt32 iState)
{
	// the objectives of the dwc script are gone once the avatar leaves the dwc world,
	// not cleared on enter, the script may register them while the world still loads
	if (iState == WORLD_STATE_EXIT)
		GetNtlSLGlobal()->GetServerScriptEventStatus()->clear();

	CNtlWorldConceptController::ChangeState(iState);
}
