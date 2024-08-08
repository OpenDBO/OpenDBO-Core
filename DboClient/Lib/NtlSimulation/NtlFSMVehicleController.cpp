#include "precomp_ntlsimulation.h"
#include "NtlFSMVehicleController.h"

// simulation
#include "NtlFSMDef.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleController )


CNtlFSMVehicleController::CNtlFSMVehicleController( void )
{
	SetControllerId( SLCONTROLLER_VEHICLE );
}

CNtlFSMVehicleController::~CNtlFSMVehicleController( void )
{
}

void CNtlFSMVehicleController::Update( RwReal fElapsed )
{
	CNtlFSMPlayerController::Update( fElapsed );
}

RwUInt32 CNtlFSMVehicleController::HandleEvents( RWS::CMsg &pMsg )
{
	return CNtlFSMPlayerController::HandleEvents( pMsg );
}

void CNtlFSMVehicleController::NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState )
{
	CNtlFSMPlayerController::NotifyUpdateChangeState( uiOldState, uiNextState );
}

void CNtlFSMVehicleController::NotifyEvents( RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 )
{
	return CNtlFSMPlayerController::NotifyEvents( pMsg, uiEventResult, uiParam1, uiParam2 );
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL( CNtlFSMVehicleSyncController )


CNtlFSMVehicleSyncController::CNtlFSMVehicleSyncController( void )
{
	SetControllerId( SLCONTROLLER_VEHICLE_SYNC );
}

CNtlFSMVehicleSyncController::~CNtlFSMVehicleSyncController( void )
{
}

void CNtlFSMVehicleSyncController::Update( RwReal fElapsed )
{
}

RwUInt32 CNtlFSMVehicleSyncController::HandleEvents( RWS::CMsg &pMsg )
{
	return NTL_FSM_EVENTRES_PASS;
}

void CNtlFSMVehicleSyncController::NotifyUpdateChangeState( RwUInt32 uiOldState, RwUInt32 uiNextState )
{
}

void CNtlFSMVehicleSyncController::NotifyEvents( RWS::CMsg &pMsg, RwUInt32 uiEventResult, RwUInt32 uiParam1, RwUInt32 uiParam2 )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
