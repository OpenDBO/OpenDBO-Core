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


// The vehicle sob has its own FSM/state independent from the driver's, so the driver-targeted
// SobVehicleEngine event never reaches it. Write the engine flag on the vehicle sob directly,
// otherwise CNtlSobVehicle::UpdateRideOnHeight() and the vehicle's own FSM states keep reading
// a stale engine state and the ride height never lowers when the engine is turned off.
static void NotifyVehicleSobEngineState(SERIAL_HANDLE hDriverHandle, RwUInt8 byMessage)
{
	CNtlSob* pDriver = GetNtlSobManager()->GetSobObject(hDriverHandle);
	if (!pDriver || pDriver->GetVehicleID() == INVALID_SERIAL_ID)
		return;

	CNtlSobActor* pVehicleSob = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pDriver->GetVehicleID()));
	if (!pVehicleSob)
		return;

	RwBool bEngineOn = (VEHICLE_ENGINE_START == byMessage);

	sCHARSTATE* pVehicleState = pVehicleSob->GetServerFullState();
	if (pVehicleState)
		pVehicleState->sCharStateBase.aspectState.sAspectStateDetail.sVehicle.bIsEngineOn = bEngineOn;

	CNtlBeCharData* pVehicleBeData = reinterpret_cast<CNtlBeCharData*>(pVehicleSob->GetBehaviorData());
	if (pVehicleBeData)
		pVehicleBeData->GetCtrlStuff()->sVehicle.bEngineOn = bEngineOn;
}

void PacketHandler_GUVehicleStartNfy(void *pPacket)
{
	// The riding on the riding starts or rides while riding appears
	sGU_VEHICLE_START_NFY* pResult = (sGU_VEHICLE_START_NFY*)pPacket;


	CNtlSLEventGenerator::SobVehicleStart(pResult->hDriverHandle,
										  pResult->hVehicleItem,
										  pResult->idVehicleItemTblidx);
}

void PacketHandler_GUVehicleEndRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_END_RES);

	sGU_VEHICLE_END_RES* pResult = (sGU_VEHICLE_END_RES*) pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_VEHICLE_END_RES"), TRUE);
		return;
	}
}

void PacketHandler_GUVehicleEngineStartRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_ENGINE_START_RES);

	sGU_VEHICLE_ENGINE_START_RES* pResult = (sGU_VEHICLE_ENGINE_START_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_VEHICLE_ENGINE_START_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_START);
}

void PacketHandler_GUVehicleEngineStartNfy(void* pPacket)
{
	sGU_VEHICLE_ENGINE_START_NFY* pResult = (sGU_VEHICLE_ENGINE_START_NFY*)pPacket;

	CNtlSLEventGenerator::SobVehicleEngine(pResult->hDriverHandle, VEHICLE_ENGINE_START);
	NotifyVehicleSobEngineState(pResult->hDriverHandle, VEHICLE_ENGINE_START);
}

void PacketHandler_GUVehicleEngineStopRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_ENGINE_STOP_RES);

	sGU_VEHICLE_ENGINE_STOP_RES* pResult = (sGU_VEHICLE_ENGINE_STOP_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_FAIL_REQUEST_STOP);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_VEHICLE_ENGINE_STOP_RES"), TRUE);
		return;
	}
}

void PacketHandler_GUVehicleEngineStopNfy(void* pPacket)
{
	sGU_VEHICLE_ENGINE_STOP_NFY* pResult = (sGU_VEHICLE_ENGINE_STOP_NFY*)pPacket;

	CNtlSLEventGenerator::SobVehicleEngine(pResult->hDriverHandle, VEHICLE_ENGINE_STOP);
	NotifyVehicleSobEngineState(pResult->hDriverHandle, VEHICLE_ENGINE_STOP);
}

void PacketHandler_GUVehicleStuntNfy(void *pPacket)
{
	sGU_VEHICLE_STUNT_NFY* pResult = (sGU_VEHICLE_STUNT_NFY*) pPacket;

	CNtlSLEventGenerator::SobVehicleStunt( pResult->hDriverHandle );
}

void PacketHandler_GUVehicleFuelInsertRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_FUEL_INSERT_RES);

	sGU_VEHICLE_FUEL_INSERT_RES* pResult = (sGU_VEHICLE_FUEL_INSERT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_VEHICLE_FUEL_INSERT_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_REG_FUEL, pResult->hItem);
}
