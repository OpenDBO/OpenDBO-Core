/*****************************************************************************
* 
*  File			: TargetHpGui.h
*  Author		: HyungSuk, Jang
*  Copyright	: (аж)NTL
*  Date			: 2005. 12. 12	
*  Abstract		: DBO Target hp gui.
* ****************************************************************************
*  Desc         : 
* 
* ****************************************************************************/

#ifndef __TARGETHP_GUI_H__
#define __TARGETHP_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

#include "NtlSLDef.h"
#include "SurfaceGui.h"
#include "NumberGui.h"
#include "InfoWndManager.h"

#include "GaugeAnimation.h"
#include "HpGui.h"

class CNtlSob;
class CBuffDispGui;

class CTargetHpGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum TYPE_BATTLEATTR { TYPE_WEAPON, TYPE_ARMOR, TYPECOUNT, TYPE_MOB };

	CTargetHpGui();
	CTargetHpGui(const RwChar* pName);
	~CTargetHpGui();

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	RwInt32 SwitchDialog(bool bOpen);

	virtual VOID HandleEvents(RWS::CMsg &pMsg);

private:
	// Implementation
	RwBool	TargetUpdateProc(VOID);

	VOID	CalcBattleAttribute( CNtlSob* pObj );
	VOID	SetBattleAttribute( TYPE_BATTLEATTR eType, RwUInt8 byBattleAttribute );
	VOID	SetRaceIcon( RwUInt8 byRace );
	VOID	SetMobNpcTypeIcon( RwUInt8 byType );
	VOID	SetMobGrade( RwUInt8 byMobGrade );
	VOID	SetNPCJobIcon( RwUInt8 byNPCJob );
	VOID	SetObjectIcon( RwUInt32 uiObjectFunction );
	VOID	SetVichicle();
		
	VOID	OnMove( RwInt32 nX, RwInt32 nY );
	VOID	OnClickMenuBtn( gui::CComponent* pComponent );
	VOID	OnClickShareTargetBtn( gui::CComponent* pComponent );
	VOID	OnPowerPaint(VOID);
	VOID	OnPowerMove( RwInt32 nOldX, RwInt32 nOldY );

	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotClickMenuBtn;
	gui::CSlot			m_slotClickShareTargetBtn;
	gui::CSlot			m_slotPowerPaint;
	gui::CSlot			m_slotPowerMove;

	gui::CStaticBox*	m_psttNickName;
	gui::CStaticBox*	m_psttJobName;
	gui::CPanel*		m_ppnlHp;
	gui::CProgressBar*	m_ppgbHp;
	gui::CStaticBox*	m_sttHp;
	gui::CPanel*		m_ppnlEp;
	gui::CProgressBar*	m_ppgbEp;
	gui::CStaticBox*	m_sttEp;
	gui::CButton*		m_pbtnMenu;
	gui::CButton*		m_pbtnShareTarget;

	// Power
	gui::CPanel*		m_ppnlPowerText;
	CNumberGui			m_numPower;
	
	gui::CPanel*		m_ppnlRaceIcon;
	gui::CPanel*		m_ppnlBattleAttribute[TYPECOUNT];
	gui::CPanel*		m_ppnlMobGrade;

	gui::CPanel*		m_ppnlNpcJob;

	CNtlSob*			m_pTargetSobObj;

//	CGaugeAnimation		m_LPGaugeAnimation;
//	CGaugeAnimation		m_EPGaugeAnimation;

	// Buff
	CBuffDispGui*		m_pBuff;

protected:
	stINFOWND_BATTLEATTR m_sBattleAttr;

	gui::CSlot			m_slotWeaponMouseEnter;
	gui::CSlot			m_slotWeaponMouseLeave;
	gui::CSlot			m_slotArmorMouseEnter;
	gui::CSlot			m_slotArmorMouseLeave;

	VOID	OnMouseWeaponAttrEnter(gui::CComponent* pComponent);
	VOID	OnMouseWeaponAttrLeave(gui::CComponent* pComponent);
	VOID	OnMouseArmorAttrEnter(gui::CComponent* pComponent);
	VOID	OnMouseArmorAttrLeave(gui::CComponent* pComponent);

	VOID	SetSourceAttr( RwUInt8 bySourceWeaponAttr, RwUInt8 bySourceArmorAttr );
	VOID	SetTargetAttr( RwUInt8 byTargetWeaponAttr, RwUInt8 byTargetArmorAttr );

	VOID	SetScrambleOwner(RwUInt8 bySealOwnerTeam);

	VOID	BattleAttributeBonusClear();
	VOID	OnBattleAttributeRefresh();
};

#endif