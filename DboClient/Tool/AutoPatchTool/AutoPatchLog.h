/*****************************************************************************
 *
 * File			: AutoPatchLog.h
 * Author		: Jang HyungSuk
 * Copyright	: (주)NTL
 * Date			: 2008. 6. 28	
 * Abstract		: patch log
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

#include <string>
#include <windows.h>
#include <time.h>


class CAutoPacthLog
{
public:
		
	CAutoPacthLog();
	~CAutoPacthLog();

	void	Out(const char *pFileName, const char *Log);
};

