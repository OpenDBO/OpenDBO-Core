#pragma once
#include "NtlSharedCommon.h"

class CNtlRawDataQueue
{
public:
	CNtlRawDataQueue(void);
	virtual ~CNtlRawDataQueue(void);

	bool Create(DWORD dwQueueLength);
	bool Destroy();

protected:
	bool Init();

public:
	DWORD GetMaxQueueLength() { return m_dwMaxQueueLength; }
	DWORD GetLength() { return m_dwLength; }
	DWORD GetFreeSpaceLength() { return (m_dwMaxQueueLength - m_dwLength); }
	Byte* GetDataHead() { return m_pbyData + m_dwHeadOffset; }

	DWORD GetFirstContinuousPartLength();
	DWORD GetEmptySpaceHeadOffset();
	Byte* GetEmptySpaceHead();
	DWORD GetContinuousEmptySpaceLength();

	DWORD PushData(void* pvData, DWORD dwLength);
	DWORD PopData(void* pvBuffer, DWORD dwLength);
	DWORD PeekData(void* pvBuffer, DWORD dwLength);

	bool FinalizeIncomingTransmission(DWORD dwTransferredBytes);
	bool FinalizeOutgoingTransmission(DWORD dwTransferredBytes);

	bool GuaranteeContinuousPacketData();

	void CleanData();

private:
	Byte* m_pbyData;
	DWORD m_dwMaxQueueLength;
	DWORD m_dwHeadOffset;
	DWORD m_dwLength;
};
