#include "precomp_dboclient.h"
#include "WebLoginGuiGroup.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlClientService.h"

// dbo
#include "LogoGui.h"
#include "ContractGui.h"
#include "DBOTitleGui.h"
#include "DeliberationRankGui.h"
#include "DboGlobal.h"
#include "DboApplication.h"
#include "MoviePlayer.h"
#include "LoginStageState.h"


#define AddDialog(pGui, classname, szName) \
	{ \
		pGui = NTL_NEW classname(szName);\
		if(!pGui->Create())			\
		{							\
			pGui->Destroy();		\
			NTL_DELETE(pGui);		\
			NTL_RETURN(FALSE);		\
		}							\
		CNtlPLGuiManager::GetInstance()->AddGui(pGui);\
	}

#define RemoveDialog(pGui) \
	{ \
		if(pGui)       \
		{              \
		CNtlPLGuiManager::GetInstance()->RemoveGui(pGui);   \
		pGui->Destroy();                                    \
		NTL_DELETE(pGui);                                   \
		} \
	}


CWebLoginGuiGroup::CWebLoginGuiGroup()
:m_pLogoGui(NULL)
,m_pContractGui(NULL)
,m_pDBOTitle(NULL)
,m_pDeliverationRankContainer(NULL)
{	
}

CWebLoginGuiGroup::~CWebLoginGuiGroup()
{
}


RwBool CWebLoginGuiGroup::Create(void)
{
	NTL_FUNCTION("CWebLoginGuiGroup::Create");

	AddDialog(m_pLogoGui,				CLogoGui,		"LogoGui");
	AddDialog(m_pContractGui,			CContractGui,	"ContractGui");
	AddDialog(m_pDBOTitle,				CDBOTitleGui,	"CDBOTitleGui");


	eDBO_SERVICE_PUBLISHER eService = GetServiceInterface()->GetPublisherID();
	if( eDBO_SERVICE_PUBLISHER_CJI_KOR == eService )
	{
		m_pDeliverationRankContainer = NTL_NEW CDeliberationRankContainer;
		if(!m_pDeliverationRankContainer->Create())
		{
			m_pDeliverationRankContainer->Destroy();
			NTL_DELETE(m_pDeliverationRankContainer);
			NTL_RETURN(FALSE);
		}
	}


	LinkMsg(g_EventLoginMessage);

	NTL_RETURN(TRUE);
}

void CWebLoginGuiGroup::Destroy(void)
{
	NTL_FUNCTION("CWebLoginGuiGroup::Destroy");

	RemoveDialog(m_pLogoGui);
	RemoveDialog(m_pContractGui);
	RemoveDialog(m_pDBOTitle);

	if( m_pDeliverationRankContainer )
	{
		m_pDeliverationRankContainer->Destroy();
		NTL_DELETE(m_pDeliverationRankContainer);
	}	

	UnLinkMsg(g_EventLoginMessage);

	NTL_RETURNVOID();
}

int CWebLoginGuiGroup::CallBackMoviePlayStop()
{
	if( FALSE == GetDboGlobal()->IsAcceptGameContract() )
	{
		CDboEventGenerator::LoginEvent(LOGIN_EVENT_SHOW_CONTRACT_PAGE);
	}

	CDboEventGenerator::LoginEvent(LOGIN_EVENT_FINISH_LOADING_AND_MOVIE);
	
	return TRUE;
}

void CWebLoginGuiGroup::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CWebLoginGuiGroup::HandleEvents");

	if(pMsg.Id == g_EventLoginMessage)
	{
		SDboEventLoginMessage* pEvent = reinterpret_cast<SDboEventLoginMessage*>( pMsg.pData );

		switch(pEvent->byMessage)
		{
		case LOGIN_EVENT_FINISH_LOGO_PLAY:
			{
				CDboEventGenerator::LoginPostEvent(LOGIN_EVENT_START_INTRO_MOVIE_PLAY);				
				break;
			}
		case LOGIN_EVENT_START_INTRO_MOVIE_PLAY:
			{
				// GetMoviePlayer()->SetCallBack 은 하나만 지정할 수 있다.
				// MoviePlayer의 stop 콜백이 날라오는 것은 아직 지정된 callback이 남아있을 때인데
				// 이 때 다시 callback을 지정한다면 MoviePlayer에서 지정된 콜백을 날린 이후 콜백을 삭제한다
				// 따라서 callback을 받은 시점에 Post Event를 날려 한 틱후에 다시 Movie를 플레이하고
				// callback을 지정한다
				if( GetMoviePlayer()->Play(dOPENING_MOVIE, MOVIE_SIZE_RATIO_FULL) )
				{
					GetMoviePlayer()->SetCallBack(this, &CWebLoginGuiGroup::CallBackMoviePlayStop);
				}
				else
				{					
					CallBackMoviePlayStop();					
				}

				break;
			}
		}
	}

	NTL_RETURNVOID();
}