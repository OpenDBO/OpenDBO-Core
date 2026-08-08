#include "precomp_ntlcore.h"
#include "NtlPackImage.h"
#include "NtlDebug.h"
#include <cstdio>

#ifdef _DEBUG
#include <crtdbg.h>
#define HEAP_CHECK() do { if (!_CrtCheckMemory()) { OutputDebugStringA("HEAP CORRUPTION DETECTED in NtlPackImage.cpp\n"); __debugbreak(); } } while(0)
#else
#define HEAP_CHECK() ((void)0)
#endif

RtFileOpenFunc pOldFileOpenFunc = NULL;
RtFileReadFunc pOldFileReadFunc = NULL;
RtFileCloseFunc pOldFileCloseFunc = NULL;

extern "C" RwImage* RtPNGImageReadStream(RwStream *stream);

RtFileSystemError PackFileOpen( RtFileSystem  *fs, RtFile *file, const RwChar *filename, RwUInt32 flags)
{
	SNtlPackImage	*pPackImage = (SNtlPackImage *)filename;
	
	RtFileSystemError eFileSystemError = pOldFileOpenFunc(fs, file, pPackImage->strPackFileName.c_str(), flags);
	if(eFileSystemError != RTFS_ERROR_NOERROR)
		return eFileSystemError;

	fs->fsFileFunc.setPosition(file, (RwInt32)pPackImage->uiPackOffset, RTFILE_POS_CURRENT);
	
	return RTFS_ERROR_NOERROR;
}

RwUInt32 PackFileRead( RtFile *file, void *pBuffer, RwUInt32 nBytes)
{
	return pOldFileReadFunc(file, pBuffer, nBytes);
}

void PackFileClose(RtFile *file)
{
    pOldFileCloseFunc(file);
}

void RegisterNewCallBack()
{
	RtFileSystem *pCurFileSystem = RtFSManagerGetDefaultFileSystem();

	pOldFileOpenFunc	= pCurFileSystem->fsFileFunc.open;
	pOldFileReadFunc	= pCurFileSystem->fsFileFunc.read;
	pOldFileCloseFunc	= pCurFileSystem->fsFileFunc.close;

	pCurFileSystem->fsFileFunc.open		= PackFileOpen;
	pCurFileSystem->fsFileFunc.read		= PackFileRead;
	pCurFileSystem->fsFileFunc.close	= PackFileClose;
}

void RegisterOldCallBack()
{
	RtFileSystem *pCurFileSystem = RtFSManagerGetDefaultFileSystem();

	pCurFileSystem->fsFileFunc.open		= pOldFileOpenFunc;
	pCurFileSystem->fsFileFunc.read		= pOldFileReadFunc;
	pCurFileSystem->fsFileFunc.close	= pOldFileCloseFunc;
}

RwTexture *Ntl_D3D9DDSTextureRead(SNtlPackImage *pPackImage)
{
	RwRaster *pRaster = NULL;
	RwTexture *pTexture = NULL;

	__try
	{
		FILE* fp = NULL;
		if (fopen_s(&fp, pPackImage->strPackFileName.c_str(), "rb") == 0)
		{
			fseek(fp, pPackImage->uiPackOffset, SEEK_SET);
			RwUInt8* buf = (RwUInt8*)RwMalloc(pPackImage->uiPackSize, rwMEMHINTDUR_GLOBAL);
			if (buf)
			{
				size_t readBytes = fread(buf, 1, pPackImage->uiPackSize, fp);
				fclose(fp);
				if (readBytes == pPackImage->uiPackSize)
				{
					RwMemory mem;
					mem.start = buf;
					mem.length = pPackImage->uiPackSize;
					RwStream *stream = RwStreamOpen(rwSTREAMMEMORY, rwSTREAMREAD, &mem);
					if (stream)
					{
						pRaster = RwD3D9RasterStreamReadDDS(stream);
						RwStreamClose(stream, NULL);
					}
				}
				RwFree(buf);
			}
			else
				fclose(fp);
		}

		if (pRaster != NULL)
		{
			pTexture = RwTextureCreate(pRaster);
			if (pTexture)
			{
				if (RwRasterGetFormat(pRaster) & rwRASTERFORMATMIPMAP)
					RwTextureSetFilterMode(pTexture, rwFILTERLINEARMIPLINEAR);
				else
					RwTextureSetFilterMode(pTexture, rwFILTERLINEAR);
				RwTextureSetName(pTexture, pPackImage->pName);
			}
			else
			{
				RwRasterDestroy(pRaster);
				pRaster = NULL;
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DBO_WARNING_MESSAGE("Ntl_D3D9DDSTextureRead: exception 0x" << std::hex << GetExceptionCode() << " for " << pPackImage->pName);
		pTexture = NULL;
	}

	return pTexture;
}

RwTexture *Ntl_PNGTextureRead(SNtlPackImage *pPackImage)
{
	RegisterNewCallBack();

	RwImage *pImage = NULL;
	RwRaster *pRaster = NULL;
	RwTexture *pTexture = NULL;

	RwInt32 nRasterWidth = 0;
	RwInt32 nRasterHeight = 0;
	RwInt32 nRasterDepth = 0;
	RwInt32 nRasterFlags = 0;

	__try
	{
		// Use direct fopen/fread + memory stream for x64
		{
			FILE* fp = NULL;
			if (fopen_s(&fp, pPackImage->strPackFileName.c_str(), "rb") == 0)
			{
				fseek(fp, pPackImage->uiPackOffset, SEEK_SET);
				RwUInt8* buf = (RwUInt8*)RwMalloc(pPackImage->uiPackSize, rwMEMHINTDUR_GLOBAL);
				if (buf)
				{
					size_t readBytes = fread(buf, 1, pPackImage->uiPackSize, fp);
					fclose(fp);
					if (readBytes == pPackImage->uiPackSize)
					{
						RwMemory mem;
						mem.start = buf;
						mem.length = pPackImage->uiPackSize;
						RwStream *stream = RwStreamOpen(rwSTREAMMEMORY, rwSTREAMREAD, &mem);
						if (stream)
						{
							pImage = RtPNGImageReadStream(stream);
							RwStreamClose(stream, NULL);
						}
					}
					RwFree(buf);
				}
				else
					fclose(fp);
			}
		}

		if (pImage == NULL)
		{
			DBO_WARNING_MESSAGE("Could not read texture: " << pPackImage->pName);
		}
		else if (!RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &nRasterWidth, &nRasterHeight, &nRasterDepth, &nRasterFlags))
		{
			DBO_WARNING_MESSAGE("Could not find raster format texture: " << pPackImage->pName);
			RwImageDestroy(pImage);
			pImage = NULL;
		}
		else
		{
			char dbg[256];
			sprintf_s(dbg, "PNG: Creating raster %dx%d depth=%d flags=0x%08x name=%s\n",
				nRasterWidth, nRasterHeight, nRasterDepth, nRasterFlags,
				pPackImage->pName ? pPackImage->pName : "(null)");
			OutputDebugStringA(dbg);

			pRaster = RwRasterCreate(nRasterWidth, nRasterHeight, nRasterDepth, nRasterFlags);
			HEAP_CHECK();
			if (pRaster == NULL)
			{
				DBO_WARNING_MESSAGE("Could not create raster texture: " << pPackImage->pName);
				RwImageDestroy(pImage);
				pImage = NULL;
			}
			else
			{
				RwImageGammaCorrect(pImage);

				if (!RwRasterSetFromImage(pRaster, pImage))
				{
					DBO_WARNING_MESSAGE("Could not set raster from image texture: " << pPackImage->pName);
					RwRasterDestroy(pRaster);
					pRaster = NULL;
					RwImageDestroy(pImage);
					pImage = NULL;
				}
				else
				{
					RwImageDestroy(pImage);
					pImage = NULL;

					pTexture = RwTextureCreate(pRaster);
					HEAP_CHECK();
					if (!pTexture)
					{
						DBO_WARNING_MESSAGE("Could not create texture: " << pPackImage->pName);
						RwRasterDestroy(pRaster);
						pRaster = NULL;
					}
					else
					{
						RwTextureSetName(pTexture, pPackImage->pName);
						if (pPackImage->pMaskName)
							RwTextureSetMaskName(pTexture, pPackImage->pMaskName);
						else
							RwTextureSetMaskName(pTexture, "");
					}
				}
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DBO_WARNING_MESSAGE("Ntl_PNGTextureRead: exception 0x" << std::hex << GetExceptionCode() << " for " << pPackImage->pName);
		pTexture = NULL;
	}

	RegisterOldCallBack();
	return pTexture;
}

RwTexture *Ntl_BMPTextureRead(SNtlPackImage *pPackImage)
{
	RegisterNewCallBack();

	RwImage *pImage = NULL;
	RwRaster *pRaster = NULL;
	RwTexture *pTexture = NULL;
	
	RwInt32 nRasterWidth = 0;
	RwInt32 nRasterHeight = 0;
	RwInt32 nRasterDepth = 0;
	RwInt32 nRasterFlags = 0;

	__try
	{
		pImage = RtBMPImageRead((const RwChar *)pPackImage);
		if(pImage == NULL)
		{
		}
		else if (!RwImageFindRasterFormat(pImage, rwRASTERTYPETEXTURE, &nRasterWidth, &nRasterHeight, &nRasterDepth, &nRasterFlags))
		{
			RwImageDestroy(pImage);
			pImage = NULL;
		}
		else
		{
			pRaster = RwRasterCreate(nRasterWidth, nRasterHeight, nRasterDepth, nRasterFlags);
			if(pRaster == NULL)
			{
				RwImageDestroy(pImage);
				pImage = NULL;
			}
			else
			{
				RwImageGammaCorrect(pImage);
				if (!RwRasterSetFromImage(pRaster, pImage))
				{
					RwRasterDestroy(pRaster);
					pRaster = NULL;
					RwImageDestroy(pImage);
					pImage = NULL;
				}
				else
				{
					RwImageDestroy(pImage);
					pImage = NULL;
					pTexture = RwTextureCreate(pRaster);
					if (!pTexture)
					{
						RwRasterDestroy(pRaster);
						pRaster = NULL;
					}
					else
					{
						RwTextureSetName(pTexture, pPackImage->pName);
						if (pPackImage->pMaskName)
							RwTextureSetMaskName(pTexture, pPackImage->pMaskName);
						else
							RwTextureSetMaskName(pTexture, "");
					}
				}
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DBO_WARNING_MESSAGE("Ntl_BMPTextureRead: exception 0x" << std::hex << GetExceptionCode() << " for " << pPackImage->pName);
		pTexture = NULL;
	}

	RegisterOldCallBack();
	return pTexture;
}

