#ifndef __DROP_ITEM_INFO_GUI_LIST_H__
#define __DROP_ITEM_INFO_GUI_LIST_H__

#include "ceventhandler.h"


#include <unordered_set>
#include "DropItemInfoGui.h"

class CNtlSobWorldItem;

class CDropItemInfoGuiList : public CNtlPLGui, public RWS::CEventHandler
{
public:


	//! Contructor & Destructor
	CDropItemInfoGuiList(const RwChar* pName);
	~CDropItemInfoGuiList(VOID);

	//! Operation
	VOID	Init();
	RwBool	Create();
	VOID	Destroy(VOID);

	//! Events
	virtual VOID HandleEvents(RWS::CMsg& msg);


private:
	CDropItemInfoGui* m_pMainDropItemInfoGui;
	std::unordered_set<CDropItemInfoGui*> m_ListDropItemGui;
};

#endif//__DROP_ITEM_INFO_GUI_LIST_H__