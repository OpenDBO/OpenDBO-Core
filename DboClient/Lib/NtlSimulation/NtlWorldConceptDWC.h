#pragma once

// simulation
#include "NtlWorldConceptController.h"

// Dragon World Challenge, active while the avatar is in a dwc world
class CNtlWorldConceptDWC : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptDWC();
	~CNtlWorldConceptDWC();

	virtual void		ChangeState(RwInt32 iState);
};
