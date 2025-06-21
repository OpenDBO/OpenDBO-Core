/*****************************************************************************
 *
 * File			: NtlSobPetAtt.h
 * Author		: Alejandro-HUB
 * Copyright	: (C)NTL
 * Date			: 2024. 11. 10
 * Abstract		: Simulation object player attribute base class
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/

#ifndef _NTL_SOB_MASCOT_ATTR_H_
#define _NTL_SOB_MASCOT_ATTR_H_

#include "NtlSharedDef.h"
#include "NtlSobAttr.h"

class CNtlSobMascotAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobMascotAttr, NTL_DEFAULT_MEMORY_POOL)

public:
	RwUInt8		m_bySummonSourceType;	/** skill or item */
	TBLIDX	m_uiSourceTblId;		/** skill or item table id */
	sNPC_TBLDAT* m_pNpcTbl;				/** mob table data pointer */

public:

	CNtlSobMascotAttr();
	virtual ~CNtlSobMascotAttr() {};

	virtual RwBool Create(void) { SetClassID(SLCLASS_PET); return TRUE; }
	virtual void Destroy(void) {}
	virtual void HandleEvents(RWS::CMsg& pMsg);

public:

	sNPC_TBLDAT* GetNpcTbl(void) const;
};

#endif