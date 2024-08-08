/*****************************************************************************
 * File			: ContractGui.h
 * Author		: Hong Sungbock
 * Copyright	: (주)NTL
 * Date			: 2009. 2. 26
 * Abstract		: DBO contract gui.
 *****************************************************************************
 * Desc         : 약관 GUI를 볼 수 있는 경우에 대한 정의
 *
 *				  1. 자동으로 보게 되는 경우
 *					 - 로컬 PC에서 계정별로 처음 접속한 경우
 *					 - 약관 내용의 갱신
 *					 - DBO의 확장팩이 나오면 로컬 PC에서 계정별로 처음 접속한 경우
 *
 *				  2. DBO Title 화면에서 "약관" 버튼을 눌렀을 경우
 *
 *				  3. 자동으로 약관을 보게 되는 경우 "동의"하지 않으면 게임 종료
 *****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// cleint
#include "SurfaceGui.h"

class CContractGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	CContractGui(const RwChar *pName);
	~CContractGui();

	RwBool		Create();
	VOID		Destroy();

protected:
	CContractGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

	VOID			OnClicked_AgreementButton(gui::CComponent* pComponent);
	VOID			OnClicked_OKButton(gui::CComponent* pComponent);
	VOID			OnClicked_CancelButton(gui::CComponent* pComponent);
		
	VOID			OnScrollChanged(RwInt32 iOffset);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotAgreementButton;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;	
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotPaint;

	gui::CHtmlBox*		m_pContractHtml;

	gui::CStaticBox*	m_pPleaseRead;

	gui::CButton*		m_pAgreementButton;
	gui::CButton*		m_pOKButton;
	gui::CButton*		m_pCancelButton;

	CSurfaceGui			m_srfBackground;					///< 검은 배경
};