/*****************************************************************************
 *
 * File			: LoginGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (��)NTL
 * Date			: 2005. 9. 20	
 * Update		: 2007. 10. 15
 * Abstract		: DBO Login gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __LOGIN_GUI_H__
#define __LOGIN_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "eventtimer.h"
#include "SurfaceGui.h"

class CLogInGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	CLogInGui(const RwChar *pName);
	~CLogInGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

// event handler
public:

	VOID			LogInStageEnterEventHandler(RWS::CMsg &msg);
	VOID			LogInStageExitEventHandler(RWS::CMsg &msg);
	VOID			LogInStageTimeOutEventHandler(RWS::CMsg &msg);
	VOID			ResizeEventHandler(RWS::CMsg &msg);

	int				KeyboardDownHandler(uintptr_t uiKeyData);
	
protected:
	CLogInGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnTimer(void);
	VOID			SwitchEnableButtons(bool bEnable);	///< Exit ��ư�� ������ ��ư�� �̿밡�� ����

	RwInt32			TabButtonHandle();

	VOID			LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

	VOID			ClickedCinemaButton(gui::CComponent* pComponent);
	VOID			ClickedCreditButton(gui::CComponent* pComponent);
	VOID			ClickedGamePolicyButton(gui::CComponent* pComponent);
	VOID			ClickedNewAccountButton(gui::CComponent* pComponent);
	VOID			ClickedLoginButton(gui::CComponent* pComponent);
	VOID			ClickedExitButton(gui::CComponent* pComponent);

	VOID			PressEnder_in_AccountBox();
	VOID			PressEnder_in_PasswarsBox();

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint();
	VOID			OnCreditPaintEnd(gui::CComponent* pComponent);

	int				CallBackMoviePlayStop(void);	// Function to be called when Movie Play is over (by HoDong)
	
protected:
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_creditPaint;
	gui::CSlot			m_slotCinemaButton;
	gui::CSlot			m_slotCreditButton;
	gui::CSlot			m_slotGamePolicyButton;
	gui::CSlot			m_slotNewAccountButton;
	gui::CSlot			m_slotLoginButton;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotEnterAccount;
	gui::CSlot			m_slotEnterPassward;
///
	gui::CSlot			m_slotSigChangedTest;
	gui::CSlot			m_slotSigActivityTest;

	gui::CButton*		m_pCinemaButton;			///< ������ ��� ��ư
	gui::CButton*		m_pCreditButton;			///< ������ ���� ��ư
	gui::CButton*		m_pGamePolicyButton;		///< 
	gui::CButton*		m_pNewAccountButton;		///< 
	gui::CButton*		m_pLoginButton;				///< �α��� ��ư
	gui::CButton*		m_pExitButton;				///< ������ ��ư

	gui::CStaticBox*	m_pAccountName;				///< '���� �̸�' ����ƽ
	gui::CStaticBox*	m_pPassward;				///< '��й�ȣ' ��й�ȣ ����ƽ
	gui::CStaticBox*	m_pNotive;					///< 'Dbo ��������' ����ƽ

	gui::CStaticBox*	m_pVersion;					///< Dbo ���� ǥ��

	gui::CInputBox*		m_pAccountInput;			///< Account Input
	gui::CInputBox*		m_pPasswardInput;			///< Passward Input

	gui::CFrame*		m_pFrame;

	gui::CFlash*		m_pFlashBackground;

	gui::CFlash*		m_pFlashCredit;

	CSurfaceGui			m_srfInputDialogBack;		///< ��ǲ�ڽ� ���
	CSurfaceGui			m_NoticeUnderLine;			///< �������� ����


	void*				m_handleKeyDown;
};

#endif