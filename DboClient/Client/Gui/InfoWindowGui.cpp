#include "precomp_dboclient.h"
#include "InfoWindowGui.h"

// Presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobActionSkill.h"
#include "NtlSobActionSkillAttr.h"
#include "NtlSobActionSkillIcon.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLLogic.h"
#include "NtlBuffContainer.h"
#include "InputActionMap.h"

// Shared and Table
#include "NtlObject.h"
#include "NtlSLApi.h"
#include "ItemTable.h"
#include "ItemOptionTable.h"
#include "SkillTable.h"
#include "UseItemTable.h"
#include "HTBSetTable.h"
#include "SystemEffectTable.h"
#include "ActionTable.h"
#include "ChatCommandTable.h"
#include "QuestItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "CharmTable.h"
#include "PortalTable.h"
#include "SetItemTable.h"
#include "WorldMapTable.h"
#include "QuestTextDataTable.h"
#include "NtlBattle.h"
#include "VehicleTable.h"
#include "NtlMovement.h"
#include "ItemRecipeTable.h"

// Dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringDef.h"
#include "DisplayStringManager.h"
#include "DialogDefine.h"
#include "DboLogic.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "PopupManager.h"
#include "SkillWindowGui.h"
#include "PartyMenuGui.h"
#include "DialogManager.h"
#include "ScouterBodyGui.h"
#include "PortalGui.h"
#include "GuiFunctor.h"
#include "SkillCustomizeItem.h"

// Define
#define INFOWND_NULLCHECK( p, s )	if( !p ) { m_pmdBox->Clear(); if( Logic_IsUIDevInfoVisible() ){ m_pmdBox->SetItem( s, "Error", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_0 ); } return; }

#define INFOWND_POINT_MARGIN_X	5
#define INFOWND_POINT_MARGIN_Y	5
#define INFOWND_BLANKLINE_HEIGHT 5

#define FONT_TITLE	1
#define FONT_TEXT	0

const RwUInt32 flagHuman =	(PC_CLASS_FLAG_HUMAN_FIGHTER|PC_CLASS_FLAG_HUMAN_MYSTIC|PC_CLASS_FLAG_HUMAN_ENGINEER|
							 PC_CLASS_FLAG_STREET_FIGHTER|PC_CLASS_FLAG_SWORD_MASTER|PC_CLASS_FLAG_CRANE_ROSHI|
							 PC_CLASS_FLAG_TURTLE_ROSHI|PC_CLASS_FLAG_GUN_MANIA|PC_CLASS_FLAG_MECH_MANIA);
const RwUInt32 flagNamek =  (PC_CLASS_FLAG_NAMEK_FIGHTER|PC_CLASS_FLAG_NAMEK_MYSTIC|PC_CLASS_FLAG_DARK_WARRIOR|
							 PC_CLASS_FLAG_SHADOW_KNIGHT|PC_CLASS_FLAG_POCO_SUMMONER|PC_CLASS_FLAG_DENDEN_HEALER);
const RwUInt32 flagMajin =  (PC_CLASS_FLAG_MIGHTY_MAJIN|PC_CLASS_FLAG_WONDER_MAJIN|PC_CLASS_FLAG_GRAND_MA|
							 PC_CLASS_FLAG_ULTI_MA|PC_CLASS_FLAG_PLAS_MA|PC_CLASS_FLAG_KAR_MA);
const RwUInt32 flagGloveUse = flagMajin | flagHuman | PC_CLASS_FLAG_NAMEK_FIGHTER | PC_CLASS_FLAG_DARK_WARRIOR | PC_CLASS_FLAG_SHADOW_KNIGHT;

#define COLOR_NORMAL				RGB( 254, 254, 254 )
#define COLOR_DISABLE				RGB( 180, 180, 180 )
#define COLOR_NUMERIC				RGB( 255, 255, 255 )

#define COLOR_SKILL_NAME			RGB( 255, 255, 255 )
#define COLOR_SKILL_GRADE			RGB( 255, 255, 255 )
#define COLOR_SKILL_ACTIVE			RGB( 255, 255, 255 )
#define COLOR_SKILL_PASSIVE			RGB( 255, 255, 255 )
#define COLOR_SKILL_REQUIRELP		RGB( 255, 255, 255 )
#define COLOR_SKILL_REQUIREEP		RGB( 255, 255, 255 )
#define COLOR_SKILL_REQUIRERP		RGB( 255, 255, 255 )
#define COLOR_SKILL_EXP				RGB( 255, 255, 255 )
#define COLOR_SKILL_NEXTNAME		RGB( 255, 255, 255 )
#define COLOR_SKILL_NEXTGRADE		RGB( 255, 255, 255 )
#define COLOR_SKILL_RQLEVEL			RGB( 255, 255, 255 )	
#define COLOR_SKILL_RQSKILL			RGB( 255, 255, 255 )	

#define COLOR_ITEM_NAME_NORMAL		RGB( 255, 255, 255 )
#define COLOR_ITEM_NAME_RARE		RGB( 255, 255, 255 )
#define COLOR_ITEM_NAME_SPECIAL		RGB( 255, 255, 255 )
#define COLOR_ITEM_NAME_UNIQUE		RGB( 255, 255, 255 )
#define COLOR_ITEM_EQUIP_ENABLE		RGB( 255, 255, 255 )
#define COLOR_ITEM_EQUIP_UNABLE		RGB( 255, 255, 255 )
#define COLOR_ITEM_ATTR_NONE		RGB( 255, 255, 255 )
#define COLOR_ITEM_ATTR_HONEST		RGB( 255, 255, 255 )
#define COLOR_ITEM_ATTR_STRANGE		RGB( 255, 255, 255 )
#define COLOR_ITEM_ATTR_WILD		RGB( 255, 255, 255 )
#define COLOR_ITEM_ATTR_ELEGANCE	RGB( 255, 255, 255 )
#define COLOR_ITEM_ATTR_FUNNY		RGB( 255, 255, 255 )
#define COLOR_ITEM_ENDURANCE_HIGH	RGB( 255, 255, 255 )
#define COLOR_ITEM_ENDURANCE_NORMAL	RGB( 255, 255, 255 )
#define COLOR_ITEM_ENDURANCE_LOW	RGB( 255, 255, 255 )
#define COLOR_ITEM_EQUIPCONDITION	RGB( 255, 255, 255 )
#define COLOR_ITEM_COST				RGB( 255, 255, 255 )
#define COLOR_ITEM_SET_ARI			RGB( 255, 255, 255 )
#define COLOR_ITEM_SET_NASI			RGB( 255, 255, 255 )
#define COLOR_ITEM_SET_BONUSEFFECT	RGB( 255, 255, 255 )

#define COLOR_QUEST					RGB( 255, 255,   0 )

////////////////////////////////////////////////////////////////////////////////
//

CInfoWindowGui::CInfoWindowGui(VOID)
{
	Init();
}

CInfoWindowGui::CInfoWindowGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CInfoWindowGui::~CInfoWindowGui(VOID)
{

}

////////////////////////////////////////////////////////////////////////////////
//

VOID CInfoWindowGui::Init(VOID)
{
	m_pmdBox = NULL;
}

RwBool CInfoWindowGui::Create(VOID)
{
	NTL_FUNCTION( "CInfowindowGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\InfoWnd.srf", "gui\\InfoWnd.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pmdBox = (gui::CMDStaticBox*)GetComponent( "mdbInfo" );

	m_pEpuipDlg[0] = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain2" ) );
	m_pmdEquipInfo[0] = reinterpret_cast<gui::CMDStaticBox*>( GetComponent( "mdbInfo2" ) );
	m_pEpuipDlg[1] = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain3" ) );
	m_pmdEquipInfo[1] = reinterpret_cast<gui::CMDStaticBox*>( GetComponent( "mdbInfo3" ) );

	m_pThis->Show( false );
	m_pThis->SetRenderTop( true );
	m_pmdBox->SetRenderTop( true );
	m_pEpuipDlg[0]->SetRenderTop( true );
	m_pEpuipDlg[1]->SetRenderTop( true );
	m_pmdEquipInfo[0]->SetRenderTop( true );
	m_pmdEquipInfo[1]->SetRenderTop( true );
	m_pEpuipDlg[0]->Show( false );
	m_pEpuipDlg[1]->Show( false );

	NTL_RETURN( TRUE );
}

VOID CInfoWindowGui::Destroy(VOID)
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CInfoWindowGui::Show( bool bShow )
{
	CNtlPLGui::Show( bShow );

	if( !bShow )
	{
		m_pEpuipDlg[0]->Show( bShow );
		m_pEpuipDlg[1]->Show( bShow );
	}
}

////////////////////////////////////////////////////////////////////////////////
//

VOID CInfoWindowGui::CalcPos( RwInt32 nXPos, RwInt32 nYPos )
{
#define INFOWND_EQUIP_INFO_OFFSET	2

	RwInt32 nScreenWidth = m_pThis->GetGuiManager()->GetWidth();
	RwInt32 nScreenHeight = m_pThis->GetGuiManager()->GetHeight();
	CRectangle rtBox = m_pmdBox->GetClientRect();

	// 기본위치는 Right Bottom
	RwInt32 nX = nXPos + NTL_ITEM_ICON_SIZE + INFOWND_POINT_MARGIN_X;
	RwInt32 nY = nYPos + NTL_ITEM_ICON_SIZE + INFOWND_POINT_MARGIN_Y;
	RwInt32 nWidth = rtBox.GetWidth(); 
	RwInt32 nHeight = rtBox.GetHeight();
	RwInt32 nEquipWidth = 0;
	RwInt32 nEquipHeight = 0;	
	RwBool bLeft = FALSE;
	
	if( m_pEpuipDlg[0]->IsVisible() )
	{
		nEquipWidth = m_pmdEquipInfo[0]->GetWidth() + INFOWND_EQUIP_INFO_OFFSET;
		nEquipHeight = m_pmdEquipInfo[0]->GetHeight() + INFOWND_EQUIP_INFO_OFFSET;

		if( m_pEpuipDlg[1]->IsVisible() )
		{
			nEquipWidth += m_pmdEquipInfo[1]->GetWidth() + INFOWND_EQUIP_INFO_OFFSET;
			
			if( m_pmdEquipInfo[0]->GetHeight() < m_pmdEquipInfo[1]->GetHeight() )
				nEquipHeight = m_pmdEquipInfo[1]->GetHeight();
		}
	}
	
	if( nX + nWidth + nEquipWidth > nScreenWidth )
	{
		nX = nXPos - nWidth - nEquipWidth - INFOWND_POINT_MARGIN_X;

		if( nX < 0 )
			nX = INFOWND_POINT_MARGIN_X;

		bLeft = TRUE;
	}

	if( nY + nHeight > nScreenHeight )
	{
		nY = nYPos - nHeight - INFOWND_POINT_MARGIN_Y;
		
		if( nY + nEquipHeight > nScreenHeight )
			nY -= nEquipHeight - nHeight + INFOWND_POINT_MARGIN_Y;

		if( nY < 0 )
			nY = INFOWND_POINT_MARGIN_Y;
	}
	else if( nY + nEquipHeight > nScreenHeight )
	{
		nY -= nEquipHeight - nHeight + INFOWND_POINT_MARGIN_Y;
	}

	if( bLeft )
	{
		CRectangle rect;

		rect.SetRectWH( nX + nEquipWidth, nY, nWidth, nHeight );
		SetPosition( rect );

		if( m_pEpuipDlg[0]->IsVisible() )
		{
			rect.SetRectWH( nX + nEquipWidth - m_pmdEquipInfo[0]->GetWidth() - INFOWND_EQUIP_INFO_OFFSET, nY, m_pmdEquipInfo[0]->GetWidth(), m_pmdEquipInfo[0]->GetHeight() );
			m_pEpuipDlg[0]->SetPosition( rect );

			if( m_pEpuipDlg[1]->IsVisible() )
			{
				rect.SetRectWH( nX + nEquipWidth - m_pmdEquipInfo[0]->GetWidth() - INFOWND_EQUIP_INFO_OFFSET - m_pmdEquipInfo[1]->GetWidth() - INFOWND_EQUIP_INFO_OFFSET, nY, m_pmdEquipInfo[1]->GetWidth(), m_pmdEquipInfo[1]->GetHeight() );
				m_pEpuipDlg[1]->SetPosition( rect );
			}
		}
	}
	else
	{
		CRectangle rect;

		rect.SetRectWH( nX, nY, nWidth, nHeight );
		SetPosition( CRectangle( nX, nY, nX + nWidth, nY + nHeight ) );		

		if( m_pEpuipDlg[0]->IsVisible() )
		{
			rect.SetRectWH( nX + nWidth + INFOWND_EQUIP_INFO_OFFSET, nY, m_pmdEquipInfo[0]->GetWidth(), m_pmdEquipInfo[0]->GetHeight() );
			m_pEpuipDlg[0]->SetPosition( rect );

			if( m_pEpuipDlg[1]->IsVisible() )
			{
				rect.SetRectWH( nX + nWidth + INFOWND_EQUIP_INFO_OFFSET + m_pmdEquipInfo[0]->GetWidth() + INFOWND_EQUIP_INFO_OFFSET, nY, m_pmdEquipInfo[1]->GetWidth(), m_pmdEquipInfo[1]->GetHeight() );
				m_pEpuipDlg[1]->SetPosition( rect );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//

VOID CInfoWindowGui::SetItemInfo( CNtlSobItem* pItem )
{
	INFOWND_NULLCHECK( pItem, "SetItemInfo CNtlSobItem* pItem is NULL" );

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
	if( pItemAttr->IsNeedToIdentify() )
	{
		SetUnIdentifiedInfo();
		return;
	}

	SetItemInfo( m_pmdBox, pItem, pItemAttr );
	EquipInfoProc( pItemAttr->GetItemTbl() );
}

VOID CInfoWindowGui::SetTableItemInfo( sITEM_TBLDAT* pData )
{
	INFOWND_NULLCHECK( pData, "SetTableItemInfo sITEM_TBLDAT* pData is NULL" );

	SetTableItemInfo( m_pmdBox, pData );
	EquipInfoProc( pData );
}

VOID CInfoWindowGui::SetProfileItemInfo( sITEM_PROFILE* pItemProfile )
{
	INFOWND_NULLCHECK( pItemProfile, "SetProfileItemInfo sITEM_PROFILE* pItemProfile is NULL" );

	if( pItemProfile->bNeedToIdentify )
	{
		SetUnIdentifiedInfo();
		return;		
	}

	sITEM_TBLDAT* pData = Logic_GetItemDataFromTable( pItemProfile->tblidx );
	if( !pData )
	{
		NTL_ASSERTFAIL( "CInfoWindowGui::SetProfileItemInfo : Invalid Item table Index" << pItemProfile->tblidx );
		return;
	}

	SetProfileItemInfo( m_pmdBox, pData, pItemProfile );
	EquipInfoProc( pData );
}

VOID CInfoWindowGui::SetItemInfo( gui::CMDStaticBox* pmdBox, CNtlSobItem* pItem, CNtlSobItemAttr* pItemAttr, RwBool bDoClear /* = TRUE  */)
{
	sITEM_TBLDAT* pData = pItemAttr->GetItemTbl();
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	RwBool bSpecialEffect = FALSE;
	RwBool bBlankLine = FALSE;

	if( bDoClear )
		pmdBox->Clear();
	////////////////////////////////////////////////////////////////////////////
	// Item

	SetItemInfo_Name( pmdBox, pData, pItemAttr->GetGrade(), pItemAttr->GetRank() );		

	SetItemInfo_Attr_Info( pmdBox, pData, pItemAttr->GetBattleAttribute() );

	switch( pData->byItem_Type )
	{
	case ITEM_TYPE_CAPSULE:
		{
			// 일단 캡슐중 탈 것만 추가한다
			RwReal fSystemEffectValue = Logic_GetItemSystemEffectValue(pData, ACTIVE_VEHICLE);
			if( 0.f == fSystemEffectValue )
				break;

			TBLIDX tblidx = (TBLIDX)fSystemEffectValue;
			sVEHICLE_TBLDAT* pVEHICLE_TBLDAT = (sVEHICLE_TBLDAT*)API_GetTableContainer()->GetVehicleTable()->FindData(tblidx);
			if( !pVEHICLE_TBLDAT )
			{
				NTL_ASSERT(false, "CInfoWindowGui::SetItemInfo, Not exist VEHICLE_TABLE of index : " << tblidx);
				break;
			}

			pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
			pmdBox->Format( "VehicleSpeed", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, GetDisplayStringManager()->GetString(DST_ITEM_MOVE_SPEED), pVEHICLE_TBLDAT->bySpeed);
			break;
		}
	case ITEM_TYPE_RECIPE:
		{
			SetItemInfo_RecipeInfo( pmdBox, pData );
		}
		break;
	default:
		break;
	}

	SetItemInfo_EquipLimit( pmdBox, pData, pAvatarAttr );

	bSpecialEffect = SetItemInfo_SpecialClass( pmdBox, pData, pAvatarAttr );

	SetItemInfo_Parameter( pmdBox, pData, pItemAttr->GetGrade() );	

	if( bSpecialEffect )
	{
		SetItemInfo_SpecialEffect( pmdBox );
		bBlankLine = FALSE;
	}
	else
		bBlankLine = TRUE;

	bBlankLine = SetItemInfo_Option( pmdBox, pData, pItemAttr->GetOptionIdx1(), bBlankLine, bBlankLine, 1 );
	SetItemInfo_Option( pmdBox, pData, pItemAttr->GetOptionIdx2(), bBlankLine, bBlankLine, 2 );

	SetItemInfo_ScouterInfo( pmdBox, pItem, pData );
	
	SetItemInfo_Durablity( pmdBox, pItemAttr->GetDur(), pItemAttr->GetMaxDur() );

	SetItemInfo_SetItem( pmdBox, pData, pAvatar );	

	SetItemInfo_LimitState( pmdBox, pData, pItemAttr );

	if( GetDialogManager()->IsOpenDialog( DIALOG_NPCSHOP ) )		
	{
		SetItemInfo_SellPrice( pmdBox, pData, pItemAttr->GetStackNum() );

		SetItemInfo_RepairCost( pmdBox, pData, pItemAttr->GetDur(), pItemAttr->GetMaxDur() );
	}
	else if( GetDialogManager()->IsMode(DIALOGMODE_ITEM_REPAIR) )
		SetItemInfo_RepairCost( pmdBox, pData, pItemAttr->GetDur(), pItemAttr->GetMaxDur() );

	////////////////////////////////////////////////////////////////////////////
	pmdBox->DrawItem();
}

////////////////////////////////////////////////////////////////////////////////
//

VOID CInfoWindowGui::SetProfileItemInfo( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, sITEM_PROFILE* pItemProfile, RwBool bDoClear /* = TRUE  */)
{
	RwBool bSpecialEffect = FALSE;
	RwBool bBlankLine = FALSE;
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
	
	if( bDoClear )
		pmdBox->Clear();

	////////////////////////////////////////////////////////////////////////////
	// Item

	SetItemInfo_Name( pmdBox, pData, pItemProfile->byGrade, pItemProfile->byRank );

	SetItemInfo_Attr_Info( pmdBox, pData, pItemProfile->byBattleAttribute );

	SetItemInfo_EquipLimit( pmdBox, pData, pAvatarAttr );

	bSpecialEffect = SetItemInfo_SpecialClass( pmdBox, pData, pAvatarAttr );

	SetItemInfo_Parameter( pmdBox, pData, pItemProfile->byGrade );	

	if( bSpecialEffect )
	{
		SetItemInfo_SpecialEffect( pmdBox );
		bBlankLine = FALSE;
	}
	else
		bBlankLine = TRUE;

	bBlankLine = SetItemInfo_Option( pmdBox, pData, pItemProfile->aOptionTblidx[0], bBlankLine, bBlankLine, 1 );
	SetItemInfo_Option( pmdBox, pData, pItemProfile->aOptionTblidx[1], bBlankLine, bBlankLine, 2 );

	SetItemInfo_ScouterInfo( pmdBox, NULL, pData );

	SetItemInfo_Durablity( pmdBox, pItemProfile->byCurDur, pData->byDurability );

	SetItemInfo_SetItem( pmdBox, pData, pAvatar );

	SetItemInfo_LimitState( pmdBox, pData, NULL, pItemProfile );

	if( GetDialogManager()->IsOpenDialog( DIALOG_NPCSHOP ) )
	{
		SetItemInfo_SellPrice( pmdBox, pData, pItemProfile->byStackcount );

		SetItemInfo_RepairCost( pmdBox, pData, pItemProfile->byCurDur, pData->byDurability );
	}
	else if( GetDialogManager()->IsMode(DIALOGMODE_ITEM_REPAIR) )
		SetItemInfo_RepairCost( pmdBox, pData, pItemProfile->byCurDur, pData->byDurability );

	////////////////////////////////////////////////////////////////////////////
	pmdBox->DrawItem();
}

////////////////////////////////////////////////////////////////////////////////
//

VOID CInfoWindowGui::SetTableItemInfo( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwBool bDoClear /* = TRUE  */)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	RwBool bSpecialEffect = FALSE;
	RwBool bBlankLine = FALSE;

	if( bDoClear )
		pmdBox->Clear();
	////////////////////////////////////////////////////////////////////////////
	// Item

	SetItemInfo_Name( pmdBox, pData, 0, pData->byRank );							

	SetItemInfo_Attr_Info( pmdBox, pData, pData->byBattle_Attribute );

	SetItemInfo_EquipLimit( pmdBox, pData, pAvatarAttr );

	bSpecialEffect = SetItemInfo_SpecialClass( pmdBox, pData, pAvatarAttr );

	SetItemInfo_Parameter( pmdBox, pData, 0 );						

	if( bSpecialEffect )
	{
		SetItemInfo_SpecialEffect( pmdBox );
		bBlankLine = FALSE;
	}
	else
		bBlankLine = TRUE;

	SetItemInfo_Option( pmdBox, pData, pData->Item_Option_Tblidx, TRUE, bBlankLine );

	SetItemInfo_ScouterInfo( pmdBox, NULL, pData );

	SetItemInfo_Durablity( pmdBox, pData->byDurability, pData->byDurability, TRUE );

	SetItemInfo_SetItem( pmdBox, pData, pAvatar );

	SetItemInfo_LimitState( pmdBox, pData );

	////////////////////////////////////////////////////////////////////////////
	pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetJustItemNameInfo( sITEM_TBLDAT* pData )
{
	INFOWND_NULLCHECK( pData, "SetJustItemNameInfo sITEM_TBLDAT* pData is NULL" );

	m_pmdBox->Clear();

	SetItemInfo_Name( m_pmdBox, pData, 0, pData->byRank );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetQuestItemInfo( sQUESTITEM_TBLDAT* pData )
{
	INFOWND_NULLCHECK( pData, "SetQuestItemInfo sQUESTITEM_TBLDAT* pData is NULL" );

	CTextTable* pQuestItemText = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();

	m_pmdBox->Clear();

	m_pmdBox->SetItem( pQuestItemText->GetText( pData->ItemName ).c_str(), "Title", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_0 );

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	m_pmdBox->SetItem( pQuestItemText->GetText( pData->Note ).c_str(), "Note", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetUnIdentifiedInfo(VOID)
{
	m_pmdBox->Clear();

	m_pmdBox->SetItem( const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_UNIDENTIFIED ) ), "Name", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_0 );

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	m_pmdBox->SetItem( GetDisplayStringManager()->GetString(DST_ITEM_IDENTIFY_MUST_IDENTIFY), "Text", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );

	m_pmdBox->Format( "identifycost", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, GetDisplayStringManager()->GetString( DST_ITEM_IDENTIFY_COST ), NTL_SHOP_ITEM_IDENTIFY_ZENNY );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetSkillInfo( CNtlSobSkill* pSkill )
{
	INFOWND_NULLCHECK( pSkill, "SetSkillInfo CNtlSobSkill* pSkill is NULL"  );

	CNtlSobSkillAttr* pAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSkill->GetSobAttr() );
	sSKILL_TBLDAT* pData = pAttr->GetSkillTbl();

	WCHAR* pString = NULL;

	m_pmdBox->Clear();
	////////////////////////////////////////////////////////////////////////////
	// Current Skill

	SetSkillInfo_NameGradeClass( pData, TRUE );

	SetSkillInfo_UseCondition( pData, pAttr->GetBaseSkillTbl(), FALSE, pSkill );

	SetSkillInfo_EffectData( pData, INFOCOLOR_7, 0, pSkill );

	SetSkillInfo_AppointApply( pData, pAttr->GetBaseSkillTbl() );

	SetSkillInfo_Parameter( pSkill, pData, pAttr->GetBaseSkillTbl(), FALSE );

	////////////////////////////////////////////////////////////////////////////
	//	Next Skill

	if( GetInfoWndManager()->GetRequestGui() != DIALOG_QUICKSLOT )
	{
		if( pData->dwNextSkillTblidx != INVALID_TBLIDX )
		{
			//// 숙련도
			//m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
			//pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_EXP ) );
			//m_pmdBox->Format( "SkillExp", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_4, 0, FALSE, pString, (FLOAT)pAttr->m_uiExp / pAttr->m_uiMaxExp * 100.0f );

			CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT* pNextData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pData->dwNextSkillTblidx ) );

			// 등급
			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_GRADE ) );
			m_pmdBox->Format( "NextSkillGrade", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_9, 0, FALSE, pString, pNextData->bySkill_Grade );

			SetSkillInfo_LearnCondition( pNextData, TRUE );

			SetSkillInfo_UseCondition( pNextData, pNextData, TRUE );

			SetSkillInfo_EffectData( pNextData, INFOCOLOR_9, 1 );

			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			SetSkillInfo_Parameter( NULL, pNextData, pNextData, TRUE );
		}
	}

	////////////////////////////////////////////////////////////////////////////
	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetRecipeInfo( sITEM_RECIPE_TBLDAT* pRecipeTblData )
{
	if( pRecipeTblData )
	{
		m_pmdBox->Clear();

		// 레시피 이름
		sTEXT_TBLDAT* pTblData = (sTEXT_TBLDAT*)API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->FindData( pRecipeTblData->dwName );
		if( pTblData )
		{
			m_pmdBox->SetItem( pTblData->wstrText.c_str(), "RecipeName", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_0 );
		}
		else
		{
			WCHAR awcBuffer[128];
			swprintf_s(awcBuffer, 128, L"Not exist ITEM_TEXT_TABLE of index : %d", pRecipeTblData->dwName );
			m_pmdBox->SetItem( awcBuffer, "RecipeName", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_0 );
		}

		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

		//  레시피 타입
		m_pmdBox->SetItem( Logic_GetHoipoiMixSkillName( pRecipeTblData->byRecipeType ), "RecipeType", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_7 );

		//  사용조건
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_USE_STATE ), "UseRequirements", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SCOUTER_SCOUTER_NAME ), "UseRequirementsData", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_5 );

		WCHAR awcBufferLevel[32];
		swprintf_s( awcBufferLevel, 32, GetDisplayStringManager()->GetString( DST_INFOWINDOW_MAKE_LEVEL ), pRecipeTblData->byNeedMixLevel );
		m_pmdBox->SetItem( awcBufferLevel, "MakeLevel", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_5 );


		// 재료
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_RECIPE_MAT ), "Materials", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );
		for( int i=0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i )
		{
			if( pRecipeTblData->asMaterial[i].materialTblidx != INVALID_TBLIDX )
			{
				// 아이템 텍스트를 꺼내온다.
				sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData( pRecipeTblData->asMaterial[i].materialTblidx );
				if( pItemData )
				{
					RwUInt32 uiColor = Logic_GetItemRankColor( pItemData->byRank );	
					std::wstring strItem = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemData->Name );

					CHAR acBuffer[16];
					sprintf_s( acBuffer, 16, "Material%d", i );

					m_pmdBox->SetItem( strItem.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_RIGHT, uiColor );
				}
			}
		}

		// 만들어질 아이템
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_CREATE_ITEM ), "CreateItems", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );
		for( int i=0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; ++i )
		{
			if( pRecipeTblData->asCreateItemTblidx[i].itemTblidx != INVALID_TBLIDX )
			{
				// 아이템 텍스트를 꺼내온다.
				sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData( pRecipeTblData->asCreateItemTblidx[i].itemTblidx );

				RwUInt32 uiColor = Logic_GetItemRankColor( pItemData->byRank );	
				std::wstring strItem = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemData->Name );

				CHAR acBuffer[16];
				sprintf_s( acBuffer, 16, "Create%d", i );

				WCHAR awcBuffer[128];
				swprintf_s( awcBuffer, 128, L"%s(%d%%)", strItem.c_str(), pRecipeTblData->asCreateItemTblidx[i].itemRate );

				m_pmdBox->SetItem( awcBuffer, acBuffer, FONT_TEXT, COMP_TEXT_RIGHT, uiColor );
			}
		}

		// 제조 비용
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_MAKE_ZENNY ), "NeedMakeZenny", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );

		WCHAR awcBuffer[128];
		swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_INFOWINDOW_ZENNY ), pRecipeTblData->dwNeedMixZenny );
		m_pmdBox->SetItem( awcBuffer, "NeedMakeZennyData", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_7 );


		m_pmdBox->DrawItem();
	}
}

VOID CInfoWindowGui::SetNotLearnSkillInfo( sSKILL_TBLDAT* pData )
{
	INFOWND_NULLCHECK( pData, "SetNotLearnSkillInfo sSKILL_TBLDAT* pData is NULL" );

	m_pmdBox->Clear();
	////////////////////////////////////////////////////////////////////////////
	// Skill

	SetSkillInfo_NameGradeClass( pData, FALSE );

	SetSkillInfo_LearnCondition( pData, FALSE );

	SetSkillInfo_UseCondition( pData, pData );

	SetSkillInfo_EffectData( pData, INFOCOLOR_7 );

	SetSkillInfo_AppointApply( pData );

	SetSkillInfo_Parameter( NULL, pData, pData, FALSE );

	////////////////////////////////////////////////////////////////////////////
	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetSkillRQInfo( RwInt32 eRQState )
{
	m_pmdBox->Clear();
	////////////////////////////////////////////////////////////////////////////
	// Skill RQ

	WCHAR* pString = NULL;

	if( eRQState & SRQ_FLAG_NOT_LEARN_LEVEL )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_LEARN_LEVEL ) );
		m_pmdBox->SetItem( pString, "NotLearnLevel", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_NOT_LEARN_SKILL )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_LEARN_SKILL ) );
		m_pmdBox->SetItem( pString, "NotLearnSkill", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_READY_LEARN_BY_SKILLMASTER ) );
		m_pmdBox->SetItem( pString, "ReadyLearn", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_CAN_LEARN_BY_SELF )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_READY_LEARN_BY_SELF ) );
		m_pmdBox->SetItem( pString, "ReadyLearn", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_NOT_UPGRADE_LEVEL )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_UPGRADE_LEVEL ) );
		m_pmdBox->SetItem( pString, "NotUpgradeLevel", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_NOT_UPGRADE_SKILLEXP )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_UPGRADE_SKILLEXP ) );
		m_pmdBox->SetItem( pString, "NotUpgradeSkillExp", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_CAN_UPGRADE )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_READY_UPGRADE ) );
		m_pmdBox->SetItem( pString, "ReadyUpgrade", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_MASTER )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_MASTERED ) );
		m_pmdBox->SetItem( pString, "Mastered", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_LEARNED )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_LEARNED ) );
		m_pmdBox->SetItem( pString, "Learned", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_NOT_LEARN_CLASS )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_LEARN_CLASS ) );
		m_pmdBox->SetItem( pString, "NotLearnClass", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	if( eRQState & SRQ_FLAG_NOT_LEARN_CHANGECLASSRIGHT )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RQ_NOT_LEARN_CHANGECLASSRIGHT ) );
		m_pmdBox->SetItem( pString, "NotLearnHTB", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
	}

	////////////////////////////////////////////////////////////////////////////
	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetBuffInfo( CNtlSobBuff* pBuff )
{
	INFOWND_NULLCHECK( pBuff, "SetBuffInfo CNtlSobBuff* pBuff is NULL" );

	m_pmdBox->Clear();

	CNtlSobBuffAttr* pAttr = reinterpret_cast<CNtlSobBuffAttr*>( pBuff->GetSobAttr() );
	if( DBO_OBJECT_SOURCE_SKILL == pAttr->GetBuffType() )
	{
		sSKILL_TBLDAT* pData = pAttr->GetSkillTbl();
		SetBuffInfo_Common( pData, pBuff->GetIcon()->GetMaxCoolingTime() );
	}
	else if ( DBO_OBJECT_SOURCE_ITEM == pAttr->GetBuffType() )
	{
		sITEM_TBLDAT* pData = pAttr->GetItemTbl();
		SetBuffInfo_Common( pData );
	}

	// 시간 출력
	SetBuffInfo_RemainTime( pBuff->GetIcon()->GetMaxCoolingTime(), pBuff->GetIcon()->GetCoolingTime() );

	// Drop Buff Text 출력
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_HP && pAttr->IsBlessType() )
		SetBuffInfo_DropInfo();

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetBuffInfo( sSKILL_TBLDAT* pData )
{
	INFOWND_NULLCHECK( pData, "SetBuffInfo sSKILL_TBLDAT* pData is NULL" );

	m_pmdBox->Clear();

	SetBuffInfo_Common( pData, (RwReal)pData->wKeep_Time );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetBuffInfo( sITEM_TBLDAT* pData )
{
	INFOWND_NULLCHECK( pData, "SetBuffInfo sITEM_TBLDAT* pData is NULL" );

	m_pmdBox->Clear();

	SetBuffInfo_Common( pData );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetFakeBuffInfo( sNtlFakeBuff* pBuff )
{
	INFOWND_NULLCHECK( pBuff, "SetFakeBuffInfo sNtlFakeBuff* pBuff is NULL" );

	m_pmdBox->Clear();

	if( pBuff->eType == DBO_OBJECT_SOURCE_SKILL )
		SetBuffInfo_Common( pBuff->pSkillTbl, pBuff->fMaxTime, pBuff->afEffectValue );
	else
		SetBuffInfo_Common( pBuff->pItemTbl );

	// 시간 출력
	SetBuffInfo_RemainTime( pBuff->fMaxTime, pBuff->fTime );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetCharmBuffInfo( sITEM_TBLDAT* pData )
{
	m_pmdBox->Clear();

	sCHARM_TBLDAT* pCharmData = reinterpret_cast<sCHARM_TBLDAT*>( API_GetTableContainer()->GetCharmTable()->FindData( pData->Charm_Tblidx ) );
	NTL_ASSERT( pCharmData, "UseItemTbl Index" << pData->Charm_Tblidx << "is Invalid" );


	const WCHAR* pwcText;
	CTextTable* pItemText = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	// 이름
	m_pmdBox->SetItem( pItemText->GetText( pData->Name ).c_str(), "CharmName", FONT_TITLE, COMP_TEXT_CENTER, INFOCOLOR_2 );

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );



	// 아이템 드롭 확률 증가
	if( pCharmData->wDrop_Rate > 0 )
	{
		pwcText = GetDisplayStringManager()->GetString(DST_CHARM_DROPRATE);
		m_pmdBox->Format( "droprate", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, pwcText, pCharmData->wDrop_Rate );
	}

	// 경험치 증가
	if( pCharmData->wEXP > 0 )
	{		
		pwcText = GetDisplayStringManager()->GetString(DST_CHARM_EXP);
		m_pmdBox->Format( "exp", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, pwcText, pCharmData->wEXP );
	}

	// RP 공유
	if( pCharmData->wRP_Sharing > 0 )
	{		
		pwcText = GetDisplayStringManager()->GetString(DST_CHARM_RP_SHARING);
		m_pmdBox->Format( "RP", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, pwcText, pCharmData->wRP_Sharing );
	}

	// 스킬 쿨타임 감소
	if( pCharmData->wCool_Time > 0 )
	{
		pwcText = GetDisplayStringManager()->GetString(DST_CHARM_COOLTIME);
		m_pmdBox->Format( "cooltime", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, pwcText, pCharmData->wCool_Time );
	}

	

	// 지속 시간
	m_pmdBox->Format( "DurationTime", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE,
		GetDisplayStringManager()->GetString(DST_SKILL_DURATIONTIME), Logic_GetTimeString(pCharmData->wKeep_Time) );

	// 시간 출력
	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
	sPartyCharm charm = pParty->GetCharmbyTableIndex(pCharmData->tblidx);
	SetBuffInfo_RemainTime(pCharmData->wKeep_Time, pCharmData->wKeep_Time - charm.fRemainTime );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetHTBSkillInfo( CNtlSobHTBSkill* pHTBSkill )
{
	INFOWND_NULLCHECK( pHTBSkill, "SetHTBSkillInfo CNtlSobHTBSkill* pHTBSkill is NULL" );

	CNtlSobHTBSkillAttr* pAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pHTBSkill->GetSobAttr() );
	sHTB_SET_TBLDAT* pData = pAttr->GetHTBSkillTbl();

	m_pmdBox->Clear();

	SetHTBInfo_NameGradeClass( pData, TRUE );

	SetHTBInfo_UseCondition( pData );

	SetHTBInfo_EffectData( pData );

	SetHTBInfo_ApplyParameter( pHTBSkill, pData );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetNotLearnHTBSkillInfo( sHTB_SET_TBLDAT* pData )
{
	INFOWND_NULLCHECK( pData, "SetNotLearnHTBSkillInfo sHTB_SET_TBLDAT* pData is NULL" );

	m_pmdBox->Clear();

	SetHTBInfo_NameGradeClass( pData, FALSE );

	SetHTBInfo_LearnCondition( pData );

	SetHTBInfo_UseCondition( pData );

	SetHTBInfo_EffectData( pData );

	SetHTBInfo_ApplyParameter( NULL, pData );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetActionSkillInfo( CNtlSobActionSkill* pActionSkill )
{
	INFOWND_NULLCHECK( pActionSkill, "SetActionSkillInfo CNtlSobActionSkill* pActionSkill is NULL" );

	WCHAR* pString = NULL;

	CNtlSobActionSkillAttr* pActionSkillAttr = reinterpret_cast<CNtlSobActionSkillAttr*>( pActionSkill->GetSobAttr() );
	sACTION_TBLDAT* pActionData = reinterpret_cast<sACTION_TBLDAT*>( pActionSkillAttr->GetActionSkillTbl() );
	CTextTable* pActionText = API_GetTableContainer()->GetTextAllTable()->GetActionTbl();

	m_pmdBox->Clear();

	// 이름
	m_pmdBox->SetItem( pActionText->GetText( pActionData->Action_Name ).c_str(), "Name", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_2 );

	// 스킬타입
	switch( pActionData->byAction_Type )
	{
	case ACTION_TYPE_NORMAL_ACTION: pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ACTION_TYPE_NORMAL ) ); break;
	case ACTION_TYPE_SOCIAL_ACTION: pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ACTION_TYPE_SOCIAL ) ); break;
		break;
	}
	m_pmdBox->SetItem( pString, "Type", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_3 );

	// 설명
	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	m_pmdBox->SetItem( pActionText->GetText( pActionData->Note ).c_str(), "Note", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );

	// 채팅명령
	if( pActionData->byAction_Type == ACTION_TYPE_SOCIAL_ACTION )
	{
		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

		CChatCommandTable* pChatCommandTable = API_GetTableContainer()->GetChatCommandTable();
		sCHAT_COMMAND_TBLDAT* pChatCommandData = reinterpret_cast<sCHAT_COMMAND_TBLDAT*>( pChatCommandTable->FindData( pActionData->chat_Command_Index ) );
		CTextTable* pChatCommandText = API_GetTableContainer()->GetTextAllTable()->GetChatCommandTbl();

		std::wstring strBuf;
		RwInt32 i;
		for( i = 0 ; i < NTL_MAX_CHAT_COMMAND ; ++i )	
		{
			if( pChatCommandData->aChat_Command[i] == INVALID_TBLIDX )
				break;

			std::wstring wstrChatCommand = pChatCommandText->GetText( pChatCommandData->aChat_Command[i] );

			if( i > 0 ) 
				strBuf += L", ";

			strBuf += L"/";
			strBuf += wstrChatCommand;
		}	

		m_pmdBox->SetItem( strBuf.c_str(), "Commands", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
	}

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetRewardInfo( stINFOWND_REWARD* pReward )
{
	INFOWND_NULLCHECK( pReward, "SetRewardInfo stINFOWND_REWARD* pReward is NULL" );

	switch( pReward->eType )
	{
	case eREWARD_TYPE_ZENY:
		SetRewardInfo_Zenny( pReward->nValue );
		break;
	case eREWARD_TYPE_EXP:
		SetRewardInfo_Exp( pReward->nValue );
		break;
	case eREWARD_TYPE_CHANGE_CLASS:
		SetRewardInfo_Class( pReward->nValue );
		break;
	case eREWARD_TYPE_REPUTATION:
		SetRewardInfo_Reputation( pReward->nValue );
		break;
	case eREWARD_TYPE_PROBABILITY:
		SetJustWTextInfo( const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_QUESTREWARD_PROB_REWARD ) ) );
		break;
	case eREWARD_TYPE_CHANGE_ADULT:
		SetJustWTextInfo( const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_QUESTREWARD_CHANGE_ADULT ) ) );
		break;
	case eREWARD_TYPE_GET_CONVERT_CLASS_RIGHT:
		SetJustWTextInfo( const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_QUESTREWARD_CONVERT_CLASS_RIGHT ) ) );
		break;
	}
}

VOID CInfoWindowGui::SetMiniMapInfo(MINIMAPINFO_LIST* pList)
{
	INFOWND_NULLCHECK( pList, "SetMiniMapInfo MINIMAPINFO_LIST* pList is NULL" );

	m_pmdBox->Clear();

	RwUInt8 byIndex = 0;
	RwChar acBuffer[4] = "";
	MINIMAPINFO_ITER it = pList->begin();
	for( ; it != pList->end() ; ++it, ++byIndex )
	{
		_itoa_s(byIndex, acBuffer, 10);

		sMINIMAPINFO& MinimapInfo = *it;

		switch(MinimapInfo.iType)
		{
		case MMIT_NPC:
			{
				m_pmdBox->SetItem( MinimapInfo.wcsString.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0 );
				break;
			}
		case MMIT_MOB:
			{
				m_pmdBox->SetItem( MinimapInfo.wcsString.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0 );
				break;
			}
		case MMIT_QUEST:
			{
				m_pmdBox->SetItem( MinimapInfo.wcsString.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_CENTER, COLOR_QUEST );
				break;
			}
		case MMIT_PARTY:
			{
				m_pmdBox->SetItem( MinimapInfo.wcsString.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_CENTER, NTL_PARTY_NAME_COLOR );
				break;
			}
		case MMIT_LANDMARK:
			{
				m_pmdBox->SetItem( MinimapInfo.wcsString.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0 );
				break;
			}
		case MMIT_BIND_POS:
			{
				m_pmdBox->SetItem( MinimapInfo.wcsString.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0 );
				break;
			}
		case MMIT_NEXTQUEST_POSITION:
			{
				m_pmdBox->SetItem( MinimapInfo.wcsString.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_CENTER, COLOR_QUEST );
				break;
			}
		default:
			{
				NTL_ASSERT(false, "CInfoWindowGui::SetMiniMapInfo, Invalid minimap info type : " << MinimapInfo.iType);
				break;
			}
		}
	}

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetScouterInfo(SERIAL_HANDLE hScouterHandle)
{	
	std::wstring& wstr = GetInputActionMap()->GetKeyName(ACTION_WINDOW_SCOUTER);			

	m_pmdBox->Clear();

	if( wstr.size() > 0 )
	{
		WCHAR awcBuffer[128] = L"";
		CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hScouterHandle));
		NTL_ASSERT(pSobItem, "CInfoWindowGui::SetScouterInfo, Not exist sob item attiribure of handle : " << hScouterHandle);
		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
		NTL_ASSERT(pSobItemAttr, "CInfoWindowGui::SetScouterInfo, Not exist sob item attiribure of handle : " << hScouterHandle);
		sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();	
		NTL_ASSERT(pITEM_TBLDAT, "CInfoWindowGui::SetScouterInfo, Not exist item table of sob item handle  : " << hScouterHandle);

		// 스카우터 이름
		SetItemInfo_Name( m_pmdBox, pITEM_TBLDAT, pSobItemAttr->GetGrade(), pSobItemAttr->GetRank());

		// 단축키 설명
		swprintf_s(awcBuffer, GetDisplayStringManager()->GetString(DST_SCOUTER_EXPLAIN_SLOT), wstr.c_str());
		m_pmdBox->SetItem( awcBuffer, "key", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );		
	}

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetJustWTextInfo( WCHAR* szText )
{	
	m_pmdBox->Clear();

	m_pmdBox->SetItem( szText, "justwtext", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetQuestIndicateInfo( RwUInt32 uiQuestTitle, RwUInt32 uiQuestGoal )
{
	m_pmdBox->Clear();

	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

	if( uiQuestTitle != INVALID_TBLIDX )
	{
		sQUEST_TEXT_DATA_TBLDAT* pQuestTitle = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( uiQuestTitle ) );
		if( pQuestTitle )
			m_pmdBox->SetItem( gui::GetHtmlFromMemoryString( pQuestTitle->wstrText.c_str(), pQuestTitle->wstrText.size() ).c_str(), "QuestInfoTitle", FONT_TITLE, COMP_TEXT_CENTER, INFOCOLOR_0 );
		else
			m_pmdBox->Format( "QuestInfoTitle", FONT_TITLE, COMP_TEXT_CENTER, INFOCOLOR_0, 0, FALSE, L"CInfoWndowGui::SetQuestIndicateInfo : QuestTextData (%u) index is invalid", uiQuestTitle );
	}

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	if( uiQuestGoal != INVALID_TBLIDX )
	{
		sQUEST_TEXT_DATA_TBLDAT* pQuestGoal = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( uiQuestGoal ) );
		if( pQuestGoal )
			m_pmdBox->SetItem( gui::GetHtmlFromMemoryString( pQuestGoal->wstrText.c_str(), pQuestGoal->wstrText.size(), TRUE ).c_str(), "QuestInfoGoal", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0 );
		else
			m_pmdBox->Format( "QuestInfoTitle", FONT_TITLE, COMP_TEXT_CENTER, INFOCOLOR_0, 0, FALSE, L"CInfoWndowGui::SetQuestIndicateInfo : QuestTextData (%u) index is invalid", uiQuestGoal );
	}	

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetSkillUpgradeInfo( stSkillUpgradeInfo* pUpgradeInfo )
{
	m_pmdBox->Clear();

	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	CTextTable* pSkillTextTable = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();

	if( pUpgradeInfo->bMaster )
	{
		m_pmdBox->Format( "SpInfoMaster", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_MASTER ), pUpgradeInfo->byCurrentLevel );
	}
	else
	{
		if( pUpgradeInfo->bLearn )
		{
			m_pmdBox->Format( "SpInfoLearned", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_CURRENT_GRADE ), pUpgradeInfo->byCurrentLevel );
		}
		else
		{
			if( pUpgradeInfo->bSelfLearnEnable )
			{
				if( pUpgradeInfo->IsEnableLearn() )
					m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_SELFTRAIN ), "SpInfoSelfLearn", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
				else
				{
					if( pUpgradeInfo->bClassOK )
						m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_CANT_SELFTRAIN ), "SpInfoSelfLearn", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
					else
						m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_CANT_LEARN_BY_CLASS ), "SpInfoLearn", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
				}
			}
			else
			{
				if( pUpgradeInfo->IsEnableLearn() )
					m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_NEED_LEARN ), "SpInfoLearn", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
				else
				{
					if( pUpgradeInfo->bClassOK )
						m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_CANT_LEARN ), "SpInfoLearn", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
					else
						m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_CANT_LEARN_BY_CLASS ), "SpInfoLearn", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
				}
			}
		}

		if( pUpgradeInfo->bClassOK )
		{
			RwUInt32 uiColor;

			if( pUpgradeInfo->bSPOK )
				uiColor = INFOCOLOR_2;
			else
				uiColor = INFOCOLOR_5;

			if( pUpgradeInfo->bLearn )
				m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_NEED_SP ), "NeedSp", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
			else
				m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_NEED_SP_LEARN ), "NeedSp", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );

			m_pmdBox->SetItem( pUpgradeInfo->uiNeedSP, "NeedSpValue", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, TRUE );

			if( !pUpgradeInfo->bAvatarLevelOK )
			{
				m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_AVATAR_LEVEL ), "AvatarLevelWarning", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
				m_pmdBox->SetItem( pUpgradeInfo->byRequreAvatarLevel, "AvatarLevelValue", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5, 0, TRUE );
			}

			if( pUpgradeInfo->RequireSkillIdx1 != INVALID_TBLIDX )
			{
				if( pUpgradeInfo->bSkill1OK )
					uiColor = INFOCOLOR_2;
				else
					uiColor = INFOCOLOR_5;

				sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pUpgradeInfo->RequireSkillIdx1 ) );

				if( pSkillData )
				{
					m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_NEED_SKILL ), "NeedSkill1", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );

					if( pSkillData->dwNextSkillTblidx != INVALID_TBLIDX )
						m_pmdBox->Format( "NeedSkillName1", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, TRUE, GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_NEED_SKILLGRADE ), pSkillTextTable->GetText( pSkillData->Skill_Name ).c_str(), pSkillData->bySkill_Grade );
					else
						m_pmdBox->SetItem( pSkillTextTable->GetText( pSkillData->Skill_Name ).c_str() , "NeedSkillName1", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, TRUE );				
				}			
			}

			if( pUpgradeInfo->RequireSkillIdx2 != INVALID_TBLIDX )
			{
				if( pUpgradeInfo->bSkill2OK )
					uiColor = INFOCOLOR_2;
				else
					uiColor = INFOCOLOR_5;

				sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pUpgradeInfo->RequireSkillIdx2 ) );

				if( pSkillData )
				{
					m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_NEED_SKILL ), "NeedSkill2", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
					
					if( pSkillData->dwNextSkillTblidx != INVALID_TBLIDX )
						m_pmdBox->Format( "NeedSkillName2", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, TRUE, GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_NEED_SKILLGRADE ), pSkillTextTable->GetText( pSkillData->Skill_Name ).c_str(), pSkillData->bySkill_Grade );
					else
						m_pmdBox->SetItem( pSkillTextTable->GetText( pSkillData->Skill_Name ).c_str() , "NeedSkillName2", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, TRUE );				
				}			
			}

			if( pUpgradeInfo->IsEnableLearn() && pUpgradeInfo->bLearn )
				m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_SP_INFO_UPGRADE ), "EnableUpgrade", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
		}		
	}

	m_pmdBox->DrawItem();
}

// ITEM SUB //////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CInfoWindowGui::SetItemInfo_Name( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 ucGrade, RwUInt8 ucRank )
{
	// 아이템 이름

	RwUInt32 uiColor = Logic_GetItemRankColor( ucRank );	
	std::wstring strBuf = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pData->Name );

	if( pData->byItem_Type != ITEM_TYPE_DRAGONBALL && ucGrade > 0 )
	{
		WCHAR buf[16];
		swprintf_s( buf, 16, L" + %u", ucGrade );
		strBuf += buf;
	}

	pmdBox->SetItem( strBuf.c_str(), "ItemName", FONT_TITLE, COMP_TEXT_LEFT, uiColor );
}

VOID CInfoWindowGui::SetItemInfo_Attr_Info( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byBattleAttribute )
{
	RwUInt32 uiColor = 0;	
	WCHAR* pString = NULL;	
	WCHAR wBuf[512];

	// 아이템 속성	
	if( byBattleAttribute != BATTLE_ATTRIBUTE_NONE )
	{
		switch( byBattleAttribute )
		{
		case 1 : uiColor = INFOCOLOR_4; pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_HONEST ) ); break;
		case 2 : uiColor = INFOCOLOR_8; pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_STRANGE ) ); break;
		case 3 : uiColor = INFOCOLOR_5; pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_WILD ) ); break;
		case 4 : uiColor = INFOCOLOR_1; pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_ELEGANCE ) ); break;
		case 5 : uiColor = INFOCOLOR_7; pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTR_FUNNY ) ); break;
		}

		swprintf_s( wBuf, 512, GetDisplayStringManager()->GetString( DST_ITEM_ATTR_INFO ), pString );
		pmdBox->SetItem( wBuf, "ItemAttribute", FONT_TEXT, COMP_TEXT_RIGHT, uiColor );
	}

	// 설명
	if( pData->Name != INVALID_TBLIDX )
	{
		pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
		pmdBox->SetItem( API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pData->Note ).c_str(), "ItemNote", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
	}	
}

VOID CInfoWindowGui::SetItemInfo_EquipLimit( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobAvatarAttr* pAvatarAttr )
{
	RwUInt32 uiColor = 0;	
	WCHAR* pString = NULL;	

	if( EQUIP_TYPE_FIRST > pData->byEquip_Type || EQUIP_TYPE_LAST < pData->byEquip_Type )
		return;

	if( pData->dwNeed_Class_Bit_Flag != NTL_ITEM_ALL_USE_FLAG || 
		( pData->byNeed_Level > 1 && pData->byItem_Type != ITEM_TYPE_STONE ) ||
		pData->wEquip_Slot_Type_Bit_Flag != INVALID_WORD )
		pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	// 종족 착용제한
	if( pData->dwNeed_Class_Bit_Flag != NTL_ITEM_ALL_USE_FLAG )
	{
		WCHAR  wBuf[64];
		const WCHAR* pEnableText = GetDisplayStringManager()->GetString( DST_ITEM_CAN_EQUIP );
		const WCHAR* pAllRace = GetDisplayStringManager()->GetString( DST_ITEM_ALL_RACE );

		if( pData->dwNeed_Class_Bit_Flag == 0x7F )
			swprintf_s( wBuf, 64, pEnableText, pAllRace );			
		else if( pData->dwNeed_Class_Bit_Flag == 0x18 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_NAMEK ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x7 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_HUMAN ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x60 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_MAJIN ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x01 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_HUMAN_FIGHTER ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x02 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_HUMAN_MYSTIC ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x08 ) 
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_NAMEK_FIGHTER ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x10 ) 
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_NAMEK_MYSTIC ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x20 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_MIGHTY_MAJIN ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x40 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_WONDER_MAJIN ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x04 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_ENGINEER ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x2000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_DARK_WARRIOR ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x4000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_SHADOW_KNIGHT ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x8000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_DENDEN_HEALER ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x10000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_POCO_SUMMONER ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x80 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_STREET_FIGHTER ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x100 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_SWORD_MASTER ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x200 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_CRANE_ROSHI ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x400 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_TURTLE_ROSHI ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x800 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_GUN_MANIA ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x1000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_MECH_MANIA ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x40000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_GRAND_MA ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x20000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_ULTI_MA ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x80000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_PLAS_MA ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x100000 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_KAR_MA ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x6B )
			swprintf_s( wBuf, 64, L"%s", GetDisplayStringManager()->GetString( DST_ITEM_ALL_RACE_ENABLE_EQUIP_BUT_NMY ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x29 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_ITEM_PHYSICAL_CLASS ) );
		else if( pData->dwNeed_Class_Bit_Flag == 0x42 )
			swprintf_s( wBuf, 64, pEnableText, GetDisplayStringManager()->GetString( DST_ITEM_ENERGY_CLASS ) );

		// 착용여부에 따른 색지정.
		if( pAvatarAttr->GetRaceFlag() & pData->dwNeed_Class_Bit_Flag )
			uiColor = INFOCOLOR_0;
		else
			uiColor = INFOCOLOR_5;

		pmdBox->SetItem( wBuf, "NeedClass", FONT_TEXT, COMP_TEXT_LEFT, uiColor );
	}

	// 레벨 착용제한
	if( pData->byNeed_Level > 1 && pData->byItem_Type != ITEM_TYPE_STONE )
	{
		if( pAvatarAttr->GetLevel() < pData->byNeed_Level )
			uiColor = INFOCOLOR_5;
		else
			uiColor = INFOCOLOR_0;

		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_EQUIP_LEVEL_TEXT ) );
		pmdBox->Format( "NeedLevel", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, pData->byNeed_Level );
	}	

	if( pData->wEquip_Slot_Type_Bit_Flag != INVALID_WORD )
	{
		std::wstring strBuf;
		RwUInt16 flagSlotType = pData->wEquip_Slot_Type_Bit_Flag;

		if( flagSlotType & EQUIP_SLOT_FLAG_HAND	)
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_HAND );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_SUB_WEAPON )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_SUB_WEAPON );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_JACKET )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_JACKET );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_PANTS )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_PANTS );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_BOOTS )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_BOOTS );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_SCOUTER )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_SCOUTER );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_COSTUME )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_DOGI );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_NECKLACE	)
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_NECKLACE );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_EARRING_1 || flagSlotType & EQUIP_SLOT_FLAG_EARRING_2 )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_EARRING );
			strBuf += L", ";
		}
		if( flagSlotType & EQUIP_SLOT_FLAG_RING_1 || flagSlotType & EQUIP_SLOT_FLAG_RING_2 )
		{
			strBuf += GetDisplayStringManager()->GetString( DST_EQUIP_SLOT_TYPE_RING );
			strBuf += L", ";
		}

		std::wstring::size_type sizeT = strBuf.find_last_of( ',' );
		strBuf.erase( sizeT );

		const WCHAR* pString = GetDisplayStringManager()->GetString( DST_ITEM_EQUIP_SLOT_TEXT );
		pmdBox->Format( "EquipSlot", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, pString, strBuf.c_str() );		
		strBuf.clear();
	}
}

RwBool CInfoWindowGui::SetItemInfo_SpecialClass( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobAvatarAttr* pAvatarAttr )
{
	RwUInt32 uiColor = 0;
	WCHAR* pString = NULL;
	std::wstring wstrBuf;
	RwBool bSpecialEffect = FALSE;

	if( pData->byClass_Special != INVALID_BYTE ||
		pData->byRace_Special != INVALID_BYTE )
		pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	// 특화클래스.
	if( pData->byClass_Special != INVALID_BYTE )
	{
		RwUInt32 FlagClassSpecial = Logic_ConvertClassToClassFlag( pData->byClass_Special );

		if( pAvatarAttr->GetRaceFlag() & FlagClassSpecial )
		{
			bSpecialEffect = TRUE;
			uiColor = INFOCOLOR_7;
		}
		else
			uiColor = INFOCOLOR_0;

		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_SPECIAL_CLASS ) );
		pmdBox->SetItem( pString, "SpcialClass", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_3 );

		if( FlagClassSpecial & PC_CLASS_FLAG_HUMAN_FIGHTER )
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_HUMAN_FIGHTER );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_HUMAN_MYSTIC )
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_HUMAN_MYSTIC );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_HUMAN_ENGINEER )		 
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_ENGINEER );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_STREET_FIGHTER )
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_STREET_FIGHTER );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_SWORD_MASTER )  
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_SWORD_MASTER );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_CRANE_ROSHI )   
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_CRANE_ROSHI );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_TURTLE_ROSHI )  
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_TURTLE_ROSHI );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_GUN_MANIA )     
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_GUN_MANIA );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_MECH_MANIA )    
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_MECH_MANIA );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_NAMEK_FIGHTER ) 
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_NAMEK_FIGHTER );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_NAMEK_MYSTIC )  
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_NAMEK_MYSTIC );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_DARK_WARRIOR )  
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_DARK_WARRIOR );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_SHADOW_KNIGHT )
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_SHADOW_KNIGHT );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_POCO_SUMMONER ) 
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_POCO_SUMMONER );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_DENDEN_HEALER )
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_DENDEN_HEALER );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_MIGHTY_MAJIN )  
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_MIGHTY_MAJIN );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_WONDER_MAJIN )  
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_WONDER_MAJIN );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_GRAND_MA )      
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_GRAND_MA );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_ULTI_MA )       
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_ULTI_MA );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_PLAS_MA )      
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_PLAS_MA );
			wstrBuf += L", ";
		}
		if( FlagClassSpecial & PC_CLASS_FLAG_KAR_MA )        
		{
			wstrBuf += GetDisplayStringManager()->GetString( DST_KAR_MA );
			wstrBuf += L", ";
		}

		std::wstring::size_type sizeT = wstrBuf.find_last_of( ',' );
		wstrBuf.erase( sizeT );
		pString = const_cast<WCHAR*>( wstrBuf.c_str() );
		pmdBox->SetItem( pString, "SpacialClassValue", FONT_TEXT, COMP_TEXT_LEFT, uiColor );
		wstrBuf.clear();
	}

	if( pData->byRace_Special != INVALID_BYTE )
	{
		//RwUInt32 FlagRaceSpecial = Logic_ConvertRaceToClassFlag( pData->byRace_Special );

		//pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_SPECIAL_CLASS ) );
		//pmdBox->SetItem( pString, "SpcialClass", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_3 );

		//if( FlagRaceSpecial & flagHuman )
		//{
		//	wstrBuf += GetDisplayStringManager()->GetString( DST_HUMAN );
		//	wstrBuf += L", ";
		//}
		//if( FlagRaceSpecial & flagNamek )
		//{
		//	wstrBuf += GetDisplayStringManager()->GetString( DST_NAMEK );
		//	wstrBuf += L", ";
		//}
		//if( FlagRaceSpecial & flagMajin )
		//{
		//	wstrBuf += GetDisplayStringManager()->GetString( DST_MAJIN );
		//	wstrBuf += L", ";
		//}

		//std::wstring::size_type sizeT = wstrBuf.find_last_of( ',' );
		//wstrBuf.erase( sizeT );
		//pString = const_cast<WCHAR*>( wstrBuf.c_str() );

		if( pAvatarAttr->GetRace() == pData->byRace_Special )
		{
			uiColor = INFOCOLOR_7;
			bSpecialEffect = TRUE;
		}
		else
			uiColor = INFOCOLOR_0;

		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_SPECIAL_RACE ) );
		pmdBox->SetItem( pString, "SpcialClass", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_3 );

		if( pData->byRace_Special == RACE_HUMAN )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_HUMAN ) );
		else if( pData->byRace_Special == RACE_NAMEK )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_NAMEK ) );
		else if( pData->byRace_Special == RACE_MAJIN )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_MAJIN ) );

		pmdBox->SetItem( pString, "SpacialClassValue", FONT_TEXT, COMP_TEXT_LEFT, uiColor );
	}	

	return bSpecialEffect;
}

VOID CInfoWindowGui::SetItemInfo_Parameter( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byGrade )
{
	if( pData->wPhysical_Offence != INVALID_WORD || pData->wEnergy_Offence != INVALID_WORD || pData->wPhysical_Defence != INVALID_WORD || 
		pData->wEnergy_Defence != INVALID_WORD || pData->fAttack_Range_Bonus != INVALID_FLOAT || pData->wAttack_Speed_Rate != INVALID_WORD || pData->Use_Item_Tblidx != INVALID_TBLIDX )
		pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	else
		return;

	// Item Parameter
	WCHAR* pString = NULL;	
	const WCHAR* pDiffString = GetDisplayStringManager()->GetString( DST_ITEM_DIFF_STATUS );

	if( pData->wPhysical_Offence != INVALID_WORD )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_PHYSICAL_OFFENCE ) );

		if( byGrade == 0 )
		{
			pmdBox->Format( "PhysicalOffence", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL, 0, FALSE, pString, pData->wPhysical_Offence );
		}
		else
		{
			RwUInt16 wFinalValue = Logic_GetFinalItemValue( pData->wPhysical_Offence, byGrade, pData->dwPhysical_OffenceUpgrade );	
			pmdBox->Format( "PhysicalOffence", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 0, FALSE, pString, pData->wPhysical_Offence );
			pmdBox->Format( "PhysicalOffenceDiff", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 5, TRUE, pDiffString, wFinalValue - pData->wPhysical_Offence );
		}		
	}
	if( pData->wEnergy_Offence != INVALID_WORD )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ENERGY_OFFENCE ) );

		if( byGrade == 0 )
		{
			pmdBox->Format( "EnergyOffence", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL, 0, FALSE, pString, pData->wEnergy_Offence );
		}
		else
		{
			RwUInt16 wFinalValue = Logic_GetFinalItemValue( pData->wEnergy_Offence, byGrade, pData->dwEnergy_OffenceUpgrade );	
			pmdBox->Format( "EnergyOffence", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 0, FALSE, pString, pData->wEnergy_Offence );
			pmdBox->Format( "EnergyOffenceDiff", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 5, TRUE, pDiffString, wFinalValue - pData->wEnergy_Offence );
		}			
	}
	if( pData->wPhysical_Defence != INVALID_WORD )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_PHYSICAL_DEFENCE ) );

		if( byGrade == 0 )
		{
			pmdBox->Format( "PhysicalDefence", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL, 0, FALSE, pString, pData->wPhysical_Defence );
		}
		else
		{
			RwUInt16 wFinalValue = Logic_GetFinalItemValue( pData->wPhysical_Defence, byGrade, pData->dwPhysical_DefenceUpgrade );	
			pmdBox->Format( "PhysicalDefence", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 0, FALSE, pString, pData->wPhysical_Defence );
			pmdBox->Format( "PhysicalDefenceDiff", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 5, TRUE, pDiffString, wFinalValue - pData->wPhysical_Defence );
		}				
	}
	if( pData->wEnergy_Defence != INVALID_WORD )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ENERGY_DEFENCE ) );

		if( byGrade == 0 )
		{
			pmdBox->Format( "EnergyDefence", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL, 0, FALSE, pString, pData->wEnergy_Defence );
		}
		else
		{
			RwUInt16 wFinalValue = Logic_GetFinalItemValue( pData->wEnergy_Defence, byGrade, pData->dwEnergy_DefenceUpgrade );	
			pmdBox->Format( "EnergyDefence", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 0, FALSE, pString, pData->wEnergy_Defence );
			pmdBox->Format( "EnergyDefenceDiff", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_8, 5, TRUE, pDiffString, wFinalValue - pData->wEnergy_Defence );
		}						
	}
	if( pData->fAttack_Range_Bonus != INVALID_FLOAT && pData->fAttack_Range_Bonus >= 0.0f )
	{
		if( pData->fAttack_Range_Bonus == 0.0f )
		{
			pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEM_CLOSE_ATTACK_RANGE ), "AttackRange", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL );
		}
		else if( pData->fAttack_Range_Bonus > 0.0f )
		{
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTACK_RANGE ) );
			pmdBox->Format( "AttackRange", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL, 0, FALSE, pString, (RwInt32)pData->fAttack_Range_Bonus );
		}		
	}
	if( pData->wAttack_Speed_Rate != INVALID_WORD )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_ATTACK_SPEED ) );
		pmdBox->Format( "AttackSpeed", FONT_TEXT, COMP_TEXT_LEFT, COLOR_NORMAL, 0, FALSE, pString, pData->wAttack_Speed_Rate );
	}
	if( pData->Use_Item_Tblidx != INVALID_TBLIDX )
	{
		CUseItemTable* pUseItemTable = API_GetTableContainer()->GetUseItemTable();
		sUSE_ITEM_TBLDAT* pUseItemData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( pUseItemTable->FindData( pData->Use_Item_Tblidx ) );
		RwChar buf[32];

		if( !pUseItemData ) 
		{
			if( Logic_IsUIDevInfoVisible() )
			{
				sprintf_s( buf, 32, "UserItemTableId %u Error", pData->Use_Item_Tblidx );
				pmdBox->Format( buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5, 0, FALSE, L"There's no UsetItemTblID %u", pData->Use_Item_Tblidx );
			}
			return;
		}

		for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++i )
		{
			std::wstring wstrText;

			if( GetSystemEffectData( pUseItemData->aSystem_Effect[i], pUseItemData->afSystem_Effect_Value[i], wstrText, pUseItemData->abySystem_Effect_Type[i] ) )
			{
				sprintf_s( buf, 32, "UseItem%d", i );
				pmdBox->SetItem( wstrText.c_str(), buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );						

				if( pUseItemData->dwKeepTimeInMilliSecs )
				{
					pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_DURATIONTIME ) );

					sprintf_s( buf, 32, "UseItemKeepTime%d", i );
					m_pmdBox->Format( buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, pString, Logic_GetTimeString( pUseItemData->dwKeepTimeInMilliSecs / 1000.0f ) );					
				}
			}			
		}
	}
}

VOID CInfoWindowGui::SetItemInfo_SpecialEffect( gui::CMDStaticBox* pmdBox )
{
	pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	WCHAR* pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_SPECIAL_EFFECT ) );
	pmdBox->SetItem( pString, "SPecialEffectTitle", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );
}

RwBool CInfoWindowGui::SetItemInfo_Option( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt32 uiOptionTblIdx, RwBool bShowTitle, RwBool bBlankLine, RwInt32 nOrder /* = 0 */, RwUInt32 uiColor /* = INFOCOLOR_7  */ )
{
	if( uiOptionTblIdx == INVALID_TBLIDX )
		return TRUE;

	// Item Option
	//RwUInt32 uiColor = 0;	
	WCHAR* pString = NULL;	
	RwChar buf[32];

	CItemOptionTable* pItemOptionTable = API_GetTableContainer()->GetItemOptionTable();
	sITEM_OPTION_TBLDAT* pItemOptionData = reinterpret_cast<sITEM_OPTION_TBLDAT*>( pItemOptionTable->FindData( uiOptionTblIdx ) );
	if( !pItemOptionData ) 
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			sprintf_s( buf, 32, "OptionIdError%d", nOrder );
			pmdBox->Format( buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5, 0, FALSE, L"Invalid OptionTbl Index ( %u )", uiOptionTblIdx );
		}		
		return TRUE;
	}

	if( bBlankLine )
		pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	if( bShowTitle )
	{
		sprintf_s( buf, 32, "OptionEffectTitle%d", nOrder );
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_OPTION_EFFECT ) );
		pmdBox->SetItem( pString, buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );
	}

	//uiColor = Logic_GetItemRankColor( pItemOptionData->byOption_Rank );
	//uiColor = INFOCOLOR_7;
	
	for( RwInt32 i = 0 ; i < NTL_MAX_SYSTEM_EFFECT_COUNT ; ++i )
	{
		std::wstring wstrText;

		if( GetSystemEffectData( pItemOptionData->system_Effect[i], (RwReal)pItemOptionData->nValue[i], wstrText, pItemOptionData->bAppliedInPercent[i] ) )
		{
			sprintf_s( buf, 32, "%doptionvalue%d", nOrder, i );
			pmdBox->SetItem( wstrText.c_str(), buf, FONT_TEXT, COMP_TEXT_LEFT, uiColor );								
		}		
	}

	// 스카우터 인포는 따로 넣어준다.
	if( pItemOptionData->byScouterInfo != INVALID_BYTE )
	{
		if( pItemOptionData->byScouterInfo >= SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX &&
			pItemOptionData->byScouterInfo < SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION )
		{
			pmdBox->Format( "ScouterInfo", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, GetDisplayStringManager()->GetString( DST_SCOUTERPARTS_ITEM_IDENTIFICATION + pItemOptionData->byScouterInfo - SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX ), pData->byScouter_Parts_Value );	
		}	
		else if( pItemOptionData->byScouterInfo >= SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION )
		{
			pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SCOUTERPARTS_SPECIAL_START + pItemOptionData->byScouterInfo - SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION ), "ScouterInfo", FONT_TEXT, COMP_TEXT_LEFT, uiColor );
		}
	}

	return FALSE;
}     

VOID CInfoWindowGui::SetItemInfo_ScouterInfo( gui::CMDStaticBox* pmdBox, CNtlSobItem* pItem, sITEM_TBLDAT* pData )
{
	if( pItem )
	{
		if( pItem->IsScouterItem() )
		{
			pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			if( pItem->GetItemPosition() == CNtlSobItem::SIP_EQUIP )
			{
				CScouterBodyGui* pGui = reinterpret_cast<CScouterBodyGui*>( GetDialogManager()->GetDialog( DIALOG_SCOUTER_BODY ) );
				if( pGui )
					pmdBox->Format( "ScouterPower", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SCOUTER_POWER_CURRENT_INFO ) ), pGui->GetCurPower(), pData->wScouter_Watt );		
			}
			else
			{
				pmdBox->Format( "ScouterPower", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SCOUTER_POWER_MAX_INFO ) ), pData->wScouter_Watt );		
			}
		}		
		else if( pItem->IsScouterPartItem() )
		{
			pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
			pmdBox->Format( "PartsPower", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SCOUTER_PARTS_POWER_CUNSUMPTION ) ), pData->wScouter_Watt );
		}
	}
	else
	{
		if( pData->byItem_Type == ITEM_TYPE_SCOUTER )
		{
			pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
			pmdBox->Format( "ScouterPower", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SCOUTER_POWER_MAX_INFO ) ), pData->wScouter_Watt );		
		}		
		else if( pData->byItem_Type == ITEM_TYPE_SCOUTER_PART )
		{
			pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
			pmdBox->Format( "PartsPower", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SCOUTER_PARTS_POWER_CUNSUMPTION ) ), pData->wScouter_Watt );
		}
	}
}

VOID CInfoWindowGui::SetItemInfo_Durablity( gui::CMDStaticBox* pmdBox, RwUInt8 ucCurDur, RwUInt8 ucMaxDur, RwBool bJustMaxDurablity /* = false */)
{
	if( ucMaxDur == INVALID_BYTE )
		return;

	RwUInt32 uiColor = 0;	
	WCHAR* pString = NULL;	

	pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	// Durability
	RwReal fDurability = (RwReal)ucCurDur / (RwReal)ucMaxDur;

	if( fDurability > ITEM_DURATION_WARNING )
		uiColor = INFOCOLOR_0;
	else if( fDurability > 0.0f )
		uiColor = INFOCOLOR_7;
	else
		uiColor = INFOCOLOR_5;

	if( bJustMaxDurablity )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_JUST_DURABILITY ) );
		pmdBox->Format( "Durability", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, ucMaxDur );
	}
	else
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_DURABILITY ) );
		pmdBox->Format( "Durability", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, ucCurDur, ucMaxDur );
	}	
}

VOID CInfoWindowGui::SetItemInfo_SetItem( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobAvatar* pAvatar )
{
	struct stSETITEMDATA
	{
		enum STATE { eFLAG_STATE_GET = 0x01,
			eFLAG_STATE_EQUIP = 0x02 };

		stSETITEMDATA(VOID) { tblidx = INVALID_TBLIDX; flagState = 0x00; }

		RwUInt32	tblidx;
		RwUInt32	flagState;			
	};

	if( pData->set_Item_Tblidx == INVALID_TBLIDX )
		return;

	// Set Item Option
	RwChar buf[32];
	RwUInt32 uiColor = 0;	
	stSETITEMDATA stSetItemList[NTL_MAX_SET_ITEM_COUNT];

	pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	CSetItemTable* pSetItemTable = API_GetTableContainer()->GetSetItemTable();
	sSET_ITEM_TBLDAT* pSetItemData = reinterpret_cast<sSET_ITEM_TBLDAT*>( pSetItemTable->FindData( pData->set_Item_Tblidx ) );

	if( !pSetItemData)
	{
		if( Logic_IsUIDevInfoVisible() )
			pmdBox->Format( "SetItemInfoError", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5, 0, FALSE, L"There is no SetItemTblID %u", pData->set_Item_Tblidx );			

		return;
	}

	for( RwInt32 i = 0 ; i < NTL_MAX_SET_ITEM_COUNT ; ++i )
	{
		stSetItemList[i].tblidx = pSetItemData->aItemTblidx[i];
	}

	// 소지품 체크
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		if( pInventory->GetBagItem( i ) != INVALID_SERIAL_ID )
		{
			CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem( i ) ) );

			if( pBagItem )
			{
				for( RwInt32 j = 0 ; j < pBagItem->GetChildNum() ; ++j )
				{
					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pBagItem->GetChildSerial( j ) ) );

					if( pItem )
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

						if( pItemAttr->IsNeedToIdentify() )
							continue;

						sITEM_TBLDAT* pItemData = pItemAttr->GetItemTbl();

						for( RwInt32 k = 0 ; k < NTL_MAX_SET_ITEM_COUNT ; ++k )
						{
							if( stSetItemList[k].tblidx == pItemData->tblidx )
								stSetItemList[k].flagState |= stSETITEMDATA::eFLAG_STATE_GET;							
						}
					}					
				}
			}			
		}			
	}

	// 장비품 체크
	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		for( RwInt32 j = EQUIP_SLOT_TYPE_JACKET ; j <= EQUIP_SLOT_TYPE_BOOTS ; ++j )
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetEquipItem( i ) ) );

			if( pItem )
			{
				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
				sITEM_TBLDAT* pItemData = pItemAttr->GetItemTbl();

				for( RwInt32 k = 0 ; k < NTL_MAX_SET_ITEM_COUNT ; ++k )
				{
					if( stSetItemList[k].tblidx == pItemData->tblidx )
						stSetItemList[k].flagState |= stSETITEMDATA::eFLAG_STATE_EQUIP | stSETITEMDATA::eFLAG_STATE_GET;
				}
			}
		}
	}

	// 출력
	INT nEquipCount = 0;

	for( RwInt32 i = 0 ; i < NTL_MAX_SET_ITEM_COUNT ; ++i )
	{
		if( stSetItemList[i].tblidx != INVALID_TBLIDX )
		{
			sprintf_s( buf, "%dsetitem", i );

			if( stSetItemList[i].flagState & stSETITEMDATA::eFLAG_STATE_EQUIP )
				uiColor = INFOCOLOR_7;
			else
				uiColor = INFOCOLOR_9;

			if( stSetItemList[i].flagState & stSETITEMDATA::eFLAG_STATE_EQUIP )
				++nEquipCount;			

			CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT* pItemData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( stSetItemList[i].tblidx ) );

			pmdBox->SetItem( API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemData->Name ).c_str(),
				buf, FONT_TEXT, COMP_TEXT_LEFT, uiColor );
		}		
	}

	switch( nEquipCount )
	{
	case 2:	SetItemInfo_Option( pmdBox, pData, pSetItemData->semiSetOption, FALSE, TRUE, 3, INFOCOLOR_7 );
			SetItemInfo_Option( pmdBox, pData, pSetItemData->fullSetOption, FALSE, FALSE, 4, INFOCOLOR_9 ); break;
	case 3: SetItemInfo_Option( pmdBox, pData, pSetItemData->semiSetOption, FALSE, TRUE, 3, INFOCOLOR_7 );
			SetItemInfo_Option( pmdBox, pData, pSetItemData->fullSetOption, FALSE, FALSE, 4, INFOCOLOR_7 ); break;
	default: SetItemInfo_Option( pmdBox, pData, pSetItemData->semiSetOption, FALSE, TRUE, 3, INFOCOLOR_9 );
			 SetItemInfo_Option( pmdBox, pData, pSetItemData->fullSetOption, FALSE, FALSE, 4, INFOCOLOR_9 ); break;
	}
}

VOID CInfoWindowGui::SetItemInfo_LimitState( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, CNtlSobItemAttr* pSobItemAttr /* = NULL */, sITEM_PROFILE* pItemProfile /* = NULL  */ )
{
	// 제한
	RwBool bSell = FALSE;
	RwBool bUserTrade = FALSE;
	RwBool bSaveWareHouse = FALSE;
	RwBool bSaveCommonWareHouse = FALSE;
	RwBool bTimeLimitItem = FALSE;

	if( pSobItemAttr )
	{		
		bSell = Logic_IsCanSellItem( pSobItemAttr );
		bUserTrade = Logic_IsCanUserTradeItem( pSobItemAttr );
		bSaveWareHouse = Logic_IsCanSaveWarehouse( pSobItemAttr );
		bSaveCommonWareHouse = Logic_IsCanSaveCommonWarehouse( pSobItemAttr );
		bTimeLimitItem = Logic_IsTimeLimitItem( pSobItemAttr );
	}
	else if( pItemProfile )
	{
		bSell = Logic_IsCanSellItem( pItemProfile );
		bUserTrade = Logic_IsCanUserTradeItem( pItemProfile );
		bSaveWareHouse = Logic_IsCanSaveWarehouse( pItemProfile );
		bSaveCommonWareHouse = Logic_IsCanSaveCommonWarehouse( pItemProfile );
		bTimeLimitItem = Logic_IsTimeLimitItem( pItemProfile );
	}
	else
	{
		bSell = Logic_IsCanSellItem( pData );
		bUserTrade = Logic_IsCanUserTradeItem( pData );
		bSaveWareHouse = Logic_IsCanSaveWarehouse( pData );
		bSaveCommonWareHouse = Logic_IsCanSaveCommonWarehouse( pData );		
	}

	if( !bSell || !bUserTrade || !bSaveWareHouse || !bSaveCommonWareHouse || bTimeLimitItem )
		pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	else
		return;

	const WCHAR* pText = NULL;

	if( bTimeLimitItem )
	{
		// 사용기한
		if( pSobItemAttr )
		{
			if( pSobItemAttr->GetDurationType() )
			{
				WCHAR buf[256];
				DBOTIME timeRemain = pSobItemAttr->GetRemainTime();

				if( timeRemain > 0 )
				{
					const WCHAR* pStringDay = GetDisplayStringManager()->GetString( DST_TIME_DAY );
					RwUInt32 uiDay = (RwUInt32)( timeRemain / 86400 );

					DBOTIME timeExceptDay = timeRemain - ( uiDay * 86400 );
					RwUInt32 uiHour = (RwUInt32)( timeExceptDay / 3600 );

					if( uiDay > 0 )
					{
						if( uiHour > 0 )
						{
							const WCHAR* pStringHour = GetDisplayStringManager()->GetString( DST_TIME_AN_HOUR );
							swprintf_s( buf, 256, L"%u%s %u%s", uiDay, pStringDay, uiHour, pStringHour );
						}
						else
							swprintf_s( buf, 256, L"%u%s", uiDay, pStringDay );
					}
					else
					{
						if( uiHour > 0 )
						{
							const WCHAR* pStringHour = GetDisplayStringManager()->GetString( DST_TIME_AN_HOUR );
							swprintf_s( buf, 256, L"%u%s",uiHour, pStringHour );
						}
						else
							swprintf_s( buf, 256, L"%s", GetDisplayStringManager()->GetString( DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_INFO_BELOW_HOUR ) );
					}

					pmdBox->Format( "TimeLimit", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5, 0, FALSE, GetDisplayStringManager()->GetString( DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_INFO ), buf );
				}
				else
				{
					pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_EXPIRED_INFO ), "TimeLimit", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
				}
			}
		}
	}

	if( !bSell )
	{
		pText = GetDisplayStringManager()->GetString( DST_ITEM_LIMITED_SELL );
		pmdBox->SetItem( pText, "SellLimit", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
	}

	if( !bUserTrade )
	{
		pText = GetDisplayStringManager()->GetString( DST_ITEM_LIMITED_USER_TRADE );
		pmdBox->SetItem( pText, "UserTradeLimit", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
	}

	if( !bSaveWareHouse )
	{
		pText = GetDisplayStringManager()->GetString( DST_ITEM_LIMITED_STORE_WAREHOUSE );
		pmdBox->SetItem( pText, "SaveWareHouseLimit", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
	}

	if( !bSaveCommonWareHouse)
	{
		pText = GetDisplayStringManager()->GetString( DST_ITEM_LIMITED_STORE_COMMON_WAREHOUSE );
		pmdBox->SetItem( pText, "SaveCommonWareHouseLimit", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
	}
}

VOID CInfoWindowGui::SetItemInfo_SellPrice( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byStackCount )
{
	WCHAR* pString;

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_SELL_COST ) );

	RwUInt32 uiSellCost = Logic_CalcPrice_for_Sell( pData, NULL, byStackCount );
	pmdBox->Format( "SellCost", FONT_TEXT, COMP_TEXT_LEFT, COLOR_ITEM_COST, 0, FALSE, pString, uiSellCost );
}

VOID CInfoWindowGui::SetItemInfo_RepairCost( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData, RwUInt8 byCurDur, RwUInt8 byMaxDur )
{
	WCHAR* pString;

	RwReal fDurRatio = (RwReal)byCurDur / (RwReal)byMaxDur;

	if( fDurRatio < 1.0f )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_ITEM_REPAIR_COST ) );
		RwUInt32 uiRepairPay = Dbo_GetRepairPay(pData->dwCost, byMaxDur, byCurDur);

		pmdBox->Format( "RepairCost", FONT_TEXT, COMP_TEXT_LEFT, COLOR_ITEM_COST, 0, FALSE, pString, uiRepairPay );
	}
}

// SKILL SUB /////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID CInfoWindowGui::SetSkillInfo_NameGradeClass( sSKILL_TBLDAT* pData, RwBool bLearned )
{
	CTextTable* pSkillText = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();

	RwUInt32 uiColor = bLearned ? INFOCOLOR_2 : INFOCOLOR_1;	
	WCHAR* pString = NULL;	

	// 이름
	m_pmdBox->SetItem( pSkillText->GetText( pData->Skill_Name ).c_str(), "SkillName", FONT_TITLE, COMP_TEXT_LEFT, uiColor );

	// 등급
	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_GRADE ) );
	m_pmdBox->Format( "SkillGrade", FONT_TEXT, COMP_TEXT_RIGHT, uiColor, 10, TRUE, pString, pData->bySkill_Grade );

	// 종류
	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	if( pData->bySkill_Class == NTL_SKILL_CLASS_PASSIVE )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_TYPE_PASSIVE ) );
		uiColor = INFOCOLOR_4;
	}
	else if( pData->bySkill_Class == NTL_SKILL_CLASS_ACTIVE )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_TYPE_ACTIVE ) );
		uiColor = INFOCOLOR_3;
	}
	else if( pData->bySkill_Class == NTL_SKILL_CLASS_HTB )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_TYPE_HTB ) );
		uiColor = INFOCOLOR_6;
	}

	m_pmdBox->SetItem( pString, "SkillClass", FONT_TEXT, COMP_TEXT_CENTER, uiColor, 0 );	

	// Note
	if( pData->Note != INVALID_TBLIDX )
	{
		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
		m_pmdBox->SetItem( pSkillText->GetText( pData->Note ).c_str(), "SkillNote", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
	}

	// 필요장착
	if( pData->byRequire_Item_Type != INVALID_BYTE )
	{
		const WCHAR* pFormatString = GetDisplayStringManager()->GetString( DST_SKILL_MUST_EQUIP_ITEM );
		const WCHAR* pFormatData = GetDisplayStringManager()->GetString( DST_GLOVE + pData->byRequire_Item_Type );

		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
		m_pmdBox->Format( "SkillItemLimit", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5, 0, FALSE, pFormatString, pFormatData );
	}
}

VOID CInfoWindowGui::SetSkillInfo_LearnCondition( sSKILL_TBLDAT* pData, RwBool bGray )
{
	WCHAR* pString = NULL;	
	CTextTable* pSkillText = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();
	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();

	if( pData->byRequire_Train_Level == INVALID_BYTE &&
		pData->uiRequire_Skill_Tblidx_Min_1 == INVALID_TBLIDX )
		return;

	RwUInt32 uiColor = bGray ? INFOCOLOR_9 : INFOCOLOR_2;	

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NEED_TEXT ) );
	m_pmdBox->SetItem( pString, "NeedText", FONT_TEXT, COMP_TEXT_LEFT, uiColor );

	if( pData->byRequire_Train_Level != INVALID_BYTE )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NEED_LEVEL ) );
		m_pmdBox->Format( "RequreLevel", FONT_TEXT, COMP_TEXT_RIGHT, uiColor, 0, FALSE, pString, (RwInt32)pData->byRequire_Train_Level );
	}

	if( pData->uiRequire_Skill_Tblidx_Min_1 != INVALID_TBLIDX )
	{
		sSKILL_TBLDAT* pRequreSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pData->uiRequire_Skill_Tblidx_Min_1 ) );
		NTL_ASSERT( pRequreSkillData, "CInfoWindowGui::SetSkillInfo_LearnCondition : SkillIdx( " << pData->byRequire_Train_Level << " ) 가 존재하지 않습니다." );

		if( pRequreSkillData )
		{
			if( pRequreSkillData->dwNextSkillTblidx != INVALID_TBLIDX )
			{
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NEED_SKILL ) );
				m_pmdBox->Format( "RequreSkill1", FONT_TEXT, COMP_TEXT_RIGHT, uiColor, 0, FALSE, pString, pSkillText->GetText( pRequreSkillData->Skill_Name ).c_str(), (RwInt32)pRequreSkillData->bySkill_Grade );
			}
			else
			{
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NEED_MASTERSKILL ) );
				m_pmdBox->Format( "RequreSkill1", FONT_TEXT, COMP_TEXT_RIGHT, uiColor, 0, FALSE, pString, pSkillText->GetText( pRequreSkillData->Skill_Name ).c_str() );
			}
		}		
	}

	if( pData->uiRequire_Skill_Tblidx_Min_2 != INVALID_TBLIDX )
	{
		sSKILL_TBLDAT* pRequreSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pData->uiRequire_Skill_Tblidx_Min_2 ) );
		NTL_ASSERT( pRequreSkillData, "CInfoWindowGui::SetSkillInfo_LearnCondition : SkillIdx( " << pData->byRequire_Train_Level << " ) 가 존재하지 않습니다." );

		if( pRequreSkillData )
		{
			if( pRequreSkillData->dwNextSkillTblidx != INVALID_TBLIDX )
			{
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NEED_SKILL ) );
				m_pmdBox->Format( "RequreSkill2", FONT_TEXT, COMP_TEXT_RIGHT, uiColor, 0, FALSE, pString, pSkillText->GetText( pRequreSkillData->Skill_Name ).c_str(), (RwInt32)pRequreSkillData->bySkill_Grade );
			}
			else
			{
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NEED_MASTERSKILL ) );
				m_pmdBox->Format( "RequreSkill2", FONT_TEXT, COMP_TEXT_RIGHT, uiColor, 0, FALSE, pString, pSkillText->GetText( pRequreSkillData->Skill_Name ).c_str() );
			}
		}		
	}
}

VOID CInfoWindowGui::SetSkillInfo_UseCondition( sSKILL_TBLDAT* pData, sSKILL_TBLDAT* pBaseData, RwBool bGrayed /* = FALSE */, CNtlSobSkill* pSkill /* = NULL  */ )
{
	RwUInt32 uiColor = INFOCOLOR_9;	
	WCHAR* pString = NULL;	
	std::string	strItemName;
	CNtlSobSkillAttr* pAttr = NULL;

	if( pSkill )
		pAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSkill->GetSobAttr() );

	if( pBaseData->wRequire_LP || pBaseData->wRequire_EP )
		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	if( pBaseData->wRequire_LP )
	{
		strItemName = "RequireLP";

		if( !bGrayed )
			uiColor = INFOCOLOR_5;
		else
		{
			uiColor = INFOCOLOR_9;
			strItemName += "Grayed";
		}

		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_LP_CONSUMPTION ) );
		m_pmdBox->Format( strItemName.c_str(), FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, pData->wRequire_LP );
	}

	if( pBaseData->wRequire_EP )
	{
		strItemName = "RequireEP";

		if( !bGrayed )
			uiColor = INFOCOLOR_3;
		else
		{
			uiColor = INFOCOLOR_9;
			strItemName += "Grayed";
		}

		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_EP_CONSUMPTION ) );
		RwReal fRequredEP = (RwReal)pData->wRequire_EP;

		if( pSkill )
		{
			CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( pSkill->GetOwnerID() ) );
			fRequredEP *= Logic_GetRequiredEPFactor( pActor );			
			uiColor = GetGuiFuntor()->ColorDecisionF( (RwReal)pAttr->GetBaseSkillTbl()->wRequire_EP, fRequredEP, CGuiFunctor::SMALL_IS_GOOD, uiColor );			
		}	

		m_pmdBox->Format( strItemName.c_str(), FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, (RwInt32)fRequredEP );
	}

	if( pAttr )
	{
		pString = NULL;

		if( pAttr->m_bRPBonusAutoMode )
		{
			switch( pAttr->m_byRPBonusType )
			{
			case DBO_RP_BONUS_TYPE_KNOCKDOWN: 
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_INFOWND_RPBONUS_KNOCKDOWN ) );
				break;
			case DBO_RP_BONUS_TYPE_RESULT_PLUS:  
				/*case DBO_RP_BONUS_TYPE_RESULT_PERCENT: */
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_INFOWND_RPBONUS_POWER ) );
				break;
			case DBO_RP_BONUS_TYPE_KEEP_TIME_PLUS: 
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_INFOWND_RPBONUS_KEEPTIME ) );
				break;			
			case DBO_RP_BONUS_TYPE_CASTING_TIME_MINUS: 
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_INFOWND_RPBONUS_CASTTIME ) );
				break;		
			case DBO_RP_BONUS_TYPE_COOL_TIME_MINUS:
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_INFOWND_RPBONUS_COOLTIME ) );
				break;			
			case DBO_RP_BONUS_TYPE_EP_MINUS:
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_INFOWND_RPBONUS_EP ) );
				break;			
			case DBO_RP_BONUS_TYPE_INVALID:
				pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_INFOWND_RPBONUS_USE ) );
				break;			
			}
		}

		if( pString )
			m_pmdBox->SetItem(  pString, "RpBonusType", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_6, 0, FALSE );
	}
}

VOID CInfoWindowGui::SetSkillInfo_EffectData( sSKILL_TBLDAT* pData, RwUInt32 uiColor, RwInt32 nOrder /* = 0 */,CNtlSobSkill* pSkill /* = NULL  */ )
{
	CSystemEffectTable* pSystemEffectTable = API_GetTableContainer()->GetSystemEffectTable();
	RwChar buf[32];
	RwUInt32 uiFinalColor = uiColor;
	RwUInt32* pFinalColor = NULL;

	if( pData->skill_Effect[0] != INVALID_TBLIDX )
		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++ i )
	{
		std::wstring wstrText;
		RwReal fSkillEffectValue = pData->fSkill_Effect_Value[i];			

		if( pSkill )
		{
			if( pData->skill_Effect[i] == INVALID_TBLIDX )
				continue;
			sSYSTEM_EFFECT_TBLDAT* pSystemEffectData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffectTable->FindData( pData->skill_Effect[i] ) );		

			if( pSystemEffectData )
			{
				CNtlSobSkillAttr* pAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSkill->GetSobAttr() );

				if( pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DOT )
				{
					CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( pSkill->GetOwnerID() ) );
					fSkillEffectValue *= Logic_GetDOTValueFactor( pActor );					
				}

				uiFinalColor = GetGuiFuntor()->ColorDecisionF( (RwReal)pAttr->GetBaseSkillTbl()->fSkill_Effect_Value[i], fSkillEffectValue, CGuiFunctor::BIG_IS_GOOD, uiColor );			

				if( uiColor != INFOCOLOR_9 && uiFinalColor == uiColor )
					pFinalColor = &uiFinalColor;

			}
		}		
		else
		{
			if( uiColor != INFOCOLOR_9 )
				pFinalColor = &uiFinalColor;
		}

		if( GetSystemEffectData( pData->skill_Effect[i], fSkillEffectValue, wstrText, pData->bySkill_Effect_Type[i], pData, pFinalColor ) )
		{
			sprintf_s( buf, 32, "%dSkillEffect%d", nOrder, i );
			m_pmdBox->SetItem( wstrText.c_str(), buf, FONT_TEXT, COMP_TEXT_LEFT, uiFinalColor );						
		}		
	}
}

#define DBO_SKILL_APPLY_TARGET_APPOINT_ERROR	-1
#define DBO_SKILL_APPLY_TARGET_APPLY_ERROR		-2
#define DBO_SKILL_APPLY_TARGET_RANGE_ERROR		-3

VOID CInfoWindowGui::SetSkillInfo_AppointApply( sSKILL_TBLDAT* pData, sSKILL_TBLDAT* pBaseData /* = NULL  */ )
{
	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	switch( pData->byAppoint_Target )
	{
	case DBO_SKILL_APPOINT_TARGET_SELF:
		switch( pData->byApply_Target )
		{
		case DBO_SKILL_APPLY_TARGET_SELF:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_SINGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO1 ); return;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_ENEMY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO2, pData->byApply_Area_Size_1, 0, pBaseData ); break;				
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO3, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;				
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_ALLIANCE:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO4, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO5, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_PARTY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO7, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO8, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_ANY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO26, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO27, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_APPLY_ERROR ); break;
		}
		break;
	case DBO_SKILL_APPOINT_TARGET_TARGET:
		switch( pData->byApply_Target )
		{			
		case DBO_SKILL_APPLY_TARGET_ENEMY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_SINGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO9 ); break;
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO11, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO12, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_FORWARD: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO13, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_ALLIANCE:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_SINGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO14 ); break;
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO15, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO16, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_PARTY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_SINGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO17 ); break;
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO18,pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO19,pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_ANY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_SINGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO28 ); break;
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO29, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO30, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_SELF:
			{
				CSystemEffectTable* pSystemEffectTable = API_GetTableContainer()->GetSystemEffectTable();
								
				for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
				{
					// peessi : Warp 인 경우 예외처리.
					sSYSTEM_EFFECT_TBLDAT* pSystemEffectData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffectTable->FindData( pData->skill_Effect[i] ) );			

					if( pSystemEffectData )
					{
						if( pSystemEffectData->effectCode == ACTIVE_WARP )
						{
							SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO6 );
							break;
						}		
						else if( pSystemEffectData->effectCode == ACTIVE_VISIT )
						{
							SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO33 );
							break;
						}
					}					
				}
			}			
			break;
		default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_APPLY_ERROR ); break;
		}
		break;
	case DBO_SKILL_APPOINT_TARGET_POINT:
		switch( pData->byApply_Target )
		{
		case DBO_SKILL_APPLY_TARGET_ENEMY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO20, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO21, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_ALLIANCE:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO22, pData->byApply_Area_Size_1, 0, pBaseData); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO23, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_PARTY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO24, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO25, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		case DBO_SKILL_APPLY_TARGET_ANY:
			switch( pData->byApply_Range )
			{
			case DBO_SKILL_APPLY_RANGE_CIRCLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO31, pData->byApply_Area_Size_1, 0, pBaseData ); break;
			case DBO_SKILL_APPLY_RANGE_RECTANGLE: SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO32, pData->byApply_Area_Size_1, pData->byApply_Area_Size_2, pBaseData ); break;
			default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_RANGE_ERROR ); break;
			}
			break;
		default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_APPLY_ERROR ); break;
		}
		break;
	default: SetSkillInfo_AppointApplySetString( DBO_SKILL_APPLY_TARGET_APPOINT_ERROR ); break;		
	}

	if( pData->wFunction_Bit_Flag & SKILL_FUNCTION_FLAG_APPLY_NOT_ME )
		SetSkillInfo_AppointApplySetString( DST_SKILL_TARGET_AND_RANGE_INFO34, 0, 0, NULL, 1 );
}

VOID CInfoWindowGui::SetSkillInfo_AppointApplySetString( RwInt32 nIndex, RwUInt8 byRange1 /* = 0 */, RwUInt8 byRange2 /* = 0 */, sSKILL_TBLDAT* pBaseData /* = NULL */, RwInt32 i /* = 0  */ )
{
	WCHAR* pString = NULL;

	if( TRUE )// Logic_IsUIDevInfoVisible() )
	{
		switch( nIndex )
		{
		case DBO_SKILL_APPLY_TARGET_APPOINT_ERROR: pString = L"Invalid Appoint Value. Check Table"; break;
		case DBO_SKILL_APPLY_TARGET_APPLY_ERROR: pString = L"Invalid Apply Value. Check Table"; break;
		case DBO_SKILL_APPLY_TARGET_RANGE_ERROR: pString = L"Invalid Range Value. Check Table"; break;
		default: pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( nIndex ) ); break;		
		}
	}
	else
	{
		if( nIndex >= 0 )
			pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( nIndex ) );
	}

	if( pString )
	{
		RwUInt32 uiColor = INFOCOLOR_0;
		RwChar buf[256];
		sprintf_s( buf, 256, "Apply%d", i );

		if( byRange1 > 0 )
		{
			if( byRange2 > 0 )
			{
				if( pBaseData )
				{
					uiColor = GetGuiFuntor()->ColorDecisionF( pBaseData->byApply_Area_Size_1, byRange1, CGuiFunctor::BIG_IS_GOOD, uiColor );
					uiColor = GetGuiFuntor()->ColorDecisionF( pBaseData->byApply_Area_Size_2, byRange2, CGuiFunctor::BIG_IS_GOOD, uiColor );
				}
				
				m_pmdBox->Format( buf, FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, byRange1, byRange2 );
			}
			else
			{
				if( pBaseData )
					uiColor = GetGuiFuntor()->ColorDecisionF( pBaseData->byApply_Area_Size_1, byRange1, CGuiFunctor::BIG_IS_GOOD, uiColor );

				m_pmdBox->Format( buf, FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, byRange1 );
			}
		}
		else
			m_pmdBox->SetItem( pString, buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 ); 
	}
	else
	{

		if( Logic_IsUIDevInfoVisible() )
		{
			RwChar buf[256];
			sprintf_s( buf, 256, "ApplyError %d", i );
			m_pmdBox->SetItem( L"Invalid UI String Table ID", buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
		}
	}
}

VOID CInfoWindowGui::SetSkillInfo_Parameter( CNtlSobSkill* pSkill, sSKILL_TBLDAT* pData, sSKILL_TBLDAT* pBaseData, RwBool bGrayed )
{
	RwUInt32 uiColor;
	WCHAR* pString = NULL;	
	std::string strItemName;
	
	if( pBaseData->fCasting_Time )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_CASTTIME ) );
		RwReal fCastingTime = pData->fCasting_Time;

		strItemName = "CastTime";

		if( pSkill )
		{
			CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( pSkill->GetOwnerID() ) );
			fCastingTime *= Logic_GetCastingTimeFactor( pActor );
			uiColor = GetGuiFuntor()->ColorDecisionF( pBaseData->fCasting_Time, fCastingTime, CGuiFunctor::SMALL_IS_GOOD, INFOCOLOR_0 );			
		}
		else
			uiColor = INFOCOLOR_0;		

		if( bGrayed )
		{
			strItemName += "Grayed";
			uiColor = INFOCOLOR_9;
		}

		m_pmdBox->Format( strItemName.c_str(), FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, Logic_GetTimeString( fCastingTime ) );
	}

	if( pBaseData->wKeep_Time )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_DURATIONTIME ) );
		RwReal fKeepingTime = (RwReal)pData->wKeep_Time;

		strItemName = "DurationTime";

		if( pSkill )
		{
			CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( pSkill->GetOwnerID() ) );
			fKeepingTime *= Logic_GetKeepingTimeFactor( pActor );

			for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++ i )
			{
				std::wstring wstrText;
				RwUInt8 bySkillEffectType = pData->bySkill_Effect_Type[i];

				if( pData->bySkill_Effect_Type[i] == SKILL_ACTIVE_TYPE_DOT )
				{
					fKeepingTime += Logic_GetDOTTimeFactor( pActor );			
					break;
				}
			}				

			uiColor = GetGuiFuntor()->ColorDecisionF( (RwReal)pBaseData->wKeep_Time, fKeepingTime, CGuiFunctor::BIG_IS_GOOD, INFOCOLOR_0 );			
		}
		else
			uiColor = INFOCOLOR_0;		

		if( bGrayed )
		{
			strItemName += "Grayed";
			uiColor = INFOCOLOR_9;
		}

		m_pmdBox->Format( strItemName.c_str(), FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, Logic_GetTimeString( fKeepingTime ) );
	}

	uiColor = bGrayed ? INFOCOLOR_9 : INFOCOLOR_0;	

	if( pBaseData->wCool_Time )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_COOLTIME ) );
		RwReal fCoolingTime = (RwReal)pData->wCool_Time;

		strItemName = "CoolTime";

		if( pSkill )
		{
			CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( pSkill->GetOwnerID() ) );
			fCoolingTime *= Logic_GetCoolingTimeFactor( pActor );
			uiColor = GetGuiFuntor()->ColorDecisionF( (RwReal)pBaseData->wCool_Time, fCoolingTime, CGuiFunctor::SMALL_IS_GOOD, INFOCOLOR_0 );
		}	
		else
			uiColor = INFOCOLOR_0;		

		if( bGrayed )
		{
			strItemName += "Grayed";
			uiColor = INFOCOLOR_9;
		}

		m_pmdBox->Format( strItemName.c_str(), FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, Logic_GetTimeString( fCoolingTime ) );
	}

	if( pBaseData->fUse_Range_Max )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_USERANGE ) );
		
		RwReal fUseRange = (RwReal)pData->byUse_Range_Max;
		
		if( pData->bDash_Able )
			fUseRange += NTL_MAX_DASH_DISTANCE_FOR_SKILL;
		
		strItemName = "UseRange";

		if( pSkill )
		{
			uiColor = GetGuiFuntor()->ColorDecisionF( (RwReal)pBaseData->byUse_Range_Max, fUseRange, CGuiFunctor::BIG_IS_GOOD, INFOCOLOR_0 );
		}
		else
			uiColor = INFOCOLOR_0;		

		if( bGrayed )
		{
			strItemName += "Grayed";
			uiColor = INFOCOLOR_9;
		}

		m_pmdBox->Format( strItemName.c_str(), FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, (RwInt32)fUseRange );
	}
}

VOID CInfoWindowGui::SetHTBInfo_NameGradeClass( sHTB_SET_TBLDAT* pData, RwBool bLearned )
{
	CTextTable* pHTBTextTable = API_GetTableContainer()->GetTextAllTable()->GetHTBSetTbl();

	RwUInt32 uiColor = bLearned ? INFOCOLOR_2 : INFOCOLOR_1;	
	WCHAR* pString = NULL;	

	// 이름
	m_pmdBox->SetItem( pHTBTextTable->GetText( pData->HTB_Skill_Name ).c_str(), "SkillName", FONT_TITLE, COMP_TEXT_LEFT, uiColor );

	// 등급
	m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SKILL_TYPE_HTB ), "grade", FONT_TEXT, COMP_TEXT_RIGHT, uiColor, 10, TRUE );

	// 종류
	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_TYPE_HTB ) );
	m_pmdBox->SetItem( pString, "SkillClass", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_6, 0 );

	// 설명
	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	m_pmdBox->SetItem( pHTBTextTable->GetText( pData->Note ).c_str(), "SkillNote", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
}

VOID CInfoWindowGui::SetHTBInfo_LearnCondition( sHTB_SET_TBLDAT* pData )
{
	WCHAR* pString = NULL;

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_NEED_LEVEL ) );
	m_pmdBox->Format( "RequireLevel", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_2, 0, FALSE, pString, (RwInt32)pData->byRequire_Train_Level );
}

VOID CInfoWindowGui::SetHTBInfo_UseCondition( sHTB_SET_TBLDAT* pData )
{
	WCHAR* pString = NULL;	

	// 사용 LP,EP,RP
	if( pData->wNeed_EP > 0 )
	{
		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_RP_CONSUMPTION ) );
		m_pmdBox->Format( "RequireRP", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_6, 0, FALSE, pString, pData->wNeed_EP );
	}
}

VOID CInfoWindowGui::SetHTBInfo_EffectData( sHTB_SET_TBLDAT* pData )
{
	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	sSKILL_TBLDAT* pSkillData = NULL;
	RwReal fDamageTotal = 0;
	WCHAR* pString = NULL;
	RwChar buf[32];

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	for( RwInt32 i = 0 ; i < NTL_HTB_MAX_SKILL_COUNT_IN_SET ; ++i )
	{
		if( pData->aHTBAction[i].bySkillType != 0 || pData->aHTBAction[i].skillTblidx == INVALID_TBLIDX )
			continue;

		pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pData->aHTBAction[i].skillTblidx ) );

		if( !pSkillData )
		{
			if( Logic_IsUIDevInfoVisible() )
			{
				sprintf_s( buf, 32, "SkillTblId%dError", i );
				m_pmdBox->Format( buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5, 0, FALSE, L"There is no %d field SkillSetID %u", i, pData->aHTBAction[i].skillTblidx );
			}		
		}
		else
		{
			if( pSkillData->skill_Effect[0] != INVALID_TBLIDX )
				fDamageTotal += pSkillData->fSkill_Effect_Value[0];
			if( pSkillData->skill_Effect[1] != INVALID_TBLIDX )
				fDamageTotal += pSkillData->fSkill_Effect_Value[1];
		}
	}

	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_TOTAL_DAMAGE ) );
	m_pmdBox->Format( "EffectTotal", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7, 0, FALSE, pString, (RwInt32)fDamageTotal );
}

VOID CInfoWindowGui::SetHTBInfo_ApplyParameter( CNtlSobHTBSkill* pHTB, sHTB_SET_TBLDAT* pData )
{
	WCHAR* pString = NULL;	
	RwUInt32 uiColor = INFOCOLOR_0;	

	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_APPLY_INSTANCE_ENEMY ) );
	m_pmdBox->SetItem( pString, "HTBApply", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );

	if( pData->wCool_Time )
	{
		pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_COOLTIME ) );
		RwReal fCoolingTime = (RwReal)pData->wCool_Time;

		if( pHTB )
		{
			CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( pHTB->GetOwnerID() ) );
			fCoolingTime *= Logic_GetCoolingTimeFactor( pActor );
			uiColor = GetGuiFuntor()->ColorDecisionF( (RwReal)pData->wCool_Time, fCoolingTime, CGuiFunctor::SMALL_IS_GOOD );
		}	

		m_pmdBox->Format( "CoolTime", FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, FALSE, pString, Logic_GetTimeString(fCoolingTime) );
	}	
}

// BUFF SUB //////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID CInfoWindowGui::SetBuffInfo_Common( sSKILL_TBLDAT* pData, RwReal fKeepTime, RwReal* pfEffectValue /* = NULL  */ )
{
	WCHAR* pString = NULL;
	RwUInt32 uiColor = INFOCOLOR_7;
	RwChar buf[32];

	CTextTable* pSkillText = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();
	
	// 이름
	m_pmdBox->SetItem( pSkillText->GetText( pData->Skill_Name ).c_str(), "SkillName", FONT_TITLE, COMP_TEXT_CENTER, INFOCOLOR_2 );

	// 정보
	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );		

	for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++ i )
	{
		std::wstring wstrText;
		RwReal fEffectValue;
		RwBool bBuffShow = TRUE;

		if( pfEffectValue )
			fEffectValue = pfEffectValue[i];
		else
			fEffectValue = pData->fSkill_Effect_Value[i];

		if( GetSystemEffectData( pData->skill_Effect[i], fEffectValue, wstrText, pData->bySkill_Effect_Type[i], pData, &uiColor, &bBuffShow ) )
		{
			sprintf_s( buf, 32, "%dSkillEffect", i );
			m_pmdBox->SetItem( wstrText.c_str(), buf, FONT_TEXT, COMP_TEXT_LEFT, uiColor );						
		}		
	}

	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_DURATIONTIME ) );
	m_pmdBox->Format( "DurationTime", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, pString, Logic_GetTimeString(fKeepTime) );
}

VOID CInfoWindowGui::SetBuffInfo_Common( sITEM_TBLDAT* pData )
{
	sUSE_ITEM_TBLDAT* pUseItemData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( API_GetTableContainer()->GetUseItemTable()->FindData( pData->Use_Item_Tblidx ) );
	NTL_ASSERT( pUseItemData, "UseItemTbl Index" << pData->Use_Item_Tblidx << "is Invalid" );

	WCHAR* pString = NULL;
    RwChar buf[32] = {0,};

	CTextTable* pItemText = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
	
	// 이름
	m_pmdBox->SetItem( pItemText->GetText( pData->Name ).c_str(), "SkillName", FONT_TITLE, COMP_TEXT_CENTER, INFOCOLOR_2 );

	// 정보
	m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );		

	for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++ i )
	{
		std::wstring wstrText;
		RwBool bBuffShow = TRUE;

		if( GetSystemEffectData( pUseItemData->aSystem_Effect[i], pUseItemData->afSystem_Effect_Value[i], wstrText, pUseItemData->abySystem_Effect_Type[i], NULL, NULL, &bBuffShow ) )
		{
			sprintf_s( buf, 32, "%dSkillEffect", i );
			m_pmdBox->SetItem( wstrText.c_str(), buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );						
		}
	}

	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_SKILL_DURATIONTIME ) );
	m_pmdBox->Format( "DurationTime", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, pString, Logic_GetTimeString(pUseItemData->wKeep_Time) );
}

VOID CInfoWindowGui::SetBuffInfo_RemainTime( RwReal fMaxTime, RwReal fTime )
{
	WCHAR* pString;

	pString = const_cast<WCHAR*>( GetDisplayStringManager()->GetString( DST_BUFF_REMAIN_TIME ) );
	RwReal fRemainTime = fMaxTime - fTime + 0.99f; // 내림연산의 부작용방지
	if( fRemainTime < 0 )
		fRemainTime = 0;
	m_pmdBox->Format( "RemainTime", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, FALSE, pString, Logic_GetTimeString(fRemainTime) );
}

VOID CInfoWindowGui::SetBuffInfo_DropInfo(VOID)
{
	const WCHAR* pString;
	pString = GetDisplayStringManager()->GetString( DST_BUFF_DROP_TEXT );

	m_pmdBox->SetItem( pString, "DropInfo", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );	
}

// REWARD SUB ////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CInfoWindowGui::SetRewardInfo_Exp( RwInt32 nValue )
{
	m_pmdBox->Clear();

	m_pmdBox->Format( "Exp", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0, 0, FALSE, GetDisplayStringManager()->GetString( DST_QUESTREWARD_INFO_EXP ), nValue );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetRewardInfo_Zenny( RwInt32 nValue )
{
	m_pmdBox->Clear();

	m_pmdBox->Format( "Zenny", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0, 0, FALSE, GetDisplayStringManager()->GetString( DST_QUESTREWARD_INFO_ZENNY ), nValue );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetRewardInfo_Class( RwInt32 nValue )
{
	m_pmdBox->Clear();

	m_pmdBox->Format( "Class Change", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0, 0, FALSE, GetDisplayStringManager()->GetString( DST_QUESTREWARD_INFO_CLASS ), GetDisplayStringManager()->GetString( DST_HUMAN_FIGHTER + nValue ) );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetRewardInfo_Reputation( RwInt32 nValue )
{
	m_pmdBox->Clear();

	m_pmdBox->Format( "Reputation", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0, 0, FALSE, GetDisplayStringManager()->GetString( DST_QUESTREWARD_INFO_REPUTATION ), nValue );

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetPortalInfo(RwUInt32 nPortalID, RwInt32 ePortalIconType, DWORD dwZenny)
{
	sPORTAL_TBLDAT* pPortalData = (sPORTAL_TBLDAT*)API_GetTableContainer()->GetPortalTable()->FindData(nPortalID);
	if(!pPortalData)
		return;

	m_pmdBox->Clear();

	std::wstring wstrMapName = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl()->GetText(pPortalData->dwPointName);                
	m_pmdBox->SetItem(wstrMapName.c_str(), "PortalName", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_0);


	// Portal Status
	switch(ePortalIconType)
	{
	case E_PORTAL_ICON_START_POINT_FOCUS:
		m_pmdBox->SetItem(GetDisplayStringManager()->GetString(DST_PORTAL_START_POINT), "PortalStartPoint", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_7);
		break;
	case E_PORTAL_ICON_ENABLE_FOCUS:     
		{
			WCHAR buf[64] = {0,};
			swprintf_s(buf, L"%s:%u", GetDisplayStringManager()->GetString(DST_PORTAL_ZENNY), dwZenny);
			m_pmdBox->SetItem(buf, "PortalZenny", FONT_TEXT, COMP_TEXT_CENTER, INFOCOLOR_1);
		}        
		break;    
	default:
		NTL_ASSERT(FALSE, "Not Enum ID");
		break;
	}

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetFriendInfo( sFriendMember* pMember ) 
{
	if(!pMember)
		return;

	m_pmdBox->Clear();

	WCHAR buf[128] = {0,};

	// 이름
	swprintf_s(buf, L"%s : %s", GetDisplayStringManager()->GetString(DST_STATUS_NAME), pMember->wszMemberName);
	m_pmdBox->SetItem(buf, "FriendName", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_6);

	// 직업
	swprintf_s(buf, L"%s : %s", GetDisplayStringManager()->GetString(DST_STATUS_JOB), Logic_GetClassName(pMember->byClass));
	m_pmdBox->SetItem(buf, "FriendJob", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_6);

	// 레벨
	swprintf_s(buf, L"%s : %u", GetDisplayStringManager()->GetString(DST_STATUS_LEVEL), pMember->byLevel);
	m_pmdBox->SetItem(buf, "FriendLevel", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0);

	// 채널
	swprintf_s(buf, L"%s : %u", GetDisplayStringManager()->GetString(DST_FRIEND_SUBJECT_CHANNEL), pMember->byChannel);
	m_pmdBox->SetItem(buf, "FriendChannel", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0);

	// 위치
	std::wstring strAreaName;
	Logic_GetAreaNameFromTblIDX(pMember->mapTblIdx, &strAreaName);
	swprintf_s(buf, L"%s : %s", GetDisplayStringManager()->GetString(DST_FRIEND_SUBJECT_LOC), strAreaName.c_str());
	m_pmdBox->SetItem(buf, "FriendLoc", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_6);

	m_pmdBox->DrawItem();
}

VOID CInfoWindowGui::SetUpgradeInfo( stINFOWND_UPGRADE* pUpgrade )
{
	m_pmdBox->Clear();

	RwChar buf[256];

	for( RwInt32 i = 0 ; i < stINFOWND_UPGRADE::MAXLINECOUNT ; ++i )
	{
		if( pUpgrade->wszText[i] )
		{
			sprintf_s( buf, 256, "%dLine", i );
			m_pmdBox->SetItem( pUpgrade->wszText[i], buf, FONT_TEXT, COMP_TEXT_LEFT, pUpgrade->uiColor[i] );
		}
	}

	m_pmdBox->DrawItem();
}

/**
* \brief 전투 속성에 관련된 Info Window를 띄운다.
* \param pBattleAttr	(stINFOWND_BATTLEATTR*) CInfoWindowGui에서 사용할 전투 속성 구조체
*/
VOID CInfoWindowGui::SetBattleAttributeDisplay( stINFOWND_BATTLEATTR* pBattleAttr )
{
	m_pmdBox->Clear();

	switch( pBattleAttr->eBattleAttrInfoType )
	{
	case stINFOWND_BATTLEATTR::TYPE_ATTR_WEAPON_INFO:
		{
			if( pBattleAttr->bySourceWeaponAttr == INVALID_BYTE )
				return;

			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_WEAPON ), "Title", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_7 );
			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->bySourceWeaponAttr ), "Title2", FONT_TITLE, COMP_TEXT_LEFT,
				Logic_GetBattleAttributeColor( pBattleAttr->bySourceWeaponAttr ) , 0, TRUE );

			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			SetBattleAttrInfo_Weapon( pBattleAttr );

			break;
		}
	case stINFOWND_BATTLEATTR::TYPE_ATTR_ARMOR_INFO:
		{
			if( pBattleAttr->bySourceArmorAttr == INVALID_BYTE )
				return;

			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_ARMOR ), "Title", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_7 );
			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->bySourceArmorAttr ), "Title2", FONT_TITLE, COMP_TEXT_LEFT,
				Logic_GetBattleAttributeColor( pBattleAttr->bySourceArmorAttr ) , 0 , TRUE );

			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			SetBattleAttrInfo_Armor( pBattleAttr );

			break;
		}
	case stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_ATTACK_RATE:
		{
			if( pBattleAttr->bySourceWeaponAttr == INVALID_BYTE || pBattleAttr->byTargetArmorAttr == INVALID_BYTE )
				return;

			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_ARMOR ), "Title", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_7 );
			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->byTargetArmorAttr ), "Title2", FONT_TITLE, COMP_TEXT_LEFT,
				Logic_GetBattleAttributeColor( pBattleAttr->byTargetArmorAttr ) , 0 , TRUE );
			/*m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_ON_TARGET ), "Target", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );*/
			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			SetBattleAttrInfo_Attack_Rate( pBattleAttr );

			break;
		}
	case stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_DEFENCE_RATE:
		{
			//if( pBattleAttr->bySource
			if( pBattleAttr->bySourceArmorAttr == INVALID_BYTE|| pBattleAttr->byTargetWeaponAttr == INVALID_BYTE )
				return;

			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_WEAPON ), "Title", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_7 );
			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->byTargetWeaponAttr ), "Title2", FONT_TITLE, COMP_TEXT_LEFT,
				Logic_GetBattleAttributeColor( pBattleAttr->byTargetWeaponAttr ), 0, TRUE );

			/*m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_ON_TARGET ), "Target", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );*/
			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			SetBattleAttrInfo_Defence_Rate( pBattleAttr );

			break;
		}
	case stINFOWND_BATTLEATTR::TYPE_ATTR_TARGET_ALL_RATE:
		{
			if( pBattleAttr->bySourceArmorAttr == INVALID_BYTE|| pBattleAttr->byTargetWeaponAttr == INVALID_BYTE )
				return;

			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_ATTR ), "Title", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_7 );
			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->byTargetArmorAttr ), "Title2", FONT_TITLE, COMP_TEXT_LEFT,
				Logic_GetBattleAttributeColor( pBattleAttr->byTargetArmorAttr ), 0, TRUE );

			/*m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_BATTLEATTR_ON_TARGET ), "Target", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );*/
			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			SetBattleAttrInfo_Attack_Rate( pBattleAttr );
			SetBattleAttrInfo_Defence_Rate( pBattleAttr );

			break;
		}
	case stINFOWND_BATTLEATTR::TYPE_ATTR_UPGRADE:
		{
			// peessi : weapon을 쓰고 있지만, 실제 무기, 방어구 둘다 통틀어서 씀.
			if( pBattleAttr->bySourceWeaponAttr == INVALID_BYTE )
				return;

			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->bySourceWeaponAttr ), "Title", FONT_TITLE, COMP_TEXT_LEFT, Logic_GetBattleAttributeColor( pBattleAttr->bySourceWeaponAttr ) );
			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_TITLE ), "Title2", FONT_TITLE, COMP_TEXT_LEFT, INFOCOLOR_0, 0, TRUE );
			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE1 ), "Line1", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->bySourceWeaponAttr ), "Line2_Attr_1", FONT_TEXT, COMP_TEXT_LEFT, Logic_GetBattleAttributeColor( pBattleAttr->bySourceWeaponAttr ) );
			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE2_1 ), "Line2_1", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, TRUE );
			m_pmdBox->SetItem( Logic_GetBattleAttributeName( pBattleAttr->bySourceWeaponAttr ), "Line2_Attr_2", FONT_TEXT, COMP_TEXT_LEFT, Logic_GetBattleAttributeColor( pBattleAttr->bySourceWeaponAttr ), 0, TRUE );
			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE2_2 ), "Line2_2", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, TRUE );
			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

			SetBattleAttrInfo_Upgrade( pBattleAttr );

			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
			m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE3 ), "Line3", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );			
			break;
		}
	default:
		return;
	}

	m_pmdBox->DrawItem();
}

/**
* \brief 퀘스트 서치된 정보를 표시한다
* \param pAttr	(QUEST_SEARCH_LIST*) 퀘스트 NPC이름, 퀘스트 이름 정보 포함
*/
VOID CInfoWindowGui::SetQuestSearch( QUEST_SEARCH_LIST* pQuestInfo )
{
	m_pmdBox->Clear();

	RwChar acBuffer[4] = "";
	RwUInt8 byNPCCount = 0;
	RwUInt8 byQuestCount = 100;

	QUEST_SEARCH_ITER it = pQuestInfo->begin();
	for( ; it != pQuestInfo->end() ; ++it )
	{
		// NPC 이름
		_itoa_s(byNPCCount, acBuffer, 10);
		m_pmdBox->SetItem( it->pwstrNPCName->c_str(), acBuffer, FONT_TITLE, COMP_TEXT_LEFT, NTL_PARTY_NAME_COLOR );

		// 퀘스트 이름
		std::list<std::wstring>* pListQuestTitle = it->pListQuestTitle;
		std::list<std::wstring>::iterator it_QuestInfo = pListQuestTitle->begin();
		for( ; it_QuestInfo != pListQuestTitle->end() ; ++it_QuestInfo )
		{
			_itoa_s(byQuestCount, acBuffer, 10);
			m_pmdBox->SetItem( (*it_QuestInfo).c_str(), acBuffer, FONT_TITLE, COMP_TEXT_LEFT, COLOR_QUEST );

			++byQuestCount;
		}		

		++byNPCCount;

		if( byNPCCount < pQuestInfo->size() )
			m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );
	}

	m_pmdBox->DrawItem();
}

/**
* \brief 현재 속성의 공격 효율 정보를 출력한다.
* \param pAttr	(stINFOWND_BATTLEATTR*) 전투 속성 정보
*/
VOID CInfoWindowGui::SetBattleAttrInfo_Weapon( stINFOWND_BATTLEATTR* pAttr )
{
	WCHAR awcBuffer[256];
	CHAR acLine[32];

	// Target Armor Attribute 의 값이 INVALID 라면 대상이 없는 것임
	if( pAttr->bySourceWeaponAttr == INVALID_BYTE )
		return;

	for(RwUInt8 i = BATTLE_ATTRIBUTE_NONE; i < BATTLE_ATTRIBUTE_COUNT; ++i )
	{
		sprintf_s( acLine, 32, "Info%d", i );
		m_pmdBox->SetItem( Logic_GetBattleAttributeName( i ), acLine, FONT_TEXT, COMP_TEXT_LEFT, Logic_GetBattleAttributeColor( i ));

		// 현재 속성에게의 공격력 + 보너스 공격력을 표시한다.
		RwReal fRate = NtlGetBattleAttributeBonusRate( pAttr->bySourceWeaponAttr, i ) + pAttr->afSourceOffenceBonus[pAttr->bySourceWeaponAttr];
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BATTLEATTR_ATTACK_RATE_TARGET ), fRate);
		
		sprintf_s( acLine, 32, "Info%d", i+BATTLE_ATTRIBUTE_COUNT );
		m_pmdBox->SetItem( awcBuffer, acLine, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, TRUE );
	}

}

/**
* \brief 현재 속성의 방어 효율 정보를 출력한다.
* \param pAttr	(stINFOWND_BATTLEATTR*) 전투 속성 정보
*/
VOID CInfoWindowGui::SetBattleAttrInfo_Armor( stINFOWND_BATTLEATTR* pAttr )
{
	WCHAR awcBuffer[256];
	CHAR acLine[32];

	// Target Weapon Attribute 의 값이 INVALID 라면 대상이 없는 것임
	if( pAttr->bySourceArmorAttr == INVALID_BYTE )
		return;

	for(RwUInt8 i = BATTLE_ATTRIBUTE_NONE; i < BATTLE_ATTRIBUTE_COUNT; ++i )
	{
		sprintf_s( acLine, 32, "Info%d", i );
		m_pmdBox->SetItem( Logic_GetBattleAttributeName( i ), acLine, FONT_TEXT, COMP_TEXT_LEFT, Logic_GetBattleAttributeColor( i ) );
		
		// 현재 속성에게의 방어력 + 보너스 방어력을 표시한다.
		RwReal fRate = NtlGetBattleAttributeBonusRate( pAttr->bySourceArmorAttr, i ) + pAttr->afSourceDefenceBonus[pAttr->bySourceArmorAttr];
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BATTLEATTR_DEFENCE_RATE_TARGET ), fRate );
		
		sprintf_s( acLine, 32, "Info%d", i+BATTLE_ATTRIBUTE_COUNT );
		m_pmdBox->SetItem( awcBuffer, acLine, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, TRUE );
	}
}

VOID CInfoWindowGui::SetBattleAttrInfo_Attack_Rate( stINFOWND_BATTLEATTR* pAttr )
{
	if( pAttr->bySourceWeaponAttr == INVALID_BYTE || pAttr->byTargetArmorAttr == INVALID_BYTE )
		return;

	WCHAR awcBuffer[256];

	// 진기맹여락 상성 효율을 계산하고 버프나 상태 변화 등으로 인한 속성 보너스를 더한다.
	RwReal fBonusRate = NtlGetBattleAttributeBonusRate( pAttr->bySourceWeaponAttr, pAttr->byTargetArmorAttr );
	fBonusRate += ( pAttr->afSourceOffenceBonus[pAttr->bySourceWeaponAttr] - pAttr->afTargetDefenceBonus[pAttr->byTargetArmorAttr] );

	RwUInt32 uiColor = INFOCOLOR_0;
	if( fBonusRate < 0.0f )
		uiColor = INFOCOLOR_5;
	else if( fBonusRate > 0.0f )
		uiColor = INFOCOLOR_3;

	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BATTLEATTR_ATTACK_RATE ), fBonusRate );
	m_pmdBox->SetItem( awcBuffer, "AttackRate", FONT_TEXT, COMP_TEXT_LEFT, uiColor );


}

VOID CInfoWindowGui::SetBattleAttrInfo_Defence_Rate( stINFOWND_BATTLEATTR* pAttr )
{
	if( pAttr->bySourceArmorAttr == INVALID_BYTE || pAttr->byTargetWeaponAttr == INVALID_BYTE )
		return;

	WCHAR awcBuffer[256];

	RwReal fBonusRate = NtlGetBattleAttributeBonusRate( pAttr->bySourceArmorAttr, pAttr->byTargetWeaponAttr );
	fBonusRate += ( pAttr->afSourceDefenceBonus[pAttr->bySourceArmorAttr] - pAttr->afTargetOffenceBonus[pAttr->byTargetWeaponAttr]); 

	RwUInt32 uiColor = INFOCOLOR_0;
	if( fBonusRate < 0.0f )
		uiColor = INFOCOLOR_5;
	else if( fBonusRate > 0.0f )
		uiColor = INFOCOLOR_3;
	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_BATTLEATTR_DEFENCE_RATE ), fBonusRate );
	m_pmdBox->SetItem( awcBuffer, "DefenceRate", FONT_TEXT, COMP_TEXT_LEFT, uiColor );
}

VOID CInfoWindowGui::SetBattleAttrInfo_Upgrade( stINFOWND_BATTLEATTR* pAttr )
{
	if( pAttr->bySourceWeaponAttr == INVALID_BYTE )
		return;

	WCHAR awcBuffer[256];
	CHAR acLine[32];

	for( RwUInt8 i = BATTLE_ATTRIBUTE_NONE ; i < BATTLE_ATTRIBUTE_COUNT ; ++i )
	{
		if( i == pAttr->bySourceWeaponAttr )
			continue;

		sprintf_s( acLine, 32, "Info%d", i );
		m_pmdBox->SetItem( Logic_GetBattleAttributeName( i ), acLine, FONT_TEXT, COMP_TEXT_LEFT, Logic_GetBattleAttributeColor( i ) );
		sprintf_s( acLine, 32, "Info%d_1", i );
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_ATTR ), acLine, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0, 0, TRUE );
		
		// 현재 속성에게의 공격력 + 보너스 공격력을 표시한다.
		RwReal fRate = NtlGetBattleAttributeBonusRate( pAttr->bySourceWeaponAttr, i ) + pAttr->afSourceOffenceBonus[pAttr->bySourceWeaponAttr];
		RwUInt32 uiColor = INFOCOLOR_0;
				
		if( fRate >= 0.0f )
		{
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_ATTR_BETTER ), fRate );			
			uiColor = INFOCOLOR_2;
		}
		else
		{
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_ATTR_WORSE ), abs( fRate ) );			
			uiColor = INFOCOLOR_5;
		}

		sprintf_s( acLine, 32, "Info%d_2", i );
		m_pmdBox->SetItem( awcBuffer, acLine, FONT_TEXT, COMP_TEXT_LEFT, uiColor, 0, TRUE );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool CInfoWindowGui::GetSystemEffectData( RwUInt32 uiSystemEffectIdx, RwReal fValue, std::wstring& wstrText, RwUInt8 byPercentType, sSKILL_TBLDAT* pSkillData /* = NULL */, RwUInt32* pColor /* = NULL */, RwBool* pBuffShow /* = NULL  */ )
{
	if( uiSystemEffectIdx == INVALID_TBLIDX )
		return FALSE;

	if( !Logic_GetSystemEffectText( uiSystemEffectIdx, fValue, wstrText, byPercentType, pSkillData, pColor, pBuffShow ) )
	{
		 if( Logic_IsUIDevInfoVisible() )
		 {
			 char buf[32];
			 sprintf_s( buf, 32, "SystemEffectId(%u) Error", uiSystemEffectIdx );
			 m_pmdBox->SetItem( wstrText.c_str(), buf, FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
		 }	
		return FALSE;
	}

	if( pBuffShow && !(*pBuffShow) )
		return FALSE;

	return TRUE;
}

VOID CInfoWindowGui::EquipInfoProc( sITEM_TBLDAT* pData )
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_STATUS )
		return;

	if( EQUIP_TYPE_FIRST > pData->byEquip_Type || EQUIP_TYPE_LAST < pData->byEquip_Type )
		return;

	RwUInt32 flagSlotType = pData->wEquip_Slot_Type_Bit_Flag;
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	// BitFlag지만 한슬롯만 지정되어 있다. 
	if( flagSlotType & EQUIP_SLOT_FLAG_HAND	)
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_HAND ) );		
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_SUB_WEAPON )
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_SUB_WEAPON ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_JACKET )
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_JACKET ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_PANTS )
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_PANTS ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_BOOTS )
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_BOOTS ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_SCOUTER )
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_SCOUTER ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_COSTUME )
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_COSTUME ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_NECKLACE )
	{
		SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_NECKLACE ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_EARRING_1 || flagSlotType & EQUIP_SLOT_FLAG_EARRING_2 )
	{
		if( SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_EARRING_1 ) ) )
			SetEquipInfo( m_pmdEquipInfo[1], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_EARRING_2 ) );
		else
			SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_EARRING_2 ) );
	}
	else if( flagSlotType & EQUIP_SLOT_FLAG_RING_1 || flagSlotType & EQUIP_SLOT_FLAG_RING_2 )
	{
		if( SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_RING_1 ) ) )
			SetEquipInfo( m_pmdEquipInfo[1], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_RING_2 ) );
		else
			SetEquipInfo( m_pmdEquipInfo[0], pInventory->GetEquipItem( EQUIP_SLOT_TYPE_RING_2 ) );
	}
}

RwBool CInfoWindowGui::SetEquipInfo( gui::CMDStaticBox* pmdBox, RwUInt32 hSerial )
{
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );

	if( pItem )
	{
		pmdBox->Clear();
		pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_ITEM_ALREADY_EQUIPPED ), "EquippedItem", FONT_TEXT, 0, INFOCOLOR_12 );

		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		SetItemInfo( pmdBox, pItem, pItemAttr, FALSE );
		pmdBox->GetParent()->Show( true );
		return TRUE;
	}

	return FALSE;
}

VOID CInfoWindowGui::SetItemInfo_RecipeInfo( gui::CMDStaticBox* pmdBox, sITEM_TBLDAT* pData )
{
	sUSE_ITEM_TBLDAT* pTblDat = (sUSE_ITEM_TBLDAT*)API_GetTableContainer()->GetUseItemTable()->FindData( pData->Use_Item_Tblidx );
	if( pTblDat == NULL )
	{
		WCHAR awcBuffer[128];
		swprintf_s( awcBuffer, 128, L"Not exist USE_ITEM_TABLEDAT of index : %d", pData->Use_Item_Tblidx );
		m_pmdBox->SetItem( awcBuffer, "ErrorUseItem", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
	}
	else
	{
		m_pmdBox->SetBlankLine( INFOWND_BLANKLINE_HEIGHT );

		sITEM_RECIPE_TBLDAT* pRecipeDat = (sITEM_RECIPE_TBLDAT*)API_GetTableContainer()->GetItemRecipeTable()->FindData( (TBLIDX)pTblDat->afSystem_Effect_Value[0] );
		if( pRecipeDat == NULL )
		{
			WCHAR awcBuffer[128];
			swprintf_s( awcBuffer, 128, L"Not exist ITEM_RECIPE_TBLDAT of index : %d", (TBLIDX)pTblDat->afSystem_Effect_Value[0] );
			m_pmdBox->SetItem( awcBuffer, "ErrorUseItem", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
			return;
		}

		//  레시피 타입
		m_pmdBox->SetItem( Logic_GetHoipoiMixSkillName( pRecipeDat->byRecipeType ), "RecipeType", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_7 );

		//  사용조건
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_USE_STATE ), "UseRequirements", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_5 );
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_SCOUTER_SCOUTER_NAME ), "UseRequirementsData", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_5 );
		
		WCHAR awcBufferLevel[32];
		swprintf_s( awcBufferLevel, 32, GetDisplayStringManager()->GetString( DST_INFOWINDOW_MAKE_LEVEL ), pRecipeDat->byNeedMixLevel );
		m_pmdBox->SetItem( awcBufferLevel, "MakeLevel", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_5 );


		// 재료
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_RECIPE_MAT ), "Materials", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );
		for( int i=0; i < DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; ++i )
		{
			if( pRecipeDat->asMaterial[i].materialTblidx != INVALID_TBLIDX )
			{
				// 아이템 텍스트를 꺼내온다.
				sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData( pRecipeDat->asMaterial[i].materialTblidx );
				if( pItemData )
				{
					RwUInt32 uiColor = Logic_GetItemRankColor( pItemData->byRank );	
					std::wstring strItem = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemData->Name );

					CHAR acBuffer[16];
					sprintf_s( acBuffer, 16, "Material%d", i );

					pmdBox->SetItem( strItem.c_str(), acBuffer, FONT_TEXT, COMP_TEXT_RIGHT, uiColor );
				}
			}
		}

		// 만들어질 아이템
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_CREATE_ITEM ), "CreateItems", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );
		for( int i=0; i < DBO_MAX_COUNT_RECIPE_CREATE_ITEM; ++i )
		{
			if( pRecipeDat->asCreateItemTblidx[i].itemTblidx != INVALID_TBLIDX )
			{
				// 아이템 텍스트를 꺼내온다.
				sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData( pRecipeDat->asCreateItemTblidx[i].itemTblidx );

				RwUInt32 uiColor = Logic_GetItemRankColor( pItemData->byRank );	
				std::wstring strItem = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pItemData->Name );

				CHAR acBuffer[16];
				sprintf_s( acBuffer, 16, "Create%d", i );

				WCHAR awcBuffer[128];
				swprintf_s( awcBuffer, 128, L"%s(%d%%)", strItem.c_str(), pRecipeDat->asCreateItemTblidx[i].itemRate );

				pmdBox->SetItem( awcBuffer, acBuffer, FONT_TEXT, COMP_TEXT_RIGHT, uiColor );
			}
		}

		// 제조 비용
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_MAKE_ZENNY ), "NeedMakeZenny", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_7 );
		
		WCHAR awcBuffer[128];
		swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_INFOWINDOW_ZENNY ), pRecipeDat->dwNeedMixZenny );
		m_pmdBox->SetItem( awcBuffer, "NeedMakeZennyData", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_7 );

		// 파는 가격
		m_pmdBox->SetItem( GetDisplayStringManager()->GetString( DST_INFOWINDOW_SELL_PRICE ), "SellingZenny", FONT_TEXT, COMP_TEXT_LEFT, INFOCOLOR_0 );
		
		swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_INFOWINDOW_ZENNY ), pData->dwCost );
		m_pmdBox->SetItem( awcBuffer, "SellingZennyData", FONT_TEXT, COMP_TEXT_RIGHT, INFOCOLOR_0 );
	}
}