#include "precomp_dboclient.h"

// core
#include "TargetHpGui.h"
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation 
#include "NtlSLEvent.h"
#include "NtlSobNpc.h"
#include "NtlSobPlayer.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSobCharProxy.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"
#include "NtlSobBuffIcon.h"
#include "NtlSobCharProxy.h"
#include "NtlSobItemAttr.h"
#include "NtlSobMonster.h"
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "NtlWorldConcept.h"
#include "NtlWorldConceptTB.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSLDojo.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// GameTable
#include "NPCTable.h"
#include "MobTable.h"
#include "ItemTable.h"
#include "ObjectTable.h"

// Dbo
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "GUISoundDefine.h"
#include "BuffDispObject.h"
#include "PopupManager.h"
#include "InfoWndManager.h"

#define TARGETSTATUS_UPDATE_INFOWND_TIME 1.0f

#define TARGETSTATUS_PC_WEAPON_ATTR_POS_X	189
#define TARGETSTATUS_PC_WEAPON_ATTR_POS_Y	29
#define TARGETSTATUS_PC_ARMOR_ATTR_POS_X	176
#define	TARGETSTATUS_PC_ARMOR_ATTR_POS_Y	51
#define TARGETSTATUS_MOB_BATTLE_ATTR_POS_X	TARGETSTATUS_PC_WEAPON_ATTR_POS_X
#define	TARGETSTATUS_MOB_BATTLE_ATTR_POS_Y	TARGETSTATUS_PC_WEAPON_ATTR_POS_Y


CTargetHpGui::CTargetHpGui()
{
	m_pTargetSobObj = NULL;
}

CTargetHpGui::CTargetHpGui(const RwChar *pName)
:CNtlPLGui(pName)
{
	m_pTargetSobObj = NULL;	
	
	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_INVALID;
	m_sBattleAttr.bySourceWeaponAttr = INVALID_BYTE;
	m_sBattleAttr.bySourceArmorAttr = INVALID_BYTE;
	m_sBattleAttr.byTargetWeaponAttr = INVALID_BYTE;
	m_sBattleAttr.byTargetArmorAttr = INVALID_BYTE;
}

CTargetHpGui::~CTargetHpGui()
{
}

RwBool CTargetHpGui::Create(void)
{
	NTL_FUNCTION("CTargetHpGui::Create");

	if(!CNtlPLGui::Create("", "gui\\StatusBar.srf", "gui\\TargetStatus.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgTargetStatus");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	m_psttNickName = (gui::CStaticBox*)GetComponent( "sttNickName" );
	m_psttJobName = (gui::CStaticBox*)GetComponent( "sttJobName" );
	m_ppgbHp = (gui::CProgressBar*)GetComponent( "pgbTargetLpGauge" );
	m_ppgbEp = (gui::CProgressBar*)GetComponent( "pgbTargetEpGauge" );
	m_sttHp = (gui::CStaticBox*)GetComponent( "sttTargetLp" );
	m_sttEp	= (gui::CStaticBox*)GetComponent( "sttTargetEp" );
	m_ppnlPowerText = (gui::CPanel*)GetComponent( "pnlPowerText" );
	m_pbtnMenu = (gui::CButton*)GetComponent( "btnMenuButton" );
	m_pbtnShareTarget = (gui::CButton*)GetComponent( "btnShareTargetButton" );
	m_ppnlHp = (gui::CPanel*)GetComponent( "pnlTargetLpBack" );
	m_ppnlEp = (gui::CPanel*)GetComponent( "pnlTargetEpBack" );

	m_ppnlRaceIcon = (gui::CPanel*)GetComponent( "pnlRaceIcon" );
	m_ppnlBattleAttribute[TYPE_WEAPON] = (gui::CPanel*)GetComponent( "pnlBattleAttributeWeapon" );
	m_ppnlBattleAttribute[TYPE_ARMOR] = (gui::CPanel*)GetComponent( "pnlBattleAttributeArmor" );
	m_ppnlMobGrade = (gui::CPanel*)GetComponent( "pnlMobGrade" );
	m_ppnlNpcJob = (gui::CPanel*)GetComponent( "pnlNpcJob" );

	// Battle attribute InfoWindow
	m_slotWeaponMouseEnter = m_ppnlBattleAttribute[TYPE_WEAPON]->SigMouseEnter().Connect( this, &CTargetHpGui::OnMouseWeaponAttrEnter );
	m_slotWeaponMouseLeave = m_ppnlBattleAttribute[TYPE_WEAPON]->SigMouseLeave().Connect( this, &CTargetHpGui::OnMouseWeaponAttrLeave );
	m_slotArmorMouseEnter = m_ppnlBattleAttribute[TYPE_ARMOR]->SigMouseEnter().Connect( this, &CTargetHpGui::OnMouseArmorAttrEnter );
	m_slotArmorMouseLeave = m_ppnlBattleAttribute[TYPE_ARMOR]->SigMouseLeave().Connect( this, &CTargetHpGui::OnMouseArmorAttrLeave );

	// Buff
	m_pBuff = NTL_NEW CBuffDispGui;
	NTL_ASSERT( m_pBuff, "CTargetHpGui::Create : Buff is not allocated. Not enough memory" );
	m_pBuff->Create( CBuffDispGui::TYPE_2X8, CBuffDispGui::NORMAL_ICONSIZE, 5, 5, DIALOG_TARGER_UI );
	m_pBuff->Show( false );

	m_pbtnMenu->SetToolTip( GetDisplayStringManager()->GetString( DST_TARGET_MENU ) );
	m_pbtnShareTarget->SetToolTip( GetDisplayStringManager()->GetString( DST_TARGET_SHARED_TARGET ) );
	m_pbtnShareTarget->Show( false );
	
	// Power Number
	m_numPower.Create( ND_LEFT );
	m_numPower.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_0" ) );
	m_numPower.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_1" ) );
	m_numPower.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_2" ) );
	m_numPower.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_3" ) );
	m_numPower.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_4" ) );
	m_numPower.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_5" ) );
	m_numPower.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_6" ) );
	m_numPower.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_7" ) );
	m_numPower.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_8" ) );
	m_numPower.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfPowerFont_9" ) );	
	m_numPower.SetParentPos( 135, 0 );
	m_numPower.SetNumGap( -5 );
			    	
	Show(false);

	GetNtlGuiManager()->AddUpdateFunc( this );

	// Signal
	m_slotMove = m_pThis->SigMove().Connect( this, &CTargetHpGui::OnMove );
	m_slotClickMenuBtn = m_pbtnMenu->SigClicked().Connect( this, &CTargetHpGui::OnClickMenuBtn );
	m_slotClickShareTargetBtn = m_pbtnShareTarget->SigClicked().Connect( this, &CTargetHpGui::OnClickShareTargetBtn );
	m_slotPowerPaint = m_ppnlPowerText->SigPaint().Connect( this, &CTargetHpGui::OnPowerPaint );
	m_slotPowerMove = m_ppnlPowerText->SigMove().Connect( this, &CTargetHpGui::OnPowerMove );

	// event 등록.
	LinkMsg(g_EventSobTargetInfoUpdate, 0);
	LinkMsg(g_EventSobInfoUpdate, 0);
	LinkMsg(g_EventCharObjDelete, 0);
	LinkMsg(g_EventIconPopupResult, 0);
	LinkMsg(g_EventSubWeaponActive, (const char*)0, 0x7000 );
	LinkMsg(g_EventSubWeaponDeActive, (const char*)0, 0x7000 );
	LinkMsg(g_EventSobEquipChange, (const char*)0, 0x7000 );
	LinkMsg(g_EventTargetStatusRefresh, 0);
	LinkMsg(g_EventPartyLeaderChange);
	LinkMsg(g_EventPartyLeave);
	LinkMsg(g_EventDojoNotify);
	LinkMsg(g_EventCharRefreshBattleAttributeModifier, 0 );
	LinkMsg(g_EventTObjectUpdateState, 0);
				
	NTL_RETURN(TRUE);
}

void CTargetHpGui::Destroy(void)
{
	NTL_FUNCTION("CTargetHpGui::Destroy");

	if( m_pBuff )
	{
		m_pBuff->Destroy();
		NTL_DELETE( m_pBuff );
	}

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg(g_EventSobTargetInfoUpdate);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventSubWeaponActive);
	UnLinkMsg(g_EventSubWeaponDeActive);
	UnLinkMsg(g_EventSobEquipChange);
	UnLinkMsg(g_EventTargetStatusRefresh);
	UnLinkMsg(g_EventPartyLeaderChange);
	UnLinkMsg(g_EventPartyLeave);
	UnLinkMsg(g_EventDojoNotify);
	UnLinkMsg(g_EventCharRefreshBattleAttributeModifier);
	UnLinkMsg(g_EventTObjectUpdateState);
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTargetHpGui::Update( RwReal fElapsed  )
{
//	m_LPGaugeAnimation.Update( fElapsed, m_ppgbHp );
//	m_EPGaugeAnimation.Update( fElapsed, m_ppgbEp );

	if( m_pTargetSobObj )
		m_pBuff->Update( fElapsed );
}

RwInt32 CTargetHpGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		// WorldConcept Check
		CNtlWorldConceptTB*	pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		if( pTBWorldConcept )
		{
			// Match Check
			if( pTBWorldConcept->GetMatchType() == MATCHWORLD_TYPE_MAJOR_MATCH || 
				pTBWorldConcept->GetMatchType() == MATCHWORLD_TYPE_FINAL_MATCH )
			{
				// Am i observer on TBMatch? ( observer is cant open target gui on major and final match )
				stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
				if( !pMember )
				{
					return -1;
				}
			}
		}
	}

	Show(bOpen);	
	m_pBuff->Show( bOpen );

	if( !bOpen )
	{
		if( GetPopupManager()->IsVisiblePopupGui( PLACE_SUB_TARGETUI ) )
			GetPopupManager()->AllPopupClose();
	}
	return 1;
}

void CTargetHpGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CTargetHpGui::HandleEvents");

	if(pMsg.Id == g_EventSobTargetInfoUpdate)
	{
		SNtlEventSobTargetInfoUpdate *pTartgetUpdate = reinterpret_cast<SNtlEventSobTargetInfoUpdate*>(pMsg.pData); 
		
		if(pTartgetUpdate->bShow)
		{
			if( m_pTargetSobObj && m_pTargetSobObj->GetSerialID() == pTartgetUpdate->hSerialId )
				return;
			
			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pTartgetUpdate->hSerialId ); 
			
			m_pTargetSobObj = pSobObj;
			if( !TargetUpdateProc() )
			{
				m_pTargetSobObj = NULL;
				return;
			}
			
			// 초기 타게팅시 초기화.
			m_pBuff->ClearBuff();
			BattleAttributeBonusClear();			
		}
		else
		{
			m_pTargetSobObj = NULL;
			m_pBuff->ClearBuff();
			BattleAttributeBonusClear();

			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI, FALSE );
		}
	}
	else if(pMsg.Id == g_EventTargetStatusRefresh)
	{
		if( m_pTargetSobObj )
			TargetUpdateProc();
	}
	else if(pMsg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate *pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(pMsg.pData); 
	
		if( m_pTargetSobObj && m_pTargetSobObj->GetSerialID() == pUpdate->hSerialId )
		{
			if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
			{
				//const WCHAR* pName = Logic_GetName( m_pTargetSobObj );
				RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
				RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
				RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
				RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );
				
				m_ppgbHp->SetRange( 0, uiMaxHp );
				m_ppgbHp->SetPos(uiHp);
//				m_LPGaugeAnimation.SetValue( uiHp, uiMaxHp, m_ppgbHp );
				m_ppgbEp->SetRange( 0, uiMaxEp );
				m_ppgbEp->SetPos(uiEp);
//				m_EPGaugeAnimation.SetValue( uiEp, uiMaxEp, m_ppgbEp );


				CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
				if( m_pTargetSobObj->GetClassID() == SLCLASS_AVATAR )
				{
					CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( m_pTargetSobObj );

					if( pGroupScouter->IsUsePlayerScouter( pSobPlayer->GetCharID() ) )
					{
						m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
						m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );						
					}
					else
					{
						const WCHAR* szLPEP = GetDisplayStringManager()->GetString( DST_STATUS_UNIDENTIFIED_LPEP );
						m_sttHp->SetText( szLPEP );
						m_sttEp->SetText( szLPEP );						
					}

					CalcBattleAttribute( m_pTargetSobObj );			
					OnBattleAttributeRefresh();
				}
				if( m_pTargetSobObj->GetClassID() == SLCLASS_MONSTER )
				{
					CNtlSobMonsterAttr* pMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pTargetSobObj->GetSobAttr() );
					sMOB_TBLDAT* pMobData = pMonsterAttr->GetMobTbl();

					if( pGroupScouter->IsUseMonsterScouter( pMobData->dwMobGroup ) )
					{
						m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
						m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );								
					}
					else
					{
						const WCHAR* szLPEP = GetDisplayStringManager()->GetString( DST_STATUS_UNIDENTIFIED_LPEP );
						m_sttHp->SetText( szLPEP );
						m_sttEp->SetText( szLPEP );
					}
				}
				else if( m_pTargetSobObj->GetClassID() == SLCLASS_PLAYER )
				{
					CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( m_pTargetSobObj );
					
					if( pGroupScouter->IsUsePlayerScouter( pSobPlayer->GetCharID() ) )
					{
						m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
						m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );						
					}
					else
					{
						const WCHAR* szLPEP = GetDisplayStringManager()->GetString( DST_STATUS_UNIDENTIFIED_LPEP );
						m_sttHp->SetText( szLPEP );
						m_sttEp->SetText( szLPEP );						
					}
				}
				else
				{
					m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
					m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );		
				}
			}
			
			if( pUpdate->uiUpdateType & EVENT_AIUT_BUFF )
				m_pBuff->SetBuffAll( m_pTargetSobObj->GetSerialID() );
		}
		else
		{
			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pSobAvatar )
			{
				if( m_pTargetSobObj && ( pSobAvatar->GetSerialID() == pUpdate->hSerialId  ) )
				{
					CalcBattleAttribute( m_pTargetSobObj );			
					OnBattleAttributeRefresh();
				}
			}
		}
	}
	else if(pMsg.Id == g_EventSubWeaponActive)
	{
		SNtlEventSubWeaponActive* pActiveData = reinterpret_cast<SNtlEventSubWeaponActive*>( pMsg.pData );

		if( m_pTargetSobObj && (m_pTargetSobObj->GetSerialID() == pActiveData->uiSerialId ||
			GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pActiveData->uiSerialId) )
		{
			CalcBattleAttribute( m_pTargetSobObj );
			OnBattleAttributeRefresh();
		}
	}
	else if(pMsg.Id == g_EventSubWeaponDeActive)
	{
		SNtlEventSubWeaponDeActive* pActiveData = reinterpret_cast<SNtlEventSubWeaponDeActive*>( pMsg.pData );

		if( m_pTargetSobObj && ( m_pTargetSobObj->GetSerialID() == pActiveData->uiSerialId ||
			GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pActiveData->uiSerialId ))
		{
			CalcBattleAttribute( m_pTargetSobObj );			
			OnBattleAttributeRefresh();
		}
	}
	else if(pMsg.Id == g_EventSobEquipChange)
	{
		SNtlEventSobEquipChange* pEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( pMsg.pData );

		if( m_pTargetSobObj )
		{
			if( m_pTargetSobObj->GetSerialID() == pEquipChange->hSerialId )
			{
				CalcBattleAttribute( m_pTargetSobObj );		
				OnBattleAttributeRefresh();

				return;
			}

			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( pSobAvatar )
			{
				if( pSobAvatar->GetSerialID() == pEquipChange->hSerialId )
				{
					CalcBattleAttribute( m_pTargetSobObj );
					OnBattleAttributeRefresh();

					return;
				}
			}
		}
	}
	else if(pMsg.Id == g_EventCharRefreshBattleAttributeModifier )
	{
		SNtlEventCharRefreshBattleAttributeModifier* pNotify = reinterpret_cast<SNtlEventCharRefreshBattleAttributeModifier*>( pMsg.pData );

		if( m_pTargetSobObj && (m_pTargetSobObj->GetSerialID() == pNotify->hSerialID ) )
		{
			m_sBattleAttr.afTargetOffenceBonus[BATTLE_ATTRIBUTE_HONEST] = pNotify->fHonestOffenceModifier;
			m_sBattleAttr.afTargetOffenceBonus[BATTLE_ATTRIBUTE_STRANGE] = pNotify->fStrangeOffenceModifier;
			m_sBattleAttr.afTargetOffenceBonus[BATTLE_ATTRIBUTE_WILD] = pNotify->fWildOffenceModifier;
			m_sBattleAttr.afTargetOffenceBonus[BATTLE_ATTRIBUTE_ELEGANCE] = pNotify->fStrangeOffenceModifier;
			m_sBattleAttr.afTargetOffenceBonus[BATTLE_ATTRIBUTE_FUNNY] = pNotify->fFunnyOffenceModifier;
			m_sBattleAttr.afTargetDefenceBonus[BATTLE_ATTRIBUTE_HONEST] = pNotify->fHonestDefenceModifier;
			m_sBattleAttr.afTargetDefenceBonus[BATTLE_ATTRIBUTE_STRANGE] = pNotify->fStrangeDefenceModifier;
			m_sBattleAttr.afTargetDefenceBonus[BATTLE_ATTRIBUTE_WILD] = pNotify->fWildDefenceModifier;
			m_sBattleAttr.afTargetDefenceBonus[BATTLE_ATTRIBUTE_ELEGANCE] = pNotify->fStrangeDefenceModifier;
			m_sBattleAttr.afTargetDefenceBonus[BATTLE_ATTRIBUTE_FUNNY] = pNotify->fFunnyDefenceModifier;

			CalcBattleAttribute( m_pTargetSobObj );		
			OnBattleAttributeRefresh();
		}
	}
	else if(pMsg.Id == g_EventCharObjDelete)
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( pMsg.pData );
		
		if( m_pTargetSobObj && m_pTargetSobObj->GetSerialID() == *pDeleteSerial )
		{
			m_pTargetSobObj = NULL;
			m_pBuff->ClearBuff();

			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI, FALSE );			
		}
	}
	else if(pMsg.Id == g_EventIconPopupResult)
	{
		SDboEventIconPopupResult* pPacket = reinterpret_cast<SDboEventIconPopupResult*>(pMsg.pData);
		if(pPacket->nSrcPlace == PLACE_SUB_TARGETUI)
		{
			if( pPacket->nWorkId == PMW_PARTY_INVITE )
			{
				// 이미 나의 파티원이 아니라면
				if( GetNtlSLGlobal()->GetSobAvatar()->GetParty()->IsMember(pPacket->uiSerial) == false )
				{
					CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pPacket->uiSerial);
					CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(pSob);

					// 플레이어만이 파티로 초대 가능하다
					if( pPlayer->GetClassID() != SLCLASS_PLAYER)
						NTL_RETURNVOID();

					CGamePacketGenerator *pGamePacketGenerator = GetDboGlobal()->GetGamePacketGenerator();
					pGamePacketGenerator->SendPartyInvite(pPlayer->GetSerialID());
				}				
			}
			else if( pPacket->nWorkId == PMW_PARTY_LEADER_CHANGE )
			{				
				// 파티장 위임
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyChangeLeader(pPacket->uiSerial);
			}
			else if(pPacket->nWorkId == PMW_PARTY_KICK_OUT)
			{
				// 파티 강퇴
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyKickOut(pPacket->uiSerial);
			}
			else if( pPacket->nWorkId == PMW_PVP_REQUEST_FIGHT )
			{
				if(GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAN_COMMUNICATION))
					GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleChallengeReq( m_pTargetSobObj->GetSerialID() );
				else
					Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
			}
			else if( pPacket->nWorkId == PMW_USER_TRADE )
			{
				if(GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAN_COMMUNICATION))
				{
					if( GetDboGlobal()->GetGamePacketGenerator()->SendTradeStartReq( m_pTargetSobObj->GetSerialID() ) )
					{
						CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(m_pTargetSobObj->GetSerialID()) );
						NTL_ASSERT(pSobPlayer, "CTargetHpGui::HandleEvents, g_EventIconPopupResult, pPacket->nWorkId == PMW_USER_TRADE, Not exist player of handle : " << m_pTargetSobObj->GetSerialID());
						CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pSobPlayer->GetSobAttr() );
						NTL_ASSERT(pSobPlayerAttr, "CTargetHpGui::HandleEvents, g_EventIconPopupResult, pPacket->nWorkId == PMW_USER_TRADE, Not exist player attr of handle : " << m_pTargetSobObj->GetSerialID());

						// %s님에게 거래를 요청하였습니다
						GetAlarmManager()->FormattedAlarmMessage(DST_TRADE_REQUEST_START, FALSE, NULL, pSobPlayerAttr->GetName());
					}
				}
				else
					Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
			}
			else if( pPacket->nWorkId == PMW_GUILD_INVITE )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendGuildInviteReq( m_pTargetSobObj->GetSerialID() );
			}
		}
	}
	else if( pMsg.Id == g_EventPartyLeaderChange )
	{
		if( Logic_I_am_PartyLeader() )
			m_pbtnShareTarget->Show( true );
		else
			m_pbtnShareTarget->Show( false );
	}
	else if( pMsg.Id == g_EventPartyLeave )
	{
		m_pbtnShareTarget->Show( false );
	}
	else if( pMsg.Id == g_EventDojoNotify )
	{
		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( pMsg.pData );

		if( false == GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			NTL_RETURNVOID();

		if( !m_pTargetSobObj )
			NTL_RETURNVOID();		

		if( DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER != pEvent->byDojoEvent )
			NTL_RETURNVOID();

		if( SLCLASS_TRIGGER_OBJECT != m_pTargetSobObj->GetClassID() )
			NTL_RETURNVOID();		

		CNtlSobTriggerObjectAttr*	pSobObjectAttr	= reinterpret_cast<CNtlSobTriggerObjectAttr*>( m_pTargetSobObj->GetSobAttr() );
		sOBJECT_TBLDAT*				pOBJECT_TBLDAT	= pSobObjectAttr->GetTriggerObjectTbl();

		if( false == BIT_FLAG_TEST(pOBJECT_TBLDAT->wFunction, eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL) )
			NTL_RETURNVOID();

		RwUInt32 idxObject = *(RwUInt32*)pEvent->pExData;

		if( idxObject != pOBJECT_TBLDAT->tblidx )
			NTL_RETURNVOID();

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			NTL_RETURNVOID();

		CNtlDojo*		pDojo			= pAvatar->GetDojo();
		sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

		MAP_SCRAMBLE_SEAL::iterator it_ScrambleSealInfo = pSCRAMBLE_INFO->mapScrambleSeal.find(idxObject);
		if( it_ScrambleSealInfo == pSCRAMBLE_INFO->mapScrambleSeal.end() )
			NTL_RETURNVOID();


		sSCRAMBLE_SEAL_INFO* pSCRAMBLE_SEAL_INFO = it_ScrambleSealInfo->second;

		SetScrambleOwner((RwUInt8)pSCRAMBLE_SEAL_INFO->eState);
	}	
	else if( pMsg.Id == g_EventTObjectUpdateState )
	{
		SNtlEventTObjectUpdateState* pEvent = reinterpret_cast<SNtlEventTObjectUpdateState*>( pMsg.pData );

		if( GetDboGlobal()->GetTargetSerial() != pEvent->hSerialId )
			NTL_RETURNVOID();

		if( FALSE == IsShow() )
			NTL_RETURNVOID();

		if( false == BIT_FLAG_TEST(pEvent->bySubState, TOBJECT_SUBSTATE_FLAG_SHOW) )
			GetDialogManager()->CloseDialog( DIALOG_TARGER_UI, FALSE );
	}
	
	NTL_RETURNVOID();
}

RwBool CTargetHpGui::TargetUpdateProc(VOID)
{
	m_pbtnMenu->Show( false );

	if( m_pTargetSobObj->GetClassID() == SLCLASS_AVATAR )
	{
		m_ppnlPowerText->Show( false );
		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_psttJobName->Show( false );
		m_pBuff->Show( true );
		m_ppnlMobGrade->Show( false );			
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( false );
		m_ppnlPowerText->Show( false );
		
		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );

		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);
		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);
		m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
		m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );		

		// 종족 아이콘
		CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( m_pTargetSobObj->GetSobAttr() );
		SetRaceIcon( pSobAvatarAttr->GetRace() );

		// 탈 것의 정보
		SetVichicle();		

		//// 진기맹여락
		//CalcBattleAttribute( m_pTargetSobObj );				
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_PLAYER )				
	{
		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_psttJobName->Show( false );
		m_ppnlMobGrade->Show( false );	
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( false );
		m_ppnlPowerText->Show( false );
		m_pbtnMenu->Show( true );
		
		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);
		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);
		
		m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );

		CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( m_pTargetSobObj );
		CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( m_pTargetSobObj->GetSobAttr() );
		CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
		SMemoryScouterData* pScouterData = pGroupScouter->IsUsePlayerScouter( pSobPlayer->GetCharID() );

		if( pScouterData )
		{
			m_numPower.SetNumber( pScouterData->uiPower );
			m_ppnlPowerText->Show( true );

			m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
			m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );

			//// 진기맹여락
			//CalcBattleAttribute( m_pTargetSobObj );

			m_ppnlBattleAttribute[TYPE_WEAPON]->Show( true );
			m_ppnlBattleAttribute[TYPE_ARMOR]->Show( true );

			m_pBuff->Show( true );
		}
		else
		{
			m_ppnlPowerText->Show( false );
			
			const WCHAR* szLPEP = GetDisplayStringManager()->GetString( DST_STATUS_UNIDENTIFIED_LPEP );
			m_sttHp->SetText( szLPEP );
			m_sttEp->SetText( szLPEP );

			//// 진기맹여락
			//SetBattleAttribute( TYPE_WEAPON, BATTLE_ATTRIBUTE_NONE );
			//SetBattleAttribute( TYPE_ARMOR, BATTLE_ATTRIBUTE_NONE );

			m_ppnlBattleAttribute[TYPE_WEAPON]->Show( false );
			m_ppnlBattleAttribute[TYPE_ARMOR]->Show( false );

			m_pBuff->Show( false );
		}

		// 종족 아이콘		
		SetRaceIcon( pSobPlayerAttr->GetRace() );

		// 탈 것의 정보
		SetVichicle();
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_MONSTER )
	{
		CNtlSobMonsterAttr* pSobMosterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( m_pTargetSobObj->GetSobAttr() );
		sMOB_TBLDAT* pMobData = pSobMosterAttr->GetMobTbl();

		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_pBuff->Show( true );
		m_ppnlMobGrade->Show( true );
		m_ppnlRaceIcon->Show( true );

		m_psttJobName->Show( false );
		m_ppnlNpcJob->Show( false );
		m_ppnlBattleAttribute[TYPE_WEAPON]->Show(false);

		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);
//		m_LPGaugeAnimation.SetValue( uiHp, uiMaxHp, m_ppgbHp, TRUE );
		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);
//		m_EPGaugeAnimation.SetValue( uiEp, uiMaxEp, m_ppgbEp, TRUE );

		CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
		SMemoryScouterData* pScouterData = pGroupScouter->IsUseMonsterScouter( pMobData->dwMobGroup );
		
		if( pScouterData )
		{
			m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );
			m_numPower.SetNumber( pScouterData->uiPower );
			m_ppnlPowerText->Show( true );
			m_ppnlBattleAttribute[TYPE_ARMOR]->Show(true);
			
			m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
			m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );		

			// 몹 그레이드 
			SetMobGrade( pMobData->byGrade );
		}
		else
		{
			if( pMobData->bShow_Name )
				m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );
			else
				m_psttNickName->SetText( GetDisplayStringManager()->GetString( DST_STATUS_UNIDENTIFIED_NAME ) );

			m_ppnlPowerText->Show( false );
			m_ppnlBattleAttribute[TYPE_ARMOR]->Show(false);

			const WCHAR* szLPEP = GetDisplayStringManager()->GetString( DST_STATUS_UNIDENTIFIED_LPEP );
			m_sttHp->SetText( szLPEP );
			m_sttEp->SetText( szLPEP );

			// 몹 그레이드 
			SetMobGrade( MOB_GRADE_NORMAL );
		}

		// 종족 아이콘
		SetMobNpcTypeIcon( pMobData->byMob_Type );	

		Logic_PlayGUISound( GSD_SYSTEM_ENEMY_SELECT );
	}	
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_NPC )
	{
		m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );

		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		
		m_ppnlPowerText->Show( false );
		m_ppnlEp->Show( false );
		m_psttJobName->Show( true );
		m_pBuff->Show( false );
		m_ppnlMobGrade->Show( false );
		m_ppnlBattleAttribute[TYPE_WEAPON]->Show( false );
		m_ppnlBattleAttribute[TYPE_ARMOR]->Show( false );
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( true );

		CNtlSobNpcAttr* pAttr = reinterpret_cast<CNtlSobNpcAttr*>( m_pTargetSobObj->GetSobAttr() );
		if( pAttr->GetNpcTbl()->dwFunc_Bit_Flag & NPC_FUNC_FLAG_DISCLOSE_LP )
		{
			m_ppnlHp->Show( true );
			m_ppgbHp->SetRange( 0, uiMaxHp );
			m_ppgbHp->SetPos(uiHp);
			m_psttJobName->SetPosition( 18, 51 );
		}		
		else
		{
			m_ppnlHp->Show( false );
			m_psttJobName->SetPosition( 18, 41 );
		}

		const WCHAR* pJobText = Logic_GetNPCJobName( pAttr->GetNpcTbl()->byJob );
		m_psttJobName->SetText( pJobText );				
		m_ppnlNpcJob->SetToolTip( std::wstring( pJobText ) );

		// 종족 아이콘
		CNtlSobNpcAttr* pNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( m_pTargetSobObj->GetSobAttr() );
		sNPC_TBLDAT* pNpcData = pNpcAttr->GetNpcTbl();

		SetMobNpcTypeIcon( pNpcData->byNpcType );
		SetNPCJobIcon( pNpcData->byJob );
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_PET )
	{
		m_psttNickName->SetText( Logic_GetName( m_pTargetSobObj ) );

		RwUInt32 uiHp = Logic_GetLp( m_pTargetSobObj );
		RwUInt32 uiEp = Logic_GetEp( m_pTargetSobObj );
		RwUInt32 uiMaxHp = Logic_GetMaxLp( m_pTargetSobObj );
		RwUInt32 uiMaxEp = Logic_GetMaxEp( m_pTargetSobObj );

		m_ppnlHp->Show( true );
		m_ppnlEp->Show( true );
		m_psttJobName->Show( false );
		m_ppnlMobGrade->Show( false );	
		m_ppnlRaceIcon->Show( true );
		m_ppnlNpcJob->Show( false );
		m_ppnlPowerText->Show( false );
		m_pBuff->Show( true );
		m_ppnlBattleAttribute[TYPE_WEAPON]->Show( false );
		m_ppnlBattleAttribute[TYPE_ARMOR]->Show( false );

		m_ppgbHp->SetRange( 0, uiMaxHp );
		m_ppgbHp->SetPos(uiHp);
		m_ppgbEp->SetRange( 0, uiMaxEp );
		m_ppgbEp->SetPos(uiEp);

		m_sttHp->Format( "%u / %u", uiHp, uiMaxHp );
		m_sttEp->Format( "%u / %u", uiEp, uiMaxEp );	

		// 종족 아이콘( Pet 은 NPC )
		CNtlSobPetAttr* pPetAttr = reinterpret_cast<CNtlSobPetAttr*>( m_pTargetSobObj->GetSobAttr() );
		sNPC_TBLDAT* pPetData = pPetAttr->GetNpcTbl();

		SetMobNpcTypeIcon( pPetData->byNpcType );
	}
	else if( m_pTargetSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT )
	{
		CNtlSobTriggerObject* pTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>( m_pTargetSobObj );
		CNtlSobTriggerObjectAttr* pTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( pTriggerObject->GetSobAttr() );
		sOBJECT_TBLDAT* pOBJECT_TBLDAT = pTriggerObjectAttr->GetTriggerObjectTbl();

		if( pOBJECT_TBLDAT->wFunction & eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
				return FALSE;

			CNtlDojo*		pDojo			= pAvatar->GetDojo();
			sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

			MAP_SCRAMBLE_SEAL::iterator it_ScrambleSealInfo = pSCRAMBLE_INFO->mapScrambleSeal.find( pOBJECT_TBLDAT->tblidx );
			if( it_ScrambleSealInfo == pSCRAMBLE_INFO->mapScrambleSeal.end() )
				return FALSE;

			std::wstring			wstrObjectName			= Logic_GetName(m_pTargetSobObj);
			sSCRAMBLE_SEAL_INFO*	pSCRAMBLE_SEAL_INFO	= it_ScrambleSealInfo->second;

			//const WCHAR* pwcText = GetDisplayStringManager()->GetString();
			//const WCHAR* pwcState;			

			m_ppnlPowerText->Show( false );
			m_ppnlHp->Show( false );
			m_ppnlEp->Show( false );
			m_psttJobName->Show( true );
			m_pBuff->Show( false );
			m_ppnlMobGrade->Show( false );
			m_ppnlBattleAttribute[TYPE_WEAPON]->Show( false );
			m_ppnlBattleAttribute[TYPE_ARMOR]->Show( false );
			m_ppnlRaceIcon->Show( true );
			m_ppnlNpcJob->Show( true );			

			m_psttNickName->SetText( wstrObjectName.c_str() );
			m_psttJobName->SetPosition( 18, 41 );
			m_ppnlNpcJob->SetToolTip( wstrObjectName.c_str() );

			SetScrambleOwner((RwUInt8)pSCRAMBLE_SEAL_INFO->eState);

			//m_psttJobName->Format(pwcText,);
			SetMobNpcTypeIcon( MOB_TYPE_ROBOT );
			SetObjectIcon( pOBJECT_TBLDAT->wFunction );
		}
		else
			return FALSE;
	}
	else 
	{
		// Pet 말고 다른 것 출력할게 있나? 
		NTL_ASSERT( FALSE, "CTargetHpGui::TargetUpdateProc - Invalid SobObj's Class ID" );
	}

	// 전투 속성 재계산
	CalcBattleAttribute( m_pTargetSobObj );
	OnBattleAttributeRefresh();

	GetDialogManager()->OpenDialog( DIALOG_TARGER_UI );
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// signal function

VOID CTargetHpGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_pBuff->SetMove( rect );
}

VOID CTargetHpGui::OnClickMenuBtn( gui::CComponent* pComponent )
{
	RwInt32 iAdjust = 37;
	CRectangle rect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow( TRUE, m_pTargetSobObj->GetSerialID(), PLACE_SUB_TARGETUI, 0, rect.right - iAdjust, -iAdjust );	
}

VOID CTargetHpGui::OnClickShareTargetBtn( gui::CComponent* pComponent )
{
	RwInt32 iAdjust = 37;
	CRectangle rect = m_pThis->GetScreenRect();
	CDboEventGenerator::IconPopupShow( TRUE, m_pTargetSobObj->GetSerialID(), PLACE_SUB_TARGETUI, 1, rect.right - iAdjust, -iAdjust );		// nSrcSlotId로  ShareTarget인지 Menu인지 구분.
}

VOID CTargetHpGui::CalcBattleAttribute( CNtlSob* pObj )
{
	CNtlSobAvatar* pSobAvatar = reinterpret_cast<CNtlSobAvatar*>( GetNtlSLGlobal()->GetSobAvatar() );
	if( pSobAvatar == NULL )
		return;

	CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );
	CNtlSobCharProxy* pSourceCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobAvatar->GetSobProxy() );

	// Source
	RwUInt8 bySourceWeaponAttr = INVALID_BYTE;
	if( pSourceCharProxy->GetActiveSubWeapon() )
		bySourceWeaponAttr = pSobAvatarAttr->GetSubWeaponAttr();
	else
		bySourceWeaponAttr = pSobAvatarAttr->GetMainWeaponAttr();

	SetSourceAttr( bySourceWeaponAttr, pSobAvatarAttr->GetArmorWeaponAttr() );

	// Source의 Bonus는 여기서 세팅. Target의 Bonus는 패킷이 날라올 때 변경한다.
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_HONEST] = pSobAvatarAttr->m_fHonestOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_STRANGE] = pSobAvatarAttr->m_fStrangeOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_WILD] = pSobAvatarAttr->m_fWildOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_ELEGANCE] = pSobAvatarAttr->m_fStrangeOffence;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_FUNNY] = pSobAvatarAttr->m_fFunnyOffence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_HONEST] = pSobAvatarAttr->m_fHonestDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_STRANGE] = pSobAvatarAttr->m_fStrangeDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_WILD] = pSobAvatarAttr->m_fWildDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_ELEGANCE] = pSobAvatarAttr->m_fStrangeDefence;
	m_sBattleAttr.afSourceDefenceBonus[BATTLE_ATTRIBUTE_FUNNY] = pSobAvatarAttr->m_fFunnyDefence;

	if( pObj->GetClassID() == SLCLASS_AVATAR )
	{
		CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pObj->GetSobAttr() );
		CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pObj->GetSobProxy() );

		RwUInt8 byWeaponAttr = INVALID_BYTE;
		if( pCharProxy->GetActiveSubWeapon() )
		{
			byWeaponAttr = pSobPlayerAttr->GetSubWeaponAttr();
			SetBattleAttribute( TYPE_WEAPON, pSobPlayerAttr->GetSubWeaponAttr() );
		}
		else
		{
			byWeaponAttr = pSobPlayerAttr->GetMainWeaponAttr();
			SetBattleAttribute( TYPE_WEAPON, pSobPlayerAttr->GetMainWeaponAttr() );
		}

		SetBattleAttribute( TYPE_ARMOR, pSobPlayerAttr->GetArmorWeaponAttr() );

		SetTargetAttr( byWeaponAttr, pSobPlayerAttr->GetArmorWeaponAttr() );
	}
	else if( pObj->GetClassID() == SLCLASS_PLAYER )
	{
		CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( pObj );
		CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pObj->GetSobAttr() );
		CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pObj->GetSobProxy() );

		CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
		SMemoryScouterData* pScouterData = pGroupScouter->IsUsePlayerScouter( pSobPlayer->GetCharID() );

		if( pScouterData )
		{
			RwUInt8 byWeaponAttr = INVALID_BYTE;
			if( pCharProxy->GetActiveSubWeapon() )
			{// 서브웨폰 사용중
				byWeaponAttr = pSobPlayerAttr->GetSubWeaponAttr();
				SetBattleAttribute( TYPE_WEAPON, pSobPlayerAttr->GetSubWeaponAttr() );		
			}
			else
			{// 
				byWeaponAttr = pSobPlayerAttr->GetMainWeaponAttr();
				SetBattleAttribute( TYPE_WEAPON, pSobPlayerAttr->GetMainWeaponAttr() );
			}

			SetBattleAttribute( TYPE_ARMOR, pSobPlayerAttr->GetArmorWeaponAttr() );

			SetTargetAttr( byWeaponAttr, pSobPlayerAttr->GetArmorWeaponAttr() );
		}
		else
		{
			SetTargetAttr( INVALID_BYTE, INVALID_BYTE );
		}
	}
	else if( pObj->GetClassID() == SLCLASS_MONSTER )
	{
		CNtlSobMonster* pSobTarMonster = reinterpret_cast<CNtlSobMonster*>( pObj );
		CNtlSobMonsterAttr* pSobTarMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pSobTarMonster->GetSobAttr() );
		sMOB_TBLDAT* pMobData = pSobTarMonsterAttr->GetMobTbl();

		CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
		SMemoryScouterData* pScouterData = pGroupScouter->IsUseMonsterScouter( pMobData->dwMobGroup );

		if( pScouterData )
		{
			SetTargetAttr( pMobData->byBattle_Attribute, pMobData->byBattle_Attribute );
			SetBattleAttribute( TYPE_MOB, pMobData->byBattle_Attribute );
		}
		else
			SetTargetAttr( INVALID_BYTE, INVALID_BYTE );
	}
}

VOID CTargetHpGui::SetBattleAttribute( TYPE_BATTLEATTR eType, RwUInt8 byBattleAttribute )
{ 
    // Show 여부도 판단.
	gui::CSurface surface;

	if( eType == TYPE_MOB )
	{
		m_ppnlBattleAttribute[TYPE_WEAPON]->Show( false );
		m_ppnlBattleAttribute[TYPE_ARMOR]->SetPosition( TARGETSTATUS_MOB_BATTLE_ATTR_POS_X, TARGETSTATUS_MOB_BATTLE_ATTR_POS_Y );
		eType = TYPE_ARMOR;
		
		/*SetSourceAttr( byBattleAttribute, byBattleAttribute );*/
	}
	else
	{
		m_ppnlBattleAttribute[TYPE_ARMOR]->SetPosition( TARGETSTATUS_PC_ARMOR_ATTR_POS_X, TARGETSTATUS_PC_ARMOR_ATTR_POS_Y );
	}

	Logic_SetBattleAttributeMark( m_ppnlBattleAttribute[eType], byBattleAttribute );	
}

VOID CTargetHpGui::SetRaceIcon( RwUInt8 byRace )
{
	gui::CSurface surface;

	switch( byRace )
	{
	case RACE_HUMAN : 
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceHuman" ); 
		m_ppnlRaceIcon->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( DST_HUMAN ) ) );
		break;
	case RACE_NAMEK : 
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceNamek" ); 
		m_ppnlRaceIcon->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( DST_NAMEK ) ) );
		break;
	case RACE_MAJIN :
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceMajin" ); 
		m_ppnlRaceIcon->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( DST_MAJIN ) ) );
		break;		
	default : 
		NTL_ASSERTFAIL( "CTargetHpGui::SetRaceIcon : Invalid Race type" );
		return;
	}
	
	m_ppnlRaceIcon->GetSurface()->clear();
	m_ppnlRaceIcon->AddSurface( surface );
}

VOID CTargetHpGui::SetMobNpcTypeIcon( RwUInt8 byType )
{
	gui::CSurface surface;
	std::wstring wstrToolTip = GetDisplayStringManager()->GetString( DST_MOB_TYPE_ANIMAL + byType );

	switch( byType )
	{
		case MOB_TYPE_ANIMAL : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceAnimal" ); break;
		case MOB_TYPE_HUMAN_ANIMAL :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceHumanAnimal" ); break;
		case MOB_TYPE_DINOSAUR : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceDinosaur" ); break;
		case MOB_TYPE_ALIEN : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceAlien" ); break;
		case MOB_TYPE_ANDROID :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceAndroid" ); break;
		case MOB_TYPE_ROBOT : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceRobot" ); break;
		case MOB_TYPE_DRAGON : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceDragon" ); break;
		case MOB_TYPE_DEVIL :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceDevil" ); break;
		case MOB_TYPE_UNDEAD : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceUndead" ); break;
		case MOB_TYPE_PLANT : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRacePlant" ); break;
		case MOB_TYPE_INSECT:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceInsect" ); break;
		case MOB_TYPE_HUMAN :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceHuman" ); break;
		case MOB_TYPE_NAMEC :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceNamek" ); break;
		case MOB_TYPE_MAJIN :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceMajin" ); break;		
		case MOB_TYPE_BUILDING:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfRaceRobot" ); break;			
	}

	m_ppnlRaceIcon->GetSurface()->clear();
	m_ppnlRaceIcon->AddSurface( surface );
	m_ppnlRaceIcon->SetToolTip( wstrToolTip );
}

VOID CTargetHpGui::SetMobGrade( RwUInt8 byMobGrade )
{ // Show 여부도 판단
	gui::CSurface	surface;
	std::wstring	wstrToolTip;

	switch( byMobGrade )
	{
		case MOB_GRADE_NORMAL :	
			m_ppnlMobGrade->Show( false );
			return;
		case MOB_GRADE_SUPER : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeSuper" );
			wstrToolTip = GetDisplayStringManager()->GetString( DST_MOB_GRADE_SUPER );
			break;
		case MOB_GRADE_ULTRA : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeUltra" ); 
			wstrToolTip = GetDisplayStringManager()->GetString( DST_MOB_GRADE_ULTRA );
			break;
		case MOB_GRADE_BOSS :
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeBoss" ); 
			wstrToolTip = GetDisplayStringManager()->GetString( DST_MOB_GRADE_BOSS );
			break;
		case MOB_GRADE_HERO : 
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGradeHero" ); 
			wstrToolTip = GetDisplayStringManager()->GetString( DST_MOB_GRADE_HERO );
			break;
	}

	m_ppnlMobGrade->Show( true );
	m_ppnlMobGrade->GetSurface()->clear();
	m_ppnlMobGrade->AddSurface( surface );
	m_ppnlMobGrade->SetToolTip( wstrToolTip );
}

VOID CTargetHpGui::SetNPCJobIcon( RwUInt8 byNPCJob )
{
	gui::CSurface surface;

	switch( byNPCJob )
	{
		case NPC_JOB_WEAPON_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfWeaponMerchantIcon" ); break;
		case NPC_JOB_ARMOR_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfArmorMerchantIcon" ); break;
		case NPC_JOB_GOODS_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfMiscMerchantIcon" ); break;
		case NPC_JOB_SCOUTER_MERCHANT :			
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScouterMerchantIcon" ); break;
		case NPC_JOB_GUARD :						
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGuardIcon" ); break;
		case NPC_JOB_SKILL_TRAINER_HFI :						
		case NPC_JOB_SKILL_TRAINER_HMY :						
		case NPC_JOB_SKILL_TRAINER_HEN :						
		case NPC_JOB_SKILL_TRAINER_NFI :						
		case NPC_JOB_SKILL_TRAINER_NMY :						
		case NPC_JOB_SKILL_TRAINER_MMI :						
		case NPC_JOB_SKILL_TRAINER_MWO :			
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSkillMasterIcon" ); break;
		case NPC_JOB_BANKER :						
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfWareHouseIcon" ); break;
		case NPC_JOB_TALKER :						
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfTalkerIcon" ); break;
		case NPC_JOB_GUILD_MANAGER :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGuildIcon" ); break;
		case NPC_JOB_SUMMON_PET :					
			//surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "" ); 
			break;
		case NPC_JOB_DOGI_MERCHANT :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfDogiMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_WEAPON_MERCHANT :	
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPWeaponMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_ARMOR_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPArmorMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_GOODS_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPMiscMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_FOODS_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPFoodMerchantIcon" ); break;
		case NPC_JOB_SPECIAL_SCOUTER_MERCHANT :	
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSPScouterMerchantIcon" ); break;
		case NPC_JOB_GRAND_SKILL_TRAINER_HFI :	
		case NPC_JOB_GRAND_SKILL_TRAINER_HMY :	
		case NPC_JOB_GRAND_SKILL_TRAINER_HEN :			
		case NPC_JOB_GRAND_SKILL_TRAINER_NFI :				
		case NPC_JOB_GRAND_SKILL_TRAINER_NMY :				
		case NPC_JOB_GRAND_SKILL_TRAINER_MMI :				
		case NPC_JOB_GRAND_SKILL_TRAINER_MWO :	
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfGrandSkillMasterIcon" ); break;
		case NPC_JOB_SUB_WEAPON_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSubWeaponMerchantIcon" ); break;
		case NPC_JOB_GATE_KEEPER :				
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfKeeperIcon" ); break;
		case NPC_JOB_VENDING_MACHINE :			
			//surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "" ); 
			break;
		case NPC_JOB_TIMEMACHINE_MERCHANT :		
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfTimeMachineIcon" ); break;
		case NPC_JOB_PORTAL_MAN :					
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfKeeperIcon" );
			break;
	}

	m_ppnlNpcJob->GetSurface()->clear();
	m_ppnlNpcJob->AddSurface( surface );
}

VOID CTargetHpGui::SetObjectIcon( RwUInt32 uiObjectFunction )
{
	gui::CSurface surface;

	if( uiObjectFunction & eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL )
	{
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSealObject" );
	}

	m_ppnlNpcJob->GetSurface()->clear();
	m_ppnlNpcJob->AddSurface( surface );
}

VOID CTargetHpGui::SetVichicle()
{
	CNtlSobActor* pSobActor = reinterpret_cast<CNtlSobActor*>( m_pTargetSobObj );
	if( false == Logic_IsVehicleDriver( pSobActor ) )
		return;

	CNtlBeCharData* pBeData = reinterpret_cast< CNtlBeCharData* >( pSobActor->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();
	sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(pCtrlStuff->sVehicle.idxVehicleItem);
	if( !pITEM_TBLDAT )
	{
		NTL_ASSERT(false, "CTargetHpGui::TargetUpdateProc, Nt exist ITEM TABLE of index : " << pCtrlStuff->sVehicle.idxVehicleItem);
		return;
	}	


	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
	std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);

	m_psttJobName->SetText( strItemName.c_str() );

	m_ppnlHp->Show( false );
	m_ppnlEp->Show( false );
	m_ppnlNpcJob->Show( true );
	m_psttJobName->Show( true );
}

VOID CTargetHpGui::OnPowerPaint(VOID)
{
	m_numPower.Render();
}

VOID CTargetHpGui::OnPowerMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_ppnlPowerText->GetScreenRect();
	m_numPower.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CTargetHpGui::OnMouseWeaponAttrEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();

	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_DEFENCE_RATE;

	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
		rtScreen.left, rtScreen.top,
		reinterpret_cast<void*>( &m_sBattleAttr ),
		DIALOG_TARGER_UI );
}

VOID CTargetHpGui::OnMouseWeaponAttrLeave( gui::CComponent* pComponent )
{
	if( DIALOG_TARGER_UI == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTargetHpGui::OnMouseArmorAttrEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();

	// INVALID값으로 구조체를 세팅한다.
	stINFOWND_BATTLEATTR sAttr;
	memset( &sAttr, INVALID_BYTE, sizeof(stINFOWND_BATTLEATTR) );

	if( m_pTargetSobObj->GetClassID() == SLCLASS_MONSTER )
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_ALL_RATE;
	else
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_ATTACK_RATE;
	
	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
		rtScreen.left, rtScreen.top,
		reinterpret_cast<void*>( &m_sBattleAttr ),
		DIALOG_TARGER_UI );
}

VOID CTargetHpGui::OnMouseArmorAttrLeave( gui::CComponent* pComponent )
{
	if( DIALOG_TARGER_UI == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTargetHpGui::SetSourceAttr( RwUInt8 bySourceWeaponAttr, RwUInt8 bySourceArmorAttr )
{
	m_sBattleAttr.bySourceWeaponAttr = bySourceWeaponAttr;
	m_sBattleAttr.bySourceArmorAttr = bySourceArmorAttr;
}

VOID CTargetHpGui::SetTargetAttr( RwUInt8 byTargetWeaponAttr, RwUInt8 byTargetArmorAttr )
{
	m_sBattleAttr.byTargetWeaponAttr = byTargetWeaponAttr;
	m_sBattleAttr.byTargetArmorAttr = byTargetArmorAttr;
}

VOID CTargetHpGui::SetScrambleOwner(RwUInt8 bySealOwnerTeam)
{
	eDBO_TEAM eDBOTeam = Logic_GetScrambleTeam((eTOBJECT_STATE_TYPE_C)bySealOwnerTeam);

	switch( eDBOTeam )
	{
	case DBO_TEAM_MY_TEAM:
		{
			m_psttJobName->SetText( GetDisplayStringManager()->GetString(DST_DOJO_SCRAMBLE_OUR_GUILD_SEAL) );
			break;
		}
	case DBO_TEAM_ENEMY:
		{
			m_psttJobName->SetText( GetDisplayStringManager()->GetString(DST_DOJO_SCRAMBLE_OTHER_GUILD_SEAL) );
			break;
		}
	case DBO_TEAM_NEUTRAILITY:
		{
			m_psttJobName->SetText( GetDisplayStringManager()->GetString(DST_DOJO_SCRAMBLE_NO_OWNER_SEAL) );
			break;
		}
	default:
		{
			DBO_FAIL("Invalid DBO team : " << eDBOTeam);
			break;
		}
	}
}

VOID CTargetHpGui::BattleAttributeBonusClear()
{
	for( int i=0; i < BATTLE_ATTRIBUTE_COUNT; ++i )
	{
		m_sBattleAttr.afSourceOffenceBonus[i] = 0.0f;
		m_sBattleAttr.afSourceDefenceBonus[i] = 0.0f;
		m_sBattleAttr.afTargetOffenceBonus[i] = 0.0f;
		m_sBattleAttr.afTargetDefenceBonus[i] = 0.0f;
	}
}

VOID CTargetHpGui::OnBattleAttributeRefresh()
{
	// 현재 마우스의 위치가 무기 전투 속성 패널에 있는지 방어구 전투 속성 패널에 있는지 판단
	TYPE_BATTLEATTR eType = TYPECOUNT;
	for( RwInt32 i = 0; i< TYPECOUNT; ++i )
	{
		if( m_ppnlBattleAttribute[i]->GetScreenRect().PtInRect( CMouse::GetX(), CMouse::GetY() ) )
		{
			if( m_ppnlBattleAttribute[i]->IsVisible() )
				eType = (TYPE_BATTLEATTR)i;
		}
	}

	// 마우스의 두 개의 패널이 아니라면 InfoWindow를 갱신할 필요가 없다.
	if( eType == TYPECOUNT )
		return;
	else if( eType == TYPE_ARMOR )
	{
		if( m_pTargetSobObj && m_pTargetSobObj->GetClassID() == SLCLASS_MONSTER )
			m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_ALL_RATE;
		else
			m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_ATTACK_RATE;
	}
	else
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_DEFENCE_RATE;

	// 현재 HpGui에서 요청한 InfoWindow인지 확인하고 전투 속성 관련 InfoWindow를 띄우는 것까지 확인
	// 하여 맞다면 현재 띄워진 InfoWindow를 닫고 새로 띄워준다.
	if( DIALOG_TARGER_UI == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );

		CRectangle rtScreen = m_ppnlBattleAttribute[eType]->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
			rtScreen.left, rtScreen.top,
			reinterpret_cast<void*>( &m_sBattleAttr ),
			DIALOG_TARGER_UI );
	}
}
