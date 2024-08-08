//***********************************************************************************
//
//	File		:	NtlVector.cpp
//
//	Begin		:	2006-03-08
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlVector.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
CNtlVector CNtlVector::ZERO(0.0f, 0.0f, 0.0f);
CNtlVector CNtlVector::UNIT_X(1.0f, 0.0f, 0.0f);
CNtlVector CNtlVector::UNIT_Y(0.0f, 1.0f, 0.0f);
CNtlVector CNtlVector::UNIT_Z(0.0f, 0.0f, 1.0f);
CNtlVector CNtlVector::INVALID(INVALID_FLOAT, INVALID_FLOAT, INVALID_FLOAT);
CNtlVector CNtlVector::INVALID_XZ(INVALID_FLOAT, 0.0f, INVALID_FLOAT);
//-----------------------------------------------------------------------------------

