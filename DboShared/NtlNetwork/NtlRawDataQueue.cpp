#include "NtlRawDataQueue.h"
#include <stdio.h>
#include "NtlNetworkLayerTypeDef.h"

CNtlRawDataQueue::CNtlRawDataQueue(void)
{
	Init();
}

CNtlRawDataQueue::~CNtlRawDataQueue(void)
{
	Destroy();
}

bool CNtlRawDataQueue::Create(DWORD dwMaxQueueLength)
{
	if (NULL == m_pbyData)
	{
		m_pbyData = new Byte[dwMaxQueueLength + MAX_LENGTH_OF_PACKET];
		if (NULL == m_pbyData)
			return false;
	}

	ZeroMemory(m_pbyData, dwMaxQueueLength);
	m_dwMaxQueueLength = dwMaxQueueLength;

	return true;
}

bool CNtlRawDataQueue::Destroy()
{
	if (NULL != m_pbyData)
	{
		delete [] m_pbyData;
		m_pbyData = NULL;
	}

	m_dwMaxQueueLength = 0;
	m_dwHeadOffset = 0;
	m_dwLength = 0;

	return true;
}

bool CNtlRawDataQueue::Init()
{
	m_pbyData = NULL;
	m_dwMaxQueueLength = 0;
	m_dwHeadOffset = 0;
	m_dwLength = 0;

	return true;
}

DWORD CNtlRawDataQueue::GetFirstContinuousPartLength()
{
	if (m_dwLength < m_dwMaxQueueLength - m_dwHeadOffset)
	{
		return m_dwLength;
	}
	else
	{
		return m_dwMaxQueueLength - m_dwHeadOffset;
	}
}

DWORD CNtlRawDataQueue::GetEmptySpaceHeadOffset()
{
	if (m_dwMaxQueueLength - m_dwHeadOffset > m_dwLength)
	{
		return m_dwHeadOffset + m_dwLength;
	}
	else
	{
		return m_dwLength - (m_dwMaxQueueLength - m_dwHeadOffset);
	}
}

Byte* CNtlRawDataQueue::GetEmptySpaceHead()
{
	return m_pbyData + GetEmptySpaceHeadOffset();
}

DWORD CNtlRawDataQueue::GetContinuousEmptySpaceLength()
{
	if (m_dwMaxQueueLength - GetEmptySpaceHeadOffset() >
		m_dwMaxQueueLength - GetLength())
	{
		return m_dwMaxQueueLength - GetLength();
	}
	else
	{
		return m_dwMaxQueueLength - GetEmptySpaceHeadOffset();
	}
}
DWORD CNtlRawDataQueue::PushData(void* pvData, DWORD dwLength)
{
	if (NULL == pvData)
		return 0;

	DWORD dwLeftBuffer = m_dwMaxQueueLength - m_dwLength;
	DWORD dwBytesToPush = 0;

	if (dwLeftBuffer > dwLength)
	{
		dwBytesToPush = dwLength;
	}
	else
	{
		dwBytesToPush = dwLeftBuffer;
	}

	DWORD dwStartingOffset = GetEmptySpaceHeadOffset();

	if (dwBytesToPush > m_dwMaxQueueLength - dwStartingOffset)
	{
		CopyMemory(m_pbyData + dwStartingOffset, pvData, m_dwMaxQueueLength - dwStartingOffset);
		CopyMemory(m_pbyData, (Byte*)pvData + (m_dwMaxQueueLength - dwStartingOffset), dwBytesToPush - (m_dwMaxQueueLength - dwStartingOffset));
	}
	else
	{
		CopyMemory(m_pbyData + dwStartingOffset, pvData, dwBytesToPush);
	}

	m_dwLength += dwBytesToPush;

	return dwBytesToPush;
}

DWORD CNtlRawDataQueue::PopData(void* pvBuffer, DWORD dwLength)
{
	DWORD dwBytesToPop = 0;
	if (dwLength > m_dwLength)
	{
		dwBytesToPop = m_dwLength;
	}
	else
	{
		dwBytesToPop = dwLength;
	}

	// pvBuffer can be NULL.
	if (NULL != pvBuffer)
	{
		if (dwBytesToPop > m_dwMaxQueueLength - m_dwHeadOffset)
		{
			CopyMemory(pvBuffer, m_pbyData + m_dwHeadOffset, m_dwMaxQueueLength - m_dwHeadOffset);
			CopyMemory((Byte*)pvBuffer + (m_dwMaxQueueLength - m_dwHeadOffset), m_pbyData, dwBytesToPop - (m_dwMaxQueueLength - m_dwHeadOffset));
		}
		else
		{
			CopyMemory(pvBuffer, m_pbyData + m_dwHeadOffset, dwBytesToPop);
		}
	}

	if (dwBytesToPop < m_dwMaxQueueLength - m_dwHeadOffset)
	{
		m_dwHeadOffset += dwBytesToPop;
	}
	else
	{
		m_dwHeadOffset = dwBytesToPop - (m_dwMaxQueueLength - m_dwHeadOffset);
	}

	m_dwLength -= dwBytesToPop;

	return dwBytesToPop;
}

DWORD CNtlRawDataQueue::PeekData(void* pvBuffer, DWORD dwLength)
{
	if (NULL == pvBuffer)
		return false;

	DWORD dwBytesToGet = 0;
	if (dwLength > m_dwLength)
	{
		dwBytesToGet = m_dwLength;
	}
	else
	{
		dwBytesToGet = dwLength;
	}

	if (dwBytesToGet > m_dwMaxQueueLength - m_dwHeadOffset)
	{
		CopyMemory(pvBuffer, m_pbyData + m_dwHeadOffset, m_dwMaxQueueLength - m_dwHeadOffset);
		CopyMemory((Byte*)pvBuffer + (m_dwMaxQueueLength - m_dwHeadOffset), m_pbyData, dwBytesToGet - (m_dwMaxQueueLength - m_dwHeadOffset));
	}
	else
	{
		CopyMemory(pvBuffer, m_pbyData + m_dwHeadOffset, dwBytesToGet);
	}

	return dwBytesToGet;
}

bool CNtlRawDataQueue::FinalizeIncomingTransmission(DWORD dwTransferredBytes)
{
	if (0 == dwTransferredBytes ||
		dwTransferredBytes > m_dwMaxQueueLength - GetEmptySpaceHeadOffset() ||
		dwTransferredBytes > m_dwMaxQueueLength - GetLength())
	{
		printf("ERROR : dwTransferredBytes is wrong(%d) in CNtlRawDataQueue::FinalizeIncomingTransmission().\n", dwTransferredBytes);
		return false;
	}

	m_dwLength += dwTransferredBytes;
	return true;
}

bool CNtlRawDataQueue::FinalizeOutgoingTransmission(DWORD dwTransferredBytes)
{
	if (0 == dwTransferredBytes ||
		dwTransferredBytes > GetFirstContinuousPartLength() ||
		dwTransferredBytes > GetLength())
	{
		printf("ERROR : dwTransferredBytes is wrong(%d) in CNtlRawDataQueue::FinalizeOutgoingTransmission().\n", dwTransferredBytes);
		return false;
	}

	if (dwTransferredBytes < m_dwMaxQueueLength - m_dwHeadOffset)
	{
		m_dwHeadOffset += dwTransferredBytes;
	}
	else
	{
		m_dwHeadOffset = dwTransferredBytes - (m_dwMaxQueueLength - m_dwHeadOffset);
	}

	if (dwTransferredBytes < m_dwMaxQueueLength - m_dwHeadOffset)
	{
		m_dwHeadOffset += dwTransferredBytes;
	}
	else
	{
		m_dwHeadOffset = dwTransferredBytes - (m_dwMaxQueueLength - m_dwHeadOffset);
	}

	m_dwLength -= dwTransferredBytes;
	return true;
}

bool CNtlRawDataQueue::GuaranteeContinuousPacketData()
{
	SPacketHeader* pHeader = (SPacketHeader*)(m_pbyData + m_dwHeadOffset);
	if (m_dwLength < sizeof(*pHeader))
		return true;

	DWORD dwResultContentOffset = 0;
	if (m_dwMaxQueueLength - m_dwHeadOffset < sizeof(*pHeader))
	{
		::CopyMemory(m_pbyData + m_dwMaxQueueLength, m_pbyData, sizeof(*pHeader) - (m_dwMaxQueueLength - m_dwHeadOffset));
		dwResultContentOffset = m_dwMaxQueueLength + sizeof(*pHeader) - (m_dwMaxQueueLength - m_dwHeadOffset);
	}
	else
	{
		dwResultContentOffset = m_dwHeadOffset + sizeof(*pHeader);
	}

	if (m_dwLength < sizeof(*pHeader) + pHeader->wContentLength)
		return true;
	if (MAX_LENGTH_OF_PACKET < sizeof(*pHeader) + pHeader->wContentLength)
		return true;

	if (dwResultContentOffset + pHeader->wContentLength > m_dwMaxQueueLength)
	{
		if (dwResultContentOffset > m_dwMaxQueueLength)
		{
			::CopyMemory(m_pbyData + dwResultContentOffset, m_pbyData + (dwResultContentOffset - m_dwMaxQueueLength), pHeader->wContentLength);
		}
		else
		{
			::CopyMemory(m_pbyData + m_dwMaxQueueLength, m_pbyData, pHeader->wContentLength - (m_dwMaxQueueLength - dwResultContentOffset));
		}
	}

	return true;
}

void
CNtlRawDataQueue::CleanData()
{
	m_dwHeadOffset = 0;
	m_dwLength = 0;
}