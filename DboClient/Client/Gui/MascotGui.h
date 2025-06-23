/*****************************************************************************
*
* File			: MascotGui.h
* Author		: Alejandro-HUB
* Copyright	: (주)NTL
* Date			: 2021. 5. 16
* Abstract		: DBO Mascot Summon/View Status gui.
*****************************************************************************
* Desc         : This is the window where the player can interact with their
*				 mascots.
*
*****************************************************************************/

#pragma once

// Presentation
#include "NtlPLGui.h"

// Simulation
#include "NtlSLDef.h"

// Core
#include "ceventhandler.h"

// Gui
#include "eventtimer.h"

// Client
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "Windowby3.h"

#include "NtlPLEmblemMaker.h"
#include <codecvt> 

// Constants
#define dMAX_MASCOT_SKILL 4
#define dMAX_MASCOT_SLOT	8
#define dMASCOT_INVALID_INDEX -1

//Classes and Struct
struct sMascot_TBLDAT;

class CMascotList;
class CMascotGui;
class CNtlSobMascot;

// MASCOT GUI
class CMascotGui : public CNtlPLGui, public RWS::CEventHandler
{

public:
    CMascotGui(const RwChar* pName);
    virtual ~CMascotGui();

    virtual VOID    Init();
    virtual RwBool  Create();
    virtual VOID    Destroy();
    virtual RwInt32 SwitchDialog(bool bOpen);

    struct MascotIcon
    {
        CRegularSlotGui		slot;			///< slot
        BYTE				byIndex;
        BYTE				byItemRank;
        DWORD				dwCurExp;
        DWORD				dwCurVP;
        DWORD				dwMaxVP;
        DWORD				dwNeedExp;
        TBLIDX				MascotTblidx;
        TBLIDX				Tblidx;
        BYTE				byUnknown;
        WORD				wunknown;
        WORD				wunknown2;
        TBLIDX				skillTblidx[4];
    };

    struct MascotRunTimeParameters
    {
        RwInt32 m_CurrentMascotSlotIndex; // Used to know which Mascot icon the mouse selected
        RwUInt32 m_CurrentMascotSerialId; // Used to get the Mascot avatar
    };

    struct MascotSkillIcon
    {
        CRegularSlotGui		slot;
        TBLIDX				skillTblidx;
    };
    enum
    {
        MASCOT_SLOT,
        SKILL_SLOT
    };

protected:
    // Event
    virtual void    HandleEvents(RWS::CMsg& pMsg);

    // Window Actions
    VOID    OnCloseBtnClicked(gui::CComponent* pComponent);
    VOID    OnMove(RwInt32 iOldX, RwInt32 iOldY);
    VOID    OnScrollChanged(RwInt32 nNewOffset);
    VOID    OnPaint();

    // Mouse Actions
    VOID    OnMouseDownMascot(const CKey& key);
    VOID    OnMouseDownSkill(const CKey& key);
    VOID    OnMouseEnterSlot(gui::CComponent* pComponent);
    VOID    OnMouseLeaveSlot(gui::CComponent* pComponent);
    VOID	OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);


    // MASCOT Actions
    VOID    OnMascotClicked(RwInt32 Slot);
    VOID    OnReMakeSkillBtnClicked(gui::CComponent* pComponent);
    VOID    OnSummonBtnClicked(gui::CComponent* pComponent);
    VOID    OnSummonEndBtnClicked(gui::CComponent* pComponent);
    VOID    OnMascotDelBtnClicked(gui::CComponent* pComponent);
    VOID    OnMascotFusionBtnClicked(gui::CComponent* pComponent);
    VOID    OnMascotRenameBtnClicked(gui::CComponent* pComponent);

    // Modals
    VOID    ShowItemInfoWindow(bool bIsShow, int slot = -1);
    VOID	ShowSkillInfoWindow(bool bIsShow, int slot = -1);

    // Effects
    VOID	FocusEffect(RwBool bPush, RwInt32 iSlotIdx = -1);
    VOID	FocusEffectSkill(RwBool bPush, RwInt32 iSlotIdx = -1);
    VOID	SelectEffect(RwBool bPush, RwInt32 iSlotIdx = -1);
    VOID	SelectEffectSkill(RwBool bPush, RwInt32 iSlotIdx = -1);
    VOID	Update(RwReal fElapsed);
    RwBool	CreateFlashEffect(RwInt32 nSlotIdx);

    // Slots
    RwInt32	PtinSlot(RwInt32 iSlotKind, RwInt32 iX, RwInt32 iY);

    // Custom methods
    void InitializeMascotIconsAndSkills();
    VOID RefreshList(RwInt32 iOffSet);
    VOID OpenMascotInfo(RwBool isShow, RwInt32 iSlotIdx);
    VOID RefreshSkillSlot();
    VOID CreateUITextureForSummonedMascot(RwInt32 summonedIndex);
    std::wstring stringToWString(const std::string& str);
    WCHAR* CMascotGui::stringToWChar(const std::string& str);
    int CMascotGui::ExtractIndex(const std::string& componentName);
    void CMascotGui::OutputDebugInfo(int tabID, const std::string& componentName, int index,
        const std::wstring& toolTipText, const std::string& descriptionText,
        const std::string& pageName);


protected:
    static CMascotGui* m_pInstance;			///< Objects

    // StaticBox Group
    gui::CStaticBox* m_sttbTitle;
    gui::CStaticBox* m_psttb_Info;
    gui::CStaticBox* m_psttb_SkillInfo;
    gui::CStaticBox* m_psttb_Summoned;
    gui::CStaticBox* m_psttb_Name;
    gui::CStaticBox* m_psttb_ClassType;
    gui::CStaticBox* m_psttb_Vp;
    gui::CStaticBox* m_psttb_Cp;

    // Button Group
    gui::CButton* m_pbtn_Summon;
    gui::CButton* m_pbtn_SummonEnd;
    gui::CButton* m_pbtn_MascotDel;
    gui::CButton* m_pbtn_MascotFusion;
    gui::CButton* m_pbtn_MascotRename;
    gui::CButton* m_pbtn_Close;
    gui::CButton* m_pbtn_MascotSkill[dMAX_MASCOT_SKILL];

    // ProgressBar Group
    gui::CProgressBar* m_ppgb_Vp;
    gui::CProgressBar* m_ppgb_Xp;

    // Panel Group
    gui::CPanel* m_pPn_IvnBack;
    gui::CPanel* m_pPn_Buff0;
    gui::CPanel* m_pPn_Buff1;
    gui::CPanel* m_pPn_Buff2;
    gui::CPanel* m_pPn_Buff3;
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

    // Panel map 
    gui::CPanel* m_pPn_IvnIcons[dMAX_MASCOT_SLOT] = {
        m_pPn_IvnIcon0, m_pPn_IvnIcon1, m_pPn_IvnIcon2, m_pPn_IvnIcon3,
        m_pPn_IvnIcon4, m_pPn_IvnIcon5, m_pPn_IvnIcon6, m_pPn_IvnIcon7
    };
    gui::CPanel* m_pPn_SkillIcons[dMAX_MASCOT_SLOT] = {
        m_pPn_SkillIcon0, m_pPn_SkillIcon1, m_pPn_SkillIcon2, m_pPn_SkillIcon3
    };

    // ScrollBar Group
    gui::CScrollBar* m_pscb_MascotScroll;

    // Slot Group
    gui::CSlot m_slotBtnClose;
    gui::CSlot m_slotBtnSummon;
    gui::CSlot m_slotBtnSummonEnd;
    gui::CSlot m_slotBtnMascotDel;
    gui::CSlot m_slotBtnMascotFusion;
    gui::CSlot m_slotBtnMascotRename;
    gui::CSlot m_slotMascotScrollChanged;
    gui::CSlot m_slotMascotScrollSliderMoved;
    gui::CSlot m_slotMouseWheel;
    gui::CSlot m_slotReMakeSkill[dMAX_MASCOT_SKILL];
    gui::CSlot m_slotSkillMouseEnter[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotSkillMouseLeave[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotSkillMouseDown[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotSkillPaint[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotSkillMove[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotMascotMouseEnter[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotMascotMouseLeave[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotMascotMouseDown[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotMascotPaint[dMAX_MASCOT_SLOT];
    gui::CSlot m_slotMascotMove[dMAX_MASCOT_SLOT];

    // Flash
    gui::CFlash* m_apflaEffect;

    // Surface
    CSurfaceGui	m_FocusEffect;
    CSurfaceGui	m_FocusEffect_Skill;
    CSurfaceGui	m_SelectEffect;
    CSurfaceGui	m_LiftEffect;
    CSurfaceGui	m_NotMascotSkill[dMAX_MASCOT_SKILL];
    CSurfaceGui	m_surMascot;

    // Icons
    MascotIcon			m_MascotIcon[dMAX_MASCOT_SLOT];
    MascotSkillIcon		m_MascotSkill[dMAX_MASCOT_SKILL];

    // Texture
    gui::CTexture m_texMascot;

    // Temporary UI mascot for preview (cleaned up when window closes, mascot summoned, or different mascot selected)
    class CNtlSobMascot* m_pTempUIMascot;

    // World mascot tracking (for proper cleanup on unsummon)
    class CNtlSobMascot* m_pWorldMascot;

    // Bool
    RwBool m_bFocus;
    RwBool m_bFocus_Skill;
    bool m_bShowItemInfo;
    bool m_bShowSkillInfo;
    bool m_bCurMaxExp;

    // Int
    RwInt32 m_iClickEffectedSlot;
    RwInt32	m_iClidkEffectedSlotSkill;
    RwInt32	m_iSelectedSlot;
    RwInt32	m_iSelectedIndex;
    RwInt32	m_iSelectedSlotSkill;
    RwInt32	m_iMouseDownSlot;
    RwInt32	m_iMouseDownSlotSkill;
    RwInt32	m_CurLine;
    RwUInt32 m_uiEffectFrame;
    RwInt32 m_SummonMascotIndex;
    RwInt32 m_CurrentSkillSlotIndex;

    // Model
    MascotRunTimeParameters m_CurrentMascotParamters;
};