/*****************************************************************************
* File			: DboPackethandler_GUVehicle.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2009. 1. 6
* Abstract		: Vechicle 관련 패킷
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"



void PacketHandler_GUVehicleStartNfy(void *pPacket)
{
	// 탈 것에 타는 동작이 시작되거나 타고 있는 채로 탈 것이 등장한다
	sGU_VEHICLE_START_NFY* pResult = (sGU_VEHICLE_START_NFY*)pPacket;

	RwV3d vSpawnPos;
	vSpawnPos.x = pResult->vStartPosition.x;
	vSpawnPos.y = pResult->vStartPosition.y;
	vSpawnPos.z = pResult->vStartPosition.z;

	CNtlSLEventGenerator::SobVehicleStart(pResult->hDriverHandle,
										  vSpawnPos,
										  pResult->hVehicleItem,
										  pResult->idVehicleItemTblidx);
}

void PacketHandler_GUVehicleEngineStartRes(void *pPacket)
{
	// 엔진에 시동이 걸렸음을 자신에게 알린다
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_ENGINE_START_RES);

	sGU_VEHICLE_ENGINE_START_RES* pResult = (sGU_VEHICLE_ENGINE_START_RES*) pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}

	CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_START);
}

void PacketHandler_GUVehicleEngineStartNfy(void *pPacket)
{
	// 엔진에 시동이 걸렸음을 주변에 알린다
	sGU_VEHICLE_ENGINE_START_NFY* pResult = (sGU_VEHICLE_ENGINE_START_NFY*) pPacket;

	CNtlSLEventGenerator::SobVehicleEngine(pResult->hDriverHandle, VEHICLE_ENGINE_START);
}

void PacketHandler_GUVehicleEngineStopRes(void *pPacket)
{
	// 엔진이 멈추었음을 자신에게 알린다
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_ENGINE_STOP_RES);

	sGU_VEHICLE_ENGINE_STOP_RES* pResult = (sGU_VEHICLE_ENGINE_STOP_RES*) pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_FAIL_REQUEST_STOP);
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}
}

void PacketHandler_GUVehicleEngineStopNfy(void *pPacket)
{
	// 엔진이 멈추었음을 주변에 알린다
	sGU_VEHICLE_ENGINE_STOP_NFY* pResult = (sGU_VEHICLE_ENGINE_STOP_NFY*) pPacket;

	CNtlSLEventGenerator::SobVehicleEngine(pResult->hDriverHandle, VEHICLE_ENGINE_STOP);
}

void PacketHandler_GUVehicleFuelUpdateNfy(void *pPacket)
{
	// 연료 정보가 업데이트 되었다
	//sGU_VEHICLE_FUEL_UPDATE_NFY* pResult = (sGU_VEHICLE_FUEL_UPDATE_NFY*) pPacket;

	CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_UPDATE_FUEL);
}

void PacketHandler_GUVehicleEndRes(void *pPacket)
{
	// 탈 것에서 내리기 요청에 대한 응답을 자신에게 알린다
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_END_RES);

	sGU_VEHICLE_END_RES* pResult = (sGU_VEHICLE_END_RES*) pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}
}

void PacketHandler_GUVehicleStuntNfy(void *pPacket)
{
	sGU_VEHICLE_STUNT_NFY* pResult = (sGU_VEHICLE_STUNT_NFY*) pPacket;

	CNtlSLEventGenerator::SobVehicleStunt( pResult->hDriverHandle );
}
