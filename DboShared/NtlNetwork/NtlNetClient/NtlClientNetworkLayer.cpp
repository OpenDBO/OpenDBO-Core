#include "NtlClientNetworkLayer.h"
#include <stdio.h>
#include "..\NtlNetworkLayerTypeDef.h"

//#define ASYNC_CONNECT

DWORD g_dwTotalBytes = 0;
time_t g_last = time(NULL);

CNtlClientNetworkLayer::CNtlClientNetworkLayer(void)
{
	Init();
}

CNtlClientNetworkLayer::~CNtlClientNetworkLayer(void)
{
	Destroy();
}

bool CNtlClientNetworkLayer::Create(HWND hwndOwnerWindow)
{
	if (NULL == hwndOwnerWindow)
	{
		printf("ERROR : hwndOwnerWindow is NULL!");
		return false;
	}

	if (false == m_log.Create("NetworkLayerLog.txt"))
		return false;

	for (DWORD i=0 ; i < CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER ; i++)
	{
		m_connectSocket[i] = INVALID_SOCKET;
		m_bIsServerConnected[i] = false;
		m_bIsServerSendIoPending[i] = false;
		m_dwPendingBytesSend[i] = 0;
	}

	WSADATA WSAData;

	if (0 != WSAStartup(MAKEWORD(1, 1), &WSAData))
	{
		int iErrorCode = WSAGetLastError();
		printf("ERROR : WSAStartup failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		return false;
	}

	printf("Network Initiation Success.\n");

	m_hwndOwnerWindow = hwndOwnerWindow;

	for (DWORD i=0 ; i<CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER ; i++)
	{
		m_incomingDataFromServer[i].Create(MAX_QUEUE_LENGTH);
		m_outgoingDataToServer[i].Create(MAX_QUEUE_LENGTH);
	}

	m_bIsInitialized = true;

	return true;
}

bool CNtlClientNetworkLayer::Destroy()
{
	for (DWORD i=0 ; i < CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER ; i++)
	{
		if (false != m_bIsServerConnected[i])
		{
			DisconnectServer(i, true);
		}
	}

	WSACleanup();

	m_hwndOwnerWindow = NULL;

	m_bIsInitialized = false;

	return true;
}

bool CNtlClientNetworkLayer::Init()
{
	m_bIsInitialized = false;
	m_hwndOwnerWindow = NULL;

//	m_log.SetDisabled();

	return true;
}

bool CNtlClientNetworkLayer::ConnectToServer(char* szServerAddress, DWORD dwPort, DWORD* pdwSocketIndex)
{
	SOCKADDR_IN serverSockAddr;

	DWORD dwConnectSocketIndex = 0;
	for (dwConnectSocketIndex=0 ; dwConnectSocketIndex < MAX_NUMBER_OF_SERVER ; dwConnectSocketIndex++)
	{
		if (false == m_bIsServerConnected[dwConnectSocketIndex])
			break;
	}

	if (CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER <= dwConnectSocketIndex)
	{
		printf("ERROR : No more available socket to be connected to a server.\n");
		return false;
	}

	m_connectSocket[dwConnectSocketIndex] = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_connectSocket[dwConnectSocketIndex])
	{
		int iErrorCode = WSAGetLastError();
		printf("ERROR : Connect socket creation failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		return false;
	}

#ifdef ASYNC_CONNECT
	if (SOCKET_ERROR == WSAAsyncSelect(m_connectSocket[dwConnectSocketIndex], m_hwndOwnerWindow, WM_NETWORK_IO, FD_CONNECT))
	{
		int iErrorCode = WSAGetLastError();
		printf("ERROR : WSAAsyncSelect(FD_CONNECT) failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		return false;
	}
#endif

	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_port = htons((u_short)dwPort);
	serverSockAddr.sin_addr.s_addr = inet_addr(szServerAddress);

	int result = connect(m_connectSocket[dwConnectSocketIndex], (const SOCKADDR*)(&serverSockAddr), sizeof(serverSockAddr));
	if (SOCKET_ERROR == result)
	{
		int iErrorCode = WSAGetLastError();
		if (WSAEWOULDBLOCK != iErrorCode)
		{
			printf("DEBUG : Couldn't connect to the server.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
			return false;
		}
	}

#ifndef ASYNC_CONNECT
	m_bIsServerConnected[dwConnectSocketIndex] = true;

	if (SOCKET_ERROR == WSAAsyncSelect(m_connectSocket[dwConnectSocketIndex], m_hwndOwnerWindow, WM_NETWORK_IO, FD_READ | FD_WRITE | FD_CLOSE))
	{
		int iErrorCode = WSAGetLastError();
		printf("ERROR : WSAAsyncSelect(FD_READ | FD_WRITE | FD_CLOSE) failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		return false;
	}
#endif

	*pdwSocketIndex = dwConnectSocketIndex;

	return true;
}

bool CNtlClientNetworkLayer::DisconnectServer(DWORD dwSocketIndex, bool bGraceful)
{
	SOCKET socketToClose = m_connectSocket[dwSocketIndex];

	if (false == bGraceful)
	{
		LINGER LingerStruct;
		LingerStruct.l_onoff = 1;
		LingerStruct.l_linger = 0;
		if (SOCKET_ERROR == setsockopt(socketToClose, SOL_SOCKET, SO_LINGER, (char*)(&LingerStruct), sizeof(LingerStruct)))
		{
			int iErrorCode = WSAGetLastError();
			printf("ERROR : Setting linger failed in CNtlClientNetworkLayer::DisconnectServer().(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		}
	}

	m_bIsServerConnected[dwSocketIndex] = false;

	closesocket(socketToClose);

	m_incomingDataFromServer[dwSocketIndex].CleanData();
	m_outgoingDataToServer[dwSocketIndex].CleanData();

	::PostMessage(m_hwndOwnerWindow, WM_DISCONNECTED, m_connectSocket[dwSocketIndex], dwSocketIndex);

	return true;
}

bool CNtlClientNetworkLayer::ProcessNetworkIo(WPARAM wParam, LPARAM lParam)
{
	DWORD dwSocketIndex = 0xFFFFFFFF;

#ifndef ASYNC_CONNECT
	for (DWORD i=0 ; i < CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER ; i++)
	{
		if (false == m_bIsServerConnected[i])
			continue;

		if (wParam == m_connectSocket[i])
		{
			dwSocketIndex = i;
			break;
		}
	}

	if (0xFFFFFFFF == dwSocketIndex)
		return false;

#else
	if (FD_CONNECT != lParam)
	{
		for (DWORD i=0 ; i < CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER ; i++)
		{
			if (false == m_bIsServerConnected[i])
				continue;

			if (wParam == m_connectSocket[i])
			{
				dwSocketIndex = i;
				break;
			}
		}

		if (0xFFFFFFFF == dwSocketIndex)
			return false;
	}
#endif

	switch (lParam)
	{
#ifdef ASYNC_CONNECT
		case FD_CONNECT :
		{
			for (DWORD i=0 ; i < CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER ; i++)
			{
				if (false == m_bIsServerConnected[i])
				{
					m_bIsServerConnected[i] = true;
					m_connectSocket[i] = wParam;
					dwSocketIndex = i;
					break;
				}
			}
			if (0xFFFFFFFF == dwSocketIndex)
				return false;

			if (SOCKET_ERROR == WSAAsyncSelect(m_connectSocket[dwSocketIndex], m_hwndOwnerWindow, WM_NETWORK_IO, FD_READ | FD_WRITE | FD_CLOSE))
			{
				int iErrorCode = WSAGetLastError();
				printf("ERROR : WSAAsyncSelect(FD_READ | FD_WRITE | FD_CLOSE) failed.(%s : %d)!\n", GetErrorMessage(iErrorCode), iErrorCode);
				return false;
			}
		}
			break;
#endif

		case FD_READ :
		{
			BYTE* pbyEmptySpaceHead = m_incomingDataFromServer[dwSocketIndex].GetEmptySpaceHead();
			DWORD dwContinuousEmptySpaceLength = m_incomingDataFromServer[dwSocketIndex].GetContinuousEmptySpaceLength();

			if (0 == dwContinuousEmptySpaceLength)
			{
				printf("ERROR : m_incomingDataFromServer[%d] is full!\n", dwSocketIndex);
			}

			int receivedBytes = recv(m_connectSocket[dwSocketIndex], (char*)pbyEmptySpaceHead, dwContinuousEmptySpaceLength, 0);
			if (SOCKET_ERROR == receivedBytes)
			{
				int iErrorCode = WSAGetLastError();
				if (WSAEWOULDBLOCK != iErrorCode)
				{
					AddLog("ERROR : recv failed in CNtlClientNetworkLayer::ProcessNetworkIo().(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
					DisconnectServer(dwSocketIndex, true);
				}
			}
			else
			{
				m_incomingDataFromServer[dwSocketIndex].FinalizeIncomingTransmission(receivedBytes);
			}

			if (sizeof(SPacketHeader) < m_incomingDataFromServer[dwSocketIndex].GetLength())
			{
				PostMessage(m_hwndOwnerWindow, WM_INCOMING_PACKET, m_connectSocket[dwSocketIndex], dwSocketIndex);
			}
		}
			break;

		case FD_WRITE :
		{
			AddLog("FD_WRITE");

			if (0 < m_dwPendingBytesSend[dwSocketIndex])
			{
				g_dwTotalBytes += m_dwPendingBytesSend[dwSocketIndex];
				m_outgoingDataToServer[dwSocketIndex].PopData(NULL, m_dwPendingBytesSend[dwSocketIndex]);
				m_dwPendingBytesSend[dwSocketIndex] = 0;
			}

			if (0 < m_outgoingDataToServer[dwSocketIndex].GetLength())
			{
				bool bLoop = true;
				do
				{
					BYTE* pbyGetDataHead = m_outgoingDataToServer[dwSocketIndex].GetDataHead();
					DWORD dwGetFirstContinuousPartLength = m_outgoingDataToServer[dwSocketIndex].GetFirstContinuousPartLength();

					LARGE_INTEGER counter1;
					LARGE_INTEGER counter2;
					counter1.QuadPart = 0;
					counter2.QuadPart = 0;

					QueryPerformanceCounter(&counter1);
					int sentBytes = send(m_connectSocket[dwSocketIndex], (const char*)pbyGetDataHead, dwGetFirstContinuousPartLength, 0);
					QueryPerformanceCounter(&counter2);
					if (SOCKET_ERROR == sentBytes)
					{
						int iErrorCode = WSAGetLastError();
						if (WSAEWOULDBLOCK != iErrorCode)
						{
							AddLog("ERROR : send failed in CNtlClientNetworkLayer::ProcessNetworkIo().(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
							DisconnectServer(dwSocketIndex, true);
							return false;
						}
						else
						{
							m_dwPendingBytesSend[dwSocketIndex] = dwGetFirstContinuousPartLength;
						}
						bLoop = false;
					}
					else
					{
						g_dwTotalBytes += sentBytes;
						m_outgoingDataToServer[dwSocketIndex].PopData(NULL, sentBytes);
					}
				} while (false != bLoop && 0 < m_outgoingDataToServer[dwSocketIndex].GetLength());

				if (0 == m_outgoingDataToServer[dwSocketIndex].GetLength())
				{
					m_bIsServerSendIoPending[dwSocketIndex] = false;
				}

				time_t now = time(NULL);
				if (now - g_last >= 1)
				{
					WriteToConsole("YOSHIKI : performance = %f\n", (float)g_dwTotalBytes / (float)(now - g_last));
					g_dwTotalBytes = 0;
					g_last = now;
				}
			}
		}
			break;

		case FD_CLOSE :
			DisconnectServer(dwSocketIndex, true);
			break;

		default :
			break;
	}

	return true;
}

DWORD CNtlClientNetworkLayer::PopPacket(DWORD dwSocketIndex, BYTE* sBuffer)
{
	if (CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER <= dwSocketIndex)
		return 0;
	if (m_incomingDataFromServer[dwSocketIndex].GetLength() < sizeof(SPacketHeader))
		return 0;

	SPacketHeader header;
	m_incomingDataFromServer[dwSocketIndex].PeekData(&header, sizeof(header));

	if (m_incomingDataFromServer[dwSocketIndex].GetLength() < sizeof(header) + header.wContentLength)
		return 0;

	m_incomingDataFromServer[dwSocketIndex].PopData(sBuffer, sizeof(header) + header.wContentLength);

	return sizeof(header) + header.wContentLength;
}

DWORD CNtlClientNetworkLayer::SendDataToServer(DWORD dwSocketIndex, void* pvContent, DWORD dwBytes, DWORD *pdwErrorCode)
{
	DWORD dwMaxQueueLength = m_outgoingDataToServer[dwSocketIndex].GetMaxQueueLength();

	if (NULL == pvContent)
	{
		if (NULL != pdwErrorCode)
			*pdwErrorCode = CNtlClientNetworkLayer::ERROR_NO_DATA_SPECIFIED;

		printf("ERROR : pvContent is NULL in CNtlClientNetworkLayer::WriteOutgoingDataToServer().\n");
		return 0;
	}
	if (dwBytes < sizeof(SPacketHeader) || MAX_LENGTH_OF_PACKET < dwBytes)
	{
		if (NULL != pdwErrorCode)
			*pdwErrorCode = CNtlClientNetworkLayer::ERROR_WRONG_DATA_LENGTH;

		printf("ERROR : dwBytes has an illegal value(%d) in CNtlClientNetworkLayer::WriteOutgoingDataToServer().\n", dwBytes);
		return 0;
	}
	if (CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER <= dwSocketIndex)
	{
		if (NULL != pdwErrorCode)
			*pdwErrorCode = CNtlClientNetworkLayer::ERROR_WRONG_SOCKET_INDEX_SPECIFIED;

		printf("ERROR : dwSocketIndex has an illegal value(%d) in CNtlClientNetworkLayer::WriteOutgoingDataToServer().\n", dwSocketIndex);
		return 0;
	}

	if (false == m_bIsServerConnected[dwSocketIndex])
	{
		if (NULL != pdwErrorCode)
			*pdwErrorCode = CNtlClientNetworkLayer::ERROR_ACTION_ON_CLOSED_SOCKET;

		return 0;
	}

	DWORD dwBytesSent = 0;
	DWORD dwOldQueueLength = m_outgoingDataToServer[dwSocketIndex].GetLength();
	bool bNeedToDisconnect = false;

	dwBytesSent = m_outgoingDataToServer[dwSocketIndex].PushData(pvContent, dwBytes);
	if (dwMaxQueueLength <= m_outgoingDataToServer[dwSocketIndex].GetLength())
	{
		printf("WARNING : m_outgoingDataToServer[%d] is full!\n", dwSocketIndex);
		bNeedToDisconnect = true;
	}
	else if (dwMaxQueueLength * 0.9 < m_outgoingDataToServer[dwSocketIndex].GetLength())
	{
		printf("WARNING : m_outgoingDataToServer[%d] is about to be full!\n", dwSocketIndex);
	}

	if (0 == dwOldQueueLength && false == m_bIsServerSendIoPending[dwSocketIndex])
	{
		m_bIsServerSendIoPending[dwSocketIndex] = true;

		PostMessage(m_hwndOwnerWindow, WM_NETWORK_IO, m_connectSocket[dwSocketIndex], FD_WRITE);
	}

//	AddLog("YOSHIKI : [%d]queue = %d/%d", dwSocketIndex, m_outgoingDataToServer[dwSocketIndex].GetLength(), m_outgoingDataToServer[dwSocketIndex].GetMaxQueueLength());
	if (false != bNeedToDisconnect)
	{
		AddLog("ERROR : [%d]Buffer is full! Closing the connection.", dwSocketIndex);
		DisconnectServer(dwSocketIndex, true);
		*pdwErrorCode = CNtlClientNetworkLayer::ERROR_NO_SPACE_IN_QUEUE;
		return 0;
	}

	*pdwErrorCode = CNtlClientNetworkLayer::ERROR_NONE;

	return dwBytesSent;
}

bool CNtlClientNetworkLayer::AddLog(char* szFormatString, ...)
{
	va_list args;
	va_start(args, szFormatString);

	bool bResult = m_log.AddLogAlternative(szFormatString, args);

	va_end(args);

	return bResult;
}

bool CNtlClientNetworkLayer::IsServerConnected(DWORD dwSocketIndex)
{
	if (CNtlClientNetworkLayer::MAX_NUMBER_OF_SERVER <= dwSocketIndex)
	{
		printf("ERROR : dwSocketIndex has an illegal value(%d) in CNtlClientNetworkLayer::IsServerConnected().\n", dwSocketIndex);
		return false;
	}

	return m_bIsServerConnected[dwSocketIndex];
}

char* CNtlClientNetworkLayer::GetErrorMessage(int iErrorCode)
{
	static char szErrorMessage[256 + 1];
	szErrorMessage[0] = '\0';
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, iErrorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), szErrorMessage, 256, NULL);
	szErrorMessage[strlen(szErrorMessage) - 1] = '\0';
	szErrorMessage[strlen(szErrorMessage) - 1] = '\0';

	return szErrorMessage;
}

void CNtlClientNetworkLayer::WriteToConsole(char* szFormatString, ...)
{
	va_list args;
	va_start(args, szFormatString);

	char szTemp[1024 + 1];
	vsprintf(szTemp, szFormatString, args);

	va_end(args);

	DWORD dwWrittenBytes = 0;
	::WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szTemp, (DWORD)strlen(szTemp), &dwWrittenBytes, NULL);
}