/*****************************************************************************
 *
 * File			: NpcHelpGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 12. 12	
 * Abstract		: DBO Npc help gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NPCHELP_GUI_H__
#define __NPCHELP_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

#include "NtlSLDef.h"

class CNpcHelpGui : public CNtlPLGui, public RWS::CEventHandler
{
private:

	gui::CSlot m_slotDialogPaint;

	gui::CStaticBox *m_psttDisplay;

	SERIAL_HANDLE m_hTargetSerail;


private:

	void SetNpcWords(RwUInt32 uiTargetSerail);

public:

	CNpcHelpGui();
	CNpcHelpGui(const RwChar *pName);
	~CNpcHelpGui();

	RwBool Create(void);
	void Destroy(void);

	RwInt32 SwitchDialog(bool bOpen);

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void OnDialogPaint(void);
};

#endif