/*****************************************************************************
* File			: NtlStorageGroupSystem.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 2. 5	
* Abstract		: System의 Storage group
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_SYSTEM_H__
#define __NTL_STORAGE_GROUP_SYSTEM_H__

#pragma once
#include "NtlStorageGroup.h"

class CNtlStorageGroupSystem : public CNtlStorageGroup
{
public:
	CNtlStorageGroupSystem(void);
	virtual ~CNtlStorageGroupSystem(void);

	virtual bool Create();
};

#endif//__NTL_STORAGE_GROUP_SYSTEM_H__
