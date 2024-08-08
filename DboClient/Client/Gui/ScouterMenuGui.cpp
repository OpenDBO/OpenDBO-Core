#include "precomp_dboclient.h"
#include "ScouterMenuGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"
#include "ActionTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "InputActionMap.h"

// dbo
#include "DboEvent.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "SideDialogManager.h"


#define dENABLED_SCOUTER_DURABILITY	1
#define dMENU_YPOS_GAP				1
#define dMENU_VISIBLE_COUNT			5
#define dDIALOG_WIDTH				160

#define dEXIT_BUTTON_NAME			"scouter_off.swf"
#define dPREPAGE_BUTTON_NAME		"scouter_up_arrow.swf"
#define dNEXTPAGE_BUTTON_NAME		"scouter_down_arrow.swf"

#define dSHORTCUT_KEY_1				 (49)
#define dSHORTCUT_KEY_2				 (50)
#define dSHORTCUT_KEY_3				 (51)
#define dSHORTCUT_KEY_4				 (52)
#define dSHORTCUT_KEY_5				 (53)


const char acMenuFile[NUM_SCOUTERMENU][64] = {
	"scouter_power_level.swf", "scouter_identify.swf", "scouter_equip_view.swf", "scouter_find_object.swf",
	"scouter_quest_search.swf"};

//////////////////////////////////////////////////////////////////////////
//					CScouterMenuItem									//
//////////////////////////////////////////////////////////////////////////

RwBool CScouterMenuItem::Create(gui::CComponent* pParent, RwUInt8 byMenuType, CRectangle& rt)
{	
	const char* pcFlashFileName = acMenuFile[byMenuType];
	m_byMenuType = byMenuType;

	m_pFlashButton = NTL_NEW gui::CFlash(pParent, GetNtlGuiManager()->GetSurfaceManager());

	if( m_pFlashButton->Load(pcFlashFileName) == false )
		return FALSE;

	m_pFlashButton->SetPosition(rt);
	m_pFlashButton->SetFrameCallback(FALSE);
	m_pFlashButton->PlayMovie(TRUE);

	m_slotFSCallback = m_pFlashButton->SigFSCallBack().Connect( this, &CScouterMenuItem::OnFSCallback );

	return TRUE;
}

VOID CScouterMenuItem::Destroy()
{
	if(m_pFlashButton)
	{
		m_pFlashButton->Unload();
		NTL_DELETE(m_pFlashButton);
	}
}

VOID CScouterMenuItem::Update(RwReal fElapsed)
{
	m_pFlashButton->Update(fElapsed);
}

VOID CScouterMenuItem::Show(bool bOpen)
{
	m_pFlashButton->Show(bOpen);
}

RwUInt8 CScouterMenuItem::GetMenuType()
{
	return m_byMenuType;
}

VOID CScouterMenuItem::SetPosition(CRectangle& rt)
{
	m_pFlashButton->SetPosition(rt);
}

CRectangle CScouterMenuItem::GetPosition()
{
	return m_pFlashButton->GetFrameResolution();
}

RwBool CScouterMenuItem::IsShow()
{
	return m_pFlashButton->IsVisible();
}

VOID CScouterMenuItem::OnFSCallback(const char* pcParam, const char* pcParam2)
{
	m_pScounterMenuGui->OnClick_MenuItem(m_byMenuType);
}

//////////////////////////////////////////////////////////////////////////
//					CScouterMenuGui										//
//////////////////////////////////////////////////////////////////////////

CScouterMenuGui::CScouterMenuGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pFlashExitButton(NULL)
,m_byVisibleMenuIndex(0)
{
}

CScouterMenuGui::~CScouterMenuGui()
{

}

RwBool CScouterMenuGui::Create()
{
	NTL_FUNCTION( "CScouterMenuGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\ScouterMenu.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;	

	// 닫기 버튼
	m_pFlashExitButton = (gui::CFlash*)GetComponent( "flashExitBtn" );	
	m_slotCloseButton = m_pFlashExitButton->SigFSCallBack().Connect( this, &CScouterMenuGui::OnClicked_CloseButton );

	// 이전 페이지로 넘김 버튼
	m_pFlashPrePageButton = (gui::CFlash*)GetComponent("flaPrePageBtn");	
	m_slotPrePageButton = m_pFlashPrePageButton->SigFSCallBack().Connect( this, &CScouterMenuGui::OnClicked_PrePageButton );

	// 다음 페이지로 넘김 버튼
	m_pFlashNextPageButton = (gui::CFlash*)GetComponent("flaNextPageBtn");	
	m_slotNextPageButton = m_pFlashNextPageButton->SigFSCallBack().Connect( this, &CScouterMenuGui::OnClicked_NextButton );

	// sig
	m_handleKeyUp		= CInputHandler::GetInstance()->LinkKeyUp(this, &CScouterMenuGui::KeyboardUpHandler);
	m_slotMove			= m_pThis->SigMove().Connect( this, &CScouterMenuGui::OnMove );

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg(g_EventScouter);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScouterMenuGui::Destroy()
{
	NTL_FUNCTION("CScouterMenuGui::Destroy");

	Clear();

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	CInputHandler::GetInstance()->UnLinkKeyUp(m_handleKeyUp);

	UnLinkMsg(g_EventScouter);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScouterMenuGui::Clear()
{
	LIST_MENUITEM_ITER it = m_listMenuItem.begin();
	for( ; it != m_listMenuItem.end() ; ++it )
	{
		CScouterMenuItem* pMenuItem = *it;
		pMenuItem->Destroy();
		NTL_DELETE(pMenuItem);
	}

	m_listMenuItem.clear();

	m_byVisibleMenuIndex = 0;

	m_pFlashExitButton->Unload();
	m_pFlashPrePageButton->Unload();
	m_pFlashNextPageButton->Unload();
}

VOID CScouterMenuGui::Update(RwReal fElapsed)
{
	if( !IsShow() )
		return;

	m_pFlashExitButton->Update(fElapsed);
	m_pFlashPrePageButton->Update(fElapsed);
	m_pFlashNextPageButton->Update(fElapsed);

	LIST_MENUITEM_ITER it = m_listMenuItem.begin();
	for( ; it != m_listMenuItem.end() ; ++it )
		(*it)->Update(fElapsed);
}

VOID CScouterMenuGui::Menu_Update()
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwUInt32 uiSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);
	CNtlSobItem* pScouterBody = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(uiSerial));		

	// 스카우터가 없다면 리턴
	if(!pScouterBody)
		return;

	CNtlSobItemAttr* pScouterBodyAttr = reinterpret_cast<CNtlSobItemAttr*>( pScouterBody->GetSobAttr() );	
	sITEM_TBLDAT* pScouterBodyTBLDAY = pScouterBodyAttr->GetItemTbl();
	NTL_ASSERT( pScouterBodyTBLDAY, "CScouterMenuGui::Menu_Update : pScouterBodyTBLDAY Must be Present!" );

	CNtlSobItem* pScouterParts;
	sITEM_TBLDAT* pPART_ITEM_TBLDAT;

	// 파츠 기능들 임시 보관
	std::map<RwUInt8, RwBool> mapTempMenuType; // RwUInt8 : 메뉴 타입, RwBool : 메뉴 존재 여부

	LIST_MENUITEM_ITER it = m_listMenuItem.begin();
	for( ; it != m_listMenuItem.end() ; ++it )
	{
		CScouterMenuItem* pMenuItem = *it;
		mapTempMenuType[pMenuItem ->GetMenuType()] = FALSE;
	}

	// 전투력 측정은 기본적으로 제공이 된다
	mapTempMenuType[SCOUTERMENU_MEASURE_POWER] = TRUE;


	for( RwInt32 i = 0 ; i < pScouterBodyTBLDAY->byBag_Size ; ++i )
	{
		// 스카우터 바디안의 각 슬롯에 파츠가 있다면 
		pScouterParts = pScouterBody->GetChildItem( i );
		if(pScouterParts)
		{
			// 파츠의 정보를 읽어와서
			pPART_ITEM_TBLDAT = Logic_GetItemDataFromSob(pScouterParts->GetSerialID());

			// 특정 파츠들만이 메뉴가 생성된다
			RwUInt8 byMenuType = PartsTypeToMenuIndex(pPART_ITEM_TBLDAT->byScouter_Parts_Type);
			if( byMenuType == INVALID_SCOUTERMENU )
				continue;

			std::map<RwUInt8, RwBool>::iterator it_deleteTemp = mapTempMenuType.find(byMenuType);
			if( it_deleteTemp != mapTempMenuType.end() )
			{
				// 기존에 존재하는 파츠이다
				it_deleteTemp->second = TRUE;
			}
			else
			{
				// 기존에 없던 파츠 기능이 등록되었다				
				if( Menu_Add(byMenuType) )
					mapTempMenuType[byMenuType] = TRUE;
			}				
		}
	}	

	std::map<RwUInt8, RwBool>::iterator it_deleteTemp = mapTempMenuType.begin();
	for( ; it_deleteTemp != mapTempMenuType.end() ; ++it_deleteTemp) 
	{
		// 같은 타입의 파츠가 모두 등록해제되어 메뉴가 사라진다.
		if( it_deleteTemp->second == FALSE )
		{
			Menu_Del(it_deleteTemp->first);
		}		
	}
}

RwBool CScouterMenuGui::Menu_Add(RwUInt8 byMenuType)
{
	CRectangle rect;
	CScouterMenuItem* pMenuItem = NTL_NEW CScouterMenuItem(this);
	if( pMenuItem->Create(m_pThis, byMenuType, rect) == FALSE )
	{
		pMenuItem->Destroy();
		return FALSE;
	}

	m_listMenuItem.push_back(pMenuItem);

	ResizeDilaog();
	Menu_JugdeVisiblePreNextButton();

	return TRUE;
}

VOID CScouterMenuGui::Menu_Del(RwUInt8 byMenuType)
{
	RwUInt8 byMenuCount = 0;
	LIST_MENUITEM_ITER it = m_listMenuItem.begin();
	for( ; it != m_listMenuItem.end() ; ++it, ++byMenuCount )
	{
		CScouterMenuItem* pMenuItem = *it;
		if( pMenuItem->GetMenuType() == byMenuType )
		{
			pMenuItem->Destroy();
			NTL_DELETE(pMenuItem);

			m_listMenuItem.erase(it);

			if( m_byVisibleMenuIndex > byMenuCount )
				--m_byVisibleMenuIndex;

			if( m_listMenuItem.size() - m_byVisibleMenuIndex > dMENU_VISIBLE_COUNT )
				--m_byVisibleMenuIndex;

			ResizeDilaog();
			break;
		}
	}

	Menu_JugdeVisiblePreNextButton();
}

VOID CScouterMenuGui::Menu_JugdeVisiblePreNextButton()
{
	if( m_listMenuItem.size() > dMENU_VISIBLE_COUNT )
	{
		m_pFlashPrePageButton->Show(true);
		m_pFlashNextPageButton->Show(true);
	}
	else
	{
		m_pFlashPrePageButton->Show(false);
		m_pFlashNextPageButton->Show(false);
	}
}

VOID CScouterMenuGui::ResizeDilaog()
{
	CRectangle rect = GetPosition();
	CScouterMenuItem* pMenuItem = *( m_listMenuItem.begin() );
	RwInt32 iHeight = m_pFlashPrePageButton->GetHeight() + dMENU_YPOS_GAP +					  
		( m_listMenuItem.size() * pMenuItem->GetPosition().GetHeight() ) +
		m_pFlashNextPageButton->GetHeight();

	if( m_listMenuItem.size() > 0 )
		iHeight += ( (m_listMenuItem.size()-1) * dMENU_YPOS_GAP );

	rect.right	= rect.left + dDIALOG_WIDTH;
	rect.bottom	= rect.top + iHeight;

	SetPosition(rect);
}

RwUInt8 CScouterMenuGui::PartsTypeToMenuIndex(RwUInt8 byType)
{
	switch(byType)
	{
	case SCOUTER_PARTS_ITEM_IDENTIFICATION:			return SCOUTERMENU_IDENTIFICATION_ITEM;
	case SCOUTER_PARTS_PC_INFO_VIEWER:				return SCOUTERMENU_VIEW_PC_EQUIP_ITEM;
	case SCOUTER_PARTS_OBJECT_FINDER:				return SCOUTERMENU_FIND_HIDDEN_OBJECT;
	case SCOUTER_PARTS_QUEST_SEARCH:				return SCOUTERMENU_QUEST_SEARCH;
	}

	return INVALID_SCOUTERMENU;
}

VOID CScouterMenuGui::OnClick_MenuItem(RwUInt8 byMenuType)
{
	if( byMenuType == SCOUTERMENU_MEASURE_POWER )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SCOUTER_MODE_POWERLEVEL ) )
			return;
	}	

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwUInt32 uiSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);
	CNtlSobItem* pScouterBody = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(uiSerial));		

	// 스카우터가 없다면 리턴
	if(!pScouterBody)
		return;

	// 스카우터가 망가졌다면 쓸 수 없다
	CNtlSobItemAttr* pScouterBodyAttr = reinterpret_cast<CNtlSobItemAttr*>( pScouterBody->GetSobAttr() );
	if( pScouterBodyAttr->GetDur() < dENABLED_SCOUTER_DURABILITY )
		return;

	switch(byMenuType)
	{
	case SCOUTERMENU_MEASURE_POWER:
		{
			// 전투력 측정 시작
			CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_MEASURE_POWER_START);
			break;
		}
	case SCOUTERMENU_IDENTIFICATION_ITEM:
		{
			eDialogMode eMode = GetDialogManager()->GetMode();

			// 미확인 아이템 감정
			if( eMode == DIALOGMODE_UNKNOWN )
			{
				GetDialogManager()->OnMode(DIALOGMODE_ITEM_IDENTIFICATION);
			}
			else
			{
				Logic_DialogModeMessage(eMode);
			}
			break;
		}
	case SCOUTERMENU_VIEW_PC_EQUIP_ITEM:
		{
			SERIAL_HANDLE hTargetSerial = GetDboGlobal()->GetTargetSerial();
			if( hTargetSerial == INVALID_SERIAL_ID )
				return;

			if( Logic_GetAvatarHandle() == hTargetSerial )
			{
				// 자기 자신은 볼 수 없다
				// 메세지
				return;
			}

			// PC의 장비 보기
			GetDboGlobal()->GetGamePacketGenerator()->SendPCInfoView(hTargetSerial);

			break;
		}
	case SCOUTERMENU_FIND_HIDDEN_OBJECT:
		{
			// 숨겨진 오브젝트 찾기
			CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_FIND_OBJECT);
			break;
		}
	case SCOUTERMENU_QUEST_SEARCH:
		{
			if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE) )
			{
				Logic_WarnningWorldConceptActionMessage(WORLD_CONCEPT_FIRST_GRADE);
				return;
			}

			// 주변 퀘스트 찾기
			CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_QUEST_SEARCH);
			break;
		}
	default:
		{
			DBO_FAIL("Invalid scouter menu type : " << byMenuType );
			break;
		}
	}
}

VOID CScouterMenuGui::ArrangeMenuButtons()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iYPos = m_pFlashPrePageButton->GetHeight() + dMENU_YPOS_GAP;
	RwInt32 iXPos = 0;
	RwInt8 byVisibleCount = 0;
	RwInt32 byMenuCount = 0;

	m_pFlashExitButton->SetPosition(131, 26);
	m_pFlashExitButton->Raise();

	m_pFlashPrePageButton->SetPosition(0, 0);

	LIST_MENUITEM_ITER it = m_listMenuItem.begin();
	for( ; it != m_listMenuItem.end() ; ++it, ++byMenuCount )
	{
		CScouterMenuItem* pMenuItem = *it;

		if( byMenuCount < m_byVisibleMenuIndex || byVisibleCount >= dMENU_VISIBLE_COUNT )
		{
			pMenuItem->Show(false);
			continue;
		}		

		// X좌표는 하드코딩
		if( byVisibleCount == 0 )				iXPos = 0;
		else if( byVisibleCount == 1 )			iXPos = 7;
		else if( byVisibleCount == 2 )			iXPos = 12;
		else if( byVisibleCount == 3 )			iXPos = 9;
		else if( byVisibleCount == 4 )			iXPos = 2;

		CRectangle rect;
		CRectangle rtButton = pMenuItem->GetPosition();

		rect.left	= iXPos;
		rect.top	= iYPos;
		rect.right	= rect.left + rtButton.GetWidth();
		rect.bottom	= rect.top + rtButton.GetHeight();

		pMenuItem->SetPosition(rect);
		pMenuItem->Show(true);

		++byVisibleCount;
		iYPos += rtButton.GetHeight() + dMENU_YPOS_GAP;
	}

	m_pFlashNextPageButton->SetPosition(5, iYPos);
}

RwInt32 CScouterMenuGui::KeyboardUpHandler(unsigned int uiKeyData)
{
	if( FALSE == IsShow() )
		return 1;


	RwInt8		byCount	= 0;
	SKeyData*	pData	= (SKeyData*)uiKeyData;

	for each( CScouterMenuItem* pScouterMenuItem in m_listMenuItem )
	{
		if( FALSE == pScouterMenuItem->IsShow() )
			continue;

		if( dMENU_VISIBLE_COUNT <= byCount )
			break;

		if( byCount == 0 && dSHORTCUT_KEY_1 == pData->uiChar )
		{
			OnClick_MenuItem( pScouterMenuItem->GetMenuType() );
			return 1;
		}
		if( byCount == 1 && dSHORTCUT_KEY_2 == pData->uiChar )
		{
			OnClick_MenuItem( pScouterMenuItem->GetMenuType() );
			return 1;
		}
		if( byCount == 2 && dSHORTCUT_KEY_3 == pData->uiChar )
		{
			OnClick_MenuItem( pScouterMenuItem->GetMenuType() );
			return 1;
		}
		if( byCount == 3 && dSHORTCUT_KEY_4 == pData->uiChar )
		{
			OnClick_MenuItem( pScouterMenuItem->GetMenuType() );
			return 1;
		}
		if( byCount == 4 && dSHORTCUT_KEY_5 == pData->uiChar )
		{
			OnClick_MenuItem( pScouterMenuItem->GetMenuType() );
			return 1;
		}

		++byCount;
	}

	return 1;
}

VOID CScouterMenuGui::OnClicked_CloseButton(const char* pcParam, const char* pcParam2)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SCOUTER_MODE_CLOSE_BTN ) )
		return;

	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_EXIT, Logic_GetAvatarHandle());
}

VOID CScouterMenuGui::OnClicked_PrePageButton(const char* pcParam, const char* pcParam2)
{
	if( 0 <= m_byVisibleMenuIndex && m_byVisibleMenuIndex < dMENU_VISIBLE_COUNT )
		m_byVisibleMenuIndex = 0;
	else if( m_byVisibleMenuIndex > 0 )
		m_byVisibleMenuIndex -= dMENU_VISIBLE_COUNT;

	ArrangeMenuButtons();
}

VOID CScouterMenuGui::OnClicked_NextButton(const char* pcParam, const char* pcParam2)
{
	if( (m_byVisibleMenuIndex + dMENU_VISIBLE_COUNT ) < (RwUInt8)(m_listMenuItem.size()) )
	{
		m_byVisibleMenuIndex += dMENU_VISIBLE_COUNT;
		ArrangeMenuButtons();
	}
}

VOID CScouterMenuGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	ArrangeMenuButtons();
}

RwInt32 CScouterMenuGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{	
		if( m_pFlashExitButton->Load(dEXIT_BUTTON_NAME) )
		{
			m_pFlashExitButton->SetPosition( m_pFlashExitButton->GetFrameResolution() );
			m_pFlashExitButton->SetFrameCallback(FALSE);
			m_pFlashExitButton->PlayMovie(TRUE);
		}

		if( m_pFlashPrePageButton->Load(dPREPAGE_BUTTON_NAME) )
		{
			m_pFlashPrePageButton->SetPosition( m_pFlashPrePageButton->GetFrameResolution() );
			m_pFlashPrePageButton->SetFrameCallback(FALSE);
			m_pFlashPrePageButton->PlayMovie(TRUE);
		}

		if( m_pFlashNextPageButton->Load(dNEXTPAGE_BUTTON_NAME) )
		{
			m_pFlashNextPageButton->SetPosition( m_pFlashNextPageButton->GetFrameResolution() );
			m_pFlashNextPageButton->SetFrameCallback(FALSE);
			m_pFlashNextPageButton->PlayMovie(TRUE);
		}

		// 전투력 측정은 기본적으로 제공이 된다
		Menu_Add(SCOUTERMENU_MEASURE_POWER);

		Menu_Update();

		// 전투력 측정 메뉴만이 있을 경우를 대비하여 한번 호출한다
		ArrangeMenuButtons();

		GetDialogManager()->OpenDialog(DIALOG_SCOUTER_BACKGROUND);
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_SCOUTER_USE);

		GetInputActionMap()->RegisterIgnoreKey(dSHORTCUT_KEY_1, ACTIONMAP_IGNORE_KEY_UNLIMITED_IGNORE);
		GetInputActionMap()->RegisterIgnoreKey(dSHORTCUT_KEY_2, ACTIONMAP_IGNORE_KEY_UNLIMITED_IGNORE);
		GetInputActionMap()->RegisterIgnoreKey(dSHORTCUT_KEY_3, ACTIONMAP_IGNORE_KEY_UNLIMITED_IGNORE);
		GetInputActionMap()->RegisterIgnoreKey(dSHORTCUT_KEY_4, ACTIONMAP_IGNORE_KEY_UNLIMITED_IGNORE);
		GetInputActionMap()->RegisterIgnoreKey(dSHORTCUT_KEY_5, ACTIONMAP_IGNORE_KEY_UNLIMITED_IGNORE);

		Show(true);
	}
	else
	{
		Clear();
		GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BACKGROUND);
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_SCOUTER_CLOSE);

		GetInputActionMap()->RemoveIgnoreKey(dSHORTCUT_KEY_1);
		GetInputActionMap()->RemoveIgnoreKey(dSHORTCUT_KEY_2);
		GetInputActionMap()->RemoveIgnoreKey(dSHORTCUT_KEY_3);
		GetInputActionMap()->RemoveIgnoreKey(dSHORTCUT_KEY_4);
		GetInputActionMap()->RemoveIgnoreKey(dSHORTCUT_KEY_5);

		eDialogMode eMode = GetDialogManager()->GetMode();
		if( eMode == DIALOGMODE_ITEM_IDENTIFICATION )
			GetDialogManager()->OffMode();

		Show(false);
	}

	return 1;
}

VOID CScouterMenuGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CScouterMenuGui::HandleEvents");

	if(msg.Id == g_EventScouter)
	{
		SDboEventScouter* pEvent = (SDboEventScouter*)msg.pData;
		switch(pEvent->iType)
		{
		case SCOUTER_EVENT_FSM_ENTER:
			{
				GetDialogManager()->OpenDialog(DIALOG_SCOUTER_MENU);
				break;
			}
		case SCOUTER_EVENT_EXIT_AFTER_OUT_OF_ORDER:
		case SCOUTER_EVENT_UNSET_SCOUTER:		
			{
				GetDialogManager()->CloseDialog(DIALOG_SCOUTER_MENU);
				break;
			}
		case SCOUTER_EVENT_FSM_EXIT:
			{
				if( Logic_GetAvatarHandle() == pEvent->uiUserData )
					GetDialogManager()->CloseDialog(DIALOG_SCOUTER_MENU);

				break;
			}
		}
	}

	NTL_RETURNVOID();
}