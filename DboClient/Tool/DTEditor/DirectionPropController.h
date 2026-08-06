/******************************************************************************
* File			: DirectionPropController.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 8. 7
* Abstract		: 
*****************************************************************************
* Desc			: 컨트롤러 속성창
*****************************************************************************/

#pragma once

class CNtlSLTBCrowdController;


class CDirectionPropController : public CWnd
{
	DECLARE_DYNAMIC(CDirectionPropController)

public:
	CDirectionPropController();
	virtual ~CDirectionPropController();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);

	void				SetProperty(CNtlSLTBCrowdController* pController);

protected:
	CXTPPropertyGrid				m_PropGrid;
	CXTPPropertyGridItemCategory*	m_pCategoryItem;

	// ControllerType
	CXTPPropertyGridItemDouble*		m_pLifeTime;
	CXTPPropertyGridItemBool*		m_pLoop;

	// Entry position	
	CXTPPropertyGridItemDouble*		m_pEntryPositionX;
	CXTPPropertyGridItemDouble*		m_pEntryPositionY;
	CXTPPropertyGridItemDouble*		m_pEntryPositionZ;

	CNtlSLTBCrowdController*		m_pCurController;
};
