/******************************************************************************************************************************
 *
 * ntlworldcommon_c
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : ntl world structure collection
 *
 ******************************************************************************************************************************/


/******************************************************************************************************************************
* headers
******************************************************************************************************************************/
#include "ntlworldcommon.h"


/******************************************************************************************************************************
* global variables
******************************************************************************************************************************/
RwInt32 NtlWorldGlobalOffset = -1;
RwInt32 NtlWorldLocalOffset = -1;
RwInt32 NtlWorldSectorLocalOffset = -1;
RwTexCoords *NtlCommonTexCoord = NULL;
RxPipeline *D3D9NtlWorldSectorAtomicPipeline = NULL;

sNtlWorldParam NtlWorldParam;