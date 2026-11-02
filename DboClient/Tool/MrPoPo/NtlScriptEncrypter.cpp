#include "StdAfx.h"
#include "NtlScriptEncrypter.h"
#include "NtlXMLDoc.h"

#define DEFAULT_LOCALIZE_FOLDER     "english"

CNtlScriptEncrypter::CNtlScriptEncrypter(void)
{
}

CNtlScriptEncrypter::~CNtlScriptEncrypter(void)
{
}

/**
 * Loads ConfigOptions.xml like the client does: op is required, BUGTRAP and localize are optional.
 * \param pConfigData the loaded data
 * \param szFileName the file to load
 */
BOOL CNtlScriptEncrypter::LoadConfigOption( OUT SConfigData* pConfigData, const char* szFileName )
{
    if(!pConfigData || !szFileName)
        return FALSE;

    CNtlXMLDoc doc;
    doc.Create();

    if(doc.Load( (char*)szFileName ) == false)
        return FALSE;

    char chBuffer[1024];

    // op: the auth server
    IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/config_options/op");
    if(!pNode)
        return FALSE;

    if(!doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
    {
        pNode->Release();
        return FALSE;
    }

    pConfigData->strAddr = chBuffer;

    if(!doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
    {
        pNode->Release();
        return FALSE;
    }

    pConfigData->dwPort = (DWORD)atoi(chBuffer);

    pNode->Release();

    // BUGTRAP
    pConfigData->strBugTrapServerIP = L"";
    pConfigData->dwBugTrapServerPort = 0;
    pConfigData->strBugTrapFolder = L"";

    pNode = doc.SelectSingleNode((char*)"/config_options/BUGTRAP");
    if(pNode)
    {
        if(doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
            pConfigData->strBugTrapServerIP = chBuffer;

        if(doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
            pConfigData->dwBugTrapServerPort = (DWORD)atoi(chBuffer);

        if(doc.GetTextWithAttributeName(pNode, "folder", chBuffer, 1024))
            pConfigData->strBugTrapFolder = chBuffer;

        pNode->Release();
    }

    // localize
    pConfigData->strLocalize = DEFAULT_LOCALIZE_FOLDER;

    pNode = doc.SelectSingleNode((char*)"/config_options/localize");
    if(pNode)
    {
        if(doc.GetTextWithAttributeName(pNode, "folder", chBuffer, 1024))
            pConfigData->strLocalize = chBuffer;

        pNode->Release();
    }

    return TRUE;
}

BOOL CNtlScriptEncrypter::SaveConfigOption( SConfigData* pConfigData, const char* szFileName )
{
    if(!pConfigData || !szFileName)
        return FALSE;

    CNtlXMLDoc doc;
    doc.Create();

    IXMLDOMProcessingInstruction* pPI    = NULL;

    doc.GetDocument()->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
    if(!pPI)
        return FALSE;
    doc.GetDocument()->appendChild(pPI, NULL);

    IXMLDOMElement*     pElemRoot = NULL;                ///< Root Element
    IXMLDOMElement*     pElemOp = NULL;
    IXMLDOMElement*     pElemBUGTrap = NULL;
    IXMLDOMElement*     pElemLocalize = NULL;

    doc.GetDocument()->createElement(L"config_options", &pElemRoot);
    doc.GetDocument()->appendChild(pElemRoot, NULL);

    WCHAR wbuf[64] = {0,};

    doc.GetDocument()->createElement(L"op", &pElemOp);
    pElemOp->setAttribute(L"ver", (_variant_t)L"0.1");
    pElemOp->setAttribute(L"ip", (_variant_t)pConfigData->strAddr);
    swprintf_s(wbuf, L"%u", pConfigData->dwPort);
    pElemOp->setAttribute(L"port", (_variant_t)wbuf);

    doc.GetDocument()->createElement(L"BUGTRAP", &pElemBUGTrap);
    pElemBUGTrap->setAttribute(L"ip", (_variant_t)pConfigData->strBugTrapServerIP);
    swprintf_s(wbuf, L"%u", pConfigData->dwBugTrapServerPort);
    pElemBUGTrap->setAttribute(L"port", (_variant_t)wbuf);
    pElemBUGTrap->setAttribute(L"folder", (_variant_t)pConfigData->strBugTrapFolder);

    doc.GetDocument()->createElement(L"localize", &pElemLocalize);
    pElemLocalize->setAttribute(L"folder", (_variant_t)pConfigData->strLocalize);

    pElemRoot->appendChild(pElemOp, NULL);
    pElemRoot->appendChild(pElemBUGTrap, NULL);
    pElemRoot->appendChild(pElemLocalize, NULL);

    doc.SetIndent(L"indent.xsl");
    HRESULT hr = doc.GetDocument()->save((_variant_t)szFileName);
    if(hr != S_OK)
        return FALSE;

    return TRUE;
}
