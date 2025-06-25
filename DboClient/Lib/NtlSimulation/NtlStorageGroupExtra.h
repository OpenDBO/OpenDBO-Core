/*****************************************************************************
* File			: NtlStorageGroupExtra.h
* Author		: Santiago A.K.A Lothkru
* Date			: 2025/05/13
* Abstract		: Extra Storage group   (for new stuff)
*****************************************************************************
* Desc         :
*
*****************************************************************************/

#ifndef __NTL_STORAGE_GROUP_EXTRA_H__
#define __NTL_STORAGE_GROUP_EXTRA_H__

#pragma once
#include "NtlStorageGroup.h"

class CNtlStorageGroupExtra : public CNtlStorageGroup
{
public:
	CNtlStorageGroupExtra(void);
	virtual ~CNtlStorageGroupExtra(void);

	virtual bool Create();
};

#endif//__NTL_STORAGE_GROUP_EXTRA_H__