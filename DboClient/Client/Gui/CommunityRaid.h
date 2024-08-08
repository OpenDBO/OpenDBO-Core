/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 9. 13S
* Abstract		: 
*****************************************************************************
* Desc			: 커뮤니티 관련 다이얼로그 형틀
*				  각각의 페이지 내용은 PageType에 따라 세팅되고
*				  메세지와 이벤트도 활성화된 Page에 전달된다.
*****************************************************************************/
/*
#pragma once

#include "NtlPLGui.h"
#include "ItemSlot.h"
#include "ceventhandler.h"

#include "CommunityBase.h"
#include "gui_panel.h"

class CCommunityGui : public CCommunityBase
{
public:

	enum eCommunityPageType
	{
		PAGETYPE_PARTY,
		PAGETYPE_GUILD,
		PAGETYPE_RAID
	};

	CCommunityGui();
	CCommunityGui(const RwChar* pName);
	virtual ~CCommunityGui();

	VOID		Init();
	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	VOID			OnPaint();

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);

protected:	
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	RwInt32				m_iMouseDownSlot;			///< 마우스로 눌린 슬롯의 인덱스

	RwInt32				m_iPageType;				///< 현재의 페이지 타입

	CItemSlot			m_slot;						///< 슬롯

	gui::CPanel*		m_pSlotFocusEffect;			///< 슬롯 포커스 이펙트
};*/