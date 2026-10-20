/*****************************************************************************
*
* File			: DLMessageDef.h
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          : Message ids of the CT_TW launcher build (DBOLauncher.exe
*                 2011-02-28). Ids 8..30 are the option dialog texts.
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// Message definition
//
//////////////////////////////////////////////////////////////////////////


enum eDL_MSG
{
	eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,
	eDL_MSG_ERROR_DOWNLOAD_FAILED,
	eDL_MSG_ERROR_CJI_GRID_DOWNLOAD_FAILED,
	eDL_MSG_ERROR_PATCH_FAILED,
	eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,
	eDL_MSG_MUTEX_CHECK_CLIENT,
	eDL_MSG_MUTEX_CHECK_LAUNCHER,
	eDL_MSG_PREPARE_NEW_PATCH,
	eDL_MSG_SELECT_RESOLUTION,

	eDL_MSG_UI_INTEGRITY,
	eDL_MSG_UI_OPTION,
	eDL_MSG_UI_FONT_NAME,

	eDL_MSG_OPTION_TITLE,
	eDL_MSG_OPTION_GRAPHIC,
	eDL_MSG_OPTION_RESOLUTION,
	eDL_MSG_OPTION_GAMMA,
	eDL_MSG_OPTION_GRAPHIC_QUALITY,
	eDL_MSG_OPTION_QUALITY_LOW,
	eDL_MSG_OPTION_QUALITY_MIDDLE,
	eDL_MSG_OPTION_QUALITY_HIGH,
	eDL_MSG_OPTION_QUALITY_USER,
	eDL_MSG_OPTION_WINDOW_MODE,
	eDL_MSG_OPTION_SOUND,
	eDL_MSG_OPTION_MAIN_VOLUME,
	eDL_MSG_OPTION_BGM_VOLUME,
	eDL_MSG_OPTION_EFFECT_VOLUME,
	eDL_MSG_OPTION_ENV_VOLUME,
	eDL_MSG_OPTION_DESC,
	eDL_MSG_OPTION_APPLY,
	eDL_MSG_OPTION_DEFAULT,
	eDL_MSG_OPTION_CLOSE,
};
