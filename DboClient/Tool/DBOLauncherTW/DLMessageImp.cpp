/*****************************************************************************
*
* File			: DLMessageImp.cpp
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          : The retail CT_TW build registers the same Korean table for
*                 KOR/JPN/ENG and the same Traditional Chinese table for
*                 CT_TW/CT_HK. Its source was saved as CP949, so the compiler
*                 dropped the hanzi that CP949 lacks (檔, 戲, 畫, 窗); they are
*                 restored here.
*
*****************************************************************************/


#include "stdafx.h"
#include "DLMessageImp.h"


#define REG_MSG( k, s )		m_defMsgList[k] = s


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder
//
//////////////////////////////////////////////////////////////////////////


CDLMsgFinder::CDLMsgFinder( void )
{
	m_strDefMsg = _T( "Can not find the message." );
}

CDLMsgFinder::~CDLMsgFinder( void )
{
	Delete();
}

bool CDLMsgFinder::Create( void )
{
	RegString();

	return true;
}

void CDLMsgFinder::Delete( void )
{
	return;
}

CString CDLMsgFinder::FindString( eDL_MSG eMsg )
{
	mapdef_MsgList::iterator it = m_defMsgList.find( eMsg );

	if ( it == m_defMsgList.end() )
	{
		return m_strDefMsg;
	}

	return it->second;
}


//////////////////////////////////////////////////////////////////////////
//
// Korean table, shared by KOR and JPN
//
//////////////////////////////////////////////////////////////////////////


static void RegKoreanString( CDLMsgFinder::mapdef_MsgList& m_defMsgList )
{
	REG_MSG( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,		_T( "현재 런쳐를 이용할 수 없습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_DOWNLOAD_FAILED,				_T( "파일 다운로드가 실패하였습니다.\n우선 네트웍 설정 및 인터넷 접속 여부를 살펴보시기 바랍니다.\n그래도 이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_CJI_GRID_DOWNLOAD_FAILED,	_T( "파일 다운로드가 실패하였습니다.\n우선 네트웍 설정 및 인터넷 접속 여부를 살펴보시기 바랍니다.\n그래도 이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_PATCH_FAILED,				_T( "패치 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,		_T( "무결성 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_CLIENT,				_T( "드래곤볼 온라인이 이미 실행 중 입니다" ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_LAUNCHER,				_T( "드래곤볼 온라인 런쳐가 이미 실행 중 입니다." ) );
	REG_MSG( eDL_MSG_PREPARE_NEW_PATCH,					_T( "현재 새로운 버전의 패치가 준비중입니다.\n잠시후에 다시 접속하여 주시기 바랍니다." ) );
	REG_MSG( eDL_MSG_SELECT_RESOLUTION,					_T( "해상도를 선택해 주세요." ) );

	REG_MSG( eDL_MSG_UI_INTEGRITY,						_T( "무결성" ) );
	REG_MSG( eDL_MSG_UI_OPTION,							_T( "옵션" ) );
	REG_MSG( eDL_MSG_UI_FONT_NAME,						_T( "돋움" ) );

	REG_MSG( eDL_MSG_OPTION_TITLE,						_T( "게임 옵션" ) );
	REG_MSG( eDL_MSG_OPTION_GRAPHIC,					_T( "그래픽 설정" ) );
	REG_MSG( eDL_MSG_OPTION_RESOLUTION,					_T( "해상도 설정" ) );
	REG_MSG( eDL_MSG_OPTION_GAMMA,						_T( "밝기 조정" ) );
	REG_MSG( eDL_MSG_OPTION_GRAPHIC_QUALITY,			_T( "전체 그래픽 품질" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_LOW,				_T( "낮음" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_MIDDLE,				_T( "보통" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_HIGH,				_T( "높음" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_USER,				_T( "사용자" ) );
	REG_MSG( eDL_MSG_OPTION_WINDOW_MODE,				_T( "창 모드" ) );
	REG_MSG( eDL_MSG_OPTION_SOUND,						_T( "사운드 설정" ) );
	REG_MSG( eDL_MSG_OPTION_MAIN_VOLUME,				_T( "메인 볼륨" ) );
	REG_MSG( eDL_MSG_OPTION_BGM_VOLUME,					_T( "배경음 볼륨" ) );
	REG_MSG( eDL_MSG_OPTION_EFFECT_VOLUME,				_T( "효과음 볼륨" ) );
	REG_MSG( eDL_MSG_OPTION_ENV_VOLUME,					_T( "환경음 볼륨" ) );
	REG_MSG( eDL_MSG_OPTION_DESC,						_T( "* 게임내의 메뉴/옵션에서 더욱 상세한 옵션을\n\n   설정하실 수 있습니다." ) );
	REG_MSG( eDL_MSG_OPTION_APPLY,						_T( "적용" ) );
	REG_MSG( eDL_MSG_OPTION_DEFAULT,					_T( "기본값" ) );
	REG_MSG( eDL_MSG_OPTION_CLOSE,						_T( "닫기" ) );
}


//////////////////////////////////////////////////////////////////////////
//
// Traditional Chinese table, shared by CT_TW and CT_HK
//
//////////////////////////////////////////////////////////////////////////


static void RegChineseTraditionalString( CDLMsgFinder::mapdef_MsgList& m_defMsgList )
{
	REG_MSG( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,		_T( "現在無法使用Launcher。\n如持續發生相同問題時請聯絡客服。" ) );
	REG_MSG( eDL_MSG_ERROR_DOWNLOAD_FAILED,				_T( "檔案下載失敗。\n請先確認網路設定和連線環境。\n如持續發生相同問題時請聯絡客服。" ) );
	REG_MSG( eDL_MSG_ERROR_CJI_GRID_DOWNLOAD_FAILED,	_T( "檔案下載失敗。\n請先確認網路設定和連線環境。\n如持續發生相同問題時請聯絡客服。" ) );
	REG_MSG( eDL_MSG_ERROR_PATCH_FAILED,				_T( "更新失敗。\n如持續發生相同問題時請聯絡客服。" ) );
	REG_MSG( eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,		_T( "檔案比對失敗。\n如持續發生相同問題時請聯絡客服。" ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_CLIENT,				_T( "七龍珠Online正在執行中。" ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_LAUNCHER,				_T( "七龍珠Online Launcher正在執行中。" ) );
	REG_MSG( eDL_MSG_PREPARE_NEW_PATCH,					_T( "正在準備最新版本更新。\n請稍候重新連線。" ) );
	REG_MSG( eDL_MSG_SELECT_RESOLUTION,					_T( "請選擇解析度。" ) );

	REG_MSG( eDL_MSG_UI_INTEGRITY,						_T( "檔案比對" ) );
	REG_MSG( eDL_MSG_UI_OPTION,							_T( "設定" ) );
	REG_MSG( eDL_MSG_UI_FONT_NAME,						_T( "說明" ) );

	REG_MSG( eDL_MSG_OPTION_TITLE,						_T( "遊戲設定" ) );
	REG_MSG( eDL_MSG_OPTION_GRAPHIC,					_T( "畫質設定" ) );
	REG_MSG( eDL_MSG_OPTION_RESOLUTION,					_T( "解析度設定" ) );
	REG_MSG( eDL_MSG_OPTION_GAMMA,						_T( "亮度調整" ) );
	REG_MSG( eDL_MSG_OPTION_GRAPHIC_QUALITY,			_T( "全部畫質" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_LOW,				_T( "低" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_MIDDLE,				_T( "普通" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_HIGH,				_T( "高" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_USER,				_T( "使用者" ) );
	REG_MSG( eDL_MSG_OPTION_WINDOW_MODE,				_T( "視窗模式" ) );
	REG_MSG( eDL_MSG_OPTION_SOUND,						_T( "聲音設定" ) );
	REG_MSG( eDL_MSG_OPTION_MAIN_VOLUME,				_T( "主音量" ) );
	REG_MSG( eDL_MSG_OPTION_BGM_VOLUME,					_T( "背景音量" ) );
	REG_MSG( eDL_MSG_OPTION_EFFECT_VOLUME,				_T( "效果音量" ) );
	REG_MSG( eDL_MSG_OPTION_ENV_VOLUME,					_T( "環境音量" ) );
	REG_MSG( eDL_MSG_OPTION_DESC,						_T( "* 使用遊戲內選單/設定功能可進行細部設定。" ) );
	REG_MSG( eDL_MSG_OPTION_APPLY,						_T( "套用" ) );
	REG_MSG( eDL_MSG_OPTION_DEFAULT,					_T( "預設" ) );
	REG_MSG( eDL_MSG_OPTION_CLOSE,						_T( "關閉" ) );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_KOR
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_KOR::RegString( void )
{
	RegKoreanString( m_defMsgList );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_JPN
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_JPN::RegString( void )
{
	RegKoreanString( m_defMsgList );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_ENG
//
// The retail build registers the Korean table here as well; English is
// used instead so the ENG finder is readable.
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_ENG::RegString( void )
{
	REG_MSG( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,		_T( "The launcher is currently unavailable.\nIf this problem persists, please contact the administrator." ) );
	REG_MSG( eDL_MSG_ERROR_DOWNLOAD_FAILED,				_T( "The file download failed.\nPlease check your network settings and internet connection.\nIf this problem persists, please contact the administrator." ) );
	REG_MSG( eDL_MSG_ERROR_CJI_GRID_DOWNLOAD_FAILED,	_T( "The file download failed.\nPlease check your network settings and internet connection.\nIf this problem persists, please contact the administrator." ) );
	REG_MSG( eDL_MSG_ERROR_PATCH_FAILED,				_T( "The patch process failed.\nIf this problem persists, please contact the administrator." ) );
	REG_MSG( eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,		_T( "The integrity check failed.\nIf this problem persists, please contact the administrator." ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_CLIENT,				_T( "Dragon Ball Online is already running." ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_LAUNCHER,				_T( "The Dragon Ball Online Launcher is already running." ) );
	REG_MSG( eDL_MSG_PREPARE_NEW_PATCH,					_T( "A new version patch is currently being prepared.\nPlease try connecting again shortly." ) );
	REG_MSG( eDL_MSG_SELECT_RESOLUTION,					_T( "Please select a resolution." ) );

	REG_MSG( eDL_MSG_UI_INTEGRITY,						_T( "Integrity" ) );
	REG_MSG( eDL_MSG_UI_OPTION,							_T( "Option" ) );
	REG_MSG( eDL_MSG_UI_FONT_NAME,						_T( "Tahoma" ) );

	REG_MSG( eDL_MSG_OPTION_TITLE,						_T( "Game Options" ) );
	REG_MSG( eDL_MSG_OPTION_GRAPHIC,					_T( "Graphics" ) );
	REG_MSG( eDL_MSG_OPTION_RESOLUTION,					_T( "Resolution" ) );
	REG_MSG( eDL_MSG_OPTION_GAMMA,						_T( "Brightness" ) );
	REG_MSG( eDL_MSG_OPTION_GRAPHIC_QUALITY,			_T( "Overall quality" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_LOW,				_T( "Low" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_MIDDLE,				_T( "Normal" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_HIGH,				_T( "High" ) );
	REG_MSG( eDL_MSG_OPTION_QUALITY_USER,				_T( "Custom" ) );
	REG_MSG( eDL_MSG_OPTION_WINDOW_MODE,				_T( "Window mode" ) );
	REG_MSG( eDL_MSG_OPTION_SOUND,						_T( "Sound" ) );
	REG_MSG( eDL_MSG_OPTION_MAIN_VOLUME,				_T( "Main volume" ) );
	REG_MSG( eDL_MSG_OPTION_BGM_VOLUME,					_T( "Music volume" ) );
	REG_MSG( eDL_MSG_OPTION_EFFECT_VOLUME,				_T( "Effect volume" ) );
	REG_MSG( eDL_MSG_OPTION_ENV_VOLUME,					_T( "Ambient volume" ) );
	REG_MSG( eDL_MSG_OPTION_DESC,						_T( "* More detailed options are available\n\n   in the in-game option menu." ) );
	REG_MSG( eDL_MSG_OPTION_APPLY,						_T( "Apply" ) );
	REG_MSG( eDL_MSG_OPTION_DEFAULT,					_T( "Default" ) );
	REG_MSG( eDL_MSG_OPTION_CLOSE,						_T( "Close" ) );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_CT_TW
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_CT_TW::RegString( void )
{
	RegChineseTraditionalString( m_defMsgList );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_CT_HK
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_CT_HK::RegString( void )
{
	RegChineseTraditionalString( m_defMsgList );
}
