////////////////////////////////////////////////////////////////////////////////
// File: SmithyGui.h
// Desc: Smithy of the item upgrade npc, port of the TW client CSmithyGui.
//		 TW builds an upgrade tab and a disassemble tab, each turned on by the
//		 server contents flags; retail only had the disassemble tab on, the
//		 upgrade is the separate CItemUpgradeGui.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SMITHY_GUI_H__
#define __SMITHY_GUI_H__

#include "NtlItem.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "Windowby3.h"
#include "SlotGui.h"

class CItemDisassembleTabGui : public CNtlPLGui
{
public:
	CItemDisassembleTabGui(const RwChar* pName);
	~CItemDisassembleTabGui(VOID);

	RwBool	Create(gui::CComponent* pParent);
	VOID	Destroy(VOID);

	VOID	Update(RwReal fElapsed);

	VOID	Activate(RwBool bActive);
	VOID	SetResult(const RwUInt32 (&ahResultItem)[ITEM_DISASSEMBLE_MAX_RESULT]);
	VOID	RemoveResultItem(RwUInt32 hItem);
	VOID	OnParentMove(VOID);

private:
	VOID	Reset(VOID);

	VOID	OnClickDisassemble(gui::CComponent* pComponent);
	VOID	OnClickCancel(gui::CComponent* pComponent);
	VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID	OnMouseLeave(gui::CComponent* pComponent);
	VOID	OnPaintBack(VOID);
	VOID	OnPaint(VOID);
	VOID	OnEffectEnd(gui::CComponent* pComponent);

	gui::CSlot			m_slotClickDisassemble;
	gui::CSlot			m_slotClickCancel;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaintBack;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotEffectEnd;

	gui::CStaticBox*	m_pstbInfo;
	gui::CStaticBox*	m_pstbName;
	gui::CStaticBox*	m_pstbResultName;
	gui::CStaticBox*	m_apstbResultTitle[ITEM_DISASSEMBLE_MAX_RESULT];
	gui::CStaticBox*	m_apstbResultInfo[ITEM_DISASSEMBLE_MAX_RESULT];
	gui::CButton*		m_pbtnDisassemble;
	gui::CButton*		m_pbtnCancel;
	gui::CFlash*		m_pflsDisassemble;

	CWindowby3			m_TopBack;
	CWindowby3			m_BottomBack;

	CRegularSlotGui		m_ResultSlot[ITEM_DISASSEMBLE_MAX_RESULT];
	RwInt32				m_iFocusSlot;

	RwBool				m_bUpgradePages;	///< holds a ref on the item upgrade surface/resource pages
};

class CSmithyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CSmithyGui(const RwChar* pName);
	~CSmithyGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update(RwReal fElapsed);

	RwInt32	SwitchDialog(bool bOpen);
	VOID	HandleEvents(RWS::CMsg& msg);

	VOID	PositionAlign(RwInt32 nScreenWidth, RwInt32 nScreenHeight);

	RwUInt32	GetNPCSerial(VOID) { return m_hNPCSerial; }

private:
	VOID	OnClickExit(gui::CComponent* pComponent);
	VOID	OnPaint(VOID);
	VOID	OnMove(RwInt32 nOldX, RwInt32 nOldY);

	gui::CSlot			m_slotClickExit;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	gui::CButton*		m_pbtnExit;
	gui::CStaticBox*	m_pstbTitle;
	gui::CTabButton*	m_ptabSmithy;

	CWindowby3			m_BackLine;

	CItemDisassembleTabGui	m_DisassembleTab;

	RwUInt32			m_hNPCSerial;
};

#endif//__SMITHY_GUI_H__
