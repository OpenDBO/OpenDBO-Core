/*****************************************************************************
* File			: DeliberationRankGui.h
* Author		: Hong Sungbock
* Copyright		: (주)NTL
* Date			: 2009. 8. 18
* Abstract		: DBO contract gui.
*****************************************************************************
* Desc          : 심의 등급을 표시하기 위한 GUI 클래스
*
*				  1. 타이틀 화면에 표시
*				  2. 게임중 일정 시간 간격으로 잠시 동안 표시
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"


class CDeliberationRankGui : public CNtlPLGui
{
public:
	CDeliberationRankGui(const RwChar* pName);
	virtual ~CDeliberationRankGui();

	RwBool			Create(RwBool bGameStage);
	VOID			Destroy();

	VOID			HandleEvents( RWS::CMsg &msg );

protected:
	CDeliberationRankGui() {}

	VOID			LocateComponent(RwInt32 iWidth, RwInt32 iHeight);

protected:
	gui::CSlot			m_slotMove;
	RwBool				m_bGameStage;

};


class CDeliberationRankContainer : public RWS::CEventHandler
{
	struct sALARM_INFO
	{
		RwBool		bGameStage;
		RwReal		fElapsed;
		RwReal		fShowTime;
	};

public:
	CDeliberationRankContainer();
	virtual ~CDeliberationRankContainer();

	RwBool			Create();
	VOID			Update(RwReal fElapsed);
	VOID			Destroy();

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			CreateGui();
	VOID			DestroyGui();

protected:
	CDeliberationRankGui*	m_pDeliberationRank;
	
	sALARM_INFO			m_tALARM_INFO;
};