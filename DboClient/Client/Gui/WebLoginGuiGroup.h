/*****************************************************************************
 *
 * File			: WebLoginGuiGroup.h
 * Author		: Hong Sungbock
 * Copyright	: (аж)NTL
 * Date			: 2009. 2. 26
 * Abstract		: DBO Web Login stage gui group.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __WEB_LOGIN_GUI_GROUP_H__
#define __WEB_LOGIN_GUI_GROUP_H__

// core
#include "ceventhandler.h"

class CLogoGui;
class CContractGui;
class CDBOTitleGui;
class CDeliberationRankContainer;


class CWebLoginGuiGroup : public RWS::CEventHandler
{
public:
	CWebLoginGuiGroup();
	~CWebLoginGuiGroup();

	RwBool			Create();
	void			Destroy();

protected:
	virtual void		HandleEvents(RWS::CMsg &pMsg);

	int					CallBackMoviePlayStop();

protected:
	CLogoGui*				m_pLogoGui;
	CContractGui*			m_pContractGui;
	CDBOTitleGui*			m_pDBOTitle;
	CDeliberationRankContainer*	m_pDeliverationRankContainer;
};

#endif
