/*****************************************************************************
*
* File			: MascotGui.h
* Author		: Alejandro-HUB
* Copyright	: (C)NTL
* Date			: 2021. 5. 16
* Abstract		: DBO Mascot Summon/View Status gui.
*****************************************************************************
* Desc         : Lists the registered mascots, previews the selected one and
*				 gives access to summoning, skills, fusion and sealing.
*
*****************************************************************************/

#pragma once

// Core
#include "ceventhandler.h"

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"
#include "NtlMascot.h"

// Client
#include "SurfaceGui.h"
#include "SlotGui.h"

#define dMAX_MASCOT_SKILL	DBO_MASCOT_MAX_SKILL_COUNT
#define dMAX_MASCOT_SLOT	8
#define dMASCOT_SLOT_ROW	4

class CNtlPLCharacter;
class CNtlPLCameraRenderTexture;

/**
* \brief Renders a mascot model into a texture for the gui, playing its idle animation
*/
class CMascotPreview
{
public:
	CMascotPreview();
	~CMascotPreview();

	void		SetModel(const RwChar* pModelKey);
	void		SetDistance(RwReal fDistance);		///< camera distance in model heights
	void		Destroy(void);
	const std::string&	GetModel(void) const { return m_strModel; }

	/// call once per frame before the camera renders, returns NULL until the model is loaded
	RwTexture*	Render(void);

private:
	void		SetupCamera(void);

	CNtlPLCharacter*			m_pCharacter;
	CNtlPLCameraRenderTexture*	m_pCamera;
	std::string					m_strModel;
	RwBool						m_bCameraReady;
	RwInt32						m_iMeasuredFrames;		///< frames rendered while the camera still frames the animated mesh
	RwBBox						m_bboxMesh;				///< mesh bounds over those frames
	RwBool						m_bMeshValid;
	RwReal						m_fDistance;
};

class CMascotGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CMascotGui(const RwChar* pName);
	virtual ~CMascotGui();

	VOID			Init();
	RwBool			Create();
	VOID			Destroy();
	RwInt32			SwitchDialog(bool bOpen);

	virtual VOID	Update(RwReal fElapsed);
	virtual VOID	UpdateBeforeCamera(RwReal fElapsed);

	BYTE			GetSelectedMascot(void) const { return m_bySelectedIndex; }

	/// the next click on a skill of the summoned mascot upgrades it with this item
	VOID			StartSkillGradeUp(BYTE byItemPlace, BYTE byItemPos);

protected:
	virtual VOID	HandleEvents(RWS::CMsg& msg);

	VOID			OnPaint();
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnScrollChanged(RwInt32 iOffset);
	VOID			OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);

	VOID			OnMouseEnterMascot(gui::CComponent* pComponent);
	VOID			OnMouseLeaveMascot(gui::CComponent* pComponent);
	VOID			OnMouseDownMascot(const CKey& key);
	VOID			OnMouseUpMascot(const CKey& key);
	VOID			OnMouseEnterSkill(gui::CComponent* pComponent);
	VOID			OnMouseLeaveSkill(gui::CComponent* pComponent);
	VOID			OnMouseDownSkill(const CKey& key);
	VOID			OnMouseUpSkill(const CKey& key);

	VOID			OnCloseBtnClicked(gui::CComponent* pComponent);
	VOID			OnSummonBtnClicked(gui::CComponent* pComponent);
	VOID			OnSummonEndBtnClicked(gui::CComponent* pComponent);
	VOID			OnDeleteBtnClicked(gui::CComponent* pComponent);
	VOID			OnFusionBtnClicked(gui::CComponent* pComponent);
	VOID			OnSkillBtnClicked(gui::CComponent* pComponent);

	VOID			RefreshList(VOID);
	VOID			RefreshInfo(VOID);
	VOID			RefreshSkills(VOID);
	VOID			Select(BYTE byIndex);

	sMASCOT_DATA_EX* GetSelectedData(VOID);
	sMASCOT_DATA_EX* GetSlotData(RwInt32 iSlot);
	RwInt32			GetMaxLine(VOID);
	RwBool			IsSkillSlotOpen(sMASCOT_DATA_EX* pData, RwInt32 iSkillSlot);

	VOID			ShowMascotInfoWnd(RwInt32 iSlot);
	VOID			ShowSkillInfoWnd(RwInt32 iSkillSlot);
	VOID			HideInfoWnd(VOID);

	VOID			DropOnMascot(RwInt32 iSlot);
	VOID			ClickSkill(RwInt32 iSkillSlot, RwBool bRightButton);

	RwInt32			FindComponent(gui::CComponent* pComponent, gui::CPanel** apPanel, RwInt32 iCount);

protected:
	// components
	gui::CStaticBox*	m_psttbTitle;
	gui::CStaticBox*	m_psttbInfo;
	gui::CStaticBox*	m_psttbSkillInfo;
	gui::CStaticBox*	m_psttbSummoned;
	gui::CStaticBox*	m_psttbName;
	gui::CStaticBox*	m_psttbGrade;
	gui::CStaticBox*	m_psttbVp;
	gui::CStaticBox*	m_psttbExp;

	gui::CButton*		m_pbtnSummon;
	gui::CButton*		m_pbtnSummonEnd;
	gui::CButton*		m_pbtnDelete;
	gui::CButton*		m_pbtnFusion;
	gui::CButton*		m_pbtnClose;
	gui::CButton*		m_pbtnSkill[dMAX_MASCOT_SKILL];

	gui::CProgressBar*	m_ppgbVp;
	gui::CProgressBar*	m_ppgbExp;

	gui::CScrollBar*	m_pscbMascot;

	gui::CPanel*		m_ppnlMascot[dMAX_MASCOT_SLOT];
	gui::CPanel*		m_ppnlSkill[dMAX_MASCOT_SKILL];

	gui::CFlash*		m_pflaSummoned;

	// signals
	gui::CSlot			m_slotClose;
	gui::CSlot			m_slotSummon;
	gui::CSlot			m_slotSummonEnd;
	gui::CSlot			m_slotDelete;
	gui::CSlot			m_slotFusion;
	gui::CSlot			m_slotSkillBtn[dMAX_MASCOT_SKILL];
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollMoved;
	gui::CSlot			m_slotWheel;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMascotEnter[dMAX_MASCOT_SLOT];
	gui::CSlot			m_slotMascotLeave[dMAX_MASCOT_SLOT];
	gui::CSlot			m_slotMascotDown[dMAX_MASCOT_SLOT];
	gui::CSlot			m_slotMascotUp[dMAX_MASCOT_SLOT];
	gui::CSlot			m_slotSkillEnter[dMAX_MASCOT_SKILL];
	gui::CSlot			m_slotSkillLeave[dMAX_MASCOT_SKILL];
	gui::CSlot			m_slotSkillDown[dMAX_MASCOT_SKILL];
	gui::CSlot			m_slotSkillUp[dMAX_MASCOT_SKILL];

	// slots and effects
	CRegularSlotGui		m_MascotSlot[dMAX_MASCOT_SLOT];
	BYTE				m_abySlotIndex[dMAX_MASCOT_SLOT];	///< mascot byIndex shown in each slot
	CRegularSlotGui		m_SkillSlot[dMAX_MASCOT_SKILL];
	CSurfaceGui			m_surSkillLock[dMAX_MASCOT_SKILL];
	gui::CRadarEffect	m_SkillCoolTime[dMAX_MASCOT_SKILL];

	CSurfaceGui			m_surFocus;
	CSurfaceGui			m_surSelect;

	// model preview
	CMascotPreview		m_Preview;
	gui::CTexture		m_texPreview;
	CSurfaceGui			m_surPreview;
	RwReal				m_fPreviewWait;			///< time the preview has rendered while hidden, its textures may still be streaming

	// state
	RwInt32				m_iCurLine;
	BYTE				m_bySelectedIndex;
	RwInt32				m_iFocusSlot;
	RwInt32				m_iMouseDownMascot;
	RwInt32				m_iMouseDownSkill;
	RwInt32				m_iFocusSkill;
	RwInt32				m_iInfoWndSlot;			///< slot whose tooltip is shown, skill slots are offset by dMAX_MASCOT_SLOT

	BYTE				m_byGradeUpItemPlace;
	BYTE				m_byGradeUpItemPos;
};
