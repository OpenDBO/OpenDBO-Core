//***********************************************************************************
//
//	File		:	NtlRandom.h
//
//	Begin		:	2006-11-20
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************


#pragma once

void				NtlSRand(unsigned int uiSeed);
int					NtlRand();

int					RandomRange(int nMinVal, int nMaxVal);
DWORD				RandomRangeU(DWORD dwMinVal, DWORD dwMaxVal);
float				RandomRangeF(float fMinVal, float fMaxVal);

bool				Dbo_CheckProbability(int nProbability);			// in percent
bool				Dbo_CheckProbabilityF(float fProbability);		// in percent