#ifndef __GUI_TEXTUREMANAGER_H__
#define __GUI_TEXTUREMANAGER_H__

#include "gui_define.h"

START_GUI

class CTexture;

class CTextureManager
{
public:

	int GetSortSize(int nSize);

public:

	CTextureManager();
	~CTextureManager();

	bool Initialize(VOID);
	VOID Terminate(VOID);
	CTexture* CreateTexture(const char *pFileName);
	CTexture* CreateTexture(int nWidth, int nHeight);
	VOID DeleteTexture(CTexture *pTexture);

private:

	// a wrapper holds one RenderWare reference, the gui owners are counted here (not with the RW refCount,
	// a reload on a cache hit could swap the RW texture and the last owner freed the wrapper too early)
	struct sTEXTURE_OWNER
	{
		std::string		strKey;
		int				nRefCount;
	};

	int m_nImtexNum;
	std::map<std::string, CTexture*> m_mapTexture;
	std::map<CTexture*, sTEXTURE_OWNER> m_mapOwner;
};

END_GUI

extern gui::CTextureManager g_TextureMgr;

#endif
