#include "precomp_dboclient.h"
#include "SecurityStage.h"

// shared
#include "NtlResultCode.h"

// network
#include "NtlClientNet.h"

// core
#include "NtlDebug.h"
#include "NtlEventQueue.h"

// simulation
#include "NtlNetSender.h"
#include "InputActionMap.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringDef.h"
#include "DisplayStringManager.h"
#include "LogInGuiGroup.h"
#include "LogInStageState.h"
#include "AlarmManager.h"

#include "DboApplication.h"
#include "MoviePlayer.h"

CSecurityStage::CSecurityStage(const char *pStageName)
:CNtlStage(pStageName)
{

}

CSecurityStage::~CSecurityStage()
{

}

bool CSecurityStage::Create()
{
	NTL_FUNCTION("CSecurityStage::Create");

	CDboEventGenerator::StageDelete(SECURITY_STAGE_NAME);
	CDboEventGenerator::StageCreate(WEB_LOGIN_STAGE_NAME);

	NTL_RETURN(TRUE);
}

void CSecurityStage::Destroy()
{
	NTL_FUNCTION("CSecurityStage::Destroy");

	NTL_RETURNVOID();
}

void CSecurityStage::HandleEvents(RWS::CMsg &pMsg)
{

}