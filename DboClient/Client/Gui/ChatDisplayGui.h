/******************************************************************************
* File			: ChatDisplayGui.h
* Author		: Hong SungBock
* Copyright		: (占쏙옙)NTL
* Date			: 2008. 7. 1
* Abstract		: 
*****************************************************************************
* Desc			: ä�� �αװ� ȭ�鿡 �������� GUI
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"
#include "InputHandler.h"

// dbo
#include "ChatDefine.h"

class CChatGui;

class CChatDisplayGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eResizeType
	{
		RESIZE_TYPE_TOP,
		RESIZE_TYPE_RIGHT_TOP,
		RESIZE_TYPE_RIGHT,
		RESIZE_TYPE_RIGHT_BOTTOM,
		RESIZE_TYPE_BOTTOM,

		INVALID_RESIZE_TYPE
	};

	struct sResizingInfo
	{
		eResizeType			eType;
		RwInt32				iConfirmXPos;		///< 변경 완료된 X 좌표
		RwInt32				iConfirmYPos;		///< 변경 완료된 Y 좌표
		RwInt32				iConfirmWidth;		///< 변경 완료된 넓이
		RwInt32				iConfirmHeight;		///< 변경 완료된 높이
	};

	struct sAlphaInfo
	{
		RwReal				fElapsed;
		RwUInt8				byAlpha;
		RwUInt8				byAlphaWeight;

		RwBool				binCleintRect;
	};

	struct sTrackingInfo
	{
		RwBool				bTracking;
		RwInt32				iCapturedX;
		RwInt32				iCapturedY;
	};

	struct sPositionInfo
	{
		RwInt32				iOldScreenHeight;		///< 클占쏙옙占싱억옙트 Resize占쏙옙 占싹단울옙占쏙옙 占쏙옙占쏙옙占쏙옙 占신몌옙 占쏙옙占쏙옙 占쏙옙占쏙옙
	};


	CChatDisplayGui(const RwChar* pName);
	virtual ~CChatDisplayGui();

	RwBool		Create(CChatGui* pChatGui);
	VOID		Destroy();

	VOID		Update( RwReal fElapsed );
	
	virtual void	SetMovable(RwBool bMovable) {} // To avoid being affected by the Movable property of DialogManager

	VOID		Show(bool bShow);
	VOID		AddText(RwUInt8 byChatType, const WCHAR* pwcText);
	VOID		AddText(RwUInt8 byChatType, const WCHAR* pwcText, unsigned int uiColor);

	VOID		SetIndex(RwUInt8 byIndex) { m_byDisplayGuiIndex = byIndex; }
	RwUInt8		GetIndex() { return m_byDisplayGuiIndex; }

	VOID		Refresh();
	VOID		SetAlphaWeight(RwUInt8 byAlpha);
	VOID		EnableLogType(eChatType eType, RwBool bEnable);

	VOID		Lock(bool bLock);

	VOID		PageUp();
	VOID		PageDown();

protected:
	CChatDisplayGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Component_Locate();
	VOID			SetAlpha(RwUInt8 byAlpha);

	eResizeType		CheckResizePosition(RwInt32 iX, RwInt32 iY);
	VOID			CursorCheck();
	VOID			ResizeDisplay(RwInt32 iCurMouseX, RwInt32 iCurMouseY);

	VOID			Event_MouseEnter();
	VOID			Event_MouseLeave();

	VOID			OnPaint();

	VOID			OnClicked_LastButton(gui::CComponent* pComponent);

	RwInt32			MouseMoveHandler(uintptr_t uiMouseData);
	RwInt32			MouseUpHandler(uintptr_t uiMouseData);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnResize(RwInt32 iOldW, RwInt32 iOldH);
	VOID			OnMouseEnter(gui::CComponent* pComponent);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);

protected:	
	gui::CSlot			m_slotLastButton;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotResize;
	gui::CSlot			m_slotMoveEnter;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotOutPutMouseDown;
	gui::CSlot			m_slotOutPutMouseUp;
	gui::CSlot			m_slotOutPutMoveEnter;
	gui::CSlot			m_slotOutPutMouseMove;
	gui::CSlot			m_slotOutPutMouseLeave;
	gui::CSlot			m_slotPaint;

	CChatGui*			m_pChatGui;

	gui::COutputBox*	m_pOutDisplay;
	gui::CButton*		m_pLastButton;	

	RwBool				m_bLock;
	INPUT_HANDLE		m_hMouseMove;
	INPUT_HANDLE		m_hMouseUp;

	sResizingInfo		m_ResizingInfo;
	sAlphaInfo			m_AlphaInfo;
	sTrackingInfo		m_Tracking;
	sPositionInfo		m_tPositionInfo;

	RwBool				m_EnableLogType[NUM_ALL_CHAT_TYPE];
	RwUInt8				m_byDisplayGuiIndex;				///< 占쏙옙 占쏙옙째 占쏙옙占시뤄옙占쏙옙 창占쏙옙占쏙옙
};
