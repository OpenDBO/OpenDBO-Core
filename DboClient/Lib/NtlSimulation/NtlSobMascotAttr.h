/*****************************************************************************
 *
 * File			: NtlSobMascotAttr.h
 * Author		: Alejandro-HUB
 * Copyright	: (C)NTL
 * Date			: 2024. 11. 10
 * Abstract		: Simulation object mascot attribute
 *****************************************************************************
 * Desc         : Model and name of a summoned mascot, read from the mascot
 *				  status table.
 *****************************************************************************/

#ifndef _NTL_SOB_MASCOT_ATTR_H_
#define _NTL_SOB_MASCOT_ATTR_H_

#include "NtlSharedDef.h"
#include "NtlSobAttr.h"
#include "NtlMascot.h"

struct sMASCOT_STATUS_TBLDAT;

class CNtlSobMascotAttr : public CNtlSobLifeAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobMascotAttr, NTL_DEFAULT_MEMORY_POOL)

private:
	TBLIDX					m_mascotTblidx;
	sMASCOT_STATUS_TBLDAT*	m_pMascotTbl;

public:

	CNtlSobMascotAttr();
	virtual ~CNtlSobMascotAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_MASCOT); return TRUE; }
	virtual void Destroy(void) {}
	virtual void HandleEvents(RWS::CMsg& pMsg) {}

public:

	RwBool					SetMascotTblidx(TBLIDX mascotTblidx);
	TBLIDX					GetMascotTblidx(void) const { return m_mascotTblidx; }
	sMASCOT_STATUS_TBLDAT*	GetMascotTbl(void) const { return m_pMascotTbl; }
};

#endif
