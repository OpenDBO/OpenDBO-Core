#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "eventtimer.h"
#include "SurfaceGui.h"

class CVirtualKeyboardGui : public CNtlPLGui
{
public:
	CVirtualKeyboardGui(const RwChar* pName);
	virtual ~CVirtualKeyboardGui();


	RwBool		Create();
	VOID		Destroy();

protected:


	VOID		OnClickCloseBtn(gui::CComponent* pComponent);
	VOID		OnClickCapsLockBtn(gui::CComponent* pComponent);

	VOID		OnClickBackSpace(gui::CComponent* pComponent);
	VOID		OnClickAllDelete(gui::CComponent* pComponent);

	VOID		OnClickKey(gui::CComponent* pComponent);

	std::string		GetVirtualKeyboardString(int index,RwBool isCapsLock);

protected:

	gui::CSlot	m_slotCloseButton;
	gui::CSlot	m_slotCapLockBtn;
	gui::CSlot	m_slotBackSpaceBtn;
	gui::CSlot	m_slotAllDeleteBtn;
	gui::CSlot	m_slotClickKeyBtn[50];

	gui::CButton* m_pCapsLockButton;
	gui::CButton* m_pBackSpaceButton;
	gui::CButton* m_pAllDeleteButton;
	gui::CButton* m_pCloseButton;

	gui::CButton* m_pBtnKey[50];


	RwBool	m_bIsCapLock;

};
