#include "precomp_dboclient.h"
#include "HpGui.h"

// core
#include "NtlDebug.h"
#include "NtlPLDef.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSLMailSystem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// GameTable
#include "ItemTable.h"
#include "MobTable.h"
#include "FormulaTable.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboLogic.h"	
#include "BuffDispObject.h"
#include "DisplayStringManager.h"
#include "CursorManager.h"
#include "LobbyManager.h"

#define PCSTATUS_UPDATE_INFOWND_TIME	1.0f

CHpGui::CHpGui()
{
	Init();
}

CHpGui::CHpGui(const RwChar *pName)
:CNtlPLGui(pName)
{
	Init();
}

CHpGui::~CHpGui()
{
}

VOID CHpGui::Init(VOID)
{
	m_fCurrentRPChargingTime = 0.0f;
	m_fRPChargingTime = 0.0f;
	m_fDestValue = 0.0f;
	m_fCurrentValue = 0.0f;
	m_nMaxValue = 100;
	m_eRPChargeType = TYPE_NONE;

	m_hSound = INVALID_SOUND_HANDLE;
	m_pBuff = NULL;

	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_INVALID;
	m_sBattleAttr.bySourceWeaponAttr = INVALID_BYTE;
	m_sBattleAttr.bySourceArmorAttr = INVALID_BYTE;
	m_sBattleAttr.byTargetWeaponAttr = INVALID_BYTE;
	m_sBattleAttr.byTargetArmorAttr = INVALID_BYTE;
}

RwBool CHpGui::Create(void)
{
	NTL_FUNCTION("CHpGui::Create");

	if(!CNtlPLGui::Create("", "gui\\StatusBar.srf", "gui\\PcStatus.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgPcStatus");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	m_ppgbHp = (gui::CProgressBar*)GetComponent("pgbLP");
	m_ppgbEp = (gui::CProgressBar*)GetComponent( "pgbEP" );
	m_ppgbRp = (gui::CProgressBar*)GetComponent( "pgbRP" );
	m_ppgbRpBall = (gui::CProgressBar*)GetComponent( "pgbRPBall" );
	m_sttLevel = (gui::CStaticBox*)GetComponent("sttLevel");
	m_sttHp = (gui::CStaticBox*)GetComponent("sttLP");
	m_sttMp = (gui::CStaticBox*)GetComponent("sttEP");
	m_ppnlRpBack	= (gui::CPanel*)GetComponent( "pnlRPBack" );
	m_ppnlRpBallBack = (gui::CPanel*)GetComponent( "pnlRPBall" );
	m_psttNickName = (gui::CStaticBox*)GetComponent("sttNickName");
	m_ppnlPowerText = (gui::CPanel*)GetComponent( "pnlPowerText" );
	m_pbtnAvatar = (gui::CButton*)GetComponent( "btnAvatarSelect" );
			
	m_ppnlBattleAttribute[TYPE_WEAPON] = (gui::CPanel*)GetComponent( "pnlBattleAttributeWeapon" );
	m_ppnlBattleAttribute[TYPE_ARMOR] = (gui::CPanel*)GetComponent( "pnlBattleAttributeArmor" );
	m_pbtnPost = (gui::CButton*)GetComponent( "btnPostButton" );

	// Battle Attr InfoWindow
	m_slotWeaponMouseEnter = m_ppnlBattleAttribute[TYPE_WEAPON]->SigMouseEnter().Connect( this, &CHpGui::OnMouseWeaponAttrEnter );
	m_slotWeaponMouseLeave = m_ppnlBattleAttribute[TYPE_WEAPON]->SigMouseLeave().Connect( this, &CHpGui::OnMouseWeaponAttrLeave );
	m_slotArmorMouseEnter = m_ppnlBattleAttribute[TYPE_ARMOR]->SigMouseEnter().Connect( this, &CHpGui::OnMouseArmorAttrEnter );
	m_slotArmorMouseLeave = m_ppnlBattleAttribute[TYPE_ARMOR]->SigMouseLeave().Connect( this, &CHpGui::OnMouseArmorAttrLeave );

		// AvatarBtn은 버튼이 아닌것처럼. 사운드 없애기
	m_pbtnAvatar->SetClickSound( NULL );
	m_pbtnAvatar->SetDisableSound( NULL );
	m_pbtnAvatar->SetFocusSound( NULL );

	// RP는 0으로 세팅
	m_ppgbRp->SetRange( 0, 100 );
	m_ppgbRp->SetPos( 0 );

	m_ppgbRpBall->SetRange( 0, 7 );
	m_ppgbRpBall->SetPos( 0 );
	
	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		NTL_RETURN(FALSE);
	}

	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURN(FALSE);
	}

	m_psttNickName->SetText(pLOBBY_CHARACTER->tSummary.awchName);

	// Tooltip
	m_ppgbRpBall->SetToolTip( GetDisplayStringManager()->GetString( DST_RPBALL_TOOLTIP ) );
	m_sttLevel->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( DST_STATUS_LEVEL ) ) );

	CRectangle rect = m_pThis->GetScreenRect();
	m_surCharacter.SetRectWH( rect.left + 10, rect.top + 7, 81, 81 );
	m_surCharacter.SetFilterTexture( Logic_CreateTexture( "8181.png" ) );		
	
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CHpGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CHpGui::OnMove );
	m_slotPaintLast = m_pbtnPost->SigPaint().Connect( this, &CHpGui::OnPaintLast );
	m_slotClickPost = m_pbtnPost->SigClicked().Connect( this, &CHpGui::OnClickPost );
	m_slogPaintPost = m_pbtnPost->SigPaint().Connect( this, &CHpGui::OnPaintPost );
	m_slotPowerPaint = m_ppnlPowerText->SigPaint().Connect( this, &CHpGui::OnPowerPaint );
	m_slotPowerMove = m_ppnlPowerText->SigMove().Connect( this, &CHpGui::OnPowerMove );
	m_slotAvatarFocused = m_pbtnAvatar->SigFocused().Connect( this, &CHpGui::OnAvatarFocused );
	m_slotAvatarClick = m_pbtnAvatar->SigClicked().Connect( this, &CHpGui::OnAvatarClick );
	
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

	// Buff
	m_pBuff = NTL_NEW CBuffDispGui;
	NTL_ASSERT( m_pBuff, "CHpGui::Create : Buff is not allocated. Not enough memory" );

	m_pBuff->Create( CBuffDispGui::TYPE_2X8, CBuffDispGui::NORMAL_ICONSIZE, 5, 5, DIALOG_HP );
	m_pBuff->Show( true );

	// Effect
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", "srfEmergencyEffect" );
	m_feEmergency.SetSurface( surface );
	m_feEmergency.SetTime( 0.0f, 0.5f );
	m_feEmergency.SetAlpha( 0, 255 );
	m_feEmergency.SetSize( surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetWidth() + 5,
						   surface.m_Original.rtRect.GetHeight(), surface.m_Original.rtRect.GetHeight() + 5 );

	m_feNewMail.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", "srfEffectNewMail" ) );
	m_feNewMail.SetTime( 0.0f, 0.5f );
	m_feNewMail.SetAlpha( 0, 150 );
	
	m_feMailFull.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", "srfEffectMailFull" ) );
	m_feMailFull.SetTime( 0.0f, 0.5f );
	m_feMailFull.SetAlpha( 0, 150 );
	
	char acBuffer[256] = "";
	WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_STATUS), -1, acBuffer, 256, NULL, NULL);
	m_pbtnPost->SetToolTip(acBuffer);

	// Event link
	LinkMsg(g_EventSobInfoUpdate, 0);
	LinkMsg(g_EventSobTargetInfoUpdate, 0 );
	LinkMsg(g_EventTargetStatusRefresh, 0 );
	LinkMsg(g_EventPartyUpdateValue, 0);
	LinkMsg(g_EventUpdateRP);
    LinkMsg(g_EventUpdateRpStock);
	LinkMsg(g_EventMailSystemNfy, 0);
	LinkMsg(g_EventSubWeaponActive, (const char*)0, 0x7000 );
	LinkMsg(g_EventSubWeaponDeActive, (const char*)0, 0x7000 );
	LinkMsg(g_EventSobEquipChange, (const char*)0, 0x7000);
	LinkMsg(g_EventGameServerChangeOut);
	LinkMsg(g_EventRPCharge);
	LinkMsg(g_EventSobUpdateLPStatusNfy);

	// Update Link
	GetNtlGuiManager()->AddUpdateBeforeFunc( this );
	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN(TRUE);
}

void CHpGui::Destroy(void)
{
	NTL_FUNCTION("CHpGui::Destroy");

	// Buff
	if( m_pBuff )
	{
		m_pBuff->Destroy();
		NTL_DELETE( m_pBuff );
	}

	Logic_DeleteTexture( m_surCharacter.GetFilterTexture() );
	m_surCharacter.UnsetFilterTexture();
	
	// Event unlink
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventSobTargetInfoUpdate);
	UnLinkMsg(g_EventTargetStatusRefresh);
	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventUpdateRP);	
    UnLinkMsg(g_EventUpdateRpStock);
	UnLinkMsg(g_EventMailSystemNfy);
	UnLinkMsg(g_EventSubWeaponActive);
	UnLinkMsg(g_EventSubWeaponDeActive);
	UnLinkMsg(g_EventSobEquipChange);
	UnLinkMsg(g_EventBlockingNfy);
	UnLinkMsg(g_EventGameServerChangeOut);
	UnLinkMsg(g_EventRPCharge);
	UnLinkMsg(g_EventSobUpdateLPStatusNfy);
	
	// Update Unlink
	GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CHpGui::Update( RwReal fElapsed )
{
	if( m_eRPChargeType != TYPE_NONE )
	{
		m_fCurrentRPChargingTime += fElapsed;
		
		if( m_fCurrentRPChargingTime >= m_fRPChargingTime )
		{
			m_fCurrentRPChargingTime = m_fRPChargingTime;
			
			if( m_eRPChargeType == TYPE_DECREASE_END )
				m_eRPChargeType = TYPE_NONE;
		}

		RwReal fValue =	gui::GetResultLineInterpolation( m_fCurrentRPChargingTime, m_fRPChargingTime, m_fCurrentValue, m_fDestValue );
		SetRPValue( (RwInt32)fValue, m_nMaxValue );
		Logic_SetRp( GetNtlSLGlobal()->GetSobAvatar(), (RwInt32)fValue );		

		//CHAR buf[256];
		//sprintf_s( buf, 256, "value : %f, charging : %d, CurrentTime : %f, ChargingTime : %f", fValue, m_eRPChargeType, m_fCurrentRPChargingTime, m_fRPChargingTime );		
		//NTL_WARNING( 0, buf );
	}	

	m_LPGaugeAnimation.Update( fElapsed, m_ppgbHp );
	m_EPGagueAnimation.Update( fElapsed, m_ppgbEp );
	
	m_pBuff->Update( fElapsed );
	
	m_feEmergency.Update( fElapsed );
	m_feNewMail.Update( fElapsed );
	m_feMailFull.Update( fElapsed );
}

void CHpGui::UpdateBeforeCamera(RwReal fElapsed)
{
	if( !IsShow() )
		return;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar == NULL)
		return;

	CNtlSobCharProxy *pCharProxy = reinterpret_cast<CNtlSobCharProxy*>(pSobAvatar->GetSobProxy());
	m_texCharacter.Load(pCharProxy->UIPcStatusBarRender());
	m_surCharacter.SetTexture( &m_texCharacter );
}

RwInt32 CHpGui::SwitchDialog(bool bOpen)
{
	//if( bOpen )
	//{
	//	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_RANK_BATTLE);
	//	if( pWorldConcept )
	//	{
	//		if( pWorldConcept->GetState() != WORLD_STATE_NONE &&
	//			pWorldConcept->GetState() != WORLD_STATE_EXIT )
	//			return -1;
	//	}
	//}
	
	Show( bOpen );
	m_pBuff->Show( bOpen );
	
	
	return 1;
}

RwBool CHpGui::IsMouseOnAvatar(VOID)
{
	return ( m_pbtnAvatar->IsFocused() ) ? TRUE : FALSE;
}

void CHpGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CHpGui::HandleEvents");

	if(pMsg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(pMsg.pData); 
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if( pSobAvatar->GetSerialID() != pUpdate->hSerialId )
			NTL_RETURNVOID();

		if(pUpdate->uiUpdateType & EVENT_AIUT_ATTR)
		{
			RwUInt32 uiHp = Logic_GetLp(pSobAvatar);
			RwUInt32 uiMaxHp = Logic_GetMaxLp(pSobAvatar);
			RwUInt32 uiEp = Logic_GetEp(pSobAvatar);
			RwUInt32 uiMaxEp = Logic_GetMaxEp(pSobAvatar);
			RwUInt8 byLevel = Logic_GetLevel(pSobAvatar);

			m_sttLevel->SetText(byLevel); 
			m_ppgbHp->SetRange(0, uiMaxHp);
			m_LPGaugeAnimation.SetValue( uiHp, uiMaxHp, m_ppgbHp );
			m_ppgbEp->SetRange( 0, uiMaxEp );
			m_EPGagueAnimation.SetValue( uiEp, uiMaxEp, m_ppgbEp );
			m_sttHp->Format("%u / %u",uiHp,uiMaxHp);
			m_sttMp->Format("%u / %u",uiEp,uiMaxEp);

			CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );
			m_numPower.SetNumber( pAttr->GetAP() );

			// Attribute를 다시 계산한다.
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}

		if( pUpdate->uiUpdateType & EVENT_AIUT_BUFF )			
		{
			m_pBuff->SetBuffAll( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
		}				

		// Avatar가 초기 생성된 후 GUI에 데이터 입력하라는 최초 단일의 시그널이 EVENT_AIUT_SKILL.
		if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL )
		{
			CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
			SetMaxRPBall( pAttr->GetMaxRpStock() );
		}

		// 실신 상태 일때는 모든 RP, RPBall은 수동으로 리셋한다.
		if( pUpdate->uiUpdateType & EVENT_AIUT_DIE )
		{
			SetRPValue( 0, m_nMaxValue );
			m_ppgbRpBall->SetPos( 0 );

			Logic_SetRp( pSobAvatar, 0, m_nMaxValue );
			Logic_SetRpStock( pSobAvatar, 0 );
			m_eRPChargeType = TYPE_NONE;
		}

		//if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL ||
		//	pUpdate->uiUpdateType & EVENT_AIUT_SKILL_LEARN )
		//{
		//	if( Logic_IsPassiveBlockingUsePossible() )
		//	{
		//		m_ppnlGuardMark->Show( true );
		//		m_surGuardGauge.Show( TRUE );				
		//	}
		//}
	}
	else if( pMsg.Id == g_EventSobUpdateLPStatusNfy )
	{
		SNtlEventSobUpdateLPStatusNfy* pData = reinterpret_cast<SNtlEventSobUpdateLPStatusNfy*>( pMsg.pData );

		if( pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			if( pData->bEmergency )
			{
				if( !m_feEmergency.IsWork() )
				{
					m_feEmergency.StartProc( TRUE );

					m_hSound = Logic_PlayGUISound(GSD_HEART_BEAT);
				}
			}
			else
			{
				if( m_feEmergency.IsWork() )
				{
					m_feEmergency.EndProc();
					GetSoundManager()->Stop( m_hSound );
				}
			}
		}		
	}
	else if(pMsg.Id == g_EventSubWeaponActive )
	{
		SNtlEventSubWeaponActive* pActiveData = reinterpret_cast<SNtlEventSubWeaponActive*>( pMsg.pData );

		if( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pActiveData->uiSerialId ||
			GetDboGlobal()->GetTargetSerial() == pActiveData->uiSerialId )
		{
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
	else if(pMsg.Id == g_EventSubWeaponDeActive )
	{
		SNtlEventSubWeaponDeActive* pActiveData = reinterpret_cast<SNtlEventSubWeaponDeActive*>( pMsg.pData );

		if( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pActiveData->uiSerialId ||
			GetDboGlobal()->GetTargetSerial() == pActiveData->uiSerialId )
		{
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
	else if(pMsg.Id == g_EventSobEquipChange )
	{
		SNtlEventSobEquipChange* pEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( pMsg.pData );

		if( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pEquipChange->hSerialId ||
			GetDboGlobal()->GetTargetSerial() == pEquipChange->hSerialId )
		{
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
	else if(pMsg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pPacket = reinterpret_cast<SNtlEventPartyUpdate*>(pMsg.pData);

		switch(pPacket->iMessage)
		{ 
		case PMT_PARTY_LEADER_CHANGE:
			{
				if( pPacket->hSerialId == Logic_GetAvatarHandle() )
					m_psttNickName->SetColor(255, 255, 0);
				else
					m_psttNickName->SetColor(255, 255, 255);

				break;
			}
		}
	}
	else if( pMsg.Id == g_EventRPCharge )
	{
		SDboEventRPCharge* pData = reinterpret_cast<SDboEventRPCharge*>( pMsg.pData );
		m_fCurrentRPChargingTime = 0.0f;

		CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );

		if( pData->bStop )
		{
			m_eRPChargeType = TYPE_NONE;
		}
		else
		{
			m_fCurrentValue = (RwReal)pAttr->GetRp();

			if( pData->bIncrease )
			{
				m_fDestValue = (RwReal)pAttr->GetMaxRp();
				m_fRPChargingTime = ( m_fDestValue - m_fCurrentValue ) / ( pAttr->m_wLastRpRegen / ( DBO_RP_CHARGE_INTERVAL_IN_MILLISECS * 0.001f ) );
				m_eRPChargeType = TYPE_INCREASE;
			}
			else
			{
				m_fDestValue = 0.0f;
				m_fRPChargingTime = ( m_fCurrentValue - m_fDestValue ) / ( pAttr->m_wLastRpDimimutionRate / ( DBO_RP_DIMIMUTION_INTERVAL_IN_MILLISECS * 0.001f ) );
				m_eRPChargeType = TYPE_DECREASE;
			}			
		}

		//CHAR buf[256];
		//sprintf_s( buf, 256, "RPCharge bIncrease : %d, bStop : %d, lastRPRegen : %u, lastDimimutionRate : %u, currentRP = %d", pData->bIncrease, pData->bStop, pAttr->m_wLastRpRegen, pAttr->m_wLastRpDimimutionRate, pAttr->GetRp() );
		//NTL_WARNING( 0, buf );
	}
	else if(pMsg.Id == g_EventUpdateRP)
	{
		SDboEventUpdateRP* pData = reinterpret_cast<SDboEventUpdateRP*>( pMsg.pData );
		CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );

		if( pData->hSerial == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			SetRPValue( pData->nRP, pData->nMaxRP );			

			if( m_eRPChargeType == TYPE_INCREASE )
			{
				m_fRPChargingTime = ( m_fDestValue - pData->nRP ) / ( pAttr->m_wLastRpRegen / ( DBO_RP_CHARGE_INTERVAL_IN_MILLISECS * 0.001f ) );
			}
			else if( m_eRPChargeType == TYPE_DECREASE ||
					 m_eRPChargeType == TYPE_DECREASE_END )
			{
				m_eRPChargeType = TYPE_NONE;
			}
			
			//CHAR buf[256];
			//sprintf_s( buf, 256, "UpdateRP. Dest : %d", pData->nRP );
			//NTL_WARNING( 0, buf );		
		}		
	}
    else if(pMsg.Id == g_EventUpdateRpStock)
    {
        SDboEventUpdateRpStock* pData = reinterpret_cast<SDboEventUpdateRpStock*>(pMsg.pData);

        if(pData->hSerial == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
        {
            CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
			RwInt32 nMaxRP = pAttr->GetMaxRp(); 
            
			// RP가 증가한 경우, RP는 0으로 바로 떨어짐 
			if( pData->nRpStock > pAttr->GetRpStock() )
			{
				if( pData->nRpStock < pAttr->GetMaxRpStock() )
				{				
					SetRPValue( 0, nMaxRP );
					Logic_SetRp( GetNtlSLGlobal()->GetSobAvatar(), 0 );					

					m_fCurrentRPChargingTime = 0.0f;
					m_fCurrentValue = 0.0f;
					m_fRPChargingTime = m_fDestValue / ( pAttr->m_wLastRpRegen / ( DBO_RP_CHARGE_INTERVAL_IN_MILLISECS * 0.001f ) );

					//RwChar buf[256];
					//sprintf_s( buf, 256, "UpdateRPBallIncrease : %d", pData->nRpStock );
					//NTL_WARNING( 0, buf );
				}    
				else
				{
					SetRPValue( 0, nMaxRP );
					Logic_SetRp( GetNtlSLGlobal()->GetSobAvatar(), 0 );

					//RwChar buf[256];
					//sprintf_s( buf, 256, "Reached MaxRPBall : %d", pData->nRpStock );
					//NTL_WARNING( 0, buf );

					m_eRPChargeType = TYPE_NONE;
				}

				Logic_PlayGUISound(GSD_RP_FULL);
			}	
			else if( pData->nRpStock < pAttr->GetRpStock() )
			{
				if( pData->bDropByTime )
				{
					SetRPValue( nMaxRP, nMaxRP );
					Logic_SetRp( GetNtlSLGlobal()->GetSobAvatar(), nMaxRP );

					m_fCurrentRPChargingTime = 0.0f;
					m_fCurrentValue = (RwReal)nMaxRP;
					m_fRPChargingTime = (RwReal)nMaxRP / ( pAttr->m_wLastRpDimimutionRate / ( DBO_RP_DIMIMUTION_INTERVAL_IN_MILLISECS * 0.001f ) );;					

					if( pData->nRpStock == 0 )
						m_eRPChargeType = TYPE_DECREASE_END;

					//RwChar buf[256];
					//sprintf_s( buf, 256, "UpdateRPBallDecrease : %d, State : %d", pData->nRpStock, m_eRPChargeType );
					//NTL_WARNING( 0, buf );
				}								
			}

			if( pData->nRpStock < pAttr->GetMaxRpStock() )
			{
				// RP Gauge이미지 변경
				RwChar buf[256];
				sprintf_s( buf, 256, "srfNewRPGauge%d", pData->nRpStock );
				m_ppgbRp->ClearSurface();
				m_ppgbRp->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", buf ) );
				m_ppgbRp->SetPos( m_ppgbRp->GetPos() );				
			}			

			m_ppgbRpBall->SetPos( pData->nRpStock );
			//RwChar buf[256];
			//sprintf_s( buf, 256, "UpdateRPBall : %d, State : %d", pData->nRpStock, m_eRPChargeType );
			//NTL_WARNING( 0, buf );


			// RpBall Max 갯수가 바뀌면 적용.
			if( pData->nMaxRpStock != pAttr->GetMaxRpStock() )
			{
				SetMaxRPBall( pData->nMaxRpStock );				
			}
		}		
    }
	else if(pMsg.Id == g_EventMailSystemNfy)
	{
		SNtlEventMailSystemNfy* pEvent = reinterpret_cast<SNtlEventMailSystemNfy*>( pMsg.pData );

		// Hp Gui 에서는 항상 발생하는 타입을 받아서 처리한다.
		if( pEvent->uiNotifyType != CNtlMailSystem::MAILSYSTEM_NOTIFY_TYPE_NONE )
			return;

		char			acBuffer[1024];
		WCHAR			awcBuffer[1024];

		swprintf_s(awcBuffer, 1024, L"%s", GetDisplayStringManager()->GetString(DST_MAILSYSTEM_STATUS));
		CNtlMailSystem* pMailSystem			= GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
		RwUInt32		uiUnreadMailCount	= pMailSystem->GetUnreadMailCountNormal() + pMailSystem->GetUnreadMailCountManager();
		RwBool			bMailFull			= FALSE;
		RwBool			bUnreadMail			= FALSE;
		
		if (uiUnreadMailCount > 0)
		{
			WCHAR awcTemp[1024];
			swprintf_s(awcTemp, 1024, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_STATUS_UNREAD), uiUnreadMailCount);
			swprintf_s(awcBuffer, 1024, L"%s\\n%s", awcBuffer, awcTemp);

			bUnreadMail = TRUE;
		}

		if (pMailSystem->GetMailCount() == NTL_MAX_MAIL_SLOT_COUNT)
		{
			swprintf_s(awcBuffer, 1024, L"%s\\n%s", awcBuffer, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_STATUS_FULL));

			bMailFull = TRUE;
		}

		SetEffectRunable(m_feNewMail, !bMailFull && bUnreadMail);
		SetEffectRunable(m_feMailFull, bMailFull);

		WideCharToMultiByte(GetACP(), 0, awcBuffer, -1, acBuffer, 1024, NULL, NULL);
		m_pbtnPost->SetToolTip(acBuffer);

		/*if (pEvent->uiNotifyValue == CNtlMailSystem::MAILSYSTEM_NOTIFY_VALUE_NONE)
		{
			SetEffectRunable(m_feMailFull, FALSE);
			SetEffectRunable(m_feNewMail, FALSE);
		}
		else if (pEvent->uiNotifyValue & CNtlMailSystem::MAILSYSTEM_NOTIFY_VALUE_MAIL_FULL)
		{
			SetEffectRunable(m_feMailFull, TRUE);
			SetEffectRunable(m_feNewMail, FALSE);
		}
		else if (pEvent->uiNotifyValue & CNtlMailSystem::MAILSYSTEM_NOTIFY_VALUE_MAIL_NORMAL ||
				 pEvent->uiNotifyValue & CNtlMailSystem::MAILSYSTEM_NOTIFY_VALUE_MAIL_MANAGER)
		{
			SetEffectRunable(m_feMailFull, FALSE);
			SetEffectRunable(m_feNewMail, TRUE);
		}*/
	}
	else if( pMsg.Id == g_EventGameServerChangeOut )
	{
		m_pBuff->ClearBuff();
	}
	
	NTL_RETURNVOID();
}

VOID CHpGui::SetEffectRunable(gui::CFlickerEffect& flickerEffect, RwBool bRun)
{
	if (bRun)
	{
		if( !flickerEffect.IsWork() )
			flickerEffect.StartProc( TRUE );
	}
	else
	{
		if( flickerEffect.IsWork() )
			flickerEffect.EndProc();
	}
}

VOID CHpGui::SetRPValue( RwInt32 nValue, RwInt32 nMaxValue )
{
	m_ppgbRp->SetRange( 0, nMaxValue );
	m_ppgbRp->SetPos( nValue );	
}

VOID CHpGui::SetMaxRPBall( RwInt32 nMaxRPBall )
{
	RwChar buf[256];
	sprintf_s( buf, "srfRPBallBack%d", nMaxRPBall );

	m_ppnlRpBallBack->GetSurface()->clear();
	m_ppnlRpBallBack->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", buf ) );
}

VOID CHpGui::CalcBattleAttribute(VOID)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );
	CNtlSobCharProxy* pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobAvatar->GetSobProxy() );

	// Battle Attributes는 서브웨폰을 사용중이라면 서브웨폰의 속성으로 사용하고 사용중이지 않다면
	// 메인웨폰의 속성으로 사용한다.
	RwUInt8 byWeaponAttr = INVALID_BYTE;
	if( pSobCharProxy->GetActiveSubWeapon() )
	{
		byWeaponAttr = pSobAvatarAttr->GetSubWeaponAttr();
		SetBattleAttribute( TYPE_WEAPON, pSobAvatarAttr->GetSubWeaponAttr() );		
	}
	else
	{// 
		byWeaponAttr = pSobAvatarAttr->GetMainWeaponAttr();
		SetBattleAttribute( TYPE_WEAPON, pSobAvatarAttr->GetMainWeaponAttr() );
	}

	SetBattleAttribute( TYPE_ARMOR, pSobAvatarAttr->GetArmorWeaponAttr() );

	// SourceAttr
	SetSourceAttr( byWeaponAttr, pSobAvatarAttr->GetArmorWeaponAttr() );
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

	// Target은 계산하지 않는다.
	SetTargetAttr( INVALID_BYTE, INVALID_BYTE );
}

VOID CHpGui::SetBattleAttribute( TYPE_BATTLEATTR eType, RwUInt8 byBattleAttribute )
{ 
	Logic_SetBattleAttributeMark( m_ppnlBattleAttribute[eType], byBattleAttribute );	
}

VOID CHpGui::OnPaint(VOID)
{
	m_surCharacter.Render();
}

VOID CHpGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_surCharacter.SetRectWH( rect.left + 10, rect.top + 7, 81, 81 );

	m_pBuff->SetMove( rect );

	RwInt32 nCenterX, nCenterY;
	gui::CSurface surface = m_feEmergency.GetSurface();
	surface.m_Original.rtRect.GetCenterPosition( &nCenterX, &nCenterY );
	m_feEmergency.SetCenterPos( rect.left + nCenterX, rect.top + nCenterY );

	rect		= m_pbtnPost->GetScreenRect();
	nCenterX	= rect.left + ((rect.right - rect.left) / 2);
	nCenterY	= rect.top + ((rect.bottom - rect.top) / 2);
	m_feNewMail.SetCenterPos(nCenterX, nCenterY);
	m_feMailFull.SetCenterPos(nCenterX, nCenterY);
}

VOID CHpGui::OnPaintLast(VOID)
{
	m_feEmergency.Render();
}

VOID CHpGui::OnClickPost( gui::CComponent* pComponent )
{
	GetDboGlobal()->GetGamePacketGenerator()->SendMailStartReq(INVALID_SERIAL_ID);
}

VOID CHpGui::OnPaintPost()
{
	m_feNewMail.Render();
	m_feMailFull.Render();
}

VOID CHpGui::OnPowerPaint(VOID)
{
	m_numPower.Render();
}

VOID CHpGui::OnPowerMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtScreen = m_ppnlPowerText->GetScreenRect();
	m_numPower.SetPosition( rtScreen.left, rtScreen.top );
}

VOID CHpGui::OnAvatarFocused( gui::CComponent* pComponent, bool bFocused )
{
	if( bFocused )
		GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_BATTLE_TARGET );
	else
		GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_INVALID_CURSOR );	
}

VOID CHpGui::OnAvatarClick( gui::CComponent* pComponent )
{
	Logic_AvatarTarget();
}

/**
* \brief 무기 전투 속성 패널에 마우스가 들어왔을 때 InfoWindow를 띄워준다.
* \param pComponent	(gui::CComponent*) Mouse enter를 받은 Component
*/
VOID CHpGui::OnMouseWeaponAttrEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();
	
	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_WEAPON_INFO;

	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
		rtScreen.left, rtScreen.top,
		reinterpret_cast<void*>( &m_sBattleAttr ),
		DIALOG_HP );
}

/**
* \brief 무기 전투 속성 패널에서 마우스가 나갔을 때 관련된 InfoWindow를 닫아준다.
* \param pComponent	(gui::CComponent*) Mouse leave를 받은 Component
*/
VOID CHpGui::OnMouseWeaponAttrLeave( gui::CComponent* pComponent )
{
	if( DIALOG_HP == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

/**
* \brief 방어구 전투 속성 패널에 마우스가 들어왔을 때 InfoWindow를 띄워준다.
* \param pComponent	(gui::CComponent*) Mouse enter를 받은 Component
*/
VOID CHpGui::OnMouseArmorAttrEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();

	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_ARMOR_INFO;

	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
		rtScreen.left, rtScreen.top,
		reinterpret_cast<void*>( &m_sBattleAttr ),
		DIALOG_HP );
}

/**
* \brief 방어구 전투 속성 패널에서 마우스가 나갔을 때 관련된 InfoWindow를 닫아준다.
* \param pComponent	(gui::CComponent*) Mouse leave를 받은 Component
*/
VOID CHpGui::OnMouseArmorAttrLeave( gui::CComponent* pComponent )
{
	if( DIALOG_HP == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

/**
* \brief 자신의 무기와 방어구 전투 속성을 세팅한다.
* \param bySourceWeaponAttr	(RwUInt8) 무기의 전투 속성
* \param bySourceArmorAttr	(RwUInt8) 방어구의 전투 속성
*/
VOID CHpGui::SetSourceAttr( RwUInt8 bySourceWeaponAttr, RwUInt8 bySourceArmorAttr )
{
	m_sBattleAttr.bySourceWeaponAttr = bySourceWeaponAttr;
	m_sBattleAttr.bySourceArmorAttr = bySourceArmorAttr;

}

/**
* \brief 타겟의 무기와 방어구 전투 속성을 세팅한다.
* \param byTargetWeaponAttr	(RwUInt8) 무기의 전투 속성
* \param byTargetArmorAttr	(RwUInt8) 방어구의 전투 속성
*/
VOID CHpGui::SetTargetAttr( RwUInt8 byTargetWeaponAttr, RwUInt8 byTargetArmorAttr )
{
	m_sBattleAttr.byTargetWeaponAttr = byTargetWeaponAttr;
	m_sBattleAttr.byTargetArmorAttr = byTargetArmorAttr;
}

/**
* \brief InfoWindow가 띄워져 있을 때 정보가 변경된다면 InfoWindow를 다시 띄워준다.
*/
VOID CHpGui::OnBattleAttributeRefresh()
{
	// 현재 마우스의 위치가 무기 전투 속성 패널에 있는지 방어구 전투 속성 패널에 있는지 판단
	TYPE_BATTLEATTR eType = TYPECOUNT;
	for( RwInt32 i = 0; i< TYPECOUNT; ++i )
	{
		if( m_ppnlBattleAttribute[i]->GetScreenRect().PtInRect( CMouse::GetX(), CMouse::GetY() ) )
			eType = (TYPE_BATTLEATTR)i;
	}

	// 마우스의 두 개의 패널이 아니라면 InfoWindow를 갱신할 필요가 없다.
	if( eType == TYPECOUNT )
		return;

	else if( eType == TYPE_WEAPON )
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_WEAPON_INFO;
	else
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_ARMOR_INFO;

	// 현재 HpGui에서 요청한 InfoWindow인지 확인하고 전투 속성 관련 InfoWindow를 띄우는 것까지 확인
	// 하여 맞다면 현재 띄워진 InfoWindow를 닫고 새로 띄워준다.
	if( DIALOG_HP == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );

		CRectangle rtScreen = m_ppnlBattleAttribute[eType]->GetScreenRect();
	
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
			rtScreen.left, rtScreen.top,
			reinterpret_cast<void*>( &m_sBattleAttr ),
			DIALOG_HP );
	}
}

