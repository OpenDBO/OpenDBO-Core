/*****************************************************************************
* File			: DboPackethandler_GUHoipoiMix.cpp
* Author		: HaeSung, Cho
* Copyright		: (ÁÖ)NTL
* Date			: 2009. 1. 8
* Abstract		: È£ÀÌÆ÷ÀÌ ¹Í½º Ä¸½¶ °ü·Ã ÇÚµé·¯
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// shared
#include "ItemTable.h"
#include "TextAllTable.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"

// Client
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"

void PacketHandler_GUHoipoiMixItemMachineDelRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_HOIPOIMIX_ITEM_MACHINE_DEL_RES );

	sGU_HOIPOIMIX_ITEM_MACHINE_DEL_RES* pResult = (sGU_HOIPOIMIX_ITEM_MACHINE_DEL_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::HoipoiMixItemMachineDel();
}

void PacketHandler_GUHoipoiMixRecipeRegNfy( void* pPacket )
{
	sGU_HOIPOIMIX_RECIPE_REG_NFY* pNotify = (sGU_HOIPOIMIX_RECIPE_REG_NFY*)pPacket;

	GetAlarmManager()->FormattedAlarmMessage( DST_HOIPOIMIX_REGISTER_RECIPE, FALSE, NULL, Logic_GetHoipoiMixRecipeName( pNotify->recipeTblidx ) );
	CNtlSLEventGenerator::HoipoiMixRecipeReg( pNotify->recipeTblidx );
}

void PacketHandler_GUHoipoiMixItemCheckRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_HOIPOIMIX_ITEM_CHECK_RES );

	sGU_HOIPOIMIX_ITEM_CHECK_RES* pResult = (sGU_HOIPOIMIX_ITEM_CHECK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::HoipoiMixItemCheck( pResult->objHandle, pResult->recipeTblidx );
}

void PacketHandler_GUHoipoiMixItemMakeRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_HOIPOIMIX_ITEM_MAKE_RES );

	sGU_HOIPOIMIX_ITEM_MAKE_RES* pResult = (sGU_HOIPOIMIX_ITEM_MAKE_RES*)pPacket;

	/*if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}*/

	CDboEventGenerator::HoipoiMixItemMake( pResult->wResultCode, pResult->bySuccessResult, pResult->objHandle, pResult->recipeTblidx,
		pResult->itemHandle );

	CNtlSLEventGenerator::TSHoipoiMix();
}

void PacketHandler_GUHoipoiMixItemMakeEpRes( void* pPacket )
{
	/*API_GetSLPacketLockManager()->Unlock( GU_HOIPOIMIX_ITEM_MAKE_EP_RES );*/

	sGU_HOIPOIMIX_ITEM_MAKE_EP_RES* pResult = (sGU_HOIPOIMIX_ITEM_MAKE_EP_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::HoipoiMixItemMakeEp();
}

void PacketHandelr_GUHoipoiMixItemRecipeInfo( void* pPacket )
{
	sGU_HOIPOIMIX_ITEM_RECIPE_INFO* pInfo = (sGU_HOIPOIMIX_ITEM_RECIPE_INFO*)pPacket;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( pSobAvatar )
		CNtlSLEventGenerator::HoipoiMixItemRecipeInfo( pInfo->byCount, reinterpret_cast<void*>(pInfo->asRecipeData) );
	else
	{
		SAvatarRecipeInfo* pRecipeInfo = GetNtlSLGlobal()->GetAvatarRecipeInfo();
		memcpy( pRecipeInfo->asRecipeData, pInfo->asRecipeData, sizeof(sRECIPE_DATA) * pInfo->byCount );
		pRecipeInfo->byCount = pInfo->byCount;
	}
}

void PacketHandler_GUHoipoiMixItemMakeExpNfy( void* pPacket )
{
	sGU_HOIPOIMIX_ITEM_MAKE_EXP_NFY* pNotify = (sGU_HOIPOIMIX_ITEM_MAKE_EXP_NFY*)pPacket;

	CNtlSLEventGenerator::HoipoiMixItemMakeExp( pNotify->dwCurExp, pNotify->byCurLevel );
}

void PacketHandler_GUHoipoiMixJobSetRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_HOIPOIMIX_JOB_SET_RES );

	sGU_HOIPOIMIX_JOB_SET_RES* pResult = (sGU_HOIPOIMIX_JOB_SET_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	GetAlarmManager()->FormattedAlarmMessage( DST_HOIPOIMIX_REGISTER_SKILL, FALSE, NULL, Logic_GetHoipoiMixSkillName( pResult->byRecipeType ) );
	CNtlSLEventGenerator::HoipoiMixJobSetRes( pResult->hNpchandle, pResult->byRecipeType );
}

void PacketHandler_GUHoipoiMixJobResetRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock( GU_HOIPOIMIX_JOB_RESET_RES );

	sGU_HOIPOIMIX_JOB_RESET_RES* pResult = (sGU_HOIPOIMIX_JOB_RESET_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	GetAlarmManager()->FormattedAlarmMessage( DST_HOIPOIMIX_SKILL_RESET_RESULT, FALSE, NULL, Logic_GetHoipoiMixSkillName( pResult->byRecipeType ) );
	CNtlSLEventGenerator::HoipoiMixJobResetRes( pResult->hNpchandle, pResult->byRecipeType );
}