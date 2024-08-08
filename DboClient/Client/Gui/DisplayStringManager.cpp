#include "precomp_dboclient.h"
#include "DisplayStringManager.h"

CDisplayStringManager* CDisplayStringManager::GetInstance(void)
{
	static CDisplayStringManager Instance;
	return &Instance;
}

void CDisplayStringManager::Resize(RwUInt32 eType, RwInt32 iSize)
{
	if(eType == CLIENT_ONLY_MSG)
	{
		m_vecClientString.clear();
		m_vecClientString.resize((size_t)iSize); 
	}
	else
	{
		m_vecServerString.clear();
		m_vecServerString.resize((size_t)iSize);  
	}
}

void CDisplayStringManager::AddString(WCHAR **pBuffer, RwInt32& iBuffLen, RwUInt32 eType, RwInt32 iIdx, const std::string& str)
{
	if((RwInt32)str.size() >= iBuffLen)
	{
		NTL_DELETE(*pBuffer);
		iBuffLen = (RwInt32)str.size()+1;
		*pBuffer = NTL_NEW WCHAR [iBuffLen];
	}

	::MultiByteToWideChar(GetACP(), 0, str.c_str(), -1, *pBuffer, iBuffLen);

	if(eType == CLIENT_ONLY_MSG)
		m_vecClientString[iIdx] = *pBuffer;
	else
		m_vecServerString[iIdx] = *pBuffer;
}

void CDisplayStringManager::AddString( WCHAR **pBuffer, RwInt32& iBuffLen, RwUInt32 eType, RwInt32 iIdx, const std::wstring& wstr )
{
	if(eType == CLIENT_ONLY_MSG)
		m_vecClientString[iIdx] = wstr;
	else
		m_vecServerString[iIdx] = wstr;
}

const WCHAR* CDisplayStringManager::GetString(RwInt32 iIdx, RwUInt32 eType) const
{
	NTL_FUNCTION("CDisplayStringManager::GetString");

	if(eType == CLIENT_ONLY_MSG)
	{
		if((size_t)iIdx >= m_vecClientString.size())
		{
			NTL_ASSERTFAIL("CDisplayStringManager::GetString find not id (" << iIdx << ")" << "type" << (RwUInt32) eType);
			NTL_RETURN(NULL);
		}
		NTL_RETURN(m_vecClientString[iIdx].c_str()); 
	}
	else
	{
		if((size_t)iIdx >= m_vecServerString.size())
		{
			NTL_ASSERTFAIL("CDisplayStringManager::GetString find not id (" << iIdx << ")" << "type" << (RwUInt32) eType);
			NTL_RETURN(NULL);
		}
		NTL_RETURN(m_vecServerString[iIdx].c_str()); 
	}	
}

void CDisplayStringManager::InsertOptions(const CDisplayParser& Parser, RwUInt32 eType /*= CLIENT_ONLY_MSG*/)
{
	NTL_FUNCTION("CDisplayStringManager::InsertOptions");

	RwInt32 iBuffLen = 1024;
	WCHAR *pBuffer = NTL_NEW WCHAR[iBuffLen];
    
	const CDisplayParser::MapDefOptionW& DefOption = Parser.GetDefOptionsW();
	const CNtlTokenOptionsW& DataOption = Parser.GetDataOptionsW();
	
	CDisplayParser::MapDefOptionW::const_iterator const_defit;
	CNtlTokenOptionsW::Const_Iterator const_datait;

	RwInt32 iCount = 0;
	for(const_defit = DefOption.begin(); const_defit != DefOption.end(); const_defit++)
	{
		if(iCount < (*const_defit).second)
			iCount = (*const_defit).second;
	}

	Resize(eType, iCount+1);
	
	for(const_datait = DataOption.Begin(); const_datait != DataOption.End(); ++const_datait)
	{
		const_defit = DefOption.find((*const_datait).first);
		if(const_defit != DefOption.end())
		{
			AddString(&pBuffer, iBuffLen, eType, (*const_defit).second, (*const_datait).second);  
		}
	}

	NTL_ARRAY_DELETE(pBuffer);

	NTL_RETURNVOID();
}
