#include "precomp_dboclient.h"
#include "DialogManager.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// gui
#include "gui_component.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"


// simulation
#include "InputActionMap.h"
#include "NtlWorldConcept.h"
#include "NtlSob.h"
#include "NtlSobNpc.h"
#include "NtlInventory.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "DboTLDefine.h"

// dbo
#include "AlarmManager.h"
#include "CursorManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "MinimapGui.h"
#include "WorldMap.h"
#include "DisplayStringManager.h"
#include "CommunityGui.h"
#include "SideDialogManager.h"
#include "DboLogic.h"
#include "MsgBoxManager.h"
#include "DiceManager.h"
#include "MsgBoxManager.h"

// 占쌓쏙옙트
#include "NtlPLVisualManager.h"
#include "NtlPLSun.h"
#include "NtlMath.h"



VOID CDialogManager::RegisteRegularDialog()
{
	m_mapRegular[DIALOG_TRADECART]				= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_STATUS]					= dRDA_NORMAL;
	m_mapRegular[DIALOG_SKILL]					= dRDA_NORMAL;
	m_mapRegular[DIALOG_COMMUNITY]				= dRDA_NORMAL;
	m_mapRegular[DIALOG_OPTIONWND]				= dRDA_NORMAL;
	m_mapRegular[DIALOG_HELPWND]				= dRDA_NORMAL;
	m_mapRegular[DIALOG_QUESTPROGRESS]			= dRDA_NORMAL;
	m_mapRegular[DIALOG_NPCSHOP]				= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_QUESTLIST]				= dRDA_NORMAL;
	m_mapRegular[DIALOG_QUESTPROPOSAL]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_QUESTREWARD]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_ITEMUPGRADE]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE] = dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_WAREHOUSEBAR]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_PRIVATESHOP]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_MAILSYSTEM]				= dRDA_NORMAL;
	m_mapRegular[DIALOG_MAILSYSTEM_READ]		= dRDA_NORMAL;
	m_mapRegular[DIALOG_RBCHALLENGE]			= dRDA_NORMAL;
	m_mapRegular[DIALOG_RBBOARD]				= dRDA_NORMAL;
	m_mapRegular[DIALOG_QUESTMESSAGE]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
    m_mapRegular[DIALOG_FRIEND_LIST]            = dRDA_NORMAL;
	m_mapRegular[DIALOG_GUILD_WAREHOUSEBAR]		= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_HOIPOIMIX_RECIPE]		= dRDA_NORMAL;
	m_mapRegular[DIALOG_HOIPOIMIX_CRAFT]		= dRDA_NORMAL;
	m_mapRegular[DIALOG_BIND_SEALING] = dRDA_NORMAL;
	m_mapRegular[DIALOG_AUCTION_HOUSE] = dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_DOJO_INFO]				= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_DOJO_ACCOUNT]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_DOGI]					= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_DOJO_UPGRADE]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_MULTIDIALOG]			= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_NETPYSHOP]				= dRDA_NORMAL | dRDA_EXCLUSIVE;

	m_mapRegular[DIALOG_ITEM_EXCHANGE_SHOP]		= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_PLAYER_TITLE] = dRDA_NORMAL;

	m_mapRegular[DIALOG_HLSHOP]				= dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_HLSHOP_WAREHOUSE] = dRDA_NORMAL | dRDA_NOT_ESC;
	m_mapRegular[DIALOG_HLSHOP_BUY] = dRDA_NORMAL | dRDA_NOT_ESC;
	m_mapRegular[DIALOG_HLSHOP_BUY_CONFIRM] = dRDA_NORMAL | dRDA_NOT_ESC;
	m_mapRegular[DIALOG_HLSHOP_GIFT] = dRDA_NORMAL | dRDA_NOT_ESC;
	m_mapRegular[DIALOG_HLSHOP_GIFT_CONFIRM] = dRDA_NORMAL | dRDA_NOT_ESC;

	m_mapRegular[DIALOG_CCBD_BOARD] = dRDA_NORMAL | dRDA_EXCLUSIVE | dRDA_NOT_ESC;

	// Mascot
	m_mapEntireFocuing[DIALOG_MASCOT] = dRDA_NORMAL;
	m_mapRegular[DIALOG_MASCOT_FUSION] = dRDA_NORMAL | dRDA_EXCLUSIVE;
	m_mapRegular[DIALOG_MASCOT_SKILL_REMAKE] = dRDA_NORMAL | dRDA_EXCLUSIVE;
}

VOID CDialogManager::RegisteEntireFocusingDialog()
{
	m_mapEntireFocuing[DIALOG_TMQBOARD]					= dEFDA_BACKBOARD;
	m_mapEntireFocuing[DIALOG_TMQ_REWARD]				= dEFDA_BACKBOARD;
	m_mapEntireFocuing[DIALOG_RANKBOARD]				= dEFDA_BACKBOARD;
	m_mapEntireFocuing[DIALOG_TBGAMBLERESULT]			= dEFDA_BACKBOARD;
	m_mapEntireFocuing[DIALOG_DURATION_EXTEND_CONFIRM]	= dEFDA_BACKBOARD;

	m_mapEntireFocuing[DIALOG_HLSHOP]					= dEFDA_BACKBOARD;
}

VOID CDialogManager::PlayOpenSound(RwInt32 iDialog)
{
	if( IsRegularDialog((eDialogType)iDialog) || IsEFDialog((eDialogType)iDialog) )
	{
		Logic_PlayGUISound(GSD_SYSTEM_WINDOW_OPEN);
	}
	else
	{
		switch(iDialog)
		{
		case DIALOG_CHAT_LOG:
		case DIALOG_MULTIDIALOG:
		case DIALOG_CAPSULE_1:
		case DIALOG_CAPSULE_2:
		case DIALOG_CAPSULE_3:
		case DIALOG_CAPSULE_4:
		case DIALOG_CAPSULE_5:
		case DIALOG_CAPSULE_NETPY:
		case DIALOG_WAREHOUSE_1:
		case DIALOG_WAREHOUSE_2:
		case DIALOG_WAREHOUSE_3:
		case DIALOG_WAREHOUSE_COMMON:
		case DIALOG_WORLDMAP:
		case DIALOG_MINIMAP:
		case DIALOG_GUILD_WAREHOUSE_1:
		case DIALOG_GUILD_WAREHOUSE_2:
		case DIALOG_GUILD_WAREHOUSE_3:
			{
				Logic_PlayGUISound(GSD_SYSTEM_WINDOW_OPEN);
				break;
			}					
		}
	}
}

VOID CDialogManager::PlayCloseSound(RwInt32 iDialog)
{
	if( IsRegularDialog((eDialogType)iDialog) || IsEFDialog((eDialogType)iDialog) )
	{
		Logic_PlayGUISound(GSD_SYSTEM_WINDOW_CLOSE);
	}
	else
	{
		switch(iDialog)
		{
			case DIALOG_CHAT_LOG:
			case DIALOG_MULTIDIALOG:
			case DIALOG_CAPSULE_1:
			case DIALOG_CAPSULE_2:
			case DIALOG_CAPSULE_3:
			case DIALOG_CAPSULE_4:
			case DIALOG_CAPSULE_5:
			case DIALOG_CAPSULE_NETPY:
			case DIALOG_WAREHOUSE_1:
			case DIALOG_WAREHOUSE_2:
			case DIALOG_WAREHOUSE_3:
			case DIALOG_WAREHOUSE_COMMON:
			case DIALOG_WORLDMAP:
			case DIALOG_MINIMAP:
			case DIALOG_GUILD_WAREHOUSE_1:
			case DIALOG_GUILD_WAREHOUSE_2:
			case DIALOG_GUILD_WAREHOUSE_3:
			{
				Logic_PlayGUISound(GSD_SYSTEM_WINDOW_CLOSE);
			}
			break;
		}
	}
}

RwBool CDialogManager::SwitchDialog(int iDialog)
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);
	if(pDialogInfo)
	{
		if( pDialogInfo->pDialog->IsShow() )
			return CloseDialog(iDialog);
		else
			return OpenDialog(iDialog);
	}

	return FALSE;
}

RwBool CDialogManager::OpenDialog(int iDialog, SERIAL_HANDLE hSerial /* = INVALID_SERIAL_ID */, RwBool bPlaySound /* = TRUE */)
{
	// GM 메뉴에서 일부 다이얼로그를 사용할 수 없도록 하였을 때
	if( m_bCanVisible_OnOberserver )
	{
		if( iDialog == DIALOG_HP			|| iDialog == DIALOG_MINIMAP ||
			iDialog == DIALOG_PET_STATUSBAR	|| iDialog == DIALOG_PET_SKILL_SLOT )
			return FALSE;
	}

	// GM Menu占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙
	if( IsOpenDialog(DIALOG_GM_QUICKSLOT) )
	{
		if( iDialog == DIALOG_QUICKSLOT		|| iDialog == DIALOG_EXP ||
			iDialog == DIALOG_BAGSLOT )
			return FALSE;
	}

	// ����Ʈ �����̼��� �帣�� ���� 
	if( iDialog != DIALOG_BUDOKAI_TOURNAMENT && 
		iDialog != DIALOG_BUDOKAI_TOURNAMENT_MATCHINFO &&
		iDialog != DIALOG_FLASH_NOTIFY &&
		IsMode( DIALOGMODE_NARRATION )  )
	{
		LIST_REQUESTED_OPENDIALOG_IN_NARRATION_ITER iter;
		for( iter = m_listRequestedOpenDialogInNarration.begin() ; iter != m_listRequestedOpenDialogInNarration.end() ; ++iter )
		{
			sRequestedOpenDialog* pData = &(*iter);

			if (pData->eRequestedDialogType == iDialog)
			{
				DBO_WARNING_MESSAGE("if (pData->eRequestedDialogType == iDialog): " << iDialog);
				return FALSE;
			}
		}

		sRequestedOpenDialog sData;
		sData.eRequestedDialogType = iDialog;
		sData.hSerial = hSerial;
		sData.bSound = bPlaySound;
		m_listRequestedOpenDialogInNarration.push_back( sData );
		return FALSE;
	}	

	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);
	if (!pDialogInfo)
	{
		DBO_WARNING_MESSAGE("NO DIALOG INFO: " << iDialog);
		return FALSE;
	}

	// Return if already open
	if (pDialogInfo->pDialog->IsShow())
	{
		return FALSE;
	}

	// If a dialog registered in m_setExclusiveDialog is opened when a request is made to open a multi-dialog
	// close the registered dialog first and open the multi-dialog
	if( iDialog == DIALOG_MULTIDIALOG )
	{
		eDialogType eOpenedNPCDialog = (eDialogType)GetOpendNPCDIalog();
		if( eOpenedNPCDialog != DIALOG_UNKNOWN )
		{
			m_bExpectingCloseNPCDialog = TRUE;

			// Some windows are closed from the server until a close response is received.
			// So, after DialogManager Update window is closed, it opens multi-dialogue
			CloseDialog(eOpenedNPCDialog);
			return FALSE;
		}		
	}	

	if( IsRegularDialog((eDialogType)iDialog) )
	{
		if( CanOpenRegularDialog() == FALSE )
		{
			GetAlarmManager()->AlarmMessage("DST_CAN_NOT_OPEN_MORE_REGULAR_DIALOG");
			return FALSE;
		}
	}

	// We call each class's SwitchDialog ().
	if( pDialogInfo->pCallSwitch->Call(true) < 0 )
		return FALSE;

	// Placement of Regular Dialog
	ProcessOpenRegular((eDialogType)iDialog);

	//Create a list of dialogs affected by the ESC key
	RaiseDialogbyUser(iDialog);	

	// Organize the order of drawing dialogs
	ProcessLayer((eDialogType)iDialog);

	// Target information opened in NPC window
	if( hSerial != INVALID_SERIAL_ID )
	{
		if( IsNPCDialog(iDialog) )
		{
			m_OpenedTarget.pOpenedTarget = GetNtlSobManager()->GetSobObject(hSerial);
			m_OpenedTarget.v3Pos = m_OpenedTarget.pOpenedTarget->GetPosition();
		}		
	}

	if( bPlaySound )
		PlayOpenSound(iDialog);

	return TRUE;
}

RwBool CDialogManager::CloseDialog(int iDialog, RwBool bPlaySound /* = TRUE */)
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);

	if(!pDialogInfo)
		return FALSE;

	// 占싱뱄옙 占쏙옙占쏙옙 占쌍다몌옙 占쏙옙占쏙옙
	if( !pDialogInfo->pDialog->IsShow() )
		return FALSE;

	if( pDialogInfo->pCallSwitch->Call(false) < 0 )
		return FALSE;

	// 더 이상 Entire Focusing Dialog이 없다면 Dialog 뒷편으로 배경을 없앤다
	if( IsEFDialog((eDialogType)iDialog) )
	{
		if( IsOpenEFDialog() == FALSE )
			CloseDialog(DIALOG_BACKBOARD);
	}

	// 닫히는 다이얼로그가 NPC 다이얼로그 일 때
	if( IsNPCDialog(iDialog) )
		m_OpenedTarget.pOpenedTarget = NULL;

	ProcessCloseRegular((eDialogType)iDialog);

	HideDialogbyUser(iDialog);	

	if(bPlaySound)
		PlayCloseSound(iDialog);	

	return TRUE;
}

VOID CDialogManager::ProcessESC()
{
	if( m_bESCSkip )
	{
		/*	Skips the ESC key for one Tick only.
		CDboApplication::PreTranslateMessage(..)
		{
		ret = pGuiMgr->Win32EventProc(window, message, wParam, lParam);in
		It processes the ESC key to activate the InputActionMap when the InputBox loses focus

		The ESC logic turns around again after the procedure below.

		At this time, if you press the ESC key while chatting in the game, the focus is removed,
		In this case, ESC key is skipped once to close.
		}
		*/
		m_bESCSkip = FALSE;
		return;
	}

	if (GetNtlSLGlobal()->IsSkillSelectMode())
	{
		CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
	}
	else if( GetMsgBoxManager()->IsShowMsgBox() )
		return;
	else
	{
		// Process from the next window of the main window	
		RwInt32 iDialog = GetTopDialog();
		if( iDialog != DIALOG_UNKNOWN )
		{
			if( IsNPCDialog(iDialog) )
			{
				CDboEventGenerator::DialogEvent(DIALOGEVENT_NPC_BYEBYE, DIALOG_UNKNOWN, iDialog);
			}		
			else
			{
				if( DIALOG_CAPSULE_1 <= iDialog && iDialog <= DIALOG_CAPSULE_NETPY )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_CAPSULE_KIT_CLOSE ) )
						return;

					SwitchBag(FALSE);
				}
				else if( DIALOG_SKILL == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_SKILL_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_STATUS == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_STATUS_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_QUESTLIST == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_QUEST_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_COMMUNITY == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_COMMUNITY_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_RANKBOARD == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_RANKBOARD_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if (DIALOG_MASCOT == iDialog)
				{
					if (!Logic_CanKeybaordInput_in_Tutorial(ETL_KEYBOARD_INPUT_TYPE_ESC_MASCOT_CLOSE))
						return;
					CloseDialog(iDialog);
				}
				else if( DIALOG_OPTIONWND == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_OPTION_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_FRIEND_LIST == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_FRIEND_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_WORLDMAP == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_WORLDMAP_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_HELPWND == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_HELP_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
				else if( DIALOG_MAINMENU == iDialog )
				{
					if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_MAINMENU_CLOSE ) )
						return;

					CloseDialog(iDialog);
				}
                else if(DIALOG_DBC_ALTAR == iDialog)
                {
                    // 주문 버튼을 클릭하고 응답을 기다리는 중이면 취소하지 않는다.
                    if(API_GetSLPacketLockManager()->IsLock(GU_DRAGONBALL_CHECK_RES))
                        return;

                    CloseDialog(iDialog);
                }
				else
					CloseDialog(iDialog);
			}
		}
		else if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TUTORIAL) )
		{
			// Ʃ�丮���� �����Ͻðڽ��ϱ�?
			GetAlarmManager()->AlarmMessage( "DST_TUTORIAL_ASK_END" );
		}
		else if(Logic_GetAvatarTargetHandle() != INVALID_SERIAL_ID )
		{
			// Ÿ���� ����Ѵ�
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_ESC_TARGETING ) )
				return;

			Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);
		}
		else if (GetDboGlobal()->GetCommunityTarget() != INVALID_SERIAL_ID)
		{
			if (!Logic_CanKeybaordInput_in_Tutorial(ETL_KEYBOARD_INPUT_TYPE_ESC_TARGETING))
				return;

			CNtlSLEventGenerator::CommuTargetRelease(GetDboGlobal()->GetCommunityTarget());
		}
		else if( IsOpenDialog(DIALOG_VEHICLE) )
		{
			GetAlarmManager()->AlarmMessage( "DST_VEHICLE_ASK_GET_OFF" );
		}
		else if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_CCBD))
		{
			CNtlCCBD* pCCBD = GetNtlSLGlobal()->GetSobAvatar()->GetCCBD();

			GetAlarmManager()->FormattedAlarmMessage("DST_CCBD_LEAVE_CCBD", FALSE, NULL, pCCBD->GetStage());
		}
		else
		{
			if( IsOpenDialog( DIALOG_ESCMENU ) )
				CloseDialog( DIALOG_ESCMENU );
			else
				OpenDialog( DIALOG_ESCMENU );

		}
	}
}

VOID CDialogManager::ProcessOpenRegular(eDialogType eDialog)
{
	if( IsRegularDialog(eDialog) == FALSE )
		return;


	if( m_bDialogMovable )
	{
		m_listOpenedRegularDilaog.push_back(eDialog);
		return;
	}

	
	RwInt32 iPosX = dREGULARDIALOG_FIRST_X;
	CNtlPLGui* pGui = NULL;
	RwBool bExistPosition = TRUE;
	LIST_OPENED_REGULAR_ITER it_OpenedRegular = m_listOpenedRegularDilaog.begin();
	
	if( HowManyOpenMaxRegular() <= (RwInt32)m_listOpenedRegularDilaog.size() )
	{
		// Regular Dialog가 열릴 수 있는 장소가 남아있지 않다
		bExistPosition = FALSE;
		for( ; it_OpenedRegular != m_listOpenedRegularDilaog.end() ; ++it_OpenedRegular )
		{
			eDialogType dialogType = *it_OpenedRegular;

			// 占쏙옙타占쏙옙 占쌈쇽옙占쏙옙 占싣댐옙 Regular dialog 占싹놂옙占쏙옙 占쌥는댐옙
			if( IsHaveAttribute((eDialogType)dialogType, dRDA_EXCLUSIVE) == FALSE )
			{
				if( CloseDialog(dialogType) )
					bExistPosition = TRUE;
				
				break;
			}
		}
	}

	if( bExistPosition == FALSE )
	{
		// avooo's : 여기에 들어오는 것은 기획 혹은 프로그램에서 레귤러 다이얼로그에 대한 정의나
		// �߸��� ���� ������ ���� ��Ÿ���� ���ַ� ���̾�αװ� ���ÿ� ���� ���ο� ���ַ�
		// ���̾�αװ� ���� ������ Ȯ������ ������ ����̴�
		DBO_FAIL("Can not open more regaulr dialog");
		return;
	}

	if( m_listOpenedRegularDilaog.size() > 0 )
	{
		it_OpenedRegular = m_listOpenedRegularDilaog.end();
		--it_OpenedRegular;

		pGui = GetDialog(*it_OpenedRegular);
		iPosX = pGui->GetLinkedArea().right + dREGULARDIALOG_GAP;
	}


	// ���ο� ���̾�α׸� ������ �ڸ��� ��ġ�Ѵ�
	pGui = GetDialog(eDialog);

	if( IsHaveAttribute(eDialog, dRDA_HALFSIZE) )
		pGui->SetPosition(iPosX, GetPosY_of_HalfRegular());
	else
		pGui->SetPosition(iPosX, m_iRegularStrartY);

	m_listOpenedRegularDilaog.push_back(eDialog);
}

VOID CDialogManager::ProcessLayer(eDialogType eDialog)
{
	// ���̾�α׸� �׸��� ������ �����Ѵ�
	CNtlPLGui* pGui = GetDialog(eDialog);
		
	// EFDialog
	if( IsEFDialog(eDialog) )
		OpenDialog(DIALOG_BACKBOARD);

	if( pGui->IsShow() )
		pGui->RaiseLinked();
}

VOID CDialogManager::ProcessCloseRegular(eDialogType eDialog)
{
	if( IsRegularDialog(eDialog) == FALSE )
		return;


	RwBool bCloseRegular = FALSE;
	RwInt32 iPosX = dREGULARDIALOG_FIRST_X;
	LIST_OPENED_REGULAR_ITER it_OpenedRegularDialog = m_listOpenedRegularDilaog.begin();
	while( it_OpenedRegularDialog != m_listOpenedRegularDilaog.end() )
	{
		eDialogType dialogType = *it_OpenedRegularDialog;

		if( eDialog == dialogType )
		{
			it_OpenedRegularDialog = m_listOpenedRegularDilaog.erase(it_OpenedRegularDialog);

			if( m_bDialogMovable )
				return;

			bCloseRegular = TRUE;
			continue;
		}


		CNtlPLGui* pGui = GetDialog(dialogType);
		if( !pGui )
		{
			DBO_FAIL("not register dialog of index : " << (RwInt32)dialogType);
			++it_OpenedRegularDialog;
			continue;
		}

		if( bCloseRegular )
		{
			if( IsHaveAttribute(dialogType, dRDA_HALFSIZE) )
				pGui->SetPosition(iPosX, GetPosY_of_HalfRegular());
			else
				pGui->SetPosition(iPosX, m_iRegularStrartY);
		}

		iPosX = pGui->GetLinkedArea().right + dREGULARDIALOG_GAP;
		++it_OpenedRegularDialog;
	}
}

VOID CDialogManager::ProcessMovableDialogs(RwBool bMovable)
{
	m_bDialogMovable = bMovable;

	CNtlPLGui* pGui = GetDialog(DIALOG_CHANNGEL_CHANGE);
	if( pGui )
		pGui->SetMovable(m_bDialogMovable);

	DIALOG_MAP_ITER it = m_mapDialog.begin();
	for( ; it != m_mapDialog.end() ; ++it )
	{
		DialogInfo& rDialogInfo = it->second;
		rDialogInfo.pDialog->SetMovable(m_bDialogMovable);
	}
}

/**
* \brief 액션에 따른 다이얼로그 처리
*
* InputAction占쏙옙占쏙옙 占쏙옙占쏙옙 占쌓쇽옙占쏙옙占쏙옙 DialogManager占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙 占쌍댐옙 처占쏙옙占쏙옙 占싼댐옙.
* 占쏙옙占쏙옙 HandleHotKey 占쏙옙 占쏙옙크占실어서 처占쏙옙占싹댐옙 키 占쏙옙占쏙옙 占쌍댐옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙 占쌉쇽옙占쏙옙 占쏙옙크占싹곤옙 占쌓쇽옙占쏙옙 占쌨곤옙 占쏙옙
*
* \param iAction (unsigned int)占쌓쇽옙. - 占쏙옙占쏙옙 InputAction.h
* \return int
*/
#include "NtlWorldConceptTutorial.h"
int CDialogManager::HandleDialogAction(uintptr_t iAction)
{
	NTL_FUNCTION("CDialogManager::HandleDialogAction");

	// Block key in narration mode
	if( IsMode( DIALOGMODE_NARRATION ) )
	{
		// But Esc does it. peessi
		if (iAction == ACTION_GLOBAL_CANCLE)
			CNtlSLEventGenerator::ESCinNarrationMode();

		NTL_RETURN( 1 );
	}

	// �׼ǿ� ���� ���̾�α� ó�� ����
	// 캐占쏙옙占쏙옙占쏙옙占쏙옙
	// 占쏙옙킬占쏙옙占쏙옙
	// 占쏙옙占쏙옙트 占쏙옙占쏙옙
	// 占싱니몌옙
	// �����
	// 占쏙옙占쏙옙캡占쏙옙킷
	// 占쏙옙카占쏙옙占쏙옙
	// 占쏙옙티占쏙옙占쏙옙
	// �������
	// 친占쏙옙占쏙옙占쏙옙
	// 占심쇽옙
	// 占쏙옙占쏙옙
	// 占쏙옙占싸메댐옙
	// �̴ϸ����
	// 占싱니몌옙확占쏙옙
	// 채占쏙옙占쏙옙占쏙옙PAGE
	// 채占시댐옙占쏙옙PAGE
	// ���

	switch( iAction )
	{	
	case ACTION_WINDOW_PROFILE:		// 캐占쏙옙占쏙옙 占쏙옙占쏙옙 창
		{
			RwBool bOpen = IsOpenDialog(DIALOG_STATUS);

			if( bOpen )
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_STATUS_CLOSE ) )
					NTL_RETURN(1);				
			}
			else
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_STATUS_OPEN ) )
					NTL_RETURN(1);
			}			

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			SwitchDialog( DIALOG_STATUS );
			break;
		}
	case ACTION_WINDOW_SKILL:		// 占쏙옙킬 占쏙옙占쏙옙 창
		{
			RwBool bOpen = IsOpenDialog(DIALOG_SKILL);

			if( bOpen )
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_SKILL_CLOSE ) )
					NTL_RETURN(1);
			}
			else
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_SKILL_OPEN ) )
					NTL_RETURN(1);
			}

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			SwitchDialog(DIALOG_SKILL);

			break;
		}
	case ACTION_WINDOW_QUEST:		// 占쏙옙占쏙옙트 占쏙옙占쏙옙 창
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUEST ) )
				NTL_RETURN(1);

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			SwitchDialog(DIALOG_QUESTLIST);
			break;
		}
	case ACTION_WINDOW_MINIMAP:		// 占싱니몌옙
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MINIMAP ) )
				NTL_RETURN(1);

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			if( IsMode( DIALOGMODE_NARRATION ) )
				NTL_RETURN(1);

			CMinimapGui* pMiniMap = reinterpret_cast<CMinimapGui*>(GetDialog(DIALOG_MINIMAP));
			NTL_ASSERT(pMiniMap, "CDialogManager::HandleDialogAction, Not exist minimap gui class");
			pMiniMap->NextMapStyle();

			break;
		}
	case ACTION_WINDOW_WORLDMAP:	// 월드맵
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_WORLDMAP ) )
				NTL_RETURN(1);

			SwitchDialog(DIALOG_WORLDMAP);
			break;
		}
	case ACTION_WINDOW_MAINCAP:	// 캡占쏙옙(占쏙옙占쏙옙)
		{
			RwBool bBagOpen = IsBagOpen();			

			if( bBagOpen )
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_CLOSE ) )
					NTL_RETURN(1);
			}
			else
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_OPEN ) )
					NTL_RETURN(1);
			}

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			// 占쏙옙占쏙옙占쏙옙 占싼곤옙占쏙옙 占쏙옙占쏙옙 占쌍다몌옙 占쏙옙占쏙옙 占쌥는댐옙.
			bBagOpen = !bBagOpen;

			SwitchBag(bBagOpen);

			break;
		}
	case ACTION_WINDOW_GUILD:		// 길드 커뮤니티
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_GUILD ) )
				NTL_RETURN(1);

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			CCommunityGui* pGui = (CCommunityGui*)GetDialog(DIALOG_COMMUNITY);

			if( IsOpenDialog(DIALOG_COMMUNITY))
			{
				if( pGui->GetPageType() == CCommunityGui::PAGETYPE_GUILD )
					CloseDialog(DIALOG_COMMUNITY);
				else
					pGui->SetPageType(CCommunityGui::PAGETYPE_GUILD);
			}
			else
			{
				if( pGui->GetPageType() != CCommunityGui::PAGETYPE_GUILD )
					pGui->SetPageType(CCommunityGui::PAGETYPE_GUILD);

				OpenDialog(DIALOG_COMMUNITY);
			}


			break;
		}
	case ACTION_WINDOW_FRIEND:
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_FRIEND ) )
				NTL_RETURN(1);

			if(IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			SwitchDialog(DIALOG_FRIEND_LIST);
		}
		break;
	case ACTION_WINDOW_OPTION: // 占심쇽옙 占쏙옙占쏙옙占쏙옙
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_OPTION ) )
				NTL_RETURN(1);

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			SwitchDialog( DIALOG_OPTIONWND );
			break;
		}
	case ACTION_WINDOW_RANKBOARD:	// 占쏙옙크占쏙옙占쏙옙
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_RANKBOARD ) )
				NTL_RETURN(1);

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			SwitchDialog( DIALOG_RANKBOARD );
			break;
		}
	case ACTION_WINDOW_MASCOT:
		{
			if (!Logic_CanKeybaordInput_in_Tutorial(ETL_KEYBOARD_INPUT_TYPE_MASCOT))
				NTL_RETURN(1);
			SwitchDialog(DIALOG_MASCOT);
			break;
		}
	case ACTION_WINDOW_HELP:	// 占쏙옙占쏙옙
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_HELP ) )
				NTL_RETURN(1);

			SwitchDialog(DIALOG_HELPWND);
			// 占쏙옙획占쏙옙 Help Data Test占쏙옙 占쏙옙占쌔쇽옙 占쏙옙트占쏙옙 占쏙옙占쏙옙占쏙옙트 占싼댐옙. 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙 占쏙옙.
			break;
		}
	case ACTION_WINDOW_MAIN: // 占쏙옙占쏙옙 占쌨댐옙
		{
			if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_MAINMENU ) )
				NTL_RETURN(1);

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			Logic_PlayGUISound(GSD_SYSTEM_BUTTON_CLICK);

			if( !IsOpenDialog( DIALOG_MAINMENU ) )
			{
				GetDialog( DIALOG_MAINMENU )->RaiseTop();
			}

			SwitchDialog( DIALOG_MAINMENU );
			break;
		}
	case ACTION_MINIMAP_ZOOMOUT:	// 미니맵 축소
		{
			CDboEventGenerator::MapEvent(MMT_MINIMAP_ZOON_OUT);
			break;
		}
	case ACTION_MINIMAP_ZOOMIN:	// 占싱니몌옙 확占쏙옙
		{
			CDboEventGenerator::MapEvent(MMT_MINIMAP_ZOON_IN);
			break;
		}
	case ACTION_GLOBAL_CANCLE:	// ESC 키 처占쏙옙
		{
			ProcessESC();
			break;
		}
	case ACTION_QUICK_2NDCAP:
		{
			RwBool bBagOpen = IsBagOpen();

			if( bBagOpen )
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_CLOSE ) )
					NTL_RETURN(1);
			}
			else
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_OPEN ) )
					NTL_RETURN(1);
			}

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			// 占쏙옙占쏙옙占쏙옙 占싼곤옙占쏙옙 占쏙옙占쏙옙 占쌍다몌옙 占쏙옙占쏙옙 占쌥는댐옙.
			bBagOpen = !bBagOpen;

			SwitchBagByIndex(1);			

			break;
		}
	case ACTION_QUICK_3RDCAP:
		{
			RwBool bBagOpen = IsBagOpen();

			if( bBagOpen )
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_CLOSE ) )
					NTL_RETURN(1);
			}
			else
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_OPEN ) )
					NTL_RETURN(1);
			}

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			// 占쏙옙占쏙옙占쏙옙 占싼곤옙占쏙옙 占쏙옙占쏙옙 占쌍다몌옙 占쏙옙占쏙옙 占쌥는댐옙.
			bBagOpen = !bBagOpen;

			SwitchBagByIndex(2);			

			break;
		}
	case ACTION_QUICK_4THCAP:
		{
			RwBool bBagOpen = IsBagOpen();

			if( bBagOpen )
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_CLOSE ) )
					NTL_RETURN(1);
			}
			else
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_OPEN ) )
					NTL_RETURN(1);
			}

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			// 占쏙옙占쏙옙占쏙옙 占싼곤옙占쏙옙 占쏙옙占쏙옙 占쌍다몌옙 占쏙옙占쏙옙 占쌥는댐옙.
			bBagOpen = !bBagOpen;

			SwitchBagByIndex(3);

			break;
		}
	case ACTION_QUICK_5THCAP:
		{
			RwBool bBagOpen = IsBagOpen();

			if( bBagOpen )
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_CLOSE ) )
					NTL_RETURN(1);
			}
			else
			{
				if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_CAPSULE_KIT_OPEN ) )
					NTL_RETURN(1);
			}

			if( IsOpenDialog(DIALOG_WORLDMAP))
				NTL_RETURN(1);

			// 占쏙옙占쏙옙占쏙옙 占싼곤옙占쏙옙 占쏙옙占쏙옙 占쌍다몌옙 占쏙옙占쏙옙 占쌥는댐옙.
			bBagOpen = !bBagOpen;

			SwitchBagByIndex(4);

			break;
		}
	default:
		NTL_RETURN(1);
	}


	NTL_RETURN(1);
}
