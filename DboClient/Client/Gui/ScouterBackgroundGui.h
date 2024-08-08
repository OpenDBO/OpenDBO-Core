/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 11. 4
* Last Update	: 2007. 8. 28
* Abstract		: 
*****************************************************************************
* Desc			: 스카우터가 작동할 때 공통으로 보여지는 배경
*****************************************************************************/

#pragma once

/// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CScouterBackgroundGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CScouterBackgroundGui(const RwChar* pName);
	virtual ~CScouterBackgroundGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	CScouterBackgroundGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	VOID			LoadFlash(const char* pcText);

	VOID			LocateComponent();

	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnResize( RwInt32 iOldW, RwInt32 iOldH );
	VOID			OnMovieScheduleEnd(gui::CComponent* pComponent);
	VOID			OnFSCallback(const char* pcParam, const char* pcParam2);

protected:	
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotResize;
	gui::CSlot			m_slotMovieEnd;

	gui::CFlash*		m_pFlashBackground;

	RwUInt32			m_uiFlag;

	RwReal				m_fAlpha;					///< 화면 알파값
};