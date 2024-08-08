#include "precomp_dboclient.h"
#include "WebLoginStage.h"

// shared
#include "NtlResultCode.h"

// network
#include "NtlClientNet.h"

// core
#include "NtlDebug.h"
#include "NtlEventQueue.h"
#include "NtlSystemInfo.h"

// sound
#include "NtlSoundEventGenerator.h"

// Presentation
#include "NtlPLSceneManager.h"
#include "NtlPLApi.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLResourcePack.h"

// simulation
#include "NtlNetSender.h"
#include "InputActionMap.h"
#include "NtlCameraManager.h"
#include "NtlStorageManager.h"
#include "NtlClientService.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringDef.h"
#include "DisplayStringManager.h"
#include "WebLogInGuiGroup.h"
#include "LogInStageState.h"
#include "AlarmManager.h"

#include "DboApplication.h"

#define dCOUNT_START		(0)
#define dCOUNT_FINISH		(3)
#define dCOUNT_NONE			(255)

#define dRETRY_LOGIN_DELAY_TIME		(5.f)


CWebLoginStage::CWebLoginStage(const char *pStageName)
:CNtlStage(pStageName)
,m_pGuiGroup(NULL)
,m_pState(NULL)
,m_pWorldEntity(NULL)
,m_bWorldAddCamera(FALSE)
,m_byCount_forCreateWorld(dCOUNT_NONE)
{
	m_tRETRY_LOGIN.bRetrying		= FALSE;
	m_tRETRY_LOGIN.bSendPacket		= FALSE;
	m_tRETRY_LOGIN.fElapsed			= 0.f;
}

CWebLoginStage::~CWebLoginStage()
{

}

bool CWebLoginStage::Create()
{
	NTL_FUNCTION("CWebLoginStage::Create");

	m_pState	= NTL_NEW CLogInStageState;

	m_pGuiGroup = NTL_NEW CWebLoginGuiGroup;
	if(!m_pGuiGroup->Create())
	{
		NTL_DELETE(m_pGuiGroup);
		NTL_RETURN(FALSE);
	}


	GetInputActionMap()->LinkGameExit(this, &CWebLoginStage::ActionGameExit);

	LinkMsg(g_EventUpdateTick);
	LinkMsg(g_EventLogInServerConnect);
	LinkMsg(g_EventLobbyServerConnect);
	LinkMsg(g_EventLogInStageStateEnter);
	LinkMsg(g_EventLogInStageStateExit);
	LinkMsg(g_EventLogInStageTimeOut);
	LinkMsg(g_EventLoginMessage);

	GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT);	

#ifndef NDEBUG
	RwBool bWindowMode = GetNtlStorageManager()->GetBoolData( dSTORAGE_GRAPHIC_WINDOW_MODE );
	RwInt32 nScreenWidth = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH );
	RwInt32 nScreenHeight = GetNtlStorageManager()->GetIntData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_WINDOW_MODE, B2b(!NTL_DEBUG_WINDOW_MODE) );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH, DEFAULT_SCREEN_WIDTH );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT, DEFAULT_SCREEN_HEIGHT );
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_GRAPHIC, eNTL_STORAGE_APPLY_ALL );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_WINDOW_MODE, B2b(bWindowMode) );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH, nScreenWidth );
	GetNtlStorageManager()->SetData( dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT, nScreenHeight );
#else
	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_GRAPHIC, eNTL_STORAGE_APPLY_ALL );		
#endif

	GetNtlStorageManager()->ApplyStorageType( eNTL_STORAGE_SOUND, eNTL_STORAGE_APPLY_ALL );


	NTL_RETURN(TRUE);
}

void CWebLoginStage::Destroy()
{
	NTL_FUNCTION("CWebLoginStage::Destroy");

	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);

	GetInputActionMap()->UnLinkGameExit();

	if(m_bWorldAddCamera)
	{
		RpWorldRemoveCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
		m_bWorldAddCamera = FALSE;
	}

	if(m_pWorldEntity)
	{
		GetSceneManager()->DeleteEntity(m_pWorldEntity);
		m_pWorldEntity = NULL;
	}

	if(m_pGuiGroup)
	{
		m_pGuiGroup->Destroy();
		NTL_DELETE(m_pGuiGroup);
	}

	NTL_DELETE(m_pState);


	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventLogInServerConnect);
	UnLinkMsg(g_EventLobbyServerConnect);
	UnLinkMsg(g_EventLogInStageStateEnter);
	UnLinkMsg(g_EventLogInStageStateExit);
	UnLinkMsg(g_EventLogInStageTimeOut);
	UnLinkMsg(g_EventLoginMessage);

	NTL_RETURNVOID();
}

RwBool CWebLoginStage::CreateWorld()
{
	if( m_pWorldEntity )
	{
		DBO_FAIL("already exist world entity");
		return FALSE;
	}


	_getcwd(dGET_WORLD_PARAM()->CurWorkingFolderName, 256);
	strcpy(dGET_WORLD_PARAM()->CurDefaultTexPath, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->CurDefaultTexPath, "\\texture\\ntlwe\\");

	std::string str = "\\world\\";	
	str += "DBO_Start_001";

	strcpy(dGET_WORLD_PARAM()->WorldProjectFolderName, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat(dGET_WORLD_PARAM()->WorldProjectFolderName, str.c_str());

	str += "\\";
	str += "#######.gwp";

	FILE* pFile = NULL;
	SPackResFileData sPackFileData;
	RwBool bPack = GetNtlResourcePackManager()->LoadTerrain(str.c_str(), sPackFileData);

	if(bPack)
	{
		pFile = fopen(sPackFileData.strPackFileName.c_str(), "rb");
		if(pFile)
			fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
	}
	else
	{
		std::string strWorldFileName = ".";
		strWorldFileName += str;
		pFile = fopen(strWorldFileName.c_str(), "rb");
	}

	// create a global world param
	if(pFile)
	{
		LoadFileWorldState(dGET_WORLD_PARAM(), pFile);
		fclose(pFile);
	}

	DBO_ASSERT(dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION), "invalid world resource version. world(" << dGET_WORLD_PARAM()->WorldProjectFolderName << ") support_version(" << dNTL_WORLD_VERSION << ") resource_version(" << dGET_WORLD_PARAM()->WorldVer << ")");

	DWORD dwCur = GetTickCount();
	RwV3d vAvatarPos;

	vAvatarPos.x	= 133.15f;
	vAvatarPos.y	= 0.65f;
	vAvatarPos.z	= -261.32f;


	m_pWorldEntity = static_cast<CNtlPLWorldEntity*>(GetSceneManager()->CreateEntity(PLENTITY_WORLD, "NtlWorld"));
	m_pWorldEntity->CreateWorld(vAvatarPos);

	RpWorldAddCamera(CNtlPLGlobal::m_pRpWorld, CNtlPLGlobal::m_RwCamera);
	m_bWorldAddCamera = TRUE;

	DWORD dwElapsedTime = GetTickCount() - dwCur;	
	char buffer[256];
	wsprintf(buffer, "World Create Elapsed: %u\n", (int)dwElapsedTime);
	OutputDebugString(buffer);

	GetNtlGameCameraManager()->SetFov(60.f);

	//Player Position을 제대로 Update를 해주어야 하늘이 제대로 나온다.
	m_pWorldEntity->SetPlayerPosition(vAvatarPos);

	CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);

	return TRUE;
}

void CWebLoginStage::SetCameraPositioin()
{
	RwV3d		v3CameraPos, v3CameraDir;

	v3CameraPos.x	= 129.39f;
	v3CameraPos.y	= 4.27f;
	v3CameraPos.z	= -279.74f;

	v3CameraDir.x	= 129.88f;
	v3CameraDir.y	= 4.33f;
	v3CameraDir.z	= -280.61f;

	API_PL_CameraLookAt(CNtlPLGlobal::m_RwCamera, &v3CameraPos, &v3CameraDir);
}

void CWebLoginStage::Update_Retry_Login(RwReal fElapsed)
{
	if( !m_tRETRY_LOGIN.bRetrying )
		return;

	m_tRETRY_LOGIN.fElapsed -= fElapsed;
	if( m_tRETRY_LOGIN.fElapsed > 0.f )
		return;


	m_tRETRY_LOGIN.fElapsed = 0.f;
	if( m_tRETRY_LOGIN.bSendPacket )
		return;


	m_tRETRY_LOGIN.bSendPacket = TRUE;

	if( FALSE == Send_LoginPacket() )
	{
		DBO_FAIL("Fail send login packet");
		return;
	}

	GetLogInStageState()->ChangeState(LOGIN_STATE_LOGINREQ);
}

void CWebLoginStage::EventProcUpdateTick(RwReal fElapsed)
{
	GetLogInStageState()->Update(fElapsed);

	Update_Retry_Login(fElapsed);	
}

RwBool CWebLoginStage::Send_LoginPacket()
{
	RwChar*	pcCookie_CP		= (RwChar*)GetServiceInterface()->GetServiceData(4);
	RwChar*	pcMacAddress	= (RwChar*)GetSystemMacAddress();

	if( !pcCookie_CP )
	{
		GetAlarmManager()->AlarmMessage(L"Fail read CP Cookie", CAlarmManager::ALARM_TYPE_LOBBY_MSGBOX, MBW_POST_QUIT_MESSAGE);
		return FALSE;
	}

	if( !pcMacAddress )
	{
		GetAlarmManager()->AlarmMessage(L"Fail read MAC address", CAlarmManager::ALARM_TYPE_LOBBY_MSGBOX, MBW_POST_QUIT_MESSAGE);
		return FALSE;
	}


	RwInt32	iCookie_CPLength	= (RwInt32)strlen(pcCookie_CP);
	RwInt32	iMacAddressLength	= (RwInt32)strlen(pcMacAddress);

	if( iCookie_CPLength > NTL_MAX_SIZE_CP_COOKIE )
	{
		NtlLogFilePrint("Too long CP Cookie length");
		return FALSE;
	}

	if( iMacAddressLength > DBO_MAX_ADAPTER_ADDRESS_LENGTH )
	{
		NtlLogFilePrint("Too long Mac Address length");
		return FALSE;
	}

	if( GetDboGlobal()->GetLoginPacketGenerator()->SendLonInReq_KoreaCJ(GetACP(),
						CLIENT_LVER, CLIENT_RVER, iCookie_CPLength, (BYTE*)pcCookie_CP, (const BYTE*)pcMacAddress) )
		return TRUE;

	return FALSE;
}

void CWebLoginStage::LogInServerConnect(RWS::CMsg &pMsg)
{
	SDboEventServerConnect *pConnect = reinterpret_cast<SDboEventServerConnect*>( pMsg.pData );

	if(pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_SUCCESS)
	{
		if( FALSE == Send_LoginPacket() )
		{
			DBO_FAIL("Fail send login packet");
			return;
		}
	
		GetLogInStageState()->ChangeState(LOGIN_STATE_LOGINREQ);
	}
	else if(pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_FAIL)
	{
		GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT_FAIL);
		GetAlarmManager()->AlarmMessage( DST_ACCOUNT_SERVER_CONNECT_FAIL );
	}
	else if(pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT)
	{
		GetAlarmManager()->AlarmMessage( DST_SERVER_DISCONNECT );
	}
}

void CWebLoginStage::LobbyServerConnect(RWS::CMsg &pMsg)
{
	SDboEventServerConnect *pConnect = reinterpret_cast<SDboEventServerConnect*>( pMsg.pData );

	if( pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_SUCCESS )
	{
		GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_LOGIN);
	}
	else if( pConnect->byConnectType == SERVER_CONNECT_TYPE_CONNENCT_FAIL )
	{
		GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_CONNECT);
	}
}

void CWebLoginStage::LoginStageStateEnter(RWS::CMsg &pMsg)
{
	SDboEventLogInStageStateEnter *pStageStateEnter = reinterpret_cast<SDboEventLogInStageStateEnter*>(pMsg.pData);

	ELogInStageState eState = (ELogInStageState)pStageStateEnter->byState;
	switch(eState)
	{
	case LOGIN_STATE_SERVER_CONNECT:
		{
			// account server connect...
			SConfigData *pConfigData = GetDboGlobal()->GetConfigData();  
			SConnectData *pConnectData = GetDboGlobal()->GetConnectData();  
			CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 

#ifdef NET_SERVER_SYNC
			pNet->Connect( (char*)pConfigData->strAddr.c_str(), (WORD)pConfigData->dwPort, &pConnectData->sAuthCon.hSocket, true, NETCLIENT_ENCODE_RANDKEY );
#else
			pNetSender->SetData(INVALID_HSERVER, NULL); 
			GetLogInStageState()->ChangeState(LOGIN_STATE_IDLE);
#endif
		}
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		{
#ifdef NET_SERVER_SYNC

			SConnectData *pConData = GetDboGlobal()->GetConnectData();  
			CNtlClientNet *pNet = GetDboGlobal()->GetNetwork(); 

			SCharServerInfo sCharServerInfo;
			RwBool bExist = pConData->PopCharacterServer(&sCharServerInfo);
			if(bExist)
				pNet->Connect( (char*)sCharServerInfo.chServerIP, sCharServerInfo.wServerPort, &pConData->sCharCon.hSocket, TRUE, NETCLIENT_ENCODE_RANDKEY );
			else
			{
				GetAlarmManager()->AlarmMessage( DST_CHAR_SERVER_CONNECT_FAIL );
				GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT_FAIL);
			}
#else
			CLobbyPacketGenerator *pLobbyPacketGenerator = GetDboGlobal()->GetLobbyPacketGenerator(); 
			CNtlNetSender *pNetSender = pLobbyPacketGenerator->GetNetSender();
			pNetSender->SetData(INVALID_HSERVER, NULL); 

			GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_LOGIN);
#endif			
		}
		break;
	}
}

void CWebLoginStage::LoginStageStateExit(RWS::CMsg &pMsg)
{
}

void CWebLoginStage::LogInStageTimeOutEventHandler(RWS::CMsg &msg)
{
	SDboEventLogInStageTimeOut *pStageTimeOut = reinterpret_cast<SDboEventLogInStageTimeOut*>(msg.pData);

	ELogInStageState eState = (ELogInStageState)pStageTimeOut->byState;
	switch(eState)
	{
	case LOGIN_STATE_LOGO:
		break;
	case LOGIN_STATE_SERVER_CONNECT: 
		GetAlarmManager()->AlarmMessage( DST_ACCOUNT_SERVER_CONNECT_FAIL );
		break;
	case LOGIN_STATE_SERVER_CONNECT_FAIL:
		break;
	case LOGIN_STATE_IDLE:
		break;
	case LOGIN_STATE_LOGINREQ:
		GetAlarmManager()->AlarmMessage( AUTH_AUTHKEY_FAIL, TRUE );
		GetLogInStageState()->ChangeState(LOGIN_STATE_IDLE);
		break;
	case LOGIN_STATE_CHAR_SERVER_CONNECT:
		GetLogInStageState()->ChangeState(LOGIN_STATE_CHAR_SERVER_CONNECT);
		break;
	case LOGIN_STATE_CHAR_SERVER_LOGIN:
		GetAlarmManager()->AlarmMessage( CHARACTER_AUTHKEY_FAIL, TRUE );
		GetLogInStageState()->ChangeState(LOGIN_STATE_SERVER_CONNECT);
		break;
	}
}

int CWebLoginStage::ActionGameExit()
{
	CDboApplication::GetInstance()->SetRenderEnable(TRUE);
	/*CDboApplication::GetInstance()->SetToggleScreenMode(TRUE);*/

	GetInputActionMap()->SetActive(FALSE);

	return TRUE;
}

void CWebLoginStage::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventUpdateTick)
	{
		if( dCOUNT_NONE != m_byCount_forCreateWorld )
		{
			// 동영상 플레이가 끝나고 바로 월드 로딩을 시작하면
			// 월드 로딩이 끝날 때까지 동영상의 마지막 프레임 혹은
			// 아무것도 랜더링 되지 않아 검은 화면이 된다.
			// 중간에 GUI를 표시한 화면으로 멈추있도록 몇 Tick을 거르고 월드를 로딩!!
			++m_byCount_forCreateWorld;

			if( dCOUNT_FINISH == m_byCount_forCreateWorld )
			{
				m_byCount_forCreateWorld = dCOUNT_NONE;

				if( CreateWorld() )
				{
					SetCameraPositioin();
					CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_START_MAIN_THEME);
				}
			}			
		}

		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		EventProcUpdateTick(fElapsed);
	}
	else if(pMsg.Id == g_EventLogInServerConnect)
	{
		LogInServerConnect(pMsg);
	}
	else if(pMsg.Id == g_EventLobbyServerConnect)
	{
		LobbyServerConnect(pMsg);
	}
	else if(pMsg.Id == g_EventLogInStageStateEnter)
	{
		LoginStageStateEnter(pMsg);
	}
	else if(pMsg.Id == g_EventLogInStageStateExit)
	{
		LoginStageStateExit(pMsg);
	}
	else if(pMsg.Id == g_EventLogInStageTimeOut)
	{
		LogInStageTimeOutEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventLoginMessage)
	{
		SDboEventLoginMessage* pEvent = reinterpret_cast<SDboEventLoginMessage*>( pMsg.pData );

		if( LOGIN_EVENT_FINISH_LOADING_AND_MOVIE == pEvent->byMessage )
		{
			Logic_SetOptionMovieStop();
			m_byCount_forCreateWorld = dCOUNT_START;
		}
		else if( LOGIN_EVENT_SUCCESS_LOGIN == pEvent->byMessage )
		{
			// avooo's commnet : 애플리케이션이 처음 실행될 때만 로고가 보이도록 하였다.
			//					 게임중 로그인 화면으로 돌아왔을 때 로고가 또 보이니 어색하다.
			static RwBool bFirstRunLogo = TRUE;

			if( bFirstRunLogo )
			{
				if( GetDboGlobal()->GetContractVerstion() == (RwUInt32)GetNtlStorageManager()->GetIntData( dSTORAGE_ACCOUNT_CONTRACT ) )
					GetDboGlobal()->SetGameContract(TRUE);
				else
					GetDboGlobal()->SetGameContract(FALSE);

				bFirstRunLogo = !bFirstRunLogo;
				GetLogInStageState()->ChangeState(LOGIN_STATE_LOGO);
			}
			else
			{
				CDboEventGenerator::LoginEvent(LOGIN_EVENT_FINISH_LOADING_AND_MOVIE);
				GetLogInStageState()->ChangeState(LOGIN_STATE_IDLE);
			}
		}
		else if( LOGIN_EVENT_RETRY_LOGIN_REQ == pEvent->byMessage )
		{
			m_tRETRY_LOGIN.bRetrying		= TRUE;
			m_tRETRY_LOGIN.bSendPacket		= FALSE;
			m_tRETRY_LOGIN.fElapsed			= dRETRY_LOGIN_DELAY_TIME;			
		}
	}
}