#include "precomp_dboclient.h"
#include "DboAddinManager.h"

#include "NtlDebug.h"
#include "NtlSLEvent.h"

#ifndef _WIN64
// Devil
#include "il.h"
#endif

CDBOAddinManager* CDBOAddinManager::m_pInstance = NULL;

CDBOAddinManager::CDBOAddinManager() 
{
	LinkMsg(g_EventScreenShot);
}

CDBOAddinManager::~CDBOAddinManager() 
{
	UnLinkMsg(g_EventScreenShot);
}

void CDBOAddinManager::Init() 
{
    if(!m_pInstance)
    {
        m_pInstance = NTL_NEW CDBOAddinManager();
#ifndef _WIN64
        ilInit();
#endif
    }    
}

void CDBOAddinManager::ShutDown() 
{
    if(m_pInstance)
    {
#ifndef _WIN64
        ilShutDown();
#endif
        NTL_DELETE(m_pInstance);
    }
}

void CDBOAddinManager::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventScreenShot)
    {
        // ��ũ������ �����Ŀ� ������� �̺�Ʈ
        OnEventScreenShot(pMsg);
    }
}

void CDBOAddinManager::OnEventScreenShot( RWS::CMsg& pMsg ) 
{
    // png�� ����� ������ jpg�� ��ȯ�Ѵ�.
    SNtlEventScreenShot* pData = (SNtlEventScreenShot*)pMsg.pData;
    std::string strOrgfileName, strDestFileName;
    strOrgfileName = pData->strFileName;
    strOrgfileName.erase(strOrgfileName.end() - 4, strOrgfileName.end());
    strDestFileName = strOrgfileName;
    strOrgfileName += ".bmp";
    strDestFileName += ".jpg";

#ifndef _WIN64
    ilLoadImage(strOrgfileName.c_str());
    ilSaveImage(strDestFileName.c_str());
#endif

    DeleteFile(strOrgfileName.c_str());
}