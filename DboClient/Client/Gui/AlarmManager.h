////////////////////////////////////////////////////////////////////////////////
// File: AlarmManager.h
// Desc:
//
// 2006.05.18 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALARM_MANAGER_H__
#define __ALARM_MANAGER_H__

#include "DboDef.h"
#include "DisplayParser.h"

struct sALARMITEM
{
	sALARMITEM() 
	{
		Init();
	}

	sALARMITEM( RwInt32 flagAlarmType, RwInt32 workid = MBW_NONE, RwInt32 flagmsgboxstyle = MBTF_OK, RwBool bPopup = TRUE, RwReal fShowTime = 0.0f, RwBool bAcceptDuplicate = TRUE )
	{
		uType = flagAlarmType;	
		uWorkID = workid;
		flagMsgBoxStyle = flagmsgboxstyle;
		bMsgBoxPopup = bPopup;
		fMsgBoxShowTime = fShowTime;
	}

	VOID Init(VOID)
	{
		uType = 0x04;			// ALARM_TYPE_CHAT_WARN;
		uWorkID = MBW_NONE;
		flagMsgBoxStyle = MBTF_OK;
		bMsgBoxPopup = TRUE;
		fMsgBoxShowTime = 0.0f;	
		bAcceptDuplicate = TRUE;
		listCustomBtn.clear();
	}

	RwUInt32	uType;
	RwUInt32	uWorkID;
	RwUInt32	flagMsgBoxStyle;
	RwBool		bMsgBoxPopup;
	RwReal		fMsgBoxShowTime;
	RwBool		bAcceptDuplicate;
	std::list<sMsgBoxCustomBtn>	listCustomBtn;
};

class CAlarmManager
{
public:
//! Enumuration
	enum eALARMTYPE { ALARM_TYPE_MSGBOX			= 0x01, 
					  ALARM_TYPE_LOBBY_MSGBOX	= 0x02,
				      ALARM_TYPE_CHAT_WARN		= 0x04,
				      ALARM_TIMEFIXED_NOTIFY	= 0x08,
					  ALARM_ACTION_NOTIFY		= 0x10,
					  ALARM_TYPE_CHAT_NOTICE	= 0x20,
					  ALARM_TYPE_LOG			= 0x40,
					  ALARM_TYPE_CONSOLE		= 0x80,					  
					};

	typedef struct _stREPEATMESSAGECHECK
	{
		RwInt32 nLastChatMessageType;
		RwInt32	nLastChatMessageID;
		RwInt32 nRepeatCount;
	}stREPEATMESSAGECHECK;
	
//! Operation
	VOID	AlarmMessage( RwInt32 eStringID, RwBool bServerSync = FALSE, sMsgBoxData* pExData = NULL, RwReal fShowTime = 0.0f );
	VOID	AlarmMessage( WCHAR* szText, RwUInt32 uType = ALARM_TYPE_CHAT_WARN, RwUInt32 uWorkID = MBW_NONE, RwUInt32 uMsgBoxStyle = MBTF_OK,
						  RwBool bPopup = TRUE, RwReal fShowTime = 0.0f, sMsgBoxData* pExData = NULL, std::list<sMsgBoxCustomBtn>* plistCustomBtn = NULL );
	VOID	FormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, ... );
	VOID	vFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList );
	VOID	FormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, ... );
	VOID	vFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, RwReal fShowTime, sMsgBoxData* pExData, va_list ArgList );
	VOID	FormattedAlarmMessage( WCHAR* szText, ... ); 
	VOID	vFormattedAlarmMessage( WCHAR* szText, va_list ArgList );

	/// MsgBox인 경우 Msg 및 Data의 교체만 이루어짐. 시간속성 사용 불가. 
	VOID	UpdateAlarmMessage( RwInt32 eStringID, RwBool bServerSync /* = FALSE */, sMsgBoxData* pExData /* = NULL */ );
	VOID	UpdateFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, ... );
	VOID	UpdatevFormattedAlarmMessage( RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, va_list ArgList );
	///

	VOID	ResetRepeatCheck( VOID );	

	RwBool	ParseData( const CDisplayParser& dispParser, std::string strFilename, RwBool bServer );
	RwBool	ParseDataW( const CDisplayParser& dispParser, std::string strFilename, RwBool bServer );
	RwBool	ParseWorkID( std::string strFilename );
	RwBool	ParseWorkIDW( std::string strFilename );
		
//! Access
	static CAlarmManager* GetInstance(VOID);

private:
//! Implementation
	VOID	AlarmProcess( const WCHAR* pAlarmMsg, RwInt32 eStringID, RwBool bServerSync, sMsgBoxData* pExData, RwReal fShowTime, RwBool bMsgBoxUpdate = FALSE );
	VOID	SetServerHardCode(VOID);
	VOID	SetClientHardCode(VOID);
	RwBool	RepeatCheck( eALARMTYPE eAlarmType, RwInt32 eStringID, RwBool bServerSync );
	VOID	ErrorMessage( const RwChar* szID, const RwChar* szErrText );
	VOID	ErrorMessage( const WCHAR* wszID, const RwChar* szErrText );

//! Variable
	std::map<RwInt32,sALARMITEM> m_mapServerAlarm;
	std::map<RwInt32,sALARMITEM> m_mapClientAlarm;	
	stREPEATMESSAGECHECK		 m_RepeatMessageCheck;

	std::map<std::string,RwInt32>  m_mapWorkID;
	std::map<std::wstring,RwInt32> m_mapWorkIDW;

//! Prevented Constructor
	CAlarmManager(VOID);
};

static CAlarmManager* GetAlarmManager(VOID)
{
	return CAlarmManager::GetInstance();
};

#endif//__ALARM_MANAGER_H__