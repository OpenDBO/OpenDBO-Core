#include "precomp_dboclient.h"
#include "CharInfoGui.h"

// core
#include "NtlDebug.h"

// share
#include "WorldMapTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLAPI.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "LobbyManager.h"


CCharacterInfoGui::CCharacterInfoGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CCharacterInfoGui::~CCharacterInfoGui()
{

}

RwBool CCharacterInfoGui::Create()
{
	NTL_FUNCTION( "CCharacterInfoGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharSelect_CharInfo.srf", "gui\\CharSelect_CharInfo.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 배경
	m_Background.SetType(CWindowby3::WT_VERTICAL);
	m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharSelect_CharInfo.srf", "srfBackgroundLeft" ));
	m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharSelect_CharInfo.srf", "srfBackgroundCenter" ));
	m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharSelect_CharInfo.srf", "srfBackgroundRight" ));	
	m_Background.SetSize(233, 298);

	// title surface
	m_srfTitle.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharSelect_CharInfo.srf", "srfCharacterInfoTitle" ));

	// 캐릭터 정보 배경
	m_srtInfoBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharSelect_CharInfo.srf", "srfBar" ));

	// '이름'
	rect.SetRectWH(30, 70, 100, 14);
	m_pNameStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pNameStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNameStatic->SetText(GetDisplayStringManager()->GetString(DST_STATUS_NAME));
	m_pNameStatic->Enable(false);

	// '레벨'
	rect.SetRectWH(30, 92, 100, 14);
	m_pLevelStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLevelStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLevelStatic->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_CHAR_LEVEL));
	m_pLevelStatic->Enable(false);

	// '클래스'
	rect.SetRectWH(30, 114, 100, 14);
	m_pClassStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pClassStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pClassStatic->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_CHAR_CLASS));
	m_pClassStatic->Enable(false);

	// '소유제니'
	rect.SetRectWH(30, 136, 100, 14);
	m_pZennyStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pZennyStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pZennyStatic->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_ZENNY));
	m_pZennyStatic->Enable(false);

	// '최종위치'
	rect.SetRectWH(30, 160, 100, 14);
	m_pLastLocationStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLastLocationStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLastLocationStatic->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_POSITION));
	m_pLastLocationStatic->Enable(false);

	// '삭제 예정일'
	rect.SetRectWH(30, 194, 100, 14);
	m_pDeleteDayStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDeleteDayStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDeleteDayStatic->SetTextColor(RGB(255, 219, 71));
	m_pDeleteDayStatic->SetText(GetDisplayStringManager()->GetString(DST_LOBBY_DEL_DAT));
	m_pDeleteDayStatic->Enable(false);

	// 캐릭터 이름
	rect.SetRectWH(110, 70, 100, 14);
	m_pName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pName->SetTextColor(RGB(255, 192, 0));
	m_pName->Enable(false);

	// 캐릭터 레벨
	rect.SetRectWH(110, 92, 100, 14);
	m_pLevel = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLevel->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLevel->SetTextColor(RGB(255, 192, 0));
	m_pLevel->Enable(false);

	// 캐릭터 클래스
	rect.SetRectWH(110, 114, 100, 14);
	m_pClass = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pClass->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pClass->SetTextColor(RGB(255, 192, 0));
	m_pClass->Enable(false);

	// 캐릭터 소유 제니
	rect.SetRectWH(110, 136, 100, 14);
	m_pZenny = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pZenny->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pZenny->SetTextColor(RGB(255, 192, 0));
	m_pZenny->Enable(false);

	// 캐릭터 최종 위치
	rect.SetRectWH(110, 160, 105, 14);
	m_pLastLocation = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLastLocation->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLastLocation->SetTextColor(RGB(255, 192, 0));
	m_pLastLocation->Enable(false);

	// 캐릭터 삭제 예정일
	rect.SetRectWH(110, 194, 105, 14);
	m_pDeleteDay = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDeleteDay->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDeleteDay->SetTextColor(RGB(255, 219, 71));
	m_pDeleteDay->Enable(false);

	// 캐릭터 삭제 예정일 상세 설명
	rect.SetRectWH(30, 218, 185, 60);
	m_pDeleteDayExplain = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT | COMP_TEXT_UP);
	m_pDeleteDayExplain->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDeleteDayExplain->SetTextColor(RGB(255, 219, 71));
	m_pDeleteDayExplain->Enable(false);	

	Show(false);

	LinkMsg(g_EventLobbyMessage);

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CCharacterInfoGui::OnPaint );

	NTL_RETURN(TRUE);
}

VOID CCharacterInfoGui::Destroy()
{
	NTL_FUNCTION("CCharacterInfoGui::Destroy");

	UnLinkMsg(g_EventLobbyMessage);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CCharacterInfoGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	m_Background.SetPosition(iXPos, iYPos);
	m_srfTitle.SetPosition(iXPos + 47, iYPos + 9);
	m_srtInfoBack.SetPosition(iXPos + 11, iYPos + 53);
}

VOID CCharacterInfoGui::SetReservatedDelInfo(RwUInt8 byCharIndex)
{
	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	if( FALSE == Logic_IsUsableIndex(byCharIndex, NTL_MAX_COUNT_USER_CHAR_SLOT, INVALID_BYTE) ||
		pLobby->GetCharacterCount() == 0 )
	{
		m_pDeleteDayStatic	->Show(false);
		m_pDeleteDay		->Show(false);
		m_pDeleteDayExplain	->Show(false);
		return;
	}

	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(byCharIndex);
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << byCharIndex);
		return;
	}

	if( pLOBBY_CHARACTER->bReservatedDelete == FALSE )
	{
		m_pDeleteDayStatic	->Show(false);
		m_pDeleteDay		->Show(false);
		m_pDeleteDayExplain	->Show(false);
		return;
	}

	// avooo's comment : 깨끗하게 지우자
	m_pDeleteDayExplain->Show(false);
	return;

	RwReal fTime_util_Deletion = pLOBBY_CHARACTER->fTime_util_Deletion;

	if( fTime_util_Deletion < 60 )
	{
		m_pDeleteDayExplain->Format(GetDisplayStringManager()->GetString(DST_LOBBY_UNTIL_DEL_CHAR),
									(RwUInt32)fTime_util_Deletion,
									GetDisplayStringManager()->GetString(DST_TIME_SECOND) );
	}
	else if( fTime_util_Deletion < (60*60) )
	{
		fTime_util_Deletion /= 60;
		m_pDeleteDayExplain->Format(GetDisplayStringManager()->GetString(DST_LOBBY_UNTIL_DEL_CHAR),
									(RwUInt32)fTime_util_Deletion,
									GetDisplayStringManager()->GetString(DST_TIME_MINUTE) );
	}
	else if( fTime_util_Deletion < (60*60*24) )
	{
		fTime_util_Deletion /= 60;
		fTime_util_Deletion /= 60;
		m_pDeleteDayExplain->Format(GetDisplayStringManager()->GetString(DST_LOBBY_UNTIL_DEL_CHAR),
									(RwUInt32)fTime_util_Deletion,
									GetDisplayStringManager()->GetString(DST_TIME_AN_HOUR) );
	}
	else
	{
		fTime_util_Deletion /= 60;
		fTime_util_Deletion /= 60;
		fTime_util_Deletion /= 24;
		m_pDeleteDayExplain->Format(GetDisplayStringManager()->GetString(DST_LOBBY_UNTIL_DEL_CHAR),
									(RwUInt32)fTime_util_Deletion,
									GetDisplayStringManager()->GetString(DST_TIME_DAY) );
	}
	
	m_pDeleteDayStatic	->Show(true);
	m_pDeleteDay		->Show(true);
	m_pDeleteDayExplain	->Show(true);
}

VOID CCharacterInfoGui::OnPaint()
{
	m_Background	.Render();
	m_srfTitle		.Render();
	m_srtInfoBack	.Render();
}

VOID CCharacterInfoGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCharacterInfoGui::HandleEvents");

	if( msg.Id == g_EventLobbyMessage )
	{
		SDboEventLobbyMessage* pEvent = reinterpret_cast<SDboEventLobbyMessage*>(msg.pData);
		if(pEvent->byMessage == LMT_SELECT_CHARACTER)
		{
			RwUInt8			byCharIndex = (RwUInt8)pEvent->fValue;
			SERVER_HANDLE	hServer		= GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby		= GetLobbyManager()->GetLobby(hServer);
			if( !pLobby )
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			if( FALSE == Logic_IsUsableIndex(byCharIndex, NTL_MAX_COUNT_USER_CHAR_SLOT, INVALID_BYTE) ||
				pLobby->GetCharacterCount() == 0 )
			{
				m_pName			->Clear();
				m_pLevel		->Clear();
				m_pClass		->Clear();
				m_pZenny		->Clear();
				m_pLastLocation	->Clear();

				m_pDeleteDayStatic	->Show(false);
				m_pDeleteDay		->Show(false);
				m_pDeleteDayExplain	->Show(false);
			}
			else
			{
				sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(byCharIndex);
				if( !pLOBBY_CHARACTER )
				{
					DBO_FAIL("Not exist character of index : " << byCharIndex);
					NTL_RETURNVOID();
				}

				RwUInt32			uiMapNameIndex	= pLOBBY_CHARACTER->tSummary.dwMapInfoIndex;
				CMapNameTextTable*	pMapNameTable	= API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
				std::wstring		wstrText;

				m_pName->SetText(pLOBBY_CHARACTER->tSummary.awchName);
				m_pLevel->Format(L"%d", pLOBBY_CHARACTER->tSummary.byLevel);
				m_pClass->SetText( Logic_GetClassName(pLOBBY_CHARACTER->tSummary.byClass) );

				if( pLOBBY_CHARACTER->tSummary.dwMoney == 0 )
				{
					m_pZenny->SetText( GetDisplayStringManager()->GetString(DST_LOBBY_NO_HAVE_ZENNY) );
				}
				else
				{
					m_pZenny->Format(L"%u %s", (RwUInt32)pLOBBY_CHARACTER->tSummary.dwMoney,
												GetDisplayStringManager()->GetString(DST_ZENNY) );
				}			


	   			// 아바타가 마지막으로 플레이한 장소의 이름
				if( FALSE == pMapNameTable->GetAreaName(uiMapNameIndex, &wstrText) )
				{
					if( FALSE == pMapNameTable->GetSectorName(uiMapNameIndex, &wstrText) )
					{
						if( FALSE == pMapNameTable->GetZoneName(uiMapNameIndex, &wstrText) )
						{
							// avooo's comment : 아래 코드를 DST_LOBBY_DEFAULT_POSITION_HUMAN + race 형식으로 간단히 스트링을 얻어올 수
							//					 있겠으나 종족에 변화가 있을 때 스트링을 추가하는 것을 잊지 않기 위해 이대로 둔다
							switch(pLOBBY_CHARACTER->tSummary.byRace)
							{
							case RACE_HUMAN:
								{
									wstrText = GetDisplayStringManager()->GetString(DST_LOBBY_DEFAULT_POSITION_HUMAN);
									break;
								}
							case RACE_NAMEK:
								{
									wstrText = GetDisplayStringManager()->GetString(DST_LOBBY_DEFAULT_POSITION_NAMEK);
									break;
								}
							case RACE_MAJIN:
								{
									wstrText = GetDisplayStringManager()->GetString(DST_LOBBY_DEFAULT_POSITION_MAJIN);
									break;
								}
							default:
								{
									DBO_FAIL("g_EventLobbyMessage, Invalid race type : " << pLOBBY_CHARACTER->tSummary.byRace);
									break;
								}
							}
						}
					}
				}
				
				m_pLastLocation->SetText(wstrText.c_str());
			}

			SetReservatedDelInfo(byCharIndex);
		}
		else if(pEvent->byMessage == LMT_UPDATE_CHARACTER_LIST)
		{
			SERVER_HANDLE	hServer		= GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby		= GetLobbyManager()->GetLobby(hServer);
			if( !pLobby )
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			SetReservatedDelInfo( pLobby->GetSelectedCharacterIndex() );
		}
		else if(pEvent->byMessage == LMT_RENAME_CHARACTER)
		{
			CHARACTERID		charID		= (CHARACTERID)pEvent->uiExData;
			SERVER_HANDLE	hServer		= GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby		= GetLobbyManager()->GetLobby(hServer);
			if( !pLobby )
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				NTL_RETURNVOID();
			}

			sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(charID);
			if( !pLOBBY_CHARACTER )
			{
				DBO_FAIL("Not exist character of index : " << charID);
				NTL_RETURNVOID();
			}

			m_pName->SetText(pLOBBY_CHARACTER->tSummary.awchName);
		}
	}

	NTL_RETURNVOID();
}