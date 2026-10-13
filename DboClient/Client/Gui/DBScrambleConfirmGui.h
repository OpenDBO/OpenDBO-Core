#pragma once

// presentation
#include "NtlPLGui.h"

/**
 * \ingroup Client
 * \brief Dragonball scramble join dialog
 *
 * Explains how the battle dragonballs are won and lost. Accepting asks for a last confirmation
 * (DST_DRAGONBALL_SCRAMBLE_JOIN_CONFIRM_MSGBOX) before the join request is sent.
 */
class CDBScrambleConfirmGui : public CNtlPLGui
{
public:
	CDBScrambleConfirmGui(const RwChar* pName);
	virtual ~CDBScrambleConfirmGui();

	RwBool			Create();
	VOID			Destroy();

	RwInt32			SwitchDialog(bool bOpen);

protected:
	void			OnOkButtonClicked(gui::CComponent* pComponent);
	void			OnCloseButtonClicked(gui::CComponent* pComponent);

	void			SetText(const RwChar* pcComponent, const RwChar* pcStringKey);

protected:
	gui::CSlot		m_slotOk;
	gui::CSlot		m_slotCancel;
	gui::CSlot		m_slotClose;
};
