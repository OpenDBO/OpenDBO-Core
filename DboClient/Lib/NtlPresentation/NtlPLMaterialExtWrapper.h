/*****************************************************************************
 *
 * File			: NtlMatExt
 * Author		: HongHoDong
 * Copyright	: (占쏙옙)NTL
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

void		NtlMatExtSetFlag(RpMaterial *pMaterial, RwUInt32 bFlag);				/**<Set Material 占쌈쇽옙 Flag Ntl_PluginToon占쏙옙 NtlMaterialExt.h 占쏙옙占쏙옙 */
RwUInt32	NtlMatExtGetFlag(RpMaterial *pMaterial);								/**<Get Material 占쌈쇽옙 Flag Ntl_PluginToon占쏙옙 NtlMaterialExt.h 占쏙옙占쏙옙 */

void		NtlMatExtSetPipeline(RpAtomic *pAtomic);
void		NtlMatExtSetToonSkinPipelineRenderCB(RpAtomic *pAtomic);

#endif
