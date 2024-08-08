/******************************************************************************
* File			: DojoAccountGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 15
* Abstract		: 
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once

// shared
#include "NtlSharedDef.h"

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// cleint
#include "Windowby3.h"


struct sDBO_DOJO_BANK_HISTORY_INFO;


class CDojoAccountGui : public CNtlPLGui, public RWS::CEventHandler
{
protected:
#define dDISPLAY_ACCOUNT_NUM		(14)

	struct sACCOUNT_ITEM
	{
		RwUInt8				byIndex;				///< 서버에서 내려준 정보의 순서(시간순으로 정렬되어 있다)
		sDBO_TIME			tTime;
		std::wstring		wstrSummary;
		RwUInt32			uiZenny;
		RwBool				bSave;
	};

	struct sACCOUNT_ITEM_GUI
	{
		gui::CStaticBox*	pDay;
		gui::CStaticBox*	pSummary;
		gui::CStaticBox*	pZenny;
	};

	typedef std::list<sACCOUNT_ITEM*>				LIST_ACCOUNT;

public:
	CDojoAccountGui(const RwChar* pName);
	virtual ~CDojoAccountGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	static RwBool	CompareAscentDay(sACCOUNT_ITEM* pACCOUNT_ITEM, sACCOUNT_ITEM* pACCOUNT_ITEM2)
	{
		return pACCOUNT_ITEM->byIndex < pACCOUNT_ITEM2->byIndex;
	}
	static RwBool	CompareDecentDay(sACCOUNT_ITEM* pACCOUNT_ITEM, sACCOUNT_ITEM* pACCOUNT_ITEM2)
	{
		return pACCOUNT_ITEM->byIndex > pACCOUNT_ITEM2->byIndex;
	}
	static RwBool	CompareAscentSummary(sACCOUNT_ITEM* pACCOUNT_ITEM, sACCOUNT_ITEM* pACCOUNT_ITEM2)
	{
		return ( wcscmp(pACCOUNT_ITEM->wstrSummary.c_str(), pACCOUNT_ITEM2->wstrSummary.c_str()) < 0 );
	}
	static RwBool	CompareDecentSummary(sACCOUNT_ITEM* pACCOUNT_ITEM, sACCOUNT_ITEM* pACCOUNT_ITEM2)
	{
		return ( wcscmp(pACCOUNT_ITEM->wstrSummary.c_str(), pACCOUNT_ITEM2->wstrSummary.c_str()) > 0 );
	}
	static RwBool	CompareAscentZenny(sACCOUNT_ITEM* pACCOUNT_ITEM, sACCOUNT_ITEM* pACCOUNT_ITEM2)
	{
		return pACCOUNT_ITEM->uiZenny < pACCOUNT_ITEM2->uiZenny;
	}
	static RwBool	CompareDecentZenny(sACCOUNT_ITEM* pACCOUNT_ITEM, sACCOUNT_ITEM* pACCOUNT_ITEM2)
	{
		return pACCOUNT_ITEM->uiZenny > pACCOUNT_ITEM2->uiZenny;
	}

protected:
	CDojoAccountGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Clear();
	VOID			SetAccount(RwUInt32 uiBalance, RwUInt8 byCount, sDBO_DOJO_BANK_HISTORY_INFO* pDBO_DOJO_BANK_HISTORY_INFO);
	VOID			RefreshList(RwInt32 iOffSet);
	VOID			ClearList();

	VOID			OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex);
	VOID			OnClicked_HelpButton(gui::CComponent* pComponent);
	VOID			OnClicked_ExitButton(gui::CComponent* pComponent);
	VOID			OnClicked_DayButton(gui::CComponent* pComponent);
	VOID			OnClicked_SummaryButton(gui::CComponent* pComponent);
	VOID			OnClicked_AmountButton(gui::CComponent* pComponent);

	VOID			OnScrollChanged(RwInt32 iOffset);
	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;
	gui::CSlot			m_slotTabChanged;
	gui::CSlot			m_slotHelpButton;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotDayButton;
	gui::CSlot			m_slotSummaryButton;
	gui::CSlot			m_slotAmountButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CTabButton*	m_pTabButton;

	gui::CButton*		m_pHelpButton;
	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pDayButton;
	gui::CButton*		m_pSummaryButton;
	gui::CButton*		m_pAmountButton;

	gui::CScrollBar*	m_pScrollbar;

	gui::CStaticBox*	m_pDialogName;
	gui::CStaticBox*	m_pCreditStatic;			///< "잔고"
	gui::CStaticBox*	m_pCredit;					///< 잔고 액수

	CSurfaceGui			m_srfCreditBackground;

	CWindowby3			m_srf3BackLine;
	CWindowby3			m_srf3Background;

	sACCOUNT_ITEM_GUI	m_aAccountItemGui[dDISPLAY_ACCOUNT_NUM];

	SERIAL_HANDLE		m_hNPCSerial;
	RwInt32				m_iVisibleStart;
	RwBool				m_bAscentDay;
	RwBool				m_bAscentSummary;
	RwBool				m_bAscentZenny;

	RwUInt32			m_uiDojoTableIndex;

	LIST_ACCOUNT		m_listAccount;	
};