#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "surfaceGui.h"
#include "Windowby3.h"
#include "NtlItem.h"
#include "NtlSobItem.h"
/**
* \ingroup Client
* \brief 상용화관련  YardratStore  HLS Shopdㅔ서 구입한 캐쉬템은 기본적으로 Yardrat 창고를 거치게 되어있다.
*
*
*
* 
* \date 2009-08-24
* \author woosung_test
*/

class sITEM_TBLDAT;

class CYardratStoreGui : public CNtlPLGui, public RWS::CEventHandler
{
protected:


public:

	CYardratStoreGui( const RwChar* pName );
	~CYardratStoreGui(VOID);

	//! Operation
	VOID						Init(VOID);
	virtual RwBool				Create(VOID);
	virtual VOID				Destroy(VOID);

	VOID						UpdateSurfacePos();

	RwInt32						SwitchDialog(bool bOpen);
/*
	VOID						OpenDialog(GUI_EXTEND_MODE eMode, sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiItemIdx);
	VOID						OpenDialog();

	VOID						StartBuyDurItemProcessNPC( RwUInt32 uiItemIdx, RwUInt32 uiNPCSerial, sSHOP_BUY_CART& ShopBuyCart ); /// NPC, 
	VOID						StartBuyDurItemProcessNetpy( RwUInt32 uiItemIdx, RwUInt32 uiMerchantTblidx, BYTE byMerchanSlotPos ); /// Netpy Shop
	sITEM_TBLDAT*				StartBuyDurItemProcessCommonForItem( RwUInt32 uiItemIdx );
	VOID						StartMoveDurItemProcessCash( RwUInt32 uiItemIdx, RwUInt32 uiProductId );						/// Yardrat
	// Net
	VOID						NetSendBuyDurItemNPC();				/// NPC Shop
	VOID						NetSendBuyDurItemNetpy();			/// Netpy Shop
	VOID						NetSendMoveDurItemFromYardrat();	/// Yardrat
*/
	//! Event
	virtual VOID				HandleEvents( RWS::CMsg& msg );
/*
	//! Access
	VOID						SetDurationItem(RwUInt32 uiItemIdx);
	VOID						SetExtendTargetItem(CNtlSobItem* pNtlSobItem);
	VOID						SetCurrItemTbl(RwUInt32 uiItemIdx);
	VOID						SetCurrItemTbl(sITEM_TBLDAT* pITEM_TBLDAT);	/// 모든 item 관련 정보 setting
	/// setting전에 GUI_EXTEND_MODE가 결정되어 있어야 함
	protected:
	//! Implementation
	VOID						ChangeExtendMode(GUI_EXTEND_MODE eMode);
	RwBool						SetIcon(const RwChar* pcIconName);

	VOID						OnMouseEnter_pnlItemButt(gui::CComponent* pComponent);			
	VOID						OnMouseLeave_pnlItemButt(gui::CComponent* pComponent);
	VOID						OnClicked_btnConfirm(gui::CComponent* pComponent);
	VOID						OnClicked_btnCancel(gui::CComponent* pComponent);
	VOID						OnClicked_btnBuyDuration(gui::CComponent* pComponent);
	VOID						OnClicked_btnRefillCash(gui::CComponent* pComponent);
	VOID						OnClicked_btnClose(gui::CComponent* pComponent);
	*/
	VOID						OnPaint(VOID);
	VOID						OnMove(int nX,int nY);

	// Gui Components
	gui::CStaticBox*			m_pStatic_stbTitle;					// title변경 가능
	gui::CButton*				m_pButt_btnClose;
	gui::CDialog*				m_pDlg_dlgContentsScrolled;
/*
	gui::CPanel*				m_pPanel_pnlItemButt;
	//gui::CPanel*				m_pPanel_pnlPanel1;
	gui::CStaticBox*			m_pStatic_stbRemainTimeTitle;
	gui::CStaticBox*			m_pStatic_stbAddableTimeTitle;
	gui::CStaticBox*			m_pStatic_stbConsumeGoldTitle;
	gui::CStaticBox*			m_pStatic_stbMyGoldTitle;
	gui::CStaticBox*			m_pStatic_stbRemainTime;
	gui::CStaticBox*			m_pStatic_stbAddableTime;
	gui::CStaticBox*			m_pStatic_stbConsumeGold;
	gui::CPanel*				m_pPanel_pnlPanelConsumeGoldIcon;	
	gui::CStaticBox*			m_pStatic_stbMyGold;
	gui::CPanel*				m_pPanel_pnlPanelMyGoldIcon;		

	gui::CPanel*				m_pPanel_pnlPanel2;					// 숨겨질수 있음
	gui::COutputBox*			m_pInput_opbDescriptDetail;

	gui::CPanel*				m_pPanel_pnlPanelName;
	gui::CStaticBox*			m_pStatic_stbItemName;
	gui::CStaticBox*			m_pStatic_stbDescript1;				// 변화 가
	gui::CStaticBox*			m_pStatic_stbDescript2;				// 변화 가
	gui::CButton*				m_pButt_btnConfirm;					// 숨겨질수 있음
	gui::CButton*				m_pButt_btnCancel;					// 위치변경
	gui::CButton*				m_pButt_btnBuyDuration;				// 숨겨짐
	gui::CButton*				m_pButt_btnRefillCash;				// 숨겨짐

	// Variable
	CSurfaceGui					m_surItemIconSlot;
	CSurfaceGui					m_surEnterSlot;
	CSurfaceGui					m_surConsumeGoldIcon;				// 텍스처 바뀔 수 있음
	CSurfaceGui					m_surMyGoldIcon;					// 텍스처 바뀔 수 있음
*/
	CWindowby3					m_DialogSurface;					///< 크기 변경 가능한 dialog이미지
/*
	// Gui Signals
	gui::CSlot					m_slotGotFocus_pnlItemButt;
	gui::CSlot					m_slotLostFocus_pnlItemButt;
	gui::CSlot					m_slotClicked_btnConfirm;
	gui::CSlot					m_slotClicked_btnCancel;
	gui::CSlot					m_slotClicked_btnBuyDuration;
	gui::CSlot					m_slotClicked_btnRefillCash;
	gui::CSlot					m_slotClicked_btnClose;
*/
	gui::CSlot					m_slotPaint_dlgMain;
	gui::CSlot					m_slotMove_dlgMain;
	
};