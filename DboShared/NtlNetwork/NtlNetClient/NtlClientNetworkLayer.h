#pragma once
#include <Winsock2.h>
#include <Windows.h>
#include <time.h>
#include "..\NtlNetworkLog.h"
#include "..\NtlRawDataQueue.h"

class CNtlClientNetworkLayer
{
public:
	enum EMaxNumberOfConnection
	{
		MAX_NUMBER_OF_SERVER = 1000
	};

	enum EMaxQueueLength
	{
		MAX_QUEUE_LENGTH = 40 * 1024
	};

	enum EErrorCode
	{
		ERROR_NONE = 0x00000000,
		ERROR_ACTION_ON_CLOSED_SOCKET = 0x00000001,
		ERROR_NO_SPACE_IN_QUEUE = 0x00000002,
//		ERROR_WRONG_ROLE = 0x00000003,
//		ERROR_NO_BUFFER_SPECIFIED = 0x0000004,		// Used in RECV operation.
		ERROR_NO_DATA_SPECIFIED = 0x0000004,		// Used in SEND operation.
//		ERROR_WRONG_BUFFER_LENGTH = 0x0000005,		// Used in RECV operation.
		ERROR_WRONG_DATA_LENGTH = 0x0000005,		// Used in SEND operation.
		ERROR_WRONG_SOCKET_INDEX_SPECIFIED = 0x0000006,
		ERROR_UNKNOWN_ERROR = 0x7FFFFFFF
	};

public:
	CNtlClientNetworkLayer(void);
	virtual ~CNtlClientNetworkLayer(void);

	bool Create(HWND hwndOwnerWindow);
	bool Destroy();

protected:
	bool Init();

public:
	bool IsInitialized() { return m_bIsInitialized; };

	bool ConnectToServer(char* szServerAddress, DWORD dwPort, DWORD* pdwSocketIndex);

	bool DisconnectServer(DWORD dwSocketIndex, bool bGraceful);

	/* These functions are executed by Proc function. */
	bool ProcessNetworkIo(WPARAM wParam, LPARAM lParam);		// WM_NETWORK_IO
	DWORD PopPacket(DWORD dwSocketIndex, BYTE *sBuffer);		// WM_INCOMING_PACKET

	DWORD SendDataToServer(DWORD dwSocketIndex, void* pvContent, DWORD dwBytes, DWORD *pdwErrorCode);

	bool AddLog(char* szFormatString, ...);

	bool IsServerConnected(DWORD dwSocketIndex);

	static void WriteToConsole(char* szFormatString, ...);

protected:
	char* GetErrorMessage(int iErrorCode);

/* For debug */
public:
	static DWORD dwTotalBytesSent;
	static time_t m_beginTime;
	static time_t m_lastTimeTotalBytesSent;

private:
	bool m_bIsInitialized;

	HWND m_hwndOwnerWindow;
	CNtlNetworkLog m_log;

	SOCKET m_connectSocket[MAX_NUMBER_OF_SERVER];
	bool m_bIsServerConnected[MAX_NUMBER_OF_SERVER];
	bool m_bIsServerSendIoPending[MAX_NUMBER_OF_SERVER];
	DWORD m_dwPendingBytesSend[MAX_NUMBER_OF_SERVER];

	CNtlRawDataQueue m_incomingDataFromServer[MAX_NUMBER_OF_SERVER];
	CNtlRawDataQueue m_outgoingDataToServer[MAX_NUMBER_OF_SERVER];
};
