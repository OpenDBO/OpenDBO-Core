#include "precomp_dboclient.h"
#include "NetMarblePCBangGui.h"

#include "NtlDebug.h"
#include "DboDef.h"
#include "NtlPLGuiManager.h"
#include "StageManager.h"
#include "NtlStage.h"
#include "DboGlobal.h"

CNetMarblePCBangGui::CNetMarblePCBangGui(void)
{
    Init();
}

CNetMarblePCBangGui::CNetMarblePCBangGui( const RwChar* pName ) 
: CNtlPLGui(pName)
{
    Init();
}

CNetMarblePCBangGui::~CNetMarblePCBangGui(void)
{
    Destroy();
}

VOID CNetMarblePCBangGui::Init() 
{

}

RwBool CNetMarblePCBangGui::Create() 
{
    NTL_FUNCTION(__FUNCTION__);

    if(!CNtlPLGui::Create("gui\\NetMarblePCBangLogo.rsr", "gui\\NetMarblePCBangLogo.srf", "gui\\NetMarblePCBangLogo.frm"))
        NTL_RETURN(FALSE);

    CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());
    m_pThis			= (gui::CDialog*)GetComponent("dlgMain");

    // 기본 위치는 로비 GUI 위치. 처음 생성될때는 로비에서 생성되니까.    
    LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

    Show(false);	        

    LinkMsg(g_EventNetMarbleMemberShipNfy);
 
    return TRUE;
}

VOID CNetMarblePCBangGui::Destroy() 
{
    UnLinkMsg(g_EventNetMarbleMemberShipNfy);
    CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy();
}

RwInt32 CNetMarblePCBangGui::SwitchDialog( bool bOpen ) 
{
    //Show(bOpen);

    return 1;
}

VOID CNetMarblePCBangGui::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventNetMarbleMemberShipNfy)
    {
        LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
        Show(true);
    }
}

VOID CNetMarblePCBangGui::LocateComponent( RwInt32 IWidth, RwInt32 IHeight ) 
{
    CNtlStage *pStage = CStageManager::GetInstance()->GetActiveStage();
    if(pStage)
    {
        std::string strStageName = pStage->GetName();
        if(strStageName == GAME_STAGE_NAME)
        {
            m_pThis->SetPosition(IWidth - E_GAME, E_POSY);            
            return;
        }
    }

    m_pThis->SetPosition(IWidth - E_LOBBY, E_POSY);
}