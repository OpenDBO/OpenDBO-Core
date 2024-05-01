////////////////////////////////////////////////////////////////////////////////
// File: DropItemInfoGui.h
// Desc: Drop된 Item의 정보를 보여주는 GUI
//
// 2007.05.22 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DROP_ITEM_INFO_GUI_H__
#define __DROP_ITEM_INFO_GUI_H__

#include "ceventhandler.h"
#include "SurfaceGui.h"

class CNtlSobWorldItem;

class CDropItemInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enumuration
	enum STATE { HIDE, FADE_IN, SHOW, SHOW_WAIT_FADE, FADE_OUT };
		
	//! Contructor & Destructor
	CDropItemInfoGui(RwUInt32 init_m_uiDropItemSerial, const RwChar* pName);
	CDropItemInfoGui( const RwChar* pName );
	~CDropItemInfoGui(VOID);

	//! Operation
	VOID	Init(RwUInt32 init_m_uiDropItemSerial, RwBool init_m_pDropItemShow);
	RwBool	Create();
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsedTime );

	RwInt32	SwitchDialog( bool bOpen );

	//! Events
	virtual VOID HandleEvents( RWS::CMsg& msg );	

	//! Refactoring method
	VOID 	ShowItemInCapsule();

	RwUInt32	m_uiDropItemSerial;


private:
	//! Implementation
	VOID	SetState( RwInt32 eState );
	VOID	PositionUpdate(VOID);
	VOID	FadeEffect(VOID);
	VOID	SetAlpha( RwUInt8 ucAlpha );

	//! CallBack
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseEnter( gui::CComponent* pComponent );
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nX, RwInt32 nY );

	gui::CSlot	m_slotMouseDown;
	gui::CSlot	m_slotMouseEnter;
	gui::CSlot	m_slotMouseLeave;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	
	//! Variables
	CSurfaceGui	m_surIcon;

	CNtlSobWorldItem* m_pDropItem;

	RwReal		m_fTime;
	RwInt32		m_eState;

	RwBool		m_bDirectToShowWaitFade;

	RwBool		m_pDropItemShow;
};

#endif//__DROP_ITEM_INFO_GUI_H__