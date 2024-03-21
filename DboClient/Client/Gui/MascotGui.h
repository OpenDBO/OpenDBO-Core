/*****************************************************************************
*
* File			: MascotGui.h
* Author		: Alex
* Copyright	: (주)NTL
* Date			: 2021. 5. 16
* Abstract		: DBO Mascot Summon/View Status gui.
*****************************************************************************
* Desc         : This is the window where the player can interact with their
*				 mascots.
*
*****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NumberGui.h"
#include "ButtonList.h"

//sound
#include "GUISoundDefine.h"
#include <GuiLineTree.h>
#define MASCOT_LIST_MAX_VISIBLE 8

//Classes and Struct
struct sMascot_TBLDAT;

class CMascotList;
class CMascotGui;



//MASCOT LIST
class CMascotList : public CGuiLineTree
{
public:
	CMascotList();
	virtual ~CMascotList();

	RwBool	Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth);

	void	AddNode(sMascot_TBLDAT* pHelpTableData, RwBool bAddCategory = TRUE, RwBool bAddContent = TRUE);

	gui::CScrollBar* GetScrollBar() { return static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar()); }
};


// MASCOT GUI
class CMascotGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum EMascotListMode                ///< UI 
	{
		MASCOT_LIST,
	};

public:
	CMascotGui(void);
	CMascotGui(const RwChar* pName);
	virtual ~CMascotGui(void);

	virtual VOID	Init();
	virtual RwBool	Create();
	virtual VOID	Destroy();
	virtual RwInt32	SwitchDialog(bool bOpen);

	//Event
	virtual void    HandleEvents(RWS::CMsg& pMsg);

protected:
	VOID	OnCloseBtnClicked(gui::CComponent* pComponent);
	VOID    OnPetClicked(gui::CComponent* pComponent);
	VOID    OnSummonBtnClicked(gui::CComponent* pComponent);
	VOID    OnSummonEndBtnClicked(gui::CComponent* pComponent);
	VOID    OnMascotDelBtnClicked(gui::CComponent* pComponent);
	VOID    OnMascotFusionBtnClicked(gui::CComponent* pComponent);
	VOID    OnMascotRenameBtnClicked(gui::CComponent* pComponent);
	VOID	OnScrollChanged(RwInt32 nNewOffset);
	VOID	OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID	OnMouseLeave(gui::CComponent* pComponent);
	VOID    OnToggled(gui::CComponent* pComponent, bool bToggled);
	VOID	OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);
	void	OnCaptureMouseDown(const CKey& key);
	VOID	OnPaint(VOID);

protected:
	static CMascotGui* m_pInstance;			///< Objects


	//StaticBox
	gui::CStaticBox* m_sttbTitle;
	gui::CStaticBox* m_psttb_Info;
	gui::CStaticBox* m_psttb_SkillInfo;
	gui::CStaticBox* m_psttb_Summoned;
	gui::CStaticBox* m_psttb_Name;
	gui::CStaticBox* m_psttb_ClassType;
	gui::CStaticBox* m_psttb_Vp;
	gui::CStaticBox* m_psttb_Cp;

	//Btn
	gui::CButton* m_pbtn_Summon;
	gui::CButton* m_pbtn_SummonEnd;
	gui::CButton* m_pbtn_MascotDel;
	gui::CButton* m_pbtn_MascotFusion;
	gui::CButton* m_pbtn_MascotRename;
	gui::CButton* m_pbtn_Close;

	//ProgressBar
	gui::CProgressBar* m_ppgb_Vp;
	gui::CProgressBar* m_ppgb_Xp;

	//Panel
	gui::CPanel* m_pPn_IvnBack;
	gui::CPanel* m_pPn_Buff0;
	gui::CPanel* m_pPn_Buff1;
	gui::CPanel* m_pPn_Buff2;
	gui::CPanel* m_pPn_Buff3;
	//gui::CPanel* m_pPn_IvnIcon[MASCOT_LIST_MAX_VISIBLE];
	gui::CPanel* m_pPn_IvnIcon0;
	gui::CPanel* m_pPn_IvnIcon1;
	gui::CPanel* m_pPn_IvnIcon2;
	gui::CPanel* m_pPn_IvnIcon3;
	gui::CPanel* m_pPn_IvnIcon4;
	gui::CPanel* m_pPn_IvnIcon5;
	gui::CPanel* m_pPn_IvnIcon6;
	gui::CPanel* m_pPn_IvnIcon7;
	gui::CPanel* m_pPn_SkillIcon0;
	gui::CPanel* m_pPn_SkillIcon1;
	gui::CPanel* m_pPn_SkillIcon2;
	gui::CPanel* m_pPn_SkillIcon3;

	//ScrollBar
	gui::CScrollBar* m_pscb_MascotScroll;

	//List
	CButtonList<CMascotGui, sCommunityMember*> m_MascotList;

	gui::CSlot          m_slotClose;
	gui::CSlot          m_slotBtnSummon;
	gui::CSlot          m_slotBtnSummonEnd;
	gui::CSlot          m_slotBtnMascotDel;
	gui::CSlot          m_slotBtnMascotFusion;
	gui::CSlot          m_slotBtnMascotRename;
	gui::CSlot          m_slotMascotScroll;
	gui::CSlot          m_slotMascotScrollMove;
	gui::CSlot          m_slotMouseLeave;
	gui::CSlot          m_slotCaptureWheelMove;
	gui::CSlot          m_slotMouseMove[MASCOT_LIST_MAX_VISIBLE];
	gui::CSlot          m_slotBtnMouseLeave[MASCOT_LIST_MAX_VISIBLE];
	gui::CSlot          m_slotListWheelMove[MASCOT_LIST_MAX_VISIBLE];
	gui::CSlot          m_slotToggled[MASCOT_LIST_MAX_VISIBLE];
	gui::CSlot			m_slotCaptureMouseDown;

	EMascotListMode    m_eListMode;

};
