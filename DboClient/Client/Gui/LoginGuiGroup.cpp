#include "precomp_dboclient.h"
#include "LoginGuiGroup.h"
#include "NtlDebug.h"
#include "NtlPLGuiManager.h"

#include "LogoGui.h"
#include "LoginGui.h"
#include "MsgBoxManager.h"

//#define dLOAD_GAME_COMMON_RESOURCE

#ifdef dLOAD_GAME_COMMON_RESOURCE
	#include "DboGlobal.h"
#endif

CLogInGuiGroup::CLogInGuiGroup()
:m_pLogoGui(NULL)
,m_pLogin(NULL)
{	
}

CLogInGuiGroup::~CLogInGuiGroup()
{
}


RwBool CLogInGuiGroup::Create(void)
{
	NTL_FUNCTION("CLogInGuiGroup::Create");

#ifdef dLOAD_GAME_COMMON_RESOURCE
	gui::CResourceManager *pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	gui::CSurfaceManager *pSurMgr = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();

	pResMgr->AddPage("gui\\Human\\Game.rsr");
	GetDboGlobal()->SetGuiFolderName(HUMAN_GUI_TEXTURE_PATH);

	pSurMgr->AddPage("gui\\GameCommon.srf");
	pSurMgr->AddPage("gui\\CommonNumber.srf");
#endif

	// logo gui
	m_pLogoGui = NTL_NEW CLogoGui("LogoGui");
	if(!m_pLogoGui->Create())
	{
		m_pLogoGui->Destroy(); 
		NTL_DELETE(m_pLogoGui);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pLogoGui); 

	// login gui
	m_pLogin = NTL_NEW CLogInGui("LogInGui");
	if(!m_pLogin->Create())
	{
		m_pLogin->Destroy(); 
		NTL_DELETE(m_pLogin);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pLogin); 

	NTL_RETURN(TRUE);
}

void CLogInGuiGroup::Destroy(void)
{
	NTL_FUNCTION("CLogInGuiGroup::Destroy");

	if(m_pLogoGui)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pLogoGui);
		m_pLogoGui->Destroy();
		NTL_DELETE(m_pLogoGui);
	}

	if(m_pLogin)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pLogin);
		m_pLogin->Destroy();
		NTL_DELETE(m_pLogin);
	}

	//if(m_pConnectBox)
	//{
	//	CNtlPLGuiManager::GetInstance()->RemoveGui(m_pConnectBox);
	//	m_pConnectBox->Destroy();
	//	NTL_DELETE(m_pConnectBox);
	//}

#ifdef dLOAD_GAME_COMMON_RESOURCE
	gui::CResourceManager *pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
	gui::CSurfaceManager *pSurMgr = CNtlPLGuiManager::GetInstance()->GetSurfaceManager();

	pResMgr->RemovePage("Game.rsr");
	pSurMgr->RemovePage("GameCommon.srf");
#endif

	NTL_RETURNVOID();
}