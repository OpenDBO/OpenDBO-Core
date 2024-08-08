#include "precomp_trigger.h"
#include "DboTSSlotMachine.h"


/**
	Slot machine
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSSlotMachine, CNtlTSEvent )


CDboTSSlotMachine::CDboTSSlotMachine( void )
{
}

void CDboTSSlotMachine::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );
}

void CDboTSSlotMachine::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );
}
