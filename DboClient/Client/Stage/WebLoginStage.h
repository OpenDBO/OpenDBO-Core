/*****************************************************************************
 *
 * File			: WebLoginStage.h
 * Author		: Hong Sungbock
 * Copyright	: (аж)NTL
 * Date			: 2009. 2. 26
 * Abstract		: Web Login stage class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __WEB_LOGIN_STAGE_H__
#define __WEB_LOGIN_STAGE_H__

#include "NtlStage.h"
#include "ceventhandler.h"
#include "eventtimer.h"

class CNtlPLWorldEntity;
class CLogInStageState;
class CWebLoginGuiGroup;


class CWebLoginStage : public CNtlStage, public RWS::CEventHandler
{
	struct sRETRY_LOGIN
	{
		RwBool		bRetrying;
		RwBool		bSendPacket;
		RwReal		fElapsed;
	};

public:
	CWebLoginStage(const char *pStageName);
	~CWebLoginStage();

	virtual bool			Create();
	virtual void			Destroy();

	virtual void			HandleEvents(RWS::CMsg &pMsg);

protected:
	RwBool			CreateWorld();
	void			SetCameraPositioin();

	RwBool			Send_LoginPacket();
	void			Update_Retry_Login(RwReal fElapsed);

	void			EventProcUpdateTick(RwReal fElapsed);
	void			LogInServerConnect(RWS::CMsg &pMsg);
	void			LobbyServerConnect(RWS::CMsg &pMsg);
	void			LoginStageStateEnter(RWS::CMsg &pMsg);
	void			LoginStageStateExit(RWS::CMsg &pMsg);
	void			LogInStageTimeOutEventHandler(RWS::CMsg &msg);

	int				ActionGameExit();

protected:
	CLogInStageState*		m_pState;
	CWebLoginGuiGroup*		m_pGuiGroup;
	CNtlPLWorldEntity*		m_pWorldEntity;

	sRETRY_LOGIN			m_tRETRY_LOGIN;

	RwBool					m_bWorldAddCamera;
	RwUInt8					m_byCount_forCreateWorld;
};

#endif