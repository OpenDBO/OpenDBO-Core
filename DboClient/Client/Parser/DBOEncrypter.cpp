#include "precomp_dboclient.h"
#include "DBOEncrypter.h"
#include "NtlXMLDoc.h"
#include "NtlFileSerializer.h"
#include "NtlResourceFileManager.h"
#include "NtlLuaState.h"

#define CRYPT_PASSWORD "!@*&(agebreak"

CDBOEncrypter::CDBOEncrypter() 
{

}

CDBOEncrypter::~CDBOEncrypter() 
{

}

RwBool CDBOEncrypter::EncryptConfig( const RwChar* szFilePath ) 
{
    // Open
    SConfigData data;
    if(!LoadConfigOptions(&data, szFilePath))
        return FALSE;

    // File Path
    char szDrive[128]   = {0,};
    char szdir[128]     = {0,};
    char szfName[128]   = {0,};
    char szExt[128]     = {0,};
    _splitpath_s(szFilePath, szDrive, szdir, szfName, szExt);

    std::string strDestFile = szDrive;
    strDestFile + szdir;
    strDestFile += szfName;
    strDestFile += ".edf";

    if(!SaveConfigOptions(&data, strDestFile.c_str(), CRYPT_PASSWORD))
        return FALSE;

    return TRUE;
}

RwBool CDBOEncrypter::LoadConfigOptions(OUT SConfigData* pConfigData, const RwChar* szFilePath)
{
    CNtlXMLDoc doc;
    doc.Create();

    // xml doc load
    if(doc.Load( (char*)szFilePath ) == false)
    {
        NTL_RETURN(FALSE);
    }

    char chBuffer[1024];

    // config operation을 얻어온다.
    IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/config_options/op");

    if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    if(!doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    pConfigData->strAddr = chBuffer;

    if(!doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    pConfigData->dwPort = (DWORD)atoi(chBuffer);

    pNode->Release(); 

    // config local을 얻어온다.

    pNode = doc.SelectSingleNode((char*)"/config_options/local");

    if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    if(!doc.GetTextWithAttributeName(pNode, "local_dsp", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    pConfigData->strLocalDsp = chBuffer;

    if(!doc.GetTextWithAttributeName(pNode, "local_sync_dsp", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    pConfigData->strLocalSyncDsp = chBuffer;

    // Bug Trap
    pNode = doc.SelectSingleNode("/config_options/BUGTRAP");
    if(doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
    {
        pConfigData->strBugTrapServerIP = chBuffer;
    }

    if(doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
    {
        pConfigData->dwBugTrapServerPort = atoi(chBuffer);        
    }

    pNode->Release(); 

    return TRUE;
}

RwBool CDBOEncrypter::SaveConfigOptions(SConfigData* pConfigData, const RwChar* szDestPath, const RwChar* szCryptPassword)
{
    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);

    nsl<<pConfigData->strAddr;
    nsl<<pConfigData->dwPort;
    nsl<<pConfigData->strLocalDsp;
    nsl<<pConfigData->strLocalSyncDsp;
    nsl<<pConfigData->strBugTrapServerIP;
    nsl<<pConfigData->dwBugTrapServerPort;

    return nsl.SaveFile((RwChar*)szDestPath, TRUE, (RwChar*)szCryptPassword);
}

RwBool CDBOEncrypter::DecryptConfig(OUT SConfigData* pConfigData, const RwChar* szFilePath)
{
    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);
    if(!nsl.LoadFile((char*)szFilePath, TRUE, CRYPT_PASSWORD))
        return FALSE;

    nsl>>pConfigData->strAddr;
    nsl>>pConfigData->dwPort;
    nsl>>pConfigData->strLocalDsp;
    nsl>>pConfigData->strLocalSyncDsp;
    nsl>>pConfigData->strBugTrapServerIP;
    nsl>>pConfigData->dwBugTrapServerPort;

    return TRUE;
}

RwBool CDBOEncrypter::CompileLuaFiles() 
{
    // Script 폴더안에 있는 모든 Lua 파일들을 컴파일한다.
    std::string strPath = ".\\script\\";

    CNtlResourceFileManager fmLua;
    fmLua.AddExt(".lua");
    fmLua.LoadPath(strPath.c_str());


    CNtlResourceFileManager::smdef_Entity::iterator it;
    for(it = fmLua.m_smEntity.begin(); it != fmLua.m_smEntity.end(); ++it)
    {
        std::string strFileName = (*it).second;
        if(!CNtlLuaState::CompileLua(strFileName.c_str()))
            return FALSE;
    }

    return TRUE;
}
