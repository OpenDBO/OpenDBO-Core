#include "precomp_dboclient.h"
#include "MsgBoxManager.h"

// Shared
#include "NtlTeleport.h"
#include "NPCTable.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"
#include "InputActionMap.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLApi.h"

#include "NtlSobItemAttr.h"

// Dbo
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "MsgBoxGui.h"
#include "LogInStageState.h"
#include "GUISoundDefine.h"
#include "CommunityGui.h"
#include "ChatGui.h"
#include "ChannelChangeGui.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "CharStageState.h"
#include "LobbyManager.h"

#include "CommercialExtendGui.h"

#define MSGBOXMGR_DEFAULT_ALPHA			255
#define MSGBOXMGR_BEHIND_POPUP_ALPHA	100

CMsgBoxManager* CMsgBoxManager::m_pInstance = NULL;

CMsgBoxManager::CMsgBoxManager(VOID)
{
}

CMsgBoxManager::~CMsgBoxManager(VOID)
{
}

RwBool CMsgBoxManager::CreateInstance(VOID)
{
	NTL_FUNCTION( "CMsgBoxManager::CreateInstance" );

	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CMsgBoxManager;
		NTL_ASSERT( m_pInstance, "CMsgBoxManager::CreateInstance : Instance Allocate Fail" );
		m_pInstance->LinkEvent();
	}

	NTL_RETURN( TRUE );
}

VOID CMsgBoxManager::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		m_pInstance->UnLinkEvent();
		m_pInstance->DeleteAllMsgBox();
		NTL_DELETE( m_pInstance );
	}
}

CMsgBoxManager* CMsgBoxManager::GetInstance(VOID)
{
	return m_pInstance;
}

VOID CMsgBoxManager::DeleteMsgBox( EMsgBoxWork eWorkID )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxWorkID( eWorkID ) )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listMsgBox.erase( iter );			
		}
		else
			++iter;
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxWorkID( eWorkID ) )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );			

			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}			
		}
		else
			++iter;
	}
}

VOID CMsgBoxManager::DeleteMsgBox( RwInt32 eStringID )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID( eStringID ) )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listMsgBox.erase( iter );			
		}
		else
			++iter;
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID( eStringID ) )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );			

			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}			
		}
		else
			++iter;
	}
}

VOID CMsgBoxManager::DeleteAllMsgBoxExceptNetConnectBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( !pMsgBox->IsNetConnectBox() )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listMsgBox.erase( iter );			
		}
		else
			++iter;
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( !pMsgBox->IsNetConnectBox() )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );			

			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}			
		}
		else
			++iter;
	}	
}

VOID CMsgBoxManager::DeleteNetConnectBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsNetConnectBox() )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listMsgBox.erase( iter );			
		}
		else
			++iter;
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsNetConnectBox() )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );			

			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}			
		}
		else
			++iter;
	}	
}

CMsgBoxGui* CMsgBoxManager::GetMsgBox( EMsgBoxWork eWorkID, LIST_MSGBOX* pList /* = NULL  */ )
{
	CMsgBoxGui* pSearchMsgBox = NULL;
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxWorkID( eWorkID ) )
		{
			pSearchMsgBox = pMsgBox;
			if( pList )
				pList->push_back( pMsgBox );
		}		
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxWorkID( eWorkID ) )
		{
			pSearchMsgBox = pMsgBox;
			if( pList )
				pList->push_back( pMsgBox );
		}		
	}

	return pSearchMsgBox;
}

CMsgBoxGui* CMsgBoxManager::GetMsgBox( RwInt32 eStringID, LIST_MSGBOX* pList /* = NULL  */ ) 
{
	CMsgBoxGui* pSearchMsgBox = NULL;
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID( eStringID ) )
		{
			pSearchMsgBox = pMsgBox;
			if( pList )
				pList->push_back( pMsgBox );
		}		
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID( eStringID ) )
		{
			pSearchMsgBox = pMsgBox;
			if( pList )
				pList->push_back( pMsgBox );
		}		
	}

	return pSearchMsgBox;
}

VOID CMsgBoxManager::ShowAllMsgBox( bool bShow )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		pMsgBox->Show( bShow );	
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		pMsgBox->Show( bShow );		
	}
}

RwBool CMsgBoxManager::IsShowMsgBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		if( pMsgBox->IsShow() )
			return TRUE;		
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		if( pMsgBox->IsShow() )
			return TRUE;
	}

	return FALSE;
}

RwBool CMsgBoxManager::IsShowPopupMsgBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		if( pMsgBox->IsShow() )
			return TRUE;
	}

	return FALSE;
}

VOID CMsgBoxManager::DeleteMsgBox( CMsgBoxGui* pDeletedMsgBox )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox == pDeletedMsgBox )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			m_listMsgBox.erase( iter );
			return;
		}
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox == pDeletedMsgBox )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );
			
			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}		

			return;
		}
	}
}

CMsgBoxGui* CMsgBoxManager::CreateMsgBox( RwBool bPopup )
{
	CMsgBoxGui* pMsgBox = NTL_NEW CMsgBoxGui( "MsgBoxGui" );
	NTL_ASSERT( pMsgBox, "CMsgBoxManager::CreateInstance : MsgBox Allocate Fail" );
	if( !pMsgBox->Create() )
	{
		pMsgBox->Destroy();
		NTL_DELETE( pMsgBox );
		return NULL;
	}
	GetNtlGuiManager()->AddGui( pMsgBox );

	if( bPopup )
	{
		SetMsgBoxAlpha( MSGBOXMGR_BEHIND_POPUP_ALPHA );

		if( !m_listPopupMsgBox.empty() )
			m_listPopupMsgBox.front()->Popup( false );

		pMsgBox->Popup( true );
		m_listPopupMsgBox.push_front( pMsgBox );
	}
	else
		m_listMsgBox.push_back( pMsgBox );

	return pMsgBox;
}

VOID CMsgBoxManager::DeleteAllMsgBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
		}
	}

	m_listMsgBox.clear();

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
		}
	}

	m_listPopupMsgBox.clear();
}

VOID CMsgBoxManager::MsgBoxShowProc( EMsgBoxTypeFlag flagType, RwBool bPopup, std::list<sMsgBoxCustomBtn>* plistCustomBtn, const WCHAR* szText, RwInt32 eStringID, 
									 EMsgBoxWork eWorkID, sMsgBoxData* pData, RwReal fShowTime, RwBool bDuplicate, RwBool bUpdate, RwBool bNetConnect /* = FALSE  */)
{
	SConnectData* pConnectData = GetDboGlobal()->GetConnectData();
	if( pConnectData->sGameCon.bBanishment )
		return;

	CMsgBoxGui* pMsgBox = GetMsgBox( eStringID );
	RwBool bCreateMsgBox = TRUE;

	if( pMsgBox )
	{
		if( !bDuplicate )
		{
			if( bUpdate )
			{
				pMsgBox->UpdateMsgBox( szText, pData );			
			}		
			else
			{
				CDboEventGenerator::MsgBoxResult( FALSE, eWorkID, pData );	
			}

			bCreateMsgBox = FALSE;
		}	
	}
	
	if( bCreateMsgBox )
	{
		pMsgBox = CreateMsgBox( bPopup );

		pMsgBox->SetBoxData( flagType, bPopup, plistCustomBtn, szText, eStringID, eWorkID, pData, fShowTime, bNetConnect );		

		Logic_PlayGUISound(GSD_SYSTEM_MESSAGEBOX);
	}
}

VOID CMsgBoxManager::SetMsgBoxAlpha( RwUInt8 byAlpha )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
			pMsgBox->SetAlpha( byAlpha );		
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
			pMsgBox->SetAlpha( byAlpha );		
	}
}

VOID CMsgBoxManager::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventMsgBoxShow )
	{
		SDboEventMsgBoxShow* pMsgBoxShow = reinterpret_cast<SDboEventMsgBoxShow*>( msg.pData ); 

		MsgBoxShowProc( (EMsgBoxTypeFlag)pMsgBoxShow->flagStyle, pMsgBoxShow->bPopup, pMsgBoxShow->plistCustomBtn, pMsgBoxShow->wstrString.c_str(), pMsgBoxShow->eStringID, 
						(EMsgBoxWork)pMsgBoxShow->eWorkId, pMsgBoxShow->pData, pMsgBoxShow->fShowTime, pMsgBoxShow->bAcceptDuplicate, pMsgBoxShow->bUpdate, FALSE );
	}
	else if( msg.Id ==g_EventConnectBoxShow )
	{
		SDboEventMsgBoxShow* pMsgBoxShow = reinterpret_cast<SDboEventMsgBoxShow*>(msg.pData);		

		MsgBoxShowProc( (EMsgBoxTypeFlag)pMsgBoxShow->flagStyle, pMsgBoxShow->bPopup, pMsgBoxShow->plistCustomBtn, pMsgBoxShow->wstrString.c_str(),	pMsgBoxShow->eStringID,
						(EMsgBoxWork)pMsgBoxShow->eWorkId, pMsgBoxShow->pData, pMsgBoxShow->fShowTime, pMsgBoxShow->bAcceptDuplicate, pMsgBoxShow->bUpdate, TRUE );
	}
	else if( msg.Id == g_EventMsgBoxResult )
	{
		SDboEventMsgBoxResult *pMsgBoxResult = reinterpret_cast<SDboEventMsgBoxResult*>( msg.pData );
		switch( pMsgBoxResult->uiWorkId )
		{
		case MBW_NET_CONNECT_FAIL:
		case MBW_NET_DISCONNECT:
		case MBW_GAME_EXIT:
		case MBW_LOGIN_CONNECT_FAIL:
		case MBW_AUTHKEY_FAIL:
		case MBW_CHARACTRE_CONNECT_FAIL:
		case MBW_POST_QUIT_MESSAGE:			
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					PostQuitMessage(0);				
			}	
			break;
		case MBW_CAHR_SERVER_EXIT:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					GetNtlWorldConcept()->EnableAction(FALSE);
					//GetNtlGuiManager()->GetGuiManager()->DisableInput();
					CNtlPLEventGenerator::CinematicFadeOut(CINEMATIC_FADEINOUT_SCREEN_PANEL, 0.5f, 1, 1, 1, 220);
					GetDboGlobal()->GetGamePacketGenerator()->SendCharServerEnterReq();                    
				}                
			}
			break;
		case MBW_REGEN_MB_MAINWORLD_MOVE_TO_POPOSTONE:				
		case MBW_REGEN_MB_TLQ_MOVE_TO_MAINWORLD:
		case MBW_REGEN_MB_TMQ_MOVE_TO_MAINWORLD:
		case MBW_REGEN_MB_PARTYDUNGEON_RETRY:
		case MBW_REGEN_MB_DOJO_RETRY:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetGamePacketGenerator()->SendCharRevivalReq( DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE );							
			}
			break;
		case MBW_REGEN_MB_TLQ_RETRY:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetGamePacketGenerator()->SendCharRevivalReq( DBO_REVIVAL_REQUEST_TYPE_CURRENT_POSITION );							
			}
			break;				
		case MBW_PARTY_INVITE_REQ:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyResponseInvite(NTL_INVITATION_RESPONSE_ACCEPT);
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyResponseInvite(NTL_INVITATION_RESPONSE_DECLINE);
			}
			break;
		case MBW_BUY_WAREHOUSE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					// 창고 NPC Serial
					SERIAL_HANDLE hNPCHandle = pMsgBoxResult->pData->hHandle;

					// NPC 테이블의 Merchant Tabe 첫번째 인덱스에 Merchant Table 인덱스가 있다
					GetDboGlobal()->GetGamePacketGenerator()->SendBankBuy(hNPCHandle, 0, pMsgBoxResult->pData->byIndex);					

					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );					
				}				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );					
				}				
			}
			break;
		//case MBW_BUY_WAREHOUSE_3:
		//	{
		//		// 창고 NPC Serial
		//		SERIAL_HANDLE hNPCHandle = pMsgBoxResult->pData->hHandle;

		//		// NPC 테이블의 Merchant Tabe 첫번째 인덱스에 Merchant Table 인덱스가 있다
		//		GetDboGlobal()->GetGamePacketGenerator()->SendBankBuy(hNPCHandle, 0, 2);

		//		CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
		//		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

		//		break;
		//	}
		case MBW_BUY_COMMON_WAREHOUSE:
			{	
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					// 창고 NPC Serial
					SERIAL_HANDLE hNPCHandle = pMsgBoxResult->pData->hHandle;

					// NPC 테이블의 Merchant Tabe 첫번째 인덱스에 Merchant Table 인덱스가 있다
					GetDboGlobal()->GetGamePacketGenerator()->SendBankBuy(hNPCHandle, 0, 3);

					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );					
				}		
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );					
				}
			}
			break;
		case MBW_FREEPVP_CHALLENGE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					if(GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAN_COMMUNICATION) )
						GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_OK);
					else
						Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
				}
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
					GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_DENY);
				else
					GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_CANCEL);			
			}
			break;
		case MBW_PARTY_CREATE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					if( !pMsgBoxResult->pData->pwcText )
						break;

					if( GetChattingFilter()->IsAnyInvalidText(pMsgBoxResult->pData->pwcText) )
					{
						// 사용할 수 없는 단어가 있습니다
						GetAlarmManager()->AlarmMessage(DST_LOBBY_CAN_NOT_USE_THE_WORD);
						break;
					}

					if( NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE < wcslen(pMsgBoxResult->pData->pwcText) )
					{
						GetAlarmManager()->AlarmMessage(DST_PARTY_NAME_RULE);
						break;
					}

					if( FALSE == Logic_CanUseLocalName(pMsgBoxResult->pData->pwcText) )
					{
						GetAlarmManager()->AlarmMessage( DST_LOBBY_ENABLE_LOCAL_TEXT );
						break;
					}

					GetDboGlobal()->GetGamePacketGenerator()->SendPartyCreate(pMsgBoxResult->pData->pwcText);
				}				
			}
			break;
		case MBW_PARTY_LEAVE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyLeave();				
			}
			break;
		case MBW_PARTY_RELEASE_CHARM:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					GetDboGlobal()->GetGamePacketGenerator()->SendPartyDeActivateCharm((RwUInt8)pMsgBoxData->uiParam);
				}				
			}
			break;
		case MBW_ITEMDROP_CONFIRM:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					RwBool bPacketLock = FALSE;

					if( !Logic_ItemDeleteProc( &pMsgBoxResult->pData->sItemDeleteInfo, &bPacketLock ) )
						CDboEventGenerator::EnableItemIcon( TRUE, pMsgBoxResult->pData->sItemDeleteInfo.ePlace, pMsgBoxResult->pData->sItemDeleteInfo.nSlotIdx, pMsgBoxResult->pData->sItemDeleteInfo.nPlaceIdx );				
					else if( bPacketLock )
					{
						LIST_MSGBOX listDropConfirmMsg;
						LIST_MSGBOX_ITER iter;
						GetMsgBox( (EMsgBoxWork)( pMsgBoxResult->uiWorkId ), &listDropConfirmMsg );

						for( iter = listDropConfirmMsg.begin() ; iter != listDropConfirmMsg.end() ; ++iter )
						{
							CMsgBoxGui* pMsgBox = (*iter);
							
							if( pMsgBoxResult->pData->sItemDeleteInfo.hSerial == pMsgBox->GetMsgBoxData()->sItemDeleteInfo.hSerial )
							{
								pMsgBox->DoNotHide();
								break;
							}
						}
					}
				}
				else
					CDboEventGenerator::EnableItemIcon( TRUE, pMsgBoxResult->pData->sItemDeleteInfo.ePlace, pMsgBoxResult->pData->sItemDeleteInfo.nSlotIdx, pMsgBoxResult->pData->sItemDeleteInfo.nPlaceIdx );				
			}
			break;
		case MBW_USER_TRADE_REQ:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(GetDboGlobal()->GetAskedPCSerial(), ACCEPT_RES_TYPE_OK);				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
					GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(GetDboGlobal()->GetAskedPCSerial(), ACCEPT_RES_TYPE_DENY);
				else 
					GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(GetDboGlobal()->GetAskedPCSerial(), ACCEPT_RES_TYPE_CANCEL);
			}
			break;
		case MBW_GUILD_CREATE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

					if( !pwcText )
						break;

					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

					if( GetChattingFilter()->IsAnyInvalidText(pwcText) )
					{
						// 사용할 수 없는 단어가 있습니다
						GetAlarmManager()->AlarmMessage(DST_LOBBY_CAN_NOT_USE_THE_WORD);
						break;
					}

					if( NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE < wcslen(pwcText) )
					{
						GetAlarmManager()->AlarmMessage(DST_GUILD_NAME_RULE);
						break;
					}

					if( FALSE == Logic_CanUseLocalName(pwcText) )
					{
						GetAlarmManager()->AlarmMessage( DST_LOBBY_ENABLE_LOCAL_TEXT );
						break;
					}

					GetDboGlobal()->GetGamePacketGenerator()->SendGuildCreateReq(GetDboGlobal()->GetTargetSerial(), pwcText);		
				}	
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );					
				}
			}
			break;
		case MBW_GUILD_INVITE_REQ:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Response_Invitation(NTL_INVITATION_RESPONSE_ACCEPT);				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Response_Invitation(NTL_INVITATION_RESPONSE_DECLINE);
				// PeessiMsgBoxOpenCancel 
			}
			break;
		case MBW_GUILD_DISSOLVE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Disband_Req();

					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}
			}
			break;
		case MBW_GUILD_DISSOLVE_CANCLE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Disband_Cancle_Req();

					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}
			}
			break;
		case MBW_GUILD_CHANGE_MASTER:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
					CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Change_Guild_Master_Req(charID);
				}				
			}
			break;
		case MBW_GUILD_APPOINT_SECOND_MASTER:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
					CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Appoint_Second_Master_Req(charID);
				}				
			}
			break;
		case MBW_GUILD_DISAPPOINT_SECOND_MASTER:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
					CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_DIsmiss_Second_Master_Req(charID);
				}				
			}
			break;
		case MBW_GUILD_KICK_OUT:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
					CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Kick_Out_Req(charID);
				}				
			}
			break;
		case MBW_GUILD_LEAVE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Leave_Req();				
			}
			break;
		case MBW_GUILD_ADD_FUNCTION:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					GetDboGlobal()->GetGamePacketGenerator()->SendGuildFunctionAddReq(GetDboGlobal()->GetTargetSerial(), pMsgBoxData->byIndex);
				}				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}
			}
			break;
		case MBW_GUILD_CONTRIBUTION:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					GetDboGlobal()->GetGamePacketGenerator()->SendGuildGiveZenny(GetDboGlobal()->GetTargetSerial(), pMsgBoxData->uiParam);
				}				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}
			}
			break;
		case MBW_GUILD_CHANGE_GUILD_NAME:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					if(!pMsgBoxData->pwcText)
						break;

					RwInt32 iLength = wcslen(pMsgBoxData->pwcText);
					if( iLength > NTL_MAX_LENGTH_OF_GUILD_NOTICE_UNICODE )
						break;

					if( GetChattingFilter()->IsAnyInvalidText(pMsgBoxData->pwcText) )
					{
						// 사용할 수 없는 단어가 있습니다
						GetAlarmManager()->AlarmMessage(DST_LOBBY_CAN_NOT_USE_THE_WORD);
						break;
					}

					if( NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE < wcslen(pMsgBoxData->pwcText) )
					{
						GetAlarmManager()->AlarmMessage(DST_GUILD_NAME_RULE);
						break;
					}

					if( FALSE == Logic_CanUseLocalName(pMsgBoxData->pwcText) )
					{
						GetAlarmManager()->AlarmMessage( DST_LOBBY_ENABLE_LOCAL_TEXT );
						break;
					}

					GetDboGlobal()->GetGamePacketGenerator()->SendGuildChangeNameReq(GetDboGlobal()->GetTargetSerial(), pMsgBoxData->pwcText);
				}				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}
			}
			break;
		case MBW_TUTORIAL_ASK_END:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
					GetDboGlobal()->GetGamePacketGenerator()->SendTutorialPlayQuitReq();				
			} 
			break;
        case MBW_GET_ON_BUS_ASK:
            {
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( pMsgBoxData->pData );
					API_GetSLPacketGenerator()->SendRideOnBus(pActor->GetSerialID());
				}				
            }
			break;
		case MBW_GET_OFF_BUS_ASK:
			{	
				if( pMsgBoxResult->eResult == MBR_OK )
					API_GetSLPacketGenerator()->SendRideOffBus();				
			}
			break;
		case MBW_ASK_CHANGE_CHANNEL:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
					CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
					if( !pLobby )
					{
						DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
						return;
					}


					CChannelChangeGui* pGui = reinterpret_cast<CChannelChangeGui*>( GetDialogManager()->GetDialog(DIALOG_CHANNGEL_CHANGE) );

					if( pGui->GetSelectedChannelIndex() == pLobby->GetSelectedChannelIndex() )
					{
						GetAlarmManager()->AlarmMessage(DST_ALREADY_SAME_CHANNEL);
						break;
					}

					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );

					GetDboGlobal()->GetGamePacketGenerator()->SendChannelChangeReq(pMsgBoxData->byIndex);
				}				
			}
			break;
        case MBW_PORTAL_CONFIRM:
            {
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pData = (sMsgBoxData*)pMsgBoxResult->pData;

					// 서버에 포탈 이동을 요청한다.
					GetDboGlobal()->GetGamePacketGenerator()->SendPortalReq(pData->hHandle, pData->byIndex);
				}
            }
            break;
		/*case MBW_HOIPOIMIX_SET_FARE:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pData = (sMsgBoxData*)pMsgBoxResult->pData;

					RwUInt8 bySetFare = (RwUInt8)_wtoi( pData->pwcText );

					if( bySetFare < 50 || bySetFare > 200 )
					{
						GetAlarmManager()->AlarmMessage( DST_HOIPOIMIX_GUIDE_FARERATE_ERROR ); 
						break;
					}

					GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixItemMachineFare(
						pData->hExtendHandle, bySetFare );
				}
			}
			break;*/
		case MBW_VEHICLE_ASK_GET_OFF:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
					API_GetSLPacketGenerator()->SendVehicle_EngineEndReq();				
			}
			break;
		case MBW_ASK_TUTORIAL:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					GetDboGlobal()->SetEnterTutorial( TRUE );
				}
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{					
					GetDboGlobal()->SetEnterTutorial( FALSE );
				}
				
				CDboEventGenerator::LobbyEvent(LMT_GAME_SERVER_WAIT_CHECK_REQ);
			}
			break;
		case MBW_DEL_CHARACTER:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

					if( !pwcText )
						return;

					if( wcslen(pwcText) == 0 )
					{
						GetAlarmManager()->AlarmMessage(DST_LOBBY_EMPTYNAME);
						return;
					}

					// 캐릭터 삭제
					SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
					CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
					if( !pLobby )
					{
						DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
						return;
					}

					sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
					if( !pLOBBY_CHARACTER )
					{
						DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
						return;
					}

					if( wcscmp(pLOBBY_CHARACTER->tSummary.awchName, pwcText) != 0 )
					{
						GetAlarmManager()->AlarmMessage(DST_LOBBY_PLEASE_CHECK_DEL_AND_RECOVERY_DATA);
						return;
					}


					CHARACTERID charID = pLOBBY_CHARACTER->tSummary.charId;

					GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDelReq(charID);
					GetCharStageState()->ChangeState(CHAR_STATE_SELECT_DEL_CHAR);
				}				
			}
			break;
		case MBW_RECOVERY_CHARACTER:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

					if( !pwcText )
						return;

					if( wcslen(pwcText) == 0 )
					{
						GetAlarmManager()->AlarmMessage(DST_LOBBY_EMPTYNAME);
						return;
					}

					// 캐릭터 복구
					SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
					CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
					if( !pLobby )
					{
						DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
						return;
					}

					sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
					if( !pLOBBY_CHARACTER )
					{
						DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
						return;
					}

					if( wcscmp(pLOBBY_CHARACTER->tSummary.awchName, pwcText) != 0 )
					{
						GetAlarmManager()->AlarmMessage(DST_LOBBY_PLEASE_CHECK_DEL_AND_RECOVERY_DATA);
						return;
					}


					CHARACTERID		charID			= pLOBBY_CHARACTER->tSummary.charId;
					SConnectData*	pConnectData	= GetDboGlobal()->GetConnectData();

					if( pConnectData )
					{
						GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDelCancelReq(pConnectData->uiAccountId, charID, pConnectData->byLastServerFarmID);
						GetCharStageState()->ChangeState(CHAR_STATE_SELECT_RECOVERY_CHAR);
					}				
				}				
			}
			break;
		case MBW_LOBBY_WAIT_GAME_SERVER_CONNECT:
			{
				if( MBR_CANCEL == pMsgBoxResult->eResult )
				{
					GetDboGlobal()->GetLobbyPacketGenerator()->SendCharConnectGameServer_WaitCancelReq( GetCharStageState()->GetSelectedChannelIndex() );
					GetCharStageState()->ChangeState(CHAR_STATE_CANCEL_WAIT_GAME_SERVER_CONNECT);				
				}
			}
			break;
		case MBW_WAIT_FOR_TUTORIAL:
			{
				if( MBR_CANCEL == pMsgBoxResult->eResult )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendTutorialWaitCancelReq();
					GetCharStageState()->ChangeState(CHAR_STATE_WAIT_CANCEL);			
				}				
			}
			break;
		case MBW_DOJO_ASK_ESTABLISHMENT:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					SERIAL_HANDLE hNPC = GetDboGlobal()->GetTargetSerial();
					sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(hNPC);
					if( pNPC_TBLDAT )
					{	
						GetDboGlobal()->GetGamePacketGenerator()->SendDojo_CreateReq(hNPC);
					}
					else
					{
						DBO_FAIL("Not exist NPC table of handle : " << hNPC);

						GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
						CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					}					
				}
				else
				{
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
				}
			}
			break;
		case MBW_DOJO_ASK_UPGRAGE_WITH_ZENNY_AND_HOIPOI:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					GetDboGlobal()->GetGamePacketGenerator()->SendDojo_FunctionAddReq(GetDboGlobal()->GetTargetSerial(),
															pMsgBoxData->ucData[0],
															pMsgBoxData->ucData[1],
															pMsgBoxData->ucData[2]);
				}
				else
				{
					CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOJO_UPRAGE, 0);
				}
			}
			break;
		case MBW_DOJO_ADD_FUNCTION:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					GetDboGlobal()->GetGamePacketGenerator()->SendDojo_FunctionAddReq(GetDboGlobal()->GetTargetSerial(),
																					  pMsgBoxData->byIndex,
																					  INVALID_BYTE,
																					  INVALID_BYTE);
				}				
				else if( pMsgBoxResult->eResult == MBR_CANCEL )
				{
					CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
					GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
				}
				break;
			}
		case MBW_DOJO_ASK_UPGRAGE_WITH_ZENNY:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
					GetDboGlobal()->GetGamePacketGenerator()->SendDojo_FunctionAddReq(GetDboGlobal()->GetTargetSerial(),
															pMsgBoxData->ucData[0],
															INVALID_BYTE,
															INVALID_BYTE);
				}
				else
				{
					CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOJO_UPRAGE, 0);
				}
				break;
			}
		case MBW_DOJO_ADD_TENKAI_SEED:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					const WCHAR* pwcName = pMsgBoxResult->pData->pwcText;

					if( !pwcName || wcslen(pwcName) == 0 )
					{
						GetAlarmManager()->AlarmMessage(DST_LOBBY_EMPTYNAME);
						return;
					}

					GetDboGlobal()->GetChatPacketGenerator()->SendDojo_BudokaiSeedAddReq(pwcName);
				}

				break;
			}
		case MBW_DOGI_ASK_APPLY_CHANGE:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					CNtlSobAvatar*	pAvatar	= GetNtlSLGlobal()->GetSobAvatar();
					CNtlGuild*		pGuild	= pAvatar->GetGuild();
					sDBO_DOGI_DATA	tDogiData;

					tDogiData.guildId		= pGuild->GetGuildID();
					tDogiData.byType		= INVALID_BYTE;
					tDogiData.byGuildColor	= pMsgBoxResult->pData->byIndex;
					tDogiData.byDojoColor	= pGuild->GetDojoDogiColorIndex();

					if( pGuild->IsChangedGuildDogiColor() )
					{
						GetDboGlobal()->GetGamePacketGenerator()->SendGuildDogi_ChagneReq(pMsgBoxResult->pData->hHandle, &tDogiData);						
					}
					else
					{
						GetDboGlobal()->GetGamePacketGenerator()->SendGuildDogi_CreateReq(pMsgBoxResult->pData->hHandle, &tDogiData);
					}
				}
				else
				{
					CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOGI, 0);
				}
				break;
			}
		case MBW_DOJO_DOGI_ASK_APPLY_CHANGE:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					CNtlSobAvatar*	pAvatar	= GetNtlSLGlobal()->GetSobAvatar();
					CNtlGuild*		pGuild	= pAvatar->GetGuild();
					sDBO_DOGI_DATA	tDogiData;

					tDogiData.guildId		= pGuild->GetGuildID();
					tDogiData.byType		= INVALID_BYTE;
					tDogiData.byGuildColor	= pGuild->GetGuildDogiColorIndex();
					tDogiData.byDojoColor	= pMsgBoxResult->pData->byIndex;

					if( pGuild->IsChangedDojoDogiColor() )
					{
						GetDboGlobal()->GetGamePacketGenerator()->SendDogi_ChagneReq(pMsgBoxResult->pData->hHandle, &tDogiData);
					}
					else
					{
						GetDboGlobal()->GetGamePacketGenerator()->SendDogi_CreateReq(pMsgBoxResult->pData->hHandle, &tDogiData);
					}					
				}
				else
				{
					CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOGI, 0);
				}
				break;
			}
		case MBW_DOJO_SCRAMBLE_ACCEPT_CHALLENGE_OK:
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_ScrambleResponseReq(true);
				break;
			}
		case MBW_DOJO_SCRAMBLE_ACCEPT_CHALLENGE_CANCEL:
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_ScrambleResponseReq(false);
				break;
			}
		case MBW_DOJO_SCRAMBLE_ASK_CHALLENGE_OK:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					SERIAL_HANDLE hTarget = GetDboGlobal()->GetTargetSerial();
					GetDboGlobal()->GetGamePacketGenerator()->SendDojo_ScrambleReq(hTarget);
				}
				break;
			}
		case MBW_DOJO_SCRAMBLE_ASK_TELEPORT_OK:
			{
				if( MBR_OK == pMsgBoxResult->eResult )
				{
					CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
					if( !pAvatar )
					{
						DBO_FAIL("Not exist avatar instance");
						break;
					}

					CNtlDojo*		pDojo			= pAvatar->GetDojo();
					sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

					if( TELEPORT_TYPE_INVALID == pSCRAMBLE_INFO->byTeleportProposalIndex )
						break;

					GetDboGlobal()->GetGamePacketGenerator()->SendTeleportConfirmReq( true, pSCRAMBLE_INFO->byTeleportProposalIndex );
				}
				break;
			}
		case MBW_HOIPOIMIX_SKILL_SET_MSGBOX:
			{
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixJobSetReq( pMsgBoxResult->pData->hExtendHandle, pMsgBoxResult->pData->byIndex );
				}
			}
			break;
		case MBW_HOIPOIMIX_SKILL_RESET_MSGBOX:
			{
				 if( pMsgBoxResult->eResult == MBR_OK )
				 {
					GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixJobResetReq( pMsgBoxResult->pData->hExtendHandle, pMsgBoxResult->pData->byIndex );
				 }
			}
		case MBW_FORCED_LOBBY_ASK_RENAME:
			{
				if( MBR_OK != pMsgBoxResult->eResult )
					return;

				const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

				if( FALSE == Logic_IsUsableCharacterName_with_WarningMessage(pwcText) )
					return;

				SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
				CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					return;
				}

				sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
				if( !pLOBBY_CHARACTER )
				{
					DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
					return;
				}


				CHARACTERID charID = pLOBBY_CHARACTER->tSummary.charId;

				GetDboGlobal()->GetLobbyPacketGenerator()->SendCharRenameReq(charID, GetACP(), (WCHAR*)pwcText);
				GetCharStageState()->ChangeState(CHAR_STATE_SELECT_RENAME_REQ);
				GetLobbyManager()->SetReservedRename(pwcText);
			}

			break;
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pData = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		switch(pData->eWorldConcept)
		{
		case WORLD_PLAY_TIME_MACHINE:
			{
				if( pData->uiState == WORLD_STATE_EXIT )
				{
					DeleteMsgBox( DST_REGEN_PLAYER_TMQ );					
				}
				break;
			}
		case WORLD_PLAY_TUTORIAL:
			{
				// peessi: 어떤 MsgBox를 지울건지 확인 해보자.
				//if( pData->uiState == WORLD_STATE_EXIT && m_pMsgBox->IsShow() )
				//	ProccessDefault();
				//break;
			}
		}
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		DeleteAllMsgBoxExceptNetConnectBox();
	}
	else if( msg.Id == g_EventSobItemDelete )
	{
		LIST_MSGBOX_ITER iter;

		for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
		{
			CMsgBoxGui* pMsgBox = (*iter);
			pMsgBox->HandleEvent_ItemDelete( msg );
		}

		for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
		{
			CMsgBoxGui* pMsgBox = (*iter);
			pMsgBox->HandleEvent_ItemDelete( msg );
		}	
	}
	else if( msg.Id == g_EventSobRevivalNotify )
	{
		GetMsgBoxManager()->DeleteMsgBox( DST_REGEN_PLAYER_MAINWORLD );
		GetMsgBoxManager()->DeleteMsgBox( DST_REGEN_PLAYER_TLQ );	
		GetMsgBoxManager()->DeleteMsgBox( DST_REGEN_PLAYER_TMQ );
		GetMsgBoxManager()->DeleteMsgBox( DST_REGEN_PLAYER_PARTY_DUNGEON );
		GetMsgBoxManager()->DeleteMsgBox( DST_REGEN_PLAYER_DOJO );
	}	
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		
		LIST_MSGBOX_ITER iter;

		for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
		{
			CMsgBoxGui* pMsgBox = (*iter);
			pMsgBox->PositionAlign( pPacket->iWidht, pPacket->iHeight );
		}

		for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
		{
			CMsgBoxGui* pMsgBox = (*iter);
			pMsgBox->PositionAlign( pPacket->iWidht, pPacket->iHeight );
		}	
	}
}

VOID CMsgBoxManager::LinkEvent(VOID)
{
	// Event Link
	LinkMsg( g_EventMsgBoxShow );
	LinkMsg( g_EventConnectBoxShow );
	LinkMsg( g_EventMsgBoxResult );	
	LinkMsg( g_EventChangeWorldConceptState );
	LinkMsg( g_EventSobRevivalNotify );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg( g_EventSobItemDelete );
	LinkMsg( g_EventResize );
}

VOID CMsgBoxManager::UnLinkEvent(VOID)
{
	// Event Unlink
	UnLinkMsg( g_EventMsgBoxShow );
	UnLinkMsg( g_EventConnectBoxShow );
	UnLinkMsg( g_EventMsgBoxResult );
	UnLinkMsg( g_EventChangeWorldConceptState );
	UnLinkMsg( g_EventSobRevivalNotify );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg( g_EventSobItemDelete );
	UnLinkMsg( g_EventResize );
}