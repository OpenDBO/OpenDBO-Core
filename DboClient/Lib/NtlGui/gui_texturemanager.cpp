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
	// every wrapper releases its one RW reference once, other RW owners keep theirs
	std::map<CTexture*, sTEXTURE_OWNER>::iterator it;
	for(it = m_mapOwner.begin(); it != m_mapOwner.end(); ++it)
	{
		CTexture* pTexture = (*it).first;

		pTexture->Unload();
		NTL_DELETE( pTexture );
	}

	m_mapOwner.clear();
	m_mapTexture.clear();
}


CTexture* CTextureManager::CreateTexture(const char *pFileName)
{
	/*
		2007. 2. 15 by avooo'comment

		Renderware recognizes filenames as separate files in case-sensitive manner.
		The texture manager's key values are all lowercase and in the actual loading section
		The input value is used as it is.
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

	// All key values in lower case
	strlwr(acBuffer);
	
	RwImageSetPath(GUI_TEXTURE_PATH);

	// the full lowercase name is the key (cut to the RW name length two files could share a wrapper)
	std::string strKey = acBuffer;

	std::map<std::string, CTexture*>::iterator itTexture = m_mapTexture.find(strKey);
	if(itTexture != m_mapTexture.end())
	{
		// cache hit: one more gui owner of the same wrapper, no second RW load
		CTexture* pTexture = (*itTexture).second;
		m_mapOwner[pTexture].nRefCount++;
		return pTexture;
	}

	CTexture *pTexture = NTL_NEW CTexture;
	if(!pTexture->Load(pFileName))
	{
		NTL_DELETE( pTexture );
		return NULL;
	}

	m_mapTexture[strKey] = pTexture;

	sTEXTURE_OWNER& sOwner = m_mapOwner[pTexture];
	sOwner.strKey		= strKey;
	sOwner.nRefCount	= 1;

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
	{
		CNtlLoadingController::GetInstance()->Unlock();
		return NULL;
	}

	RwTexture *pRwTexture = RwTextureCreate(pRaster);
	if(pRwTexture == NULL)
	{
		RwRasterDestroy(pRaster);
		CNtlLoadingController::GetInstance()->Unlock();
		return NULL;
	}

	CNtlLoadingController::GetInstance()->Unlock();

	char chBuff[rwTEXTUREBASENAMELENGTH];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	sprintf_s(chBuff, rwTEXTUREBASENAMELENGTH, "%d", m_nImtexNum++);
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

	sTEXTURE_OWNER& sOwner = m_mapOwner[pTexture];
	sOwner.strKey		= strName;
	sOwner.nRefCount	= 1;

	return pTexture;
}

VOID CTextureManager::DeleteTexture(CTexture *pTexture)
{
	if(!pTexture)
		return;

	std::map<CTexture*, sTEXTURE_OWNER>::iterator itOwner = m_mapOwner.find(pTexture);
	if(itOwner == m_mapOwner.end())
	{
		// not a wrapper of this manager (or already freed), releasing it again would corrupt the heap
		DBO_WARNING_MESSAGE("CTextureManager::DeleteTexture - unknown texture " << (void*)pTexture);
		return;
	}

	sTEXTURE_OWNER& sOwner = (*itOwner).second;
	if(--sOwner.nRefCount > 0)
		return;

	// last gui owner: the wrapper's one RW reference goes, the entry is removed by wrapper identity
	std::map<std::string, CTexture*>::iterator itTexture = m_mapTexture.find(sOwner.strKey);
	if(itTexture != m_mapTexture.end() && (*itTexture).second == pTexture)
		m_mapTexture.erase(itTexture);

	m_mapOwner.erase(itOwner);

	pTexture->Unload();
	NTL_DELETE( pTexture );
}

END_GUI