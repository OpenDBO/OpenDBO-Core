#include "gui_precomp.h"
#include "gui_texturemanager.h"
#include "gui_texture.h"
#include "NtlLoadingController.h"
#include "gui_define.h"

#define GUI_TEXTURE_PATH	".\\texture\\gui\\"
	
gui::CTextureManager g_TextureMgr;

START_GUI

CTextureManager::CTextureManager()
{
	m_nImtexNum = 0;
}

CTextureManager::~CTextureManager()
{
	Terminate();
}

int CTextureManager::GetSortSize(int nSize)
{
	assert( nSize > 0 && "[CTextureManager::GetSortSize] nSize is 0" );

	--nSize;
	int nNum = 0;
	while(nSize)
	{
		nSize = nSize >> 1;
		nNum++;
	}

	return (1 << nNum);
}

bool CTextureManager::Initialize(VOID)
{
	return true;
}

VOID CTextureManager::Terminate(VOID)
{
	std::map<std::string, CTexture*>::iterator the;
	for(the = m_mapTexture.begin(); the != m_mapTexture.end(); the++)
	{
		while((*the).second && (*the).second->IsValid())
		{
			// std::string strName = (*the).first;
			(*the).second->Unload(); 
		}
	}

	m_mapTexture.clear();
}


CTexture* CTextureManager::CreateTexture(const char *pFileName)
{
	/*
	2007. 2. 15 by avooo'comment

	Renderware는 파일이름의 대소문자를 구분하여 별도의 파일로 인식한다.
	텍스처 매니저의 키값은 모두 소문자로 만들고 실제 로딩부분에서는
	입력된 값을 그대로 쓴다.
	*/

	static char acBuffer[256];

	if(!pFileName)
	{
		return NULL;
	}

	RwInt32 iLength = (RwInt32)strlen(pFileName);
	if(iLength == 0)
	{
		return NULL;
	}

	strcpy(acBuffer, pFileName);

	// 모든 키값을 소문자로
	strlwr(acBuffer);

	RwImageSetPath(GUI_TEXTURE_PATH);
	CTexture *pTexture = NULL;
	pTexture = m_mapTexture[acBuffer];
	if(pTexture == NULL)
	{
		pTexture = NTL_NEW CTexture;
		if(!pTexture->Load(pFileName))
		{
			NTL_DELETE( pTexture );
			return NULL;
		}

		m_mapTexture[acBuffer] = pTexture;
	}
	else
	{
		pTexture->Load(pFileName);
	}

	return pTexture;
}

CTexture* CTextureManager::CreateTexture(int nWidth, int nHeight)
{
	nWidth = GetSortSize(nWidth);
	nHeight = GetSortSize(nHeight);

	CNtlLoadingController::GetInstance()->Lock();

#ifdef FONT_USE_32BIT
	RwRaster *pRaster = RwRasterCreate(nWidth, nHeight, 16, rwRASTERTYPETEXTURE | rwRASTERFORMAT8888);
#else
	RwRaster *pRaster = RwRasterCreate(nWidth, nHeight, 16, rwRASTERTYPETEXTURE | rwRASTERFORMAT4444);
#endif
	if(pRaster == NULL)
		return NULL;

	RwTexture *pRwTexture = RwTextureCreate(pRaster); 
	if(pRwTexture == NULL)
	{
		RwRasterDestroy(pRaster);
		return NULL;
	}

	CNtlLoadingController::GetInstance()->Unlock();

	char chBuff[32];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuff, 32, "%d", m_nImtexNum++);
#else
	sprintf(chBuff, "%d", m_nImtexNum++);
#endif
	std::string strName = "imtex"; 
	strName += chBuff;

	RwTextureSetName(pRwTexture, strName.c_str()); 

	CTexture *pTexture = NTL_NEW CTexture;
	if(!pTexture->Load(pRwTexture))
	{
		NTL_DELETE( pTexture );

		RwTextureDestroy(pRwTexture);
		return NULL;
	}

	m_mapTexture[strName] = pTexture;

	return pTexture;
}

VOID CTextureManager::DeleteTexture(CTexture *pTexture)
{
	static char buf[256];

	if(pTexture)
	{		
		char* pcName = (char*)pTexture->GetName();		
		strcpy( buf, pcName );
		strlwr( buf );

		pTexture->Unload();

		if(!pTexture->IsValid())
		{
			std::map<std::string, CTexture*>::iterator the = m_mapTexture.find( buf );
			if(the != m_mapTexture.end())
			{
				m_mapTexture.erase(the); 
			}

			NTL_DELETE( pTexture );
		}
	}
}

END_GUI