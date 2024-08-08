#include "precomp_dboclient.h"
#include "SayFilter.h"

// core
#include "NtlDebug.h"
#include "NtlClientService.h"

// shared
#include "TableContainer.h"
#include "ChattingFilterTable.h"
#include "TextAllTable.h"

// simulation
#include "NtlSLApi.h"
#include "NtlService.h"


CFilterManager::CFilterManager() 
: m_bIsEmpty( TRUE )
{
	InitOnce();
}

CFilterManager::~CFilterManager()
{
	Destroy();
}

void CFilterManager::InitOnce() 
{
	m_defaultReplace = L"--";

	m_pFilterWord = NTL_NEW CWFILTERWORD;
}

void CFilterManager::Destroy() 
{
	if( m_pFilterWord )
		NTL_DELETE(m_pFilterWord);
}

void CFilterManager::Create(void) 
{
	// TODO : 테이블에서 데이터를 읽어들여와 추가를 해줌.
	// 필터링 단어 테이블 추가
	CChattingFilterTable* pFilterTable = API_GetTableContainer()->GetChattingFilterTable();
	
	// 테이블이 없어도 게임은 진행이 되게 해야 한다.
	DBO_ASSERT( pFilterTable, "CFilterManager: filter table pointer is NULL");
	if( NULL == pFilterTable )
		return;

	sCHAT_FILTER_TBLDAT* pFilter = NULL;

	for(CChattingFilterTable::TABLEIT it = pFilterTable->Begin(); it != pFilterTable->End(); it++)
	{
		pFilter = (sCHAT_FILTER_TBLDAT*)it->second;
		AddFilter( pFilter->tblidx, pFilter->wszSlangText, pFilter->filteringTextIndex );
	}

	// 단어가 1개 이상 등록되어 있으면 테이블은 비어 있는것이 아니다라고 판단한다.
	if( m_pFilterWord->GetNumFilter() > 0 )
		m_bIsEmpty = FALSE;
}

void CFilterManager::AddFilter(int nIndex, wchar_t* pText, int nLink)
{
	m_pFilterWord->AddText( nIndex, pText, lstrlenW( pText ), nLink );
}

/**
* \brief UNICODE 문자열 필터링 함수
*
* UNICODE 문자열의 욕설을 치환하여 리턴한다.
*
* \param pText (std::wstring&)필터링할 wstring
* \return (std::wstring)필터링된 wstring
*/
std::wstring CFilterManager::Filtering(std::wstring& pText)
{
	// 테이블이 로드 안되어 있다면 그냥 리턴해버린다.
	if( m_bIsEmpty )
	{
		DBO_FAIL( "CFilterManager:: Filter word is empty.");
		return pText;
	}

	// 원본 문자열을 복사해서 필터링한 다음 필터링된 문자열을 리턴한다.
	std::wstring		strText = pText;

	// vecResult 구조체에 필터링될 단어들의 위치와 정보값을 채워넣는다.
	std::vector<CWFILTERWORD::SResult> vecResult;
	m_pFilterWord->FindText(strText.c_str(), (int)strText.length(), vecResult);

	// 테이블을 미리 가지고 있는다.
	CTextTable* pReplaceTable = API_GetTableContainer()->GetTextAllTable()->GetFilteringTable();
	
	// 테이블을 로드하지 못했다면 단어를 바꾸지 않고 리턴한다.
	DBO_ASSERT( pReplaceTable , "CFilterManager: Replace table pointer is NULL");
	if( NULL == pReplaceTable )
		return strText;
		
	// vecResult에 들어온 결과에 역순으로 치환을 실행한다.
	// 이것은 문자열의 앞에서 치환했을 경우 문자열의 글자 길이가 바뀌는 경우를 대비한다.
	for (int i = (int)vecResult.size() - 1; i >= 0; --i)
	{
		CWFILTERWORD::SResult& resultRef = vecResult.at(i);

		// 필터링될 단어를 치환한다. strText.erase( StartPos, Offset );
		strText.erase(resultRef.uPosStart, resultRef.uPosEnd-resultRef.uPosStart+1);
		

		// 만약 치환될 단어의 인덱스가 없다면 기본으로 지정되어 있는 단어로 변경해준다.
		sTEXT_TBLDAT* pReplace = (sTEXT_TBLDAT*)pReplaceTable->FindData(resultRef.nLink);

		if( NULL == pReplace )
			strText.insert(resultRef.uPosStart, m_defaultReplace.c_str() );
		else
			strText.insert(resultRef.uPosStart, pReplace->wstrText.c_str() );
		
	}

	return strText;
}

/**
* \brief MULTIBYTE 문자열 필터링 함수
*
* MULTIBYTE 문자열을 UNICODE로 변환한 후 std::wstring CWSayFilterManager::Filtering( std::wstring& pText );
* 을 호출하여 필터링 하고 다시 MULTIBYTE로 변환한 후 리턴한다. (ATL 매크로 사용)
*
* \param pText (std::string&)필터링할 string
* \return (std::string)필터링된 string
*/
std::string CFilterManager::Filtering(std::string& pText)
{
	// MULTIBYTE를 UNICODE로 변환해서 Filtering을 수행한 후
	// 다시 MULTIBYTE로 변환해서 리턴한다.
	// WinAPI로 바꿔야함.
	
	WCHAR awcBuffer[64] = L" ";
	::MultiByteToWideChar( GetACP(), 0, pText.c_str(), -1, awcBuffer, 64 );
	
	std::wstring wstrResult = Filtering( awcBuffer );
	
	CHAR acBuffer[64]= " ";
	::WideCharToMultiByte( GetACP(), 0, wstrResult.c_str(), -1, acBuffer, 64, NULL, NULL );
	std::string strResult = acBuffer;

	return strResult;
}

/**
* \brief UNICODE 문자열 필터링 함수
*
* \param pText (wchar_t*)필터링할 UNICODE 문자열 포인터
* \return (std::wstring)필터링된 wstring
*/
std::wstring CFilterManager::Filtering(const wchar_t* pText)
{
	std::wstring strText = pText;

	return Filtering( strText );
}

/**
* \brief UNICODE 문자열 욕설 체크 함수
*
* \param pText (std::wstring)체크할 문자열
* \return (BOOL)욕설이라면 TRUE, 아니라면 FALSE를 리턴
*/
RwBool CFilterManager::IsSlang(std::wstring& pText )
{
	// 테이블이 로드 안되어 있다면 그냥 리턴해버린다.
	if( m_bIsEmpty )
	{
		DBO_FAIL("CFilterManager: Filterword is empty.");
		return FALSE;
	}

	std::wstring strText = pText;

	std::vector<CWFILTERWORD::SResult> vecResult;
	m_pFilterWord->FindText(strText.c_str(), (int)strText.length(), vecResult);

	// 필터 단어가 하나라도 검색되면 이 문자열은 욕설이 된다.
	if( vecResult.size() > 0 )
		return TRUE;

	return FALSE;
}

RwBool CFilterManager::IsSlang(std::string& pText)
{
	WCHAR awcBuffer[64] = L" ";
	::MultiByteToWideChar( GetACP(), 0, pText.c_str(), -1, awcBuffer, 64 );


	std::wstring buf = awcBuffer;

	return IsSlang( buf );
}

RwBool CFilterManager::IsSlang( const wchar_t* pText )
{
	std::wstring buf = pText;

	return IsSlang( buf );
}

RwBool CFilterManager::IsAnyInvalidText( const WCHAR* pText )
{
	std::wstring buf = pText;

	if( IsSlang( buf ) )
		return TRUE;

	eDBO_SERVICE_PUBLISHER eService = GetServiceInterface()->GetPublisherID();
	if( Dbo_AnyInvalidCharacterInName( (BYTE)eService, const_cast<WCHAR*>(pText), wcslen( pText ) ) )
		return TRUE;

	return FALSE;
}