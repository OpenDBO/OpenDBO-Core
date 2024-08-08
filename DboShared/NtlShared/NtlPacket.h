#pragma once

#include "NtlPacketGTC.h"
#include "NtlPacketCTG.h"
#include "NtlPacketUTC.h"
#include "NtlPacketCTU.h"
#include "NtlPacketUTG.h"
#include "NtlPacketATU.h"
#include "NtlPacketUTA.h"
#include "NtlPacketGTU.h"
#include "NtlPacketATCh.h"
#include "NtlPacketUTCh.h"
#include "NtlPacketChTU.h"
#include "NtlPacketChTG.h"
#include "NtlPacketGTCh.h"
#include "NtlPacketCTCh.h"
#include "NtlPacketChTC.h"
#include "NtlPacketChTA.h"
#include "NtlMsgPacket.h"

/*
	NOTICE :

	1) Naming Op-code

		To distinguish type of a packet from other types, we use some alphabet letters in op-codes.
			U - User(Client)
			A - Account Server
			G - Game Server
			C - Chat Server
			D - DB Server
			L - Log Server
			M - Monitoring Server
			Ch - Character Server

		For example, if we design a packet that is sent from a game server to a DB server, its op-code must be named as "OpGTD".

	2) Generating Op-code's value

		Each server type has a code value. See the following table.

			0x0 : U - User(Client)
			0x1 : A - Account Server
			0x2 : G - Game Server
			0x3 : C - Chat Server
			0x4 : D - DB Server
			0x5 : L - Log Server
			0x6 : M - Monitoring Server
			0x7 : Ch - Character Server

		For example, if we design a packet that is sent from a game server to a DB server, we pick up the corresponding values which are 0x2 and 0x4.
		Then we generate the number 0x24 with combining 0x2 and 0x4.
		Now the new packet's Op code must be in [0x24000000, 0x24FFFFFF].

	3) Conclusion

		UTA series packets' Op code should be in [0x01000000, 0x01FFFFFF].
		ATU series packets' Op code should be in [0x10000000, 0x10FFFFFF].


		UTG series packets' Op code should be in [0x02000000, 0x02FFFFFF].
		GTU series packets' Op code should be in [0x20000000, 0x20FFFFFF].

		ATG series packets' Op code should be in [0x12000000, 0x12FFFFFF].
		GTA series packets' Op code should be in [0x21000000, 0x21FFFFFF].


		UTC series packets' Op code should be in [0x03000000, 0x03FFFFFF].
		CTU series packets' Op code should be in [0x30000000, 0x30FFFFFF].

		ATC series packets' Op code should be in [0x13000000, 0x13FFFFFF].
		CTA series packets' Op code should be in [0x31000000, 0x31FFFFFF].

		GTC series packets' Op code should be in [0x23000000, 0x23FFFFFF].
		CTG series packets' Op code should be in [0x32000000, 0x32FFFFFF].


		ATD series packets' Op code should be in [0x14000000, 0x14FFFFFF].
		DTA series packets' Op code should be in [0x41000000, 0x41FFFFFF].

		GTD series packets' Op code should be in [0x24000000, 0x24FFFFFF].
		DTG series packets' Op code should be in [0x42000000, 0x42FFFFFF].


		ATL series packets' Op code should be in [0x15000000, 0x15FFFFFF].
		LTA series packets' Op code should be in [0x51000000, 0x51FFFFFF].

		GTL series packets' Op code should be in [0x25000000, 0x25FFFFFF].
		LTG series packets' Op code should be in [0x52000000, 0x52FFFFFF].

		CTL series packets' Op code should be in [0x35000000, 0x35FFFFFF].
		LTC series packets' Op code should be in [0x53000000, 0x53FFFFFF].

		DTL series packets' Op code should be in [0x45000000, 0x45FFFFFF].
		LTD series packets' Op code should be in [0x54000000, 0x54FFFFFF].


		ATM series packets' Op code should be in [0x16000000, 0x16FFFFFF].
		MTA series packets' Op code should be in [0x61000000, 0x61FFFFFF].

		GTM series packets' Op code should be in [0x26000000, 0x26FFFFFF].
		MTG series packets' Op code should be in [0x62000000, 0x62FFFFFF].

		CTM series packets' Op code should be in [0x36000000, 0x36FFFFFF].
		MTC series packets' Op code should be in [0x63000000, 0x63FFFFFF].

		DTM series packets' Op code should be in [0x46000000, 0x46FFFFFF].
		MTD series packets' Op code should be in [0x64000000, 0x64FFFFFF].

		LTM series packets' Op code should be in [0x56000000, 0x56FFFFFF].
		MTL series packets' Op code should be in [0x65000000, 0x65FFFFFF].


		UTCh series packets' Op code should be in [0x07000000, 0x07FFFFFF].
		ChTU series packets' Op code should be in [0x70000000, 0x70FFFFFF].

		ATCh series packets' Op code should be in [0x17000000, 0x17FFFFFF].
		ChTA series packets' Op code should be in [0x71000000, 0x71FFFFFF].

		GTCh series packets' Op code should be in [0x27000000, 0x27FFFFFF].
		ChTG series packets' Op code should be in [0x72000000, 0x72FFFFFF].

		CTCh series packets' Op code should be in [0x37000000, 0x37FFFFFF].
		ChTC series packets' Op code should be in [0x73000000, 0x73FFFFFF].

		DTCh series packets' Op code should be in [0x47000000, 0x47FFFFFF].
		ChTD series packets' Op code should be in [0x74000000, 0x74FFFFFF].

		LTCh series packets' Op code should be in [0x57000000, 0x57FFFFFF].
		ChTL series packets' Op code should be in [0x75000000, 0x75FFFFFF].

		MTCh series packets' Op code should be in [0x67000000, 0x67FFFFFF].
		ChTM series packets' Op code should be in [0x76000000, 0x76FFFFFF].
*/

enum ENtlPacketSentFrom
{
	SENT_FROM_USER = 0x00000000,
	SENT_FROM_ACCOUNT_SERVER = 0x10000000,
	SENT_FROM_GAME_SERVER = 0x20000000,
	SENT_FROM_CHAT_SERVER = 0x30000000,
	SENT_FROM_DB_SERVER = 0x40000000,
	SENT_FROM_LOG_SERVER = 0x50000000,
	SENT_FROM_MONITORING_SERVER = 0x60000000,
	SENT_FROM_CHARACTER_SERVER = 0x70000000,
	SENT_FROM_UNKNOWN_SERVER = 0xFFFFFFFF,
};

enum ENtlPacketSentTo
{
	SENT_TO_USER = 0x00000000,
	SENT_TO_ACCOUNT_SERVER = 0x01000000,
	SENT_TO_GAME_SERVER = 0x02000000,
	SENT_TO_CHAT_SERVER = 0x03000000,
	SENT_TO_DB_SERVER = 0x04000000,
	SENT_TO_LOG_SERVER = 0x05000000,
	SENT_TO_MONITORING_SERVER = 0x06000000,
	SENT_TO_CHARACTER_SERVER = 0x07000000,
	SENT_TO_UNKNOWN_SERVER = 0xFFFFFFFF,
};

UInt32 NtlPacket_GetOpCode(void* pvPacket);
ENtlPacketSentFrom NtlPacket_SentFrom(Byte* pbyPacket);
ENtlPacketSentTo NtlPacket_SentTo(Byte* pbyPacket);
char* NtlPacket_GetPacketName(void* pvPacket);