#pragma once

#include "NtlSharedCommon.h"

// These WM_ series definitions are defined for client.
#define WM_NETWORK_IO		(WM_USER + 0x1000)		// WM_USER = 0x0400
#define WM_INCOMING_PACKET		(WM_USER + 0x1001)
#define WM_DISCONNECTED			(WM_USER + 0x1002)

#define MAX_BUFFER		(16)

#define MAX_LENGTH_OF_PACKET		(512)

struct SWSAOverlappedEx
{
	WSAOVERLAPPED overlapped;
	WSABUF WSABuf;
	DWORD dwAction;
	DWORD dwSocketIndex;
	char sBuffer[MAX_BUFFER];
};

struct SPerSocketContext
{
	SOCKET socket;
	DWORD dwSocketIndex;
	bool bIsForRecvShutdown;
	bool bIsForSendShutdown;
	SWSAOverlappedEx overlappedRecv;
	SWSAOverlappedEx overlappedSend;
};

#pragma pack(1)

struct SPacketHeader
{
	DWORD	dwOp;
	WORD	wContentLength;
};

#pragma pack()