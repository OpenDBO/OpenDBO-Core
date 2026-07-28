/*****************************************************************************
 *
 * File			: NtlMatExt
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 27	
 * Abstract		: NTl Mat Extension
 *****************************************************************************
 * Desc         : Material Extension
 *
 *****************************************************************************/
#ifndef _NTL_MATERIAL_EXT_CORE_H_
#define _NTL_MATERIAL_EXT_CORE_H_

#include <rwcore.h>
#include <rpworld.h>
#include "../Ntl_Plugin_Toon/toon/NtlMaterialExt.h"

RwBool		NtlMatExtPluginAttach(void);

RwBool		NtlMatExtSetMultiTexture(RpMaterial *pMaterial, RwTexture *pTexture);	/**<Set MultiTexture ���� 1�常 ���� */
RwTexture  *NtlMatExtGetMultiTexture(RpMaterial *pMaterial);						/**<Get MultiTexture ���� 1�常 ���� */

void		NtlMatExtSetFlag(RpMaterial *pMaterial, RwUInt32 bFlag);				/**<Set Material �Ӽ� Flag Ntl_PluginToon�� NtlMaterialExt.h ���� */
RwUInt32	NtlMatExtGetFlag(RpMaterial *pMaterial);								/**<Get Material �Ӽ� Flag Ntl_PluginToon�� NtlMaterialExt.h ���� */

void		NtlMatExtSetPipeline(RpAtomic *pAtomic);
void		NtlMatExtSetToonSkinPipelineRenderCB(RpAtomic *pAtomic);

#endif