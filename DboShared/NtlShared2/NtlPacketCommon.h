#pragma once

#include "NtlSharedCommon.h"
#include "NtlSharedType.h"
#include "NtlSharedDef.h"

#include <string>


#pragma warning(disable : 4328)



//------------------------------------------------------------------
//
//------------------------------------------------------------------
struct sNTLPACKETHEADER
{
	sNTLPACKETHEADER(WORD wGivenOpCode) :
		wOpCode(wGivenOpCode) {}

	WORD			wOpCode;
};
//------------------------------------------------------------------



//------------------------------------------------------------------
//
//------------------------------------------------------------------
#define BEGIN_PROTOCOL(opcode)						\
struct s##opcode :									\
	public sNTLPACKETHEADER							\
{													\
	s##opcode() :									\
		sNTLPACKETHEADER(opcode)					\
	{												\
	}

#define END_PROTOCOL()	};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
//------------------------------------------------------------------
#define BEGIN_PROTOCOL_IDENTITY( opcode, identity )	\
typedef s##identity s##opcode						\

#define END_PROTOCOL_IDENTITY()	;
//------------------------------------------------------------------


//------------------------------------------------------------------
#define DECLARE_PACKET_NAME( opcode )	{ #opcode }
//------------------------------------------------------------------