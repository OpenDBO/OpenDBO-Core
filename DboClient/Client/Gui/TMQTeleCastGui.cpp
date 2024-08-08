#include "precomp_dboclient.h"
#include "TMQTeleCastGui.h"

// core
#include "NtlDebug.h"

// PL
#include "NtlPLSceneManager.h"

// SL
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "NtlWorldconceptTMQ.h"

// DBO
#include "NtlPLGuiManager.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "MinimapGui.h"

// Share
#include "NtlDirection.h"
#include "NPCTable.h"
#include "NpcSpeechTable.h"
#include "TextAllTable.h"
#include "DboEventGenerator.h"

#define BALLOON_FLASH_WIDTH 300
#define BALLOON_FLASH_HEIGHT 150
#define BALLOON_TEXT_WIDTH 211
#define BALLOON_TEXT_HEIGHT 70
#define BALLOON_TEXT_X 44
#define BALLOON_TEXT_Y 43

CTMQTeleCastGui::CTMQTeleCastGui(const RwChar* pName)
: CNtlPLGui(pName)
{
    Init();
}

CTMQTeleCastGui::~CTMQTeleCastGui( void ) 
{    
}

VOID CTMQTeleCastGui::Init() 
{
    m_eTeleCastState = TELECAST_CLOSED;
    m_pFlashOpening = NULL;
    m_pFlashClosing = NULL;
    m_pFlashNoraml = NULL;
    m_pFlashWarn = NULL;    
    m_pFlashFade = NULL;
    m_pFlashBalloon = NULL;
    m_pNPCModel = NULL;
    m_uiPrevNPCIdx = NULL;
    m_fFreqTime = NULL;
    m_bShakeLeft = TRUE;
    m_dwDisplayTime  = 0;
    m_nPrevBalloonType = NULL;
    m_pUIConfigTeleCast = NULL;    
    ZeroMemory(&m_eventInfo, sizeof(m_eventInfo));                
}

RwBool CTMQTeleCastGui::Create() 
{
    if(!CNtlPLGui::Create("", "gui\\TMQTeleCast.srf", "gui\\TMQTeleCast.frm"))
        return FALSE;

    CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

    m_pThis         = (gui::CDialog*)GetComponent("dlgMain");    
    m_pPnlBack      = (gui::CPanel*)GetComponent("pnlBack");
    m_pFlashFade  = (gui::CFlash*)GetComponent("swfFade");
    m_pFlashOpening = (gui::CFlash*)GetComponent("swfOpening");
    m_pFlashClosing = (gui::CFlash*)GetComponent("swfClosing");
    m_pFlashNoraml  = (gui::CFlash*)GetComponent("swfNormal");
    m_pFlashWarn    = (gui::CFlash*)GetComponent("swfWarn");    
    m_pSttBalloon   = (gui::CStaticBox*)GetComponent("sttBalloon");
    
    m_slotPaint = m_pPnlBack->SigPaint().Connect(this, &CTMQTeleCastGui::OnPaint);    
    m_slotMove  = m_pThis->SigMove().Connect(this, &CTMQTeleCastGui::OnMove);
    m_slotOpeningMovieEnd = m_pFlashOpening->SigMovieEnd().Connect(this, &CTMQTeleCastGui::OnOpeningMovieEnd);
    m_slotClosingMovieEnd = m_pFlashClosing->SigMovieEnd().Connect(this, &CTMQTeleCastGui::OnClosingMovieEnd);
    m_slotFadeMovieEnd    = m_pFlashFade->SigMovieEnd().Connect(this, &CTMQTeleCastGui::OnFadeMovieEnd);

    GetNtlGuiManager()->AddUpdateBeforeFunc( this );
    GetNtlGuiManager()->AddUpdateFunc(this);

    m_pUIConfigTeleCast = GetDboGlobal()->GetDBOUIConfig()->GetTeleCastConfig();

    // 위치, 크기 설정
    m_pThis->SetSize((RwInt32)m_pUIConfigTeleCast->v2dSize.x, (RwInt32)m_pUIConfigTeleCast->v2dSize.y);        
    m_pThis->SetPosition(GetDboGlobal()->GetScreenWidth(), m_pUIConfigTeleCast->nYPos);    

    // 플래시 객체들을 생성한다.    
    LoadFlash(m_pThis->GetWidth(), m_pThis->GetHeight());

    // NPC 렌더 카메라를 생성한다.    
    m_NPCCamera.Create(256, 256, m_pThis->GetWidth(), m_pThis->GetHeight());    

    LinkMsg(g_EventUIConfigReload);
    LinkMsg(g_EventTMQCleintState);
    LinkMsg(g_EventWorldChange);
    LinkMsg(g_EventCreateWorld);
    LinkMsg(g_EventTMQCleintState);
    LinkMsg(g_EventNotify);

    Show(false);
    
    return TRUE;
}

VOID CTMQTeleCastGui::Destroy() 
{
 
    UnLinkMsg(g_EventUIConfigReload);
	UnLinkMsg(g_EventTMQCleintState);
    UnLinkMsg(g_EventWorldChange);
    UnLinkMsg(g_EventCreateWorld);
    UnLinkMsg(g_EventTMQCleintState);
    UnLinkMsg(g_EventNotify);

    if(m_pNPCModel)
    {
        GetSceneManager()->DeleteEntity(m_pNPCModel);
        m_pNPCModel = NULL;
    }

    m_NPCCamera.Destroy();
    
    if(m_pFlashFade)
        m_pFlashFade->Unload();
    if(m_pFlashOpening)
        m_pFlashOpening->Unload();
    if(m_pFlashClosing)
        m_pFlashClosing->Unload();
    if(m_pFlashNoraml)
        m_pFlashNoraml->Unload();
    if(m_pFlashWarn)
        m_pFlashWarn->Unload();    

    std::map<RwInt32, gui::CFlash*>::iterator it;
    for(it = m_mapFlashBalloon.begin(); it != m_mapFlashBalloon.end(); ++it)
    {
        gui::CFlash* pFlash = (*it).second;
        if(pFlash)
            pFlash->Unload();
    }

    GetNtlGuiManager()->RemoveUpdateBeforeFunc(this);
    GetNtlGuiManager()->RemoveUpdateFunc(this);

    CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy();
}

// UIConfig.XML에 설정되어 있는 플래시 파일들을 로딩한다.
VOID CTMQTeleCastGui::LoadFlash(RwInt32 nWidth, RwInt32 nHeight)
{
    m_pFlashFade->Load(m_pUIConfigTeleCast->strFadeFlashFile.c_str());
    m_pFlashOpening->Load(m_pUIConfigTeleCast->strOpeningFlashFile.c_str());
    m_pFlashClosing->Load(m_pUIConfigTeleCast->strClosingFlashFile.c_str());
    m_pFlashNoraml->Load(m_pUIConfigTeleCast->strBackNormalFlashFile.c_str());
    m_pFlashWarn->Load(m_pUIConfigTeleCast->strBackWarnFlashFile.c_str());
    
    m_pFlashFade->PlayMovie(FALSE);
    m_pFlashOpening->PlayMovie(FALSE);
    m_pFlashClosing->PlayMovie(FALSE);
    m_pFlashNoraml->PlayMovie(FALSE);
    m_pFlashWarn->PlayMovie(FALSE);
    
    m_pFlashFade->AutoRewind(FALSE);
    m_pFlashOpening->AutoRewind(FALSE);
    m_pFlashClosing->AutoRewind(FALSE);
    
    m_pFlashFade->SetResizeType(gui::CFlash::EXACTFIT);
    m_pFlashOpening->SetResizeType(gui::CFlash::EXACTFIT);
    m_pFlashClosing->SetResizeType(gui::CFlash::EXACTFIT);
    m_pFlashNoraml->SetResizeType(gui::CFlash::EXACTFIT);
    m_pFlashWarn->SetResizeType(gui::CFlash::EXACTFIT);

    // 말풍선 플래시들을 생성한다.
    CreateDialogBalloon();
}

VOID CTMQTeleCastGui::Update( RwReal fElapsed ) 
{
    if(!IsShow())
        return;

    // Fade Flash Update
    if(m_pFlashFade && m_pFlashFade->IsPlayMovie() && m_pFlashFade->IsVisible())
    {
        RwReal fOrgPlayTime = m_pFlashFade->GetFrameCount() / m_pFlashFade->GetFrameRate();
        RwReal fRatio = fOrgPlayTime / m_pUIConfigTeleCast->fFadeTime;
        m_pFlashFade->Update(fElapsed * fRatio);
    }

    switch(m_eTeleCastState)
    {
    case TELECAST_SLIDING_IN:
        UpdateSliding(fElapsed);        
        break;
    case TELECAST_OPENING:
        if(m_pFlashOpening) m_pFlashOpening->Update(fElapsed);                
        break;
    case TELECAST_OPEN:              
        if(m_pFlashBalloon && m_pFlashBalloon->IsVisible()) // 말풍선 업데이트        
        {
            m_pSttBalloon->Show(true);
            m_pFlashBalloon->Update(fElapsed);            
        }
        break;
    case TELECAST_CLOSING:
        if(m_pFlashClosing) m_pFlashClosing->Update(fElapsed);
        break;    
    case TELECAST_SLIDING_OUT:
        UpdateSliding(fElapsed);
        break;
    case TELECAST_CLOSED:        
        break;
    }

    // 상황에 맞는 전경 플래시를 재생한다.
    if(m_eTeleCastState != TELECAST_CLOSED)
    {
        switch(m_eventInfo.byTelecastType)
        {
        case TELECAST_MESSAGE_TYPE_NORMAL:
        case TELECAST_MESSAGE_TYPE_NOTIFY:
            if(m_pFlashNoraml)
                m_pFlashNoraml->Update(fElapsed);
            break;
        case TELECAST_MESSAGE_TYPE_DANGER:     
            if(m_eTeleCastState == TELECAST_OPEN)
            {
                UpdateShake(fElapsed);              // 전경 플래시는 경고것을 같이 쓴다.
            }            
        case TELECAST_MESSAGE_TYPE_WARNING:
            if(m_pFlashWarn)
                m_pFlashWarn->Update(fElapsed);
            break;        
        }
    }

    // NPC 애니메이션 업데이트 및 텍스쳐에 렌더링    
    if(m_surNPC.IsShow() && m_pNPCModel)
    {
        m_pNPCModel->Update(fElapsed);        
    }    

    if(m_eTeleCastState == TELECAST_OPEN)
    {
        // 타임 베이스라면 제한 시간 체크
        if(m_eventInfo.dwDisplayTime != INVALID_DWORD && m_eventInfo.dwDisplayTime > 0)
        {
            m_dwDisplayTime += (DWORD)(fElapsed * 1000.0f);
            if(m_dwDisplayTime >= m_eventInfo.dwDisplayTime)            
            {
                Closing();
                m_dwDisplayTime = 0L;
            }
        }
    }    
}

VOID CTMQTeleCastGui::OnPaint() 
{
    if(!IsShow())
        return;

    // NPC가 렌더링된 텍스쳐를 메인 화면에 렌더링
    if(m_eTeleCastState != TELECAST_CLOSED && m_pNPCModel && m_surNPC.IsShow())
    {        
        m_surNPC.Render();
    }    
}

RwInt32 CTMQTeleCastGui::SwitchDialog( bool bOpen ) 
{
    if(bOpen)
    {
		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
		if( pWorldConcept )
		{
			if( pWorldConcept->GetConceptType() == WORLD_PLAY_TUTORIAL )
			{
				if( pWorldConcept->GetState() == WORLD_STATE_ENTER )
				{
					CMinimapGui* pMinimapGui = reinterpret_cast<CMinimapGui*>( GetDialogManager()->GetDialog(DIALOG_MINIMAP) );
					pMinimapGui->ChangeMapStyle(CMinimapGui::MS_MINIMAM);
				}
			}
		}
		
		//if( pWorldConcept->GetConceptType() == WORLD_PLAY_TIME_MACHINE )
		//{
		//	CNtlWorldConceptTMQ* pTMQWorldConcept = reinterpret_cast<CNtlWorldConceptTMQ*>(pWorldConcept);

		//	if( pTMQWorldConcept->GetTMQState() < WORLD_STATE_TMQ_WAIT ||
		//		pTMQWorldConcept->GetTMQState() >= WORLD_STATE_TMQ_END )
		//		return -1;
		//}
		//else if( pWorldConcept->GetConceptType() != WORLD_PLAY_TUTORIAL )
		//	return -1;

		RaiseTop();

        Show(TRUE);
    }
    else
    {
		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
		if( pWorldConcept )
		{
			if( pWorldConcept->GetConceptType() == WORLD_PLAY_TUTORIAL )
			{
				CMinimapGui* pMinimapGui = reinterpret_cast<CMinimapGui*>( GetDialogManager()->GetDialog(DIALOG_MINIMAP) );
				pMinimapGui->ChangeMapStyle(CMinimapGui::MS_NORMAL);
			}
		}

        // Side Dialog는 매니저에서 알아서 없애주기 때문에 화면에서 지우면 안된다.        
         Show(FALSE);
         m_surNPC.Show(FALSE);
    }
 
    return TRUE;
}

VOID CTMQTeleCastGui::HandleEvents( RWS::CMsg &pMsg ) 
{
    //if(pMsg.Id == g_EventTeleCastBegNfy)
    //{
    //    // 서버 알림중일때는 방송창을 쌩깐다.
    //    if(IsShow() && m_eventInfo.byTelecastType == TELECAST_MESSAGE_TYPE_NOTIFY)
    //        return;

    //    SNtlEventTeleCastBegNfy* pData = (SNtlEventTeleCastBegNfy*)pMsg.pData;
    //    m_eventInfo = *pData;
    //    m_dwDisplayTime = 0;

    //    RaiseLinked();
    //    OnSlidingIn();
    //}
    //else if(pMsg.Id == g_EventTeleCastEndNfy)
    //{
    //    Closing();
    //}
    if(pMsg.Id == g_EventNotify)       // 서버로부터 알림이 내려왔을때
    {
        SDboEventNotify* data = (SDboEventNotify*)pMsg.pData;
        if(data->eType == SDboEventNotify::NPC)
        {
            // 데이터가 다르면 리로드
            if(m_pUIConfigTeleCast->m_eTelecastType != TELECAST_TYPE_SERVER_NOTIFY)
            {
                GetDboGlobal()->GetDBOUIConfig()->LoadTeleCast(TELECAST_TYPE_SERVER_NOTIFY);

                // 위치, 크기 설정
                m_pThis->SetSize((RwInt32)m_pUIConfigTeleCast->v2dSize.x, (RwInt32)m_pUIConfigTeleCast->v2dSize.y);        
                m_pThis->SetPosition(GetDboGlobal()->GetScreenWidth(), m_pUIConfigTeleCast->nYPos);   
                AdjustDialogBalloon();
            }

            // 데이터 설정
            m_eventInfo.npcTblidx = m_pUIConfigTeleCast->uiNPCID;
            m_eventInfo.byTelecastType = TELECAST_MESSAGE_TYPE_NOTIFY;
            m_eventInfo.speechTblidx = INVALID_INDEX;      // 사용하지 않는다.            
            m_eventInfo.wstrNotifyMsg = data->wchMessage;
            m_eventInfo.dwDisplayTime = (DWORD)(m_pUIConfigTeleCast->fShowTime * 1000.0f);

            m_dwDisplayTime = 0;

            RaiseLinked();
            OnSlidingIn();
        }
    }
    else if(pMsg.Id == g_EventUIConfigReload)       // UIConfig.XML의 내용를 리로드 했을때 (디버깅용)
    {
        // 위치, 크기 설정
        m_pThis->SetSize((RwInt32)m_pUIConfigTeleCast->v2dSize.x, (RwInt32)m_pUIConfigTeleCast->v2dSize.y);        
        m_pThis->SetPosition(GetDboGlobal()->GetScreenWidth() - m_pThis->GetWidth(), m_pUIConfigTeleCast->nYPos);    

        // 아직 플래시 데이터는 실시간으로 변경하지는 않는다.
        // 차후 요청이 있을시에는 플래시 데이터도 리로드하는 구현을 추가한다.

        // 말풍선 
        AdjustDialogBalloon();        
    }
    else if(pMsg.Id == g_EventTMQCleintState)      // 월드 컨셉이 변경되었을때.
    {
        // 설정 데이터를 새로 로딩한다.
        SNtlEventTMQServerState* pData = (SNtlEventTMQServerState*)pMsg.pData;
        if(pData->uiState == WORLD_STATE_TMQ_BEGIN)
        {
            if(pData->eWorldConcept == WORLD_PLAY_TUTORIAL)
            {
                GetDboGlobal()->GetDBOUIConfig()->LoadTeleCast(TELECAST_TYPE_TUTORIAL);            
            }
            else if(pData->eWorldConcept == WORLD_PLAY_TIME_MACHINE)
            {
                GetDboGlobal()->GetDBOUIConfig()->LoadTeleCast(TELECAST_TYPE_TMQ);     
            }

            // 위치, 크기 설정
            m_pThis->SetSize((RwInt32)m_pUIConfigTeleCast->v2dSize.x, (RwInt32)m_pUIConfigTeleCast->v2dSize.y);        
            m_pThis->SetPosition(GetDboGlobal()->GetScreenWidth(), m_pUIConfigTeleCast->nYPos);   
            AdjustDialogBalloon();
        }
    }
	else if( pMsg.Id == g_EventTMQCleintState ) // TMQ 진입에 실패하는 경우에는 화면에서 없애준다.
	{
		SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( pMsg.pData );

		if( pEvent->uiState == WORLD_STATE_TMQ_FAIL )
		{
			GetDialogManager()->CloseDialog(DIALOG_TELECAST);
		}
	}
    else if(pMsg.Id == g_EventWorldChange)      // 월드가 변경되는 경우에는 화면에서 없애준다.
    {
        SNtlEventWorldChange* pWorldChange = reinterpret_cast<SNtlEventWorldChange*>(pMsg.pData);
        
        if(pWorldChange->bWorldChange)      
        {            
            if(m_pNPCModel)
            {
                GetSceneManager()->DeleteEntity(m_pNPCModel);
                m_pNPCModel		= NULL;
				m_uiPrevNPCIdx	= NULL;
            }
            SwitchDialog(false);
        }
    }
    else if(pMsg.Id == g_EventCreateWorld)      // 맵이 생성/해제 될때는 모델을 없애주어야만한다.
    {
        if(m_pNPCModel)
        {
            GetSceneManager()->DeleteEntity(m_pNPCModel);
            m_pNPCModel		= NULL;
			m_uiPrevNPCIdx	= NULL;
        }
    }
}

VOID CTMQTeleCastGui::OnSlidingIn() 
{
    SwitchDialog(true);
    m_eTeleCastState = TELECAST_SLIDING_IN;

    // 시작 위치 설정
    m_pThis->SetPosition(GetDboGlobal()->GetScreenWidth() + 1, m_pUIConfigTeleCast->nYPos);

    // 슬라이딩 연출을 위한 가속도 컨트롤러 세팅
    RwReal fStartPos = (RwReal)m_pThis->GetPosition().left;
    RwReal fEndPos =(RwReal)GetDboGlobal()->GetScreenWidth() - m_pThis->GetScreenRect().GetWidth();
    m_AccelController.SetAccel(fStartPos, fEndPos, m_pUIConfigTeleCast->fSlideStartVel, m_pUIConfigTeleCast->fSlideAccel);

    // 전체 화면 플래쉬 재생
    m_pFlashFade->PlayMovie(TRUE);
    m_pFlashFade->Show(TRUE);
    m_pFlashFade->RestartMovie();

    m_surNPC.Show(FALSE);
}

// 오프닝 플래시 연출
VOID CTMQTeleCastGui::Opening() 
{
    m_eTeleCastState = TELECAST_OPENING;

    if(m_eventInfo.byTelecastType == TELECAST_MESSAGE_TYPE_NORMAL ||
       m_eventInfo.byTelecastType == TELECAST_MESSAGE_TYPE_NOTIFY)
    {
        m_pFlashNoraml->PlayMovie(TRUE);
        m_pFlashNoraml->Show(TRUE); 
        m_pFlashNoraml->RestartMovie();

        m_pFlashWarn->PlayMovie(FALSE);
        m_pFlashWarn->Show(FALSE);
    }
    else
    {
        m_pFlashWarn->PlayMovie(TRUE);
        m_pFlashWarn->Show(TRUE);
        m_pFlashWarn->RestartMovie();

        m_pFlashNoraml->PlayMovie(FALSE);
        m_pFlashNoraml->Show(FALSE);
    }

    m_pFlashOpening->PlayMovie(TRUE);
    m_pFlashOpening->Show(TRUE);
    m_pFlashOpening->RestartMovie();

    // NPC 모델 생성
    CreateNPCModel();
}

// 클로징 플래시 연출
VOID CTMQTeleCastGui::Closing() 
{
    m_eTeleCastState = TELECAST_CLOSING;        
    
    // Shake한 경우 원래 위치로 돌려준다.
    if(m_eventInfo.byTelecastType == TELECAST_MESSAGE_TYPE_DANGER)
    {
        if(!m_bShakeLeft)   // 왼쪽으로 움직인 경우
        {
            CRectangle rect = m_pThis->GetScreenRect();            
            SetPosition(rect.left + m_pUIConfigTeleCast->nShakeAmp, rect.top);
        }           
    }

    if(m_pFlashFade->IsPlayMovie())
    {
        m_pFlashFade->PlayMovie(FALSE);
        m_pFlashFade->Show(FALSE);
    }

    m_pFlashClosing->PlayMovie(TRUE);
    m_pFlashClosing->Show(TRUE);
    m_pFlashClosing->RestartMovie();

    // 말풍선을 끈다.
    CloseDialogBalloon();
}

VOID CTMQTeleCastGui::OnOpeningMovieEnd( gui::CComponent* pComponent ) 
{
    m_eTeleCastState = TELECAST_OPEN;
    
    m_pFlashOpening->PlayMovie(FALSE);
    m_pFlashOpening->Show(FALSE);

    m_surNPC.Show(TRUE);

    // 말풍선을 생성한다.
    SetDialogBalloon();    
}
VOID CTMQTeleCastGui::OnClosingMovieEnd( gui::CComponent* pComponent ) 
{
    m_eTeleCastState = TELECAST_SLIDING_OUT;

    m_pFlashNoraml->PlayMovie(FALSE);
    m_pFlashNoraml->Show(false);
    m_pFlashWarn->PlayMovie(FALSE);
    m_pFlashWarn->Show(false);
    m_pFlashClosing->PlayMovie(FALSE);
    m_pFlashClosing->Show(FALSE);

    // NPC는 캐싱하고 있기 때문에 삭제하지 않는다.   
    m_surNPC.Show(FALSE);

    // 슬라이딩 아웃값 설정
    RwReal fStartPos = (RwReal)m_pThis->GetPosition().left;
    RwReal fEndPos = GetDboGlobal()->GetScreenWidth() + 1.0f;
    m_AccelController.SetAccel(fStartPos, fEndPos, m_pUIConfigTeleCast->fSlideStartVel, m_pUIConfigTeleCast->fSlideAccel);
}

VOID CTMQTeleCastGui::OnFadeMovieEnd( gui::CComponent* pComponent ) 
{
    m_pFlashFade->PlayMovie(FALSE);
    m_pFlashFade->Show(false);
}

VOID CTMQTeleCastGui::UpdateShake( RwReal fElapsed ) 
{
    m_fFreqTime += fElapsed;
    if(m_fFreqTime >= m_pUIConfigTeleCast->fShakeFreq)
    {
        m_pThis->RemoveChild(m_pSttBalloon);
        m_pThis->RemoveChild(m_pFlashBalloon);      // 말풍선은 Shake되면 안된다.
        

        m_fFreqTime = 0.0;
        RwInt32 nAmp = m_pUIConfigTeleCast->nShakeAmp;
        if(m_bShakeLeft) // 왼쪽->원래위치->왼쪽->원래위치..순으로 진동한다.
        {
            nAmp = -nAmp;
        }
        m_bShakeLeft = !m_bShakeLeft;
        CRectangle rect = m_pThis->GetScreenRect();        
        SetPosition(rect.left + nAmp, rect.top);

        m_pThis->AddChild(m_pFlashBalloon);
        m_pThis->AddChild(m_pSttBalloon);        
    }
}

VOID CTMQTeleCastGui::OnMove( RwInt32 iOldX, RwInt32 iOldY ) 
{
    // 배경
    m_pPnlBack->SetSize(m_pThis->GetWidth(), m_pThis->GetHeight());
    
    // NPC Render 타겟의 위치 (배경 그림보다 조금 작게한다)
    CRectangle rect = m_pThis->GetScreenRect();
    rect.left += 18;
    rect.top += 10;
    rect.right -= 12;
    rect.bottom -= 10;
    m_surNPC.SetRect(rect);      

    // 플래시들의 크기 설정 (위치는 상위 클래스에서 맞춰진다)
    RwInt32 nWidth = m_pThis->GetWidth();
    RwInt32 nHeight = m_pThis->GetHeight();
    m_pFlashOpening->SetSize(nWidth, nHeight);
    m_pFlashClosing->SetSize(nWidth, nHeight);
    m_pFlashNoraml->SetSize(nWidth, nHeight);
    m_pFlashWarn->SetSize(nWidth, nHeight);    

    // 전체화면 플래쉬
    rect = m_pThis->GetScreenRect();
    m_pFlashFade->SetSize(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());    
    m_pFlashFade->SetPosition(-rect.left, -rect.top);    
}

void CTMQTeleCastGui::Show( bool bShow ) 
{
    CNtlPLGui::Show(bShow);    

    // 플래시는 상황에 맞게 Show 되기 때문에 이 함수에 영향을 받으면 안된다.    
    m_pFlashFade->Show(false);
    m_pFlashOpening->Show(false);
    m_pFlashClosing->Show(false);
    m_pFlashNoraml->Show(false);
    m_pFlashWarn->Show(false);
    m_pSttBalloon->Show(FALSE);
}

VOID CTMQTeleCastGui::CreateNPCModel() 
{
    // 이전에 로딩한 NPC와 같은 NPC라면 새로 로딩하지 않는다.    
    if(m_uiPrevNPCIdx == m_eventInfo.npcTblidx)
    {
        return;
    }
    else if(m_pNPCModel)
    {
        GetSceneManager()->DeleteEntity(m_pNPCModel);
        m_pNPCModel = NULL;
    }

    sBOT_TBLDAT* pTableData = (sBOT_TBLDAT*)API_GetTableContainer()->GetNpcTable()->FindData(m_eventInfo.npcTblidx);
    if(!pTableData)
    {
        m_uiPrevNPCIdx = NULL;
        return;
    }

    SPLCharacterCreateParam param;
    ZeroMemory(&param, sizeof(SPLCharacterCreateParam)); 
    param.bPcFlag = FALSE;    
    m_pNPCModel = (CNtlPLCharacter*)GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, pTableData->szModel, &param);
    if(!m_pNPCModel)
        return;

	m_uiPrevNPCIdx = m_eventInfo.npcTblidx;

    // 카메라 위치 설정
    RwV3d vFaceCamPos = m_pNPCModel->GetFaceCameraPos();
    RwV3d vFaceCamLookAt = m_pNPCModel->GetFaceCameraLookAt();
    m_NPCCamera.SetCameraPosition(&vFaceCamPos, &vFaceCamLookAt);

    m_pNPCModel->SetVisible(FALSE);
    m_pNPCModel->SetPosition(&ZeroAxis);
    m_pNPCModel->SetBaseAnimation(NML_IDLE_LOOP, 0.0f, TRUE);
    m_NPCTexture.Load(m_NPCCamera.GetTexture());
    m_surNPC.SetTexture(&m_NPCTexture);
}

VOID CTMQTeleCastGui::UpdateBeforeCamera( RwReal fElapsed ) 
{
    if(!IsShow())
        return;

    if(m_pNPCModel)
    {
        m_NPCCamera.RenderTexture(m_pNPCModel);                
    }    
}

VOID CTMQTeleCastGui::CreateDialogBalloon() 
{
    // 말풍선들을 전부 미리 생성해둔다.
    std::map<RwInt32, STeleCastBalloon>::iterator it;
    for(it = m_pUIConfigTeleCast->mapBalloonRes.begin(); it != m_pUIConfigTeleCast->mapBalloonRes.end(); ++it)
    {
        STeleCastBalloon* data = &((*it).second);
        RwInt32 nPosX = (RwInt32)(-data->v2dOffset.x - data->v2dSize.x);
        RwInt32 nPosY = (RwInt32)(-data->v2dOffset.y);
        
        gui::CFlash* pFlash = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
        pFlash->Load(data->strFileName.c_str());
        pFlash->SetSize((RwInt32)data->v2dSize.x, (RwInt32)data->v2dSize.y);
        pFlash->SetPosition(nPosX, nPosY);
        pFlash->SetResizeType(gui::CFlash::EXACTFIT);
        pFlash->SetFrameCallback(FALSE);
        pFlash->PlayMovie(FALSE);
        pFlash->Show(FALSE);        

        m_mapFlashBalloon[(*it).first] = pFlash;
    }

    m_pSttBalloon->Raise();
}

VOID CTMQTeleCastGui::SetDialogBalloon() 
{
    if(m_eventInfo.byTelecastType == TELECAST_MESSAGE_TYPE_NOTIFY)          // 서버 알림창일때는 텍스트가 날라온다.
    {
        m_pSttBalloon->SetText(m_eventInfo.wstrNotifyMsg.c_str());
        
        m_pFlashBalloon = m_mapFlashBalloon[1];
        m_nPrevBalloonType = 1;
        AdjustDialogBalloon();
    }
    else
    {
        sNPC_SPEECH_TBLDAT* ptblData = (sNPC_SPEECH_TBLDAT*)API_GetTableContainer()->GetNpcSpeechTable()->FindData(m_eventInfo.speechTblidx);
        if(!ptblData)
            return;

        // 텍스트 테이블
        std::wstring strDialog = API_GetTableContainer()->GetTextAllTable()->GetNpcDialogTbl()->GetText(ptblData->textIndex);
        m_pSttBalloon->SetText(strDialog.c_str());

        // 이전과 말풍선 타입이 다를때만 새로 설정한다.
        if(m_nPrevBalloonType != ptblData->byBallonType)
        {
            if(m_pFlashBalloon)
            {
                m_pFlashBalloon->Show(FALSE);
                m_pFlashBalloon->PlayMovie(FALSE);
            }

            STeleCastBalloon sTeleCastBalloon = m_pUIConfigTeleCast->mapBalloonRes[ptblData->byBallonType];        
            m_pFlashBalloon = m_mapFlashBalloon[ptblData->byBallonType];
            m_nPrevBalloonType = ptblData->byBallonType;

            AdjustDialogBalloon();        
        }
    }

    m_pFlashBalloon->Show(TRUE);
    m_pFlashBalloon->PlayMovie(TRUE);    

    m_pSttBalloon->Show(TRUE);
}

VOID CTMQTeleCastGui::AdjustDialogBalloon() 
{
    if(!m_pFlashBalloon)
        return;
    
    STeleCastBalloon sTeleCastBalloon = m_pUIConfigTeleCast->mapBalloonRes[m_nPrevBalloonType];        

    // 플래시 위치및 크기 설정
    RwInt32 nPosX = (RwInt32)(-sTeleCastBalloon.v2dOffset.x - sTeleCastBalloon.v2dSize.x);
    RwInt32 nPosY = (RwInt32)(-sTeleCastBalloon.v2dOffset.y);
    m_pFlashBalloon->SetSize((RwInt32)sTeleCastBalloon.v2dSize.x, (RwInt32)sTeleCastBalloon.v2dSize.y);
    m_pFlashBalloon->SetPosition(nPosX, nPosY);

    // 말풍선용 텍스트 위치및 크기 설정
    RwV2d   v2dRatio;
    v2dRatio.x = BALLOON_TEXT_WIDTH * sTeleCastBalloon.v2dSize.x / BALLOON_FLASH_WIDTH;
    v2dRatio.y = BALLOON_TEXT_HEIGHT * sTeleCastBalloon.v2dSize.y / BALLOON_FLASH_HEIGHT;
    m_pSttBalloon->SetSize((RwInt32)v2dRatio.x, (RwInt32)v2dRatio.y);
    m_pSttBalloon->SetPosition(m_pFlashBalloon->GetPosition().left + BALLOON_TEXT_X, 
                               m_pFlashBalloon->GetPosition().top + BALLOON_TEXT_Y); 
}

VOID CTMQTeleCastGui::CloseDialogBalloon() 
{
    if(m_pFlashBalloon)
    {
        m_pFlashBalloon->Show(FALSE);
        m_pFlashBalloon->PlayMovie(FALSE);
    }
    
    m_pSttBalloon->Show(FALSE);    
}

VOID CTMQTeleCastGui::UpdateSliding( RwReal fElapsed ) 
{
    RwBool bUpdate = m_AccelController.Update(fElapsed);

    m_pThis->SetPosition((RwInt32)m_AccelController.GetCurrentValue(), m_pThis->GetScreenRect().top);

    if(!bUpdate)
    {
        if(m_eTeleCastState == TELECAST_SLIDING_IN)    // 슬라이딩인 완료
        {
            Opening();
        }
        else if(m_eTeleCastState == TELECAST_SLIDING_OUT)   // 슬라이딩 아웃 완료
        {
            m_eTeleCastState = TELECAST_CLOSED;
            SwitchDialog(false);
        }
    }
}

