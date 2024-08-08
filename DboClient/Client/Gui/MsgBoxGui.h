/*****************************************************************************
 *
 * File			: MsgBoxGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 28	
 * Abstract		: DBO MessageBox gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __MSGBOX_GUI_H__
#define __MSGBOX_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "DboDef.h"
#include "SurfaceGui.h"
#include "Windowby3.h"

struct SDboEventMsgBoxShow;

struct sMsgBoxCustomBtnData : public sMsgBoxCustomBtn
{
	gui::CComponent*	m_pComponent;
	gui::CSlot			m_slotClickCustomButton;
};

class CMsgBoxGui : public CNtlPLGui
{
public:
	CMsgBoxGui(VOID);
	CMsgBoxGui( const RwChar* pName );
	~CMsgBoxGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	VOID	HandleEvent_ItemDelete( RWS::CMsg& msg );

	VOID	Show( bool bShow );
	VOID	Popup( bool bPopup );
	VOID	CenterAlign( RwUInt32 uiWidth, RwUInt32 uiHeight );
	
	VOID	SetBoxData( EMsgBoxTypeFlag flagType, RwBool bPopup, std::list<sMsgBoxCustomBtn>* plistCustomBtn, const WCHAR* szText,
						RwInt32 eStringID, EMsgBoxWork eWorkID, sMsgBoxData* pData, RwReal fShowTime, RwBool bNetConnect = FALSE );
	VOID	UpdateMsgBox( const WCHAR* szText, sMsgBoxData* pData );

	sMsgBoxData*	GetMsgBoxData(VOID) { return &m_Data; }
	EMsgBoxTypeFlag GetMsgBoxType(VOID) { return m_flagType; }

	RwBool	IsMsgBoxWorkID( EMsgBoxWork workID );
	RwBool	IsMsgBoxStringID( RwInt32 eStringID );
	RwBool	IsNetConnectBox(VOID) { return m_bNetConnect; }

	VOID	SendOK(VOID);
	VOID	SendCancel(VOID);
	VOID	DoNotHide(VOID);

	VOID	ProccessDefault();
	VOID	PositionAlign( RwInt32 nWidth, RwInt32 nHeight );	
		
private:
	VOID	Init(VOID);

	VOID	SetText(VOID);

	VOID	MsgBoxSizeProc( RwBool bCenterAlign );
	VOID	ButtonPositionProc(VOID);	
	VOID	SetButtonPosition( gui::CComponent* pComponent, RwInt32 nIndex, RwInt32 nTotalButtonCount );
	VOID	CalcPosRate(VOID);

	VOID	AddCustomBtn( sMsgBoxCustomBtn* pCustomBtnData );
	VOID	DeleteCustomBtnAll(VOID);

	VOID	OnReturn();
	VOID	OnGotFocus();
	VOID	OnLostFocus();
	VOID	OnClickedOk( gui::CComponent* pComponent );
	VOID	OnClickedCancel( gui::CComponent* pComponent );
	VOID	OnClickedCustomBtn( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnSetAlpha( RwUInt8 byAlpha );

	VOID	OnMove( RwInt32 iOldX, RwInt32 iOldY );

	gui::CSlot			m_slotReturn;
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;
	gui::CSlot			m_slotOk;
	gui::CSlot			m_slotCancel;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotSetAlpha;
	gui::CSlot			m_slotMove;

	gui::CButton*		m_pbtnOk;
	gui::CButton*		m_pbtnCancel;
	gui::CHtmlBox*		m_phtmDisplay;
	gui::CStaticBox*	m_pstbTitle;
	gui::CInputBox*		m_pInput;

	CRectangle			m_rtOk;
	CRectangle			m_rtCancel;
	CWindowby3			m_srfBack;
	CSurfaceGui			m_srfBlackPanel;
		
	EMsgBoxTypeFlag		m_flagType;
	EMsgBoxWork			m_eWorkId;
	RwInt32				m_eStringID;
	
	RwReal				m_fShowTime;
	RwReal				m_fCurrentTime;	
	RwBool				m_bPopup;
	RwBool				m_bNetConnect;

	RwBool				m_bHide;

	std::wstring		m_wstrDisplayText;

	sMsgBoxData			m_Data;
	std::list<sMsgBoxCustomBtnData> m_listCustomBtn;

	RwReal				m_fPosXRate;
	RwReal				m_fPosYRate;
};

#endif