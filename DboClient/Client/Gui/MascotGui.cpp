#include "precomp_dboclient.h"
#include "MascotGui.h"
// core
#include "NtlDebug.h"

// gui
#include "NtlPLGuiManager.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"

// table
#include "MascotGradeTable.h"

// dbo
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include <InfoWndManager.h>
#include "IconMoveManager.h"
#include "NtlSobCharProxy.h"

#define dMASCOTEX_CON_START_X 25
#define dMASCOTEX_CON_START_Y 343

#define dMASCOTEX_SLOT_GAP_HORI_X 75
#define dMASCOTEX_SLOT_GAP_HORI_Y 41

#define dMASCOTEX_SKILL_X 236
#define dMASCOTEX_SKILL_Y 54

#define dMASCOTEX_SLOT_SKILL_Y 62

CMascotGui::CMascotGui(const RwChar* pName)
    :CNtlPLGui(pName)
{
    Init();
}

CMascotGui::~CMascotGui()
{
}

void CMascotGui::Init()
{
    // Bool
    m_bShowItemInfo = false;
    m_bShowSkillInfo = false;
    m_bFocus = false;
    m_bFocus_Skill = false;
    m_bCurMaxExp = false;

    // Int
    m_CurLine = 0;
    m_uiEffectFrame = 0;
    m_iSelectedIndex = dMASCOT_INVALID_INDEX;
    m_SummonMascotIndex = dMASCOT_INVALID_INDEX;
    m_iClickEffectedSlot = dMASCOT_INVALID_INDEX;
    m_iClidkEffectedSlotSkill = dMASCOT_INVALID_INDEX;
    m_iSelectedSlot = dMASCOT_INVALID_INDEX;
    m_iSelectedSlotSkill = dMASCOT_INVALID_INDEX;
    m_iMouseDownSlot = dMASCOT_INVALID_INDEX;
    m_iMouseDownSlotSkill = dMASCOT_INVALID_INDEX;

    // Pointer
    m_apflaEffect = NULL;
}

RwBool CMascotGui::Create()
{
    NTL_FUNCTION("CMascotGui::Create");
    if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotEx.srf", "gui\\MascotEx.frm"))
        return FALSE;

    CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());
    m_pThis = (gui::CDialog*)GetComponent("dlgMain");
    m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

    // StaticBox
    m_sttbTitle = (gui::CStaticBox*)GetComponent("Stb_Title");
    m_psttb_Info = (gui::CStaticBox*)GetComponent("Stb_Info");
    m_psttb_SkillInfo = (gui::CStaticBox*)GetComponent("Stb_SkillInfo");
    m_psttb_Summoned = (gui::CStaticBox*)GetComponent("Stb_Summoned");
    m_psttb_Name = (gui::CStaticBox*)GetComponent("Stb_Name");
    m_psttb_ClassType = (gui::CStaticBox*)GetComponent("Stb_ClassType");
    m_psttb_Vp = (gui::CStaticBox*)GetComponent("Stb_Vp");
    m_psttb_Cp = (gui::CStaticBox*)GetComponent("Stb_Cp");

    // Btn
    m_pbtn_Summon = (gui::CButton*)GetComponent("Btn_Summon");
    m_pbtn_SummonEnd = (gui::CButton*)GetComponent("Btn_SummonEnd");
    m_pbtn_MascotDel = (gui::CButton*)GetComponent("Btn_MascotDel");
    m_pbtn_MascotFusion = (gui::CButton*)GetComponent("Btn_MascotFusion");
    m_pbtn_MascotRename = (gui::CButton*)GetComponent("Btn_MascotRename");
    m_pbtn_Close = (gui::CButton*)GetComponent("Btn_Close");

    // ProgressBar
    m_ppgb_Vp = (gui::CProgressBar*)GetComponent("Pgb_Vp");
    m_ppgb_Xp = (gui::CProgressBar*)GetComponent("Pgb_Xp");
    m_ppgb_Vp->SetPos(0);
    m_ppgb_Xp->SetPos(0);

    // Panel
    m_pPn_IvnBack = (gui::CPanel*)GetComponent("Pn_IvnBack");
    m_pPn_Buff0 = (gui::CPanel*)GetComponent("Pn_Buff0");
    m_pPn_Buff1 = (gui::CPanel*)GetComponent("Pn_Buff1");
    m_pPn_Buff2 = (gui::CPanel*)GetComponent("Pn_Buff2");
    m_pPn_Buff3 = (gui::CPanel*)GetComponent("Pn_Buff3");
    m_pPn_IvnIcon0 = (gui::CPanel*)GetComponent("Pn_IvnIcon0");
    m_pPn_IvnIcon1 = (gui::CPanel*)GetComponent("Pn_IvnIcon1");
    m_pPn_IvnIcon2 = (gui::CPanel*)GetComponent("Pn_IvnIcon2");
    m_pPn_IvnIcon3 = (gui::CPanel*)GetComponent("Pn_IvnIcon3");
    m_pPn_IvnIcon4 = (gui::CPanel*)GetComponent("Pn_IvnIcon4");
    m_pPn_IvnIcon5 = (gui::CPanel*)GetComponent("Pn_IvnIcon5");
    m_pPn_IvnIcon6 = (gui::CPanel*)GetComponent("Pn_IvnIcon6");
    m_pPn_IvnIcon7 = (gui::CPanel*)GetComponent("Pn_IvnIcon7");
    m_pPn_SkillIcon0 = (gui::CPanel*)GetComponent("Pn_SkillIcon0");
    m_pPn_SkillIcon1 = (gui::CPanel*)GetComponent("Pn_SkillIcon1");
    m_pPn_SkillIcon2 = (gui::CPanel*)GetComponent("Pn_SkillIcon2");
    m_pPn_SkillIcon3 = (gui::CPanel*)GetComponent("Pn_SkillIcon3");

    // ScrollBar
    m_pscb_MascotScroll = (gui::CScrollBar*)GetComponent("Scb_MascotScroll");
    m_pscb_MascotScroll->SetMaxValue(8);

    // From local_data.dat
    m_pbtn_Summon->SetToolTip(GetDisplayStringManager()->GetString("DST_RECALL")); 
    m_pbtn_SummonEnd->SetToolTip(GetDisplayStringManager()->GetString("DST_RELEASE"));
    m_pbtn_MascotDel->SetToolTip(GetDisplayStringManager()->GetString("DST_DELETE"));
    m_pbtn_MascotFusion->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));
    m_pbtn_MascotRename->SetToolTip(stringToWString("Raname your mascot. -NOT IMPLEMENTED-"));

    // Effects
    m_FocusEffect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect")); // The mouse is over the icon effect
    m_SelectEffect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotPitchEffect")); // Selected item effect
    m_FocusEffect_Skill.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect")); // The mouse is over the skill effect
    m_LiftEffect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect")); // Effect when the mouse lifts up the icon (hover effect)

    // Events - Btn
    m_slotBtnClose = m_pbtn_Close->SigClicked().Connect(this, &CMascotGui::OnCloseBtnClicked);
    m_slotBtnSummon = m_pbtn_Summon->SigClicked().Connect(this, &CMascotGui::OnSummonBtnClicked);
    m_slotBtnSummonEnd = m_pbtn_SummonEnd->SigClicked().Connect(this, &CMascotGui::OnSummonEndBtnClicked);
    m_slotBtnMascotDel = m_pbtn_MascotDel->SigClicked().Connect(this, &CMascotGui::OnMascotDelBtnClicked);
    m_slotBtnMascotFusion = m_pbtn_MascotFusion->SigClicked().Connect(this, &CMascotGui::OnMascotFusionBtnClicked);
    m_slotBtnMascotRename = m_pbtn_MascotRename->SigClicked().Connect(this, &CMascotGui::OnMascotRenameBtnClicked);

    // Events - Scroll
    m_slotMascotScrollChanged = m_pscb_MascotScroll->SigValueChanged().Connect(this, &CMascotGui::OnScrollChanged);
    m_slotMascotScrollSliderMoved = m_pscb_MascotScroll->SigSliderMoved().Connect(this, &CMascotGui::OnScrollChanged);
    m_slotMouseWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CMascotGui::OnMouseWheel);

    // Events - Mouse
    m_slotMouseDown = m_pThis->SigMouseDown().Connect(this, &CMascotGui::OnMouseDown);
    m_slotMouseUp = m_pThis->SigMouseUp().Connect(this, &CMascotGui::OnMouseUp);
    m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CMascotGui::OnMouseMove);
    m_slotMove = m_pThis->SigMove().Connect(this, &CMascotGui::OnMove);
    m_slotPaint = m_pThis->SigPaint().Connect(this, &CMascotGui::OnPaint);

    // Window Name
    m_sttbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
    m_sttbTitle->SetText(GetDisplayStringManager()->GetString("DST_MASCOT"));

    // Set Name of the Buttons
    m_pbtn_Summon->SetText(GetDisplayStringManager()->GetString("DST_RECALL"));
    m_pbtn_SummonEnd->SetText(GetDisplayStringManager()->GetString("DST_RELEASE"));
    m_pbtn_MascotDel->SetText(GetDisplayStringManager()->GetString("DST_DELETE"));
    m_pbtn_MascotFusion->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));
    m_pbtn_MascotRename->SetText(stringToWString("C.Name"));

    // Icons and Skills
    InitializeMascotIconsAndSkills();
    
    // Gui Manager
    GetNtlGuiManager()->AddUpdateFunc(this);

    // Link Events
    LinkMsg(g_EventMascotRegister);
    LinkMsg(g_EventPickedUpHide);
    LinkMsg(g_EventMascotDelete);
    LinkMsg(g_EventMascotSummon);
    LinkMsg(g_EventMascotUnSummon);

    Show(false);

    NTL_RETURN(TRUE);
}

// Function to convert std::string to std::wstring
std::wstring CMascotGui::stringToWString(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

void CMascotGui::InitializeMascotIconsAndSkills()
{
    // Initialize mascot icon slots
    RwInt32 iIconX = dMASCOTEX_CON_START_X;
    RwInt32 iIconY = dMASCOTEX_CON_START_Y;

    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        // Set up mascot icon slots and positions
        m_MascotIcon[i].slot.Create(m_pThis, DIALOG_MASCOT, REGULAR_SLOT_ITEM_TABLE);
        m_MascotIcon[i].slot.SetPosition_fromParent((i + 1) % 4 == 0 ? iIconX - 0.5 : iIconX, iIconY);

        (i + 1) % 4 == 0 ? iIconX = dMASCOTEX_CON_START_X : iIconX += dMASCOTEX_SLOT_GAP_HORI_X;
        (i + 1) % 4 == 0 ? iIconY += dMASCOTEX_SLOT_GAP_HORI_Y : 0;
    }

    // Initialize skill icons and buttons
    RwInt32 iSkillIconY = dMASCOTEX_SKILL_Y;
    RwInt32 iButtonY = dMASCOTEX_SKILL_Y + 15;
    CRectangle rect;

    for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
    {
        m_MascotSkill[i].slot.Create(m_pThis, DIALOG_MASCOT, REGULAR_SLOT_SKILL_TABLE);
        m_MascotSkill[i].slot.SetPosition_fromParent(dMASCOTEX_SKILL_X, iSkillIconY);

        m_NotMascotSkill[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect"));
        m_NotMascotSkill[i].SetPositionfromParent(dMASCOTEX_SKILL_X, iSkillIconY);

        rect.SetRectWH(dMASCOTEX_SKILL_X + 40, iButtonY, 19, 19);
        m_pbtn_MascotSkill[i] = NTL_NEW gui::CButton(rect, "",
            GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfPlusBtnUp"),
            GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfPlusBtnDown"),
            GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfPlusBtnDown"),
            GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfPlusBtnFocus"),
            NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
            GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager());

        m_pbtn_MascotSkill[i]->Enable(false);
        m_pbtn_MascotSkill[i]->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_TITLE"));

        iButtonY += dMASCOTEX_SLOT_SKILL_Y;
        iSkillIconY += dMASCOTEX_SLOT_SKILL_Y;

        m_slotReMakeSkill[i] = m_pbtn_MascotSkill[i]->SigClicked().Connect(this, &CMascotGui::OnReMakeSkillBtnClicked);
    }

    // Initialize flash effect
    CRectangle flashEffectRect;
    flashEffectRect.SetRectWH(0, 0, 32, 32);
    m_apflaEffect = NTL_NEW gui::CFlash(flashEffectRect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "Skill_action.swf");

    m_apflaEffect->Show(false);
    m_apflaEffect->Enable(false);
}


void CMascotGui::Destroy()
{
    // Remove Mascot icons
    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
        m_MascotIcon[i].slot.Destroy();

    // Unlink Events
    UnLinkMsg(g_EventMascotRegister);
    UnLinkMsg(g_EventPickedUpHide);
    UnLinkMsg(g_EventMascotDelete);
    UnLinkMsg(g_EventMascotSummon);
    UnLinkMsg(g_EventMascotUnSummon);

    NTL_FUNCTION("CMascotGui::Destroy");
    CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy();
    gui::CResourceManager* pResMgr = CNtlPLGuiManager::GetInstance()->GetReourceManager();
    GetNtlGuiManager()->GetReourceManager()->RemovePage("gui\\mascotex.rsr");
}

RwInt32 CMascotGui::SwitchDialog(bool bOpen)
{
    // Check if the dialog is being opened
    if (bOpen)
    {
        // Refresh the list based on the current line
        RefreshList(m_CurLine);

        // Show the dialog
        Show(bOpen);
    }
    else
    {
        // Close the mascot fusion dialog if it is currently open
        if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_FUSION))
            GetDialogManager()->CloseDialog(DIALOG_MASCOT_FUSION);

        // Close the mascot skill remake dialog if it is currently open
        if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_SKILL_REMAKE))
            GetDialogManager()->CloseDialog(DIALOG_MASCOT_SKILL_REMAKE);

        // Hide item info window
        ShowItemInfoWindow(false);

        // Hide skill info window
        ShowSkillInfoWindow(false);

        // Show the dialog (which will actually hide it in this case since bOpen is false)
        Show(bOpen);
    }

    // Return 1 indicating the method completed successfully
    return 1;
}

void CMascotGui::HandleEvents(RWS::CMsg& msg)
{
    NTL_FUNCTION("CMascotGui::HandleEvents"); // Start of the function for handling events

    // Check if the event is a mascot registration
    if (msg.Id == g_EventMascotRegister)
    {
        SDboEventMascotRegister* pData = reinterpret_cast<SDboEventMascotRegister*>(msg.pData);

        // If the mascot dialog is open, refresh the list of mascots
        if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT))
        {
            RefreshList(m_CurLine);
        }
        else
        {
            // If the dialog is not open, open the mascot dialog
            GetDialogManager()->OpenDialog(DIALOG_MASCOT);
        }
    }
    // Check if the event is for picking up and hiding an item
    else if (msg.Id == g_EventPickedUpHide)
    {
        RwInt32 nSlotIdx = (RwInt32)msg.pData;

        // If the slot index is not for a skill, exit the function
        if (nSlotIdx != PLACE_SKILL)
            return;

        // Hide the skill selection effect
        SelectEffectSkill(false);
    }
    // Check if the event is for deleting a mascot
    else if (msg.Id == g_EventMascotDelete)
    {
        // Refresh the list of mascots
        RefreshList(m_CurLine);
    }
    // Check if the event is for summoning a mascot
    else if (msg.Id == g_EventMascotSummon)
    {
        SDboEventMascotOperate* pData = reinterpret_cast<SDboEventMascotOperate*>(msg.pData);
        m_SummonMascotIndex = pData->index; // Store the index of the summoned mascot
        RefreshSkillSlot(); // Refresh the skill slot UI
        // Set the summoned status text
        m_psttb_Summoned->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_STATUS_SUBTITLE_SUMMON"));  // The mascot has been successfully summoned.
        // Create the flash effect for the summoned mascot
        CreateFlashEffect(pData->index);
    }
    // Check if the event is for unsummoning a mascot
    else if (msg.Id == g_EventMascotUnSummon)
    {
        m_SummonMascotIndex = dMASCOT_INVALID_INDEX; // Reset the summoned mascot index
        RefreshSkillSlot(); // Refresh the skill slot UI
        m_psttb_Summoned->Clear(); // Clear the summoned status text

        // Hide and disable the flash effect for the unsummoned mascot
        m_apflaEffect->Show(false);
        m_apflaEffect->Enable(false);
    }

    NTL_RETURNVOID(); // End of function, returning void
}

// Method to handle the event when the "ReMake Skill" button is clicked
VOID CMascotGui::OnReMakeSkillBtnClicked(gui::CComponent* pComponent)
{
    // Prepare to switch to the skill remake GUI

    // Get the current position of the dialog
    CRectangle rect = GetPosition();

    // Retrieve the dialog for skill remake and set its position relative to the current dialog
    CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_MASCOT_SKILL_REMAKE);
    pPLGui->SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);

    // Switch to the skill remake dialog
    GetDialogManager()->SwitchDialog(DIALOG_MASCOT_SKILL_REMAKE);
}

VOID CMascotGui::OnPaint()
{
    // Paint each mascot slot
    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        m_MascotIcon[i].slot.Paint();
    }
    // Paint each mascot skill slot and render non-mascot skills
    for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
    {
        m_MascotSkill[i].slot.Paint();
        m_NotMascotSkill[i].Render();
    }
    // Render the selection effects if a slot or skill is selected
    if (m_iSelectedSlot != dMASCOT_INVALID_INDEX)
        m_SelectEffect.Render();

    if (m_iSelectedSlotSkill != dMASCOT_INVALID_INDEX)
        m_LiftEffect.Render();

    // Render focus effects based on whether the GUI has focus
    if (m_bFocus)
        m_FocusEffect.Render();

    if (m_bFocus_Skill)
        m_FocusEffect_Skill.Render();

    // Render the mascot surface
    m_surMascot.Render();
}

VOID CMascotGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
    // Determine if the mouse is within a mascot slot
    RwInt32 iPtinSlot = PtinSlot(nX, nY);

    // Determine if the mouse is within a skill slot
    RwInt32 iPtinSlotSkill = PtinSlotSkill(nX, nY);
    if (iPtinSlotSkill != dMASCOT_INVALID_INDEX)
    {
        // Highlight the skill if within the slot and show skill info
        FocusEffectSkill(true, iPtinSlotSkill);
        if (m_MascotSkill[iPtinSlotSkill].slot.GetSkillTable())
            ShowSkillInfoWindow(true, iPtinSlotSkill);
    }
    else {
        // Remove highlight and hide skill info if outside the slot
        FocusEffectSkill(false, iPtinSlotSkill);
        ShowSkillInfoWindow(false, iPtinSlotSkill);
    }

    // Highlight the mascot if within a slot and show item info
    if (iPtinSlot != dMASCOT_INVALID_INDEX)
    {
        FocusEffect(true, iPtinSlot);
        if (m_MascotIcon[iPtinSlot].slot.GetItemTable())
            ShowItemInfoWindow(true, iPtinSlot);
    }
    else {
        // Remove highlight and hide item info if outside the slot
        FocusEffect(false, iPtinSlot);
        ShowItemInfoWindow(false, iPtinSlot);
    }
}

// Handles mouse button down events.
// Captures the mouse if the focus is on this GUI and checks if any mascot slots or skill slots are clicked.
// Sets the clicked slot and initiates visual effects.
VOID CMascotGui::OnMouseDown(const CKey& key)
{
    gui::CGUIManager* pGuiMgr = CNtlPLGuiManager::GetInstance()->GetGuiManager();
    if (pGuiMgr->GetFocus() == m_pThis)
        RaiseLinked();

    if (GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN)
        return;

    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        // Check if the mascot icon has a valid serial and is clicked.
        if (m_MascotIcon[i].slot.GetSerial() != INVALID_SERIAL_ID &&
            m_MascotIcon[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
        {
            m_iMouseDownSlot = i;
            m_pThis->CaptureMouse();

            // Set the clicked effect for the mascot icon.
            m_iClickEffectedSlot = i;
            m_MascotIcon[m_iClickEffectedSlot].slot.ClickEffect(true);

            return;
        }
    }

    for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
    {
        if (m_MascotSkill[i].slot.GetSerial() != INVALID_SERIAL_ID &&
            m_MascotSkill[i].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) &&
            m_SummonMascotIndex == m_MascotIcon[m_iSelectedSlot].byIndex
            ) {
            m_iMouseDownSlotSkill = i;
            m_pThis->CaptureMouse();

            // Set the clicked effect for the skill icon.
            m_iClidkEffectedSlotSkill = i;
            m_MascotSkill[m_iClidkEffectedSlotSkill].slot.ClickEffect(true);
        }
    }
}

// Handles mouse button up events.
// Releases the mouse and checks for valid clicks on mascot and skill slots.
// Executes actions based on the type of button clicked (left or right).
VOID CMascotGui::OnMouseUp(const CKey& key)
{
    m_pThis->ReleaseMouse();

    // Clear the click effect for the mascot icon if it was clicked.
    if (m_iClickEffectedSlot != dMASCOT_INVALID_INDEX)
    {
        m_MascotIcon[m_iClickEffectedSlot].slot.ClickEffect(false);
        m_iClickEffectedSlot = dMASCOT_INVALID_INDEX;
    }

    // Clear the click effect for the skill icon if it was clicked.
    if (m_iClidkEffectedSlotSkill != dMASCOT_INVALID_INDEX)
    {
        m_MascotSkill[m_iClidkEffectedSlotSkill].slot.ClickEffect(false);
        m_iClidkEffectedSlotSkill = dMASCOT_INVALID_INDEX;
    }

    if (!IsShow())
    {
        m_iMouseDownSlot = dMASCOT_INVALID_INDEX;
        m_iMouseDownSlotSkill = dMASCOT_INVALID_INDEX;
        return;
    }

    // Check for valid clicks on the mascot icon and execute appropriate actions.
    if (m_MascotIcon[m_iMouseDownSlot].slot.GetSerial() != INVALID_SERIAL_ID &&
        m_MascotIcon[m_iMouseDownSlot].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
    {
        if (key.m_nID == UD_LEFT_BUTTON) // Left button click
        {
            // Select and open the mascot info window.
            SelectEffect(true, m_iMouseDownSlot);
            OpenMascotInfo(true, m_iMouseDownSlot);
            OnMascotClicked(m_iMouseDownSlot);
        }
        else if (key.m_nID == UD_RIGHT_BUTTON) // Right button click
        {
            SelectEffect(true, m_iMouseDownSlot);
            OnSummonBtnClicked(NULL);
            OpenMascotInfo(true, m_iMouseDownSlot);
            OnMascotClicked(m_iMouseDownSlot);

            // The following lines of code are responsible for handling item selection in the gift shop interface.
            // The item index is calculated based on the current page and the selected mouse-down slot.
            // This functionality appears to be related to registering an item for an event, either for a maximum item action
            // (e.g., gifting the entire stack) or for a regular item action (e.g., gifting a single item).
            //
            // The code is commented out possibly because the item registration logic was not fully implemented,
            // may have caused unintended behavior, or was deemed unnecessary for the current functionality.
            //
            // RwInt32 iItemIndex = (m_iCurPage * dMAX_ITEM_PANEL) + m_iMouseDownSlot;

            // if (key.m_dwVKey == UD_MK_CONTROL)
            // {
            //     // If the Control key is pressed, register a gift shop event for the maximum item action.
            //     CDboEventGenerator::GiftShopEvent(eGIFTSHOP_EVENT_REG_ITEM_MAX,
            //         m_aShopItem[m_iCurTab][iItemIndex].hItem,
            //         m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
            //         (wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
            //         m_iCurTab, iItemIndex, m_aShopItem[m_iCurTab][iItemIndex].pITEM_DATA->byMax_Stack);
            // }
            // else
            // {
            //     // Otherwise, register a gift shop event for a regular item action.
            //     CDboEventGenerator::GiftShopEvent(eGIFTSHOP_EVENT_REG_ITEM,
            //         m_aShopItem[m_iCurTab][iItemIndex].hItem,
            //         m_aShopItem[m_iCurTab][iItemIndex].uiPrice,
            //         (wchar_t*)m_aShopItem[m_iCurTab][iItemIndex].wstrItemName.c_str(),
            //         m_iCurTab, iItemIndex, 1);
            // }
        }
    }

    // Check for valid clicks on the skill icon and execute appropriate actions.
    if (m_MascotSkill[m_iMouseDownSlotSkill].slot.GetSerial() != INVALID_SERIAL_ID &&
        m_MascotSkill[m_iMouseDownSlotSkill].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY))
    {
        if (m_SummonMascotIndex == m_MascotIcon[m_iSelectedSlot].byIndex)
        {
            if (key.m_nID == UD_LEFT_BUTTON) // Left button click
            {
                if (GetIconMoveManager()->IsActive())
                {
                    if (GetIconMoveManager()->GetSrcSerial() == m_MascotSkill[m_iMouseDownSlotSkill].slot.GetSerial())
                        GetIconMoveManager()->IconMoveEnd();
                }
                else if (GetIconMoveManager()->IconMovePickUp(m_MascotSkill[m_iMouseDownSlotSkill].slot.GetSerial(), PLACE_SKILL, 0, 0, m_MascotSkill[m_iMouseDownSlotSkill].slot.GetTexture()))
                {
                    SelectEffectSkill(true, m_iMouseDownSlotSkill);
                }
            }
            else if (key.m_nID == UD_RIGHT_BUTTON) { // Right button click
                // Logic for using the skill (not implemented in the provided code).
                // Logic_UseProc(m_MascotSkill[m_iMouseDownSlotSkill].slot.GetSerial());
            }
        }
    }

    // Reset mouse down slots after processing.
    m_iMouseDownSlot = dMASCOT_INVALID_INDEX;
    m_iMouseDownSlotSkill = dMASCOT_INVALID_INDEX;
}

// Handles mouse wheel events to scroll through the mascot GUI
VOID CMascotGui::OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
    // Check if the mascot scroll bar is visible; if not, exit the function
    if (m_pscb_MascotScroll->IsVisible() == false)
        return;

    // Initialize the mouse down slot to an invalid index
    m_iMouseDownSlot = dMASCOT_INVALID_INDEX;

    // Calculate the scroll offset based on the scroll bar's maximum value and the delta from the mouse wheel
    int nOffset = (int)((float)m_pscb_MascotScroll->GetMaxValue() * 0.5f) * sDelta;

    // Capture the mouse wheel movement and update the scroll bar position accordingly
    CAPTURE_MOUSEWHEELMOVE_SCROLLBAR(IsShow(), m_pThis, m_pThis, m_pscb_MascotScroll, nOffset, pos);
}

VOID CMascotGui::OnScrollChanged(RwInt32 iOffset)
{
    RefreshList(iOffset);
}

VOID CMascotGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
    // Get the current screen rectangle for the GUI
    CRectangle rtScreen = m_pThis->GetScreenRect();

    // Update the parent position for each mascot slot
    for (int i = 0; i < dMAX_MASCOT_SLOT; ++i) {
        m_MascotIcon[i].slot.SetParentPosition(rtScreen.left, rtScreen.top);
    }

    // Update the parent position for each mascot skill slot and adjust non-mascot skill position
    for (int i = 0; i < dMAX_MASCOT_SKILL; ++i) {
        m_MascotSkill[i].slot.SetParentPosition(rtScreen.left, rtScreen.top);
        m_NotMascotSkill[i].SetRectWH(rtScreen.left + m_MascotSkill[i].slot.GetX_fromParent(), rtScreen.top + m_MascotSkill[i].slot.GetY_fromParent(), 32, 32);
    }

    // Adjust selection effect rectangle for the selected mascot slot
    if (m_iSelectedSlot != dMASCOT_INVALID_INDEX)
        m_SelectEffect.SetRectWH(rtScreen.left + m_MascotIcon[m_iSelectedSlot].slot.GetX_fromParent(), rtScreen.top + m_MascotIcon[m_iSelectedSlot].slot.GetY_fromParent(), 32, 32);

    // Adjust lift effect rectangle for the selected skill slot
    if (m_iSelectedSlotSkill != dMASCOT_INVALID_INDEX)
        m_LiftEffect.SetRectWH(rtScreen.left + m_MascotSkill[m_iSelectedSlotSkill].slot.GetX_fromParent(), rtScreen.top + m_MascotSkill[m_iSelectedSlotSkill].slot.GetY_fromParent(), 32, 32);

    // Set the position of the mascot surface
    m_surMascot.SetPosition(rtScreen.left + 50, rtScreen.top + 51);

    // Move linked GUI elements based on the change in position
    MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

// Method to check if a point is within a mascot slot
RwInt32 CMascotGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        // Check if the mouse coordinates are within the rectangle of a mascot slot
        if (m_MascotIcon[i].slot.PtInRect(iX, iY))
            return i; // Return the index of the slot if found
    }

    return dMASCOT_INVALID_INDEX; // Return invalid index if not found
}

// Method to check if a point is within a skill slot
RwInt32 CMascotGui::PtinSlotSkill(RwInt32 iX, RwInt32 iY)
{
    for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i) {
        // Check if the mouse coordinates are within the rectangle of a skill slot
        if (m_MascotSkill[i].slot.PtInRect(iX, iY))
            return i; // Return the index of the skill slot if found
    }
    return dMASCOT_INVALID_INDEX; // Return invalid index if not found
}

// Method to show or hide the item information window
VOID CMascotGui::ShowItemInfoWindow(bool bIsShow, int slot)
{
    if (bIsShow)
    {
        // If already showing item info, do nothing
        if (m_bShowItemInfo)
            return;

        // Get the current screen rectangle for positioning the info window
        CRectangle rtScreen = m_pThis->GetScreenRect();

        // Prepare mascot info structure
        stMACOT_INFO stMascot;
        stMascot.byItemRank = m_MascotIcon[slot].byItemRank;
        stMascot.dwCurExp = m_MascotIcon[slot].dwCurExp;
        stMascot.dwCurVP = m_MascotIcon[slot].dwCurVP;
        stMascot.dwMaxVP = m_MascotIcon[slot].dwMaxVP;
        stMascot.tblidx = m_MascotIcon[slot].MascotTblidx;
        stMascot.dwMaxExp = m_MascotIcon[slot].dwNeedExp;

        // Show the item info window with mascot information
        GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_MASCOT_INFO,
            rtScreen.left + m_MascotIcon[slot].slot.GetX_fromParent(),
            rtScreen.top + m_MascotIcon[slot].slot.GetY_fromParent() - 20,
            &stMascot, DIALOG_MASCOT);
    }
    else
    {
        // If not showing item info, do nothing
        if (m_bShowItemInfo == false)
            return;

        // Hide the item info window if it is currently open
        if (GetInfoWndManager()->GetRequestGui() == DIALOG_MASCOT)
            GetInfoWndManager()->ShowInfoWindow(FALSE);
    }

    // Update the state of item info visibility
    m_bShowItemInfo = bIsShow;
}

// Displays the skill information window for the specified slot if bIsShow is true.
// If the skill information is already shown, it returns immediately.
// Otherwise, it retrieves the screen rectangle and displays the skill info window.
VOID CMascotGui::ShowSkillInfoWindow(bool bIsShow, int slot)
{
    if (bIsShow)
    {
        if (m_bShowSkillInfo)
            return;

        CRectangle rtScreen = m_pThis->GetScreenRect();

        GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_MASCOT_SKILL_INFO,
            rtScreen.left + m_MascotSkill[slot].slot.GetX_fromParent(),
            rtScreen.top + m_MascotSkill[slot].slot.GetY_fromParent() - 20,
            (void*)m_MascotSkill[slot].slot.GetSkillTable(), DIALOG_MASCOT);
    }
    else
    {
        if (m_bShowSkillInfo == false)
            return;

        if (GetInfoWndManager()->GetRequestGui() == DIALOG_MASCOT)
            GetInfoWndManager()->ShowInfoWindow(FALSE);
    }
    m_bShowSkillInfo = bIsShow;
}

// Handles the focus effect for a specified slot.
// If bPush is true, sets the focus effect rectangle based on the slot's position.
// If bPush is false, simply clears the focus state.
VOID CMascotGui::FocusEffect(RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
    if (bPush)
    {
        CRectangle rect = m_pThis->GetScreenRect();

        m_FocusEffect.SetRectWH(rect.left + m_MascotIcon[iSlotIdx].slot.GetX_fromParent(), rect.top + m_MascotIcon[iSlotIdx].slot.GetY_fromParent(), 32, 32);
        m_bFocus = true;
    }
    else
    {
        m_bFocus = false;
    }
}

// Handles the focus effect for a specified skill slot.
// If bPush is true, sets the focus effect rectangle based on the skill slot's position.
// If bPush is false, clears the focus state for skills.
VOID CMascotGui::FocusEffectSkill(RwBool bPush, RwInt32 iSlotIdx)
{
    if (bPush)
    {
        CRectangle rect = m_pThis->GetScreenRect();

        m_FocusEffect_Skill.SetRectWH(rect.left + m_MascotSkill[iSlotIdx].slot.GetX_fromParent(), rect.top + m_MascotSkill[iSlotIdx].slot.GetY_fromParent(), 32, 32);
        m_bFocus_Skill = true;
    }
    else
    {
        m_bFocus_Skill = false;
    }
}

// Method to select a skill effect based on the skill slot index
VOID CMascotGui::SelectEffectSkill(RwBool bPush, RwInt32 iSlotIdx)
{
    if (bPush)
    {
        // Get the current screen rectangle for positioning the lift effect
        CRectangle rect = m_pThis->GetScreenRect();

        // Set the position of the lift effect based on the selected skill slot
        m_LiftEffect.SetRectWH(rect.left + m_MascotSkill[iSlotIdx].slot.GetX_fromParent(), rect.top + m_MascotSkill[iSlotIdx].slot.GetY_fromParent(), 32, 32);
        m_iSelectedSlotSkill = iSlotIdx; // Update the selected slot index
    }
    else
    {
        // Reset the selected slot index if the effect is not pushed
        m_iSelectedSlotSkill = dMASCOT_INVALID_INDEX;
    }
}

// Method to select an effect based on the mascot icon slot index
VOID CMascotGui::SelectEffect(RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
    if (bPush)
    {
        // Get the current screen rectangle for positioning the selection effect
        CRectangle rect = m_pThis->GetScreenRect();

        // Set the position of the selection effect based on the selected mascot slot
        m_SelectEffect.SetRectWH(rect.left + m_MascotIcon[iSlotIdx].slot.GetX_fromParent(), rect.top + m_MascotIcon[iSlotIdx].slot.GetY_fromParent(), 32, 32);
        m_iSelectedSlot = iSlotIdx; // Update the selected slot index
        m_iSelectedIndex = m_MascotIcon[iSlotIdx].byIndex; // Update the selected index
    }
    else
    {
        // Reset the selected slot index if the effect is not pushed
        m_iSelectedSlot = dMASCOT_INVALID_INDEX;
    }
}

// Creates a flash effect for the mascot in the specified slot
RwBool CMascotGui::CreateFlashEffect(RwInt32 nSlotIdx)
{
    // Log the function call for debugging purposes
    NTL_FUNCTION("CMascotGui::CreateFlashEffect");

    // Check if the flash effect is already visible; if so, return TRUE
    if (m_apflaEffect->IsVisible())
        NTL_RETURN(TRUE);

    // Iterate through all mascot slots to find the summoned mascot
    for (int i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        // Check if the current mascot icon's index matches the summoned mascot index
        if (m_MascotIcon[i].byIndex == m_SummonMascotIndex)
        {
            // Create a rectangle for positioning the flash effect
            CRectangle rect;
            // Set the rectangle's position to be slightly offset from the mascot's position
            rect.SetRectWH(m_MascotIcon[i].slot.GetX_fromParent() - 1,
                m_MascotIcon[i].slot.GetY_fromParent() - 1,
                32, 32);

            // Set the flash effect's position based on the rectangle
            m_apflaEffect->SetPosition(rect);
            // Set the effect to start from the specified frame
            m_apflaEffect->GotoFrame(m_uiEffectFrame);
            // Play the effect as a movie
            m_apflaEffect->PlayMovie(TRUE);
            // Make the effect visible
            m_apflaEffect->Show(true);
            break; // Exit the loop after finding the summoned mascot
        }
    }

    // Return TRUE to indicate the flash effect creation was successful
    NTL_RETURN(TRUE);
}


VOID CMascotGui::OnCloseBtnClicked(gui::CComponent* pComponent)
{
    if (!Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_MAINMENU_MASCOT_CLOSE))
        return;

    GetDialogManager()->CloseDialog(DIALOG_MASCOT);
}

// Handles actions when a mascot is clicked based on the specified slot.
// Loads the character's texture and updates the mascot's display rectangle.
VOID CMascotGui::OnMascotClicked(RwInt32 Slot)
{
    CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>(GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy());

    // Load the character's UI texture for the mascot display.
    m_texMascot.Load(pCharProxy->UIPcStatusBarRender());
    m_surMascot.SetTexture(&m_texMascot);

    CRectangle rtScreen = m_pThis->GetScreenRect();
    m_surMascot.SetRectWH(rtScreen.left + 50, rtScreen.top + 51, 132, 176);
}

VOID CMascotGui::OnSummonBtnClicked(gui::CComponent* pComponent)
{
    // Check if a mascot slot is selected
    if (m_iSelectedSlot != dMASCOT_INVALID_INDEX)
    {
        // Summon the mascot associated with the selected slot
        CDboEventGenerator::SummonMascot(m_MascotIcon[m_iSelectedSlot].byIndex);
    }
    else {
        // Show an alarm message if no mascot is selected
        GetAlarmManager()->AlarmMessage("DST_MASCOTEX_NOT_SELECT"); 
    }
}

VOID CMascotGui::OnSummonEndBtnClicked(gui::CComponent* pComponent)
{
    // Check if there is a currently summoned mascot
    if (dMASCOT_INVALID_INDEX != m_SummonMascotIndex)
    {
        // Unsummon the currently summoned mascot
        CDboEventGenerator::UnSummonMascot(m_SummonMascotIndex);
    }
    else
    {
        // Show an alarm message if no mascot is summoned
        GetAlarmManager()->AlarmMessage("DST_WARN_NO_EXIST_RECALLED_MASCOT"); 
    }
}

VOID CMascotGui::OnMascotDelBtnClicked(gui::CComponent* pComponent)
{
    // Check if a mascot slot is selected
    if (m_iSelectedSlot != dMASCOT_INVALID_INDEX)
    {
        // Delete the mascot associated with the selected slot
        CDboEventGenerator::DeleteMascot(m_MascotIcon[m_iSelectedSlot].byIndex);
    }
    else {
        // Show an alarm message if no mascot is selected
        GetAlarmManager()->AlarmMessage("DST_MASCOTEX_NOT_SELECT");
    }
}

VOID CMascotGui::OnMascotFusionBtnClicked(gui::CComponent* pComponent)
{
    // Check if a mascot slot is selected
    if (m_iSelectedSlot != dMASCOT_INVALID_INDEX)
    {
        // Check if the current mascot is at max experience and not a max rank
        if (m_bCurMaxExp && m_MascotIcon[m_iSelectedSlot].byItemRank < 4)
        {
            // Ensure the current summoned mascot is not the mascot selected for fusion
            if (m_SummonMascotIndex != m_MascotIcon[m_iSelectedSlot].byIndex)
            {
                // Get the current position of the dialog
                CRectangle rect = GetPosition();

                // Retrieve the fusion dialog and set its position relative to the current dialog
                CNtlPLGui* pPLGui = GetDialogManager()->GetDialog(DIALOG_MASCOT_FUSION);
                pPLGui->SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);

                // Switch to the fusion dialog
                GetDialogManager()->SwitchDialog(DIALOG_MASCOT_FUSION);
            }
            else {
                // Show an alarm message if the selected mascot is currently summoned
                GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_MAINMASCOT_SUMMONING"); 
            }
        }
        else {
            // Show an alarm message if the conditions for fusion are not met
            GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_EXPLACK"); 
        }
    }
    else {
        // Show an alarm message if no mascot is selected
        GetAlarmManager()->AlarmMessage("DST_MASCOTEX_NOT_SELECT"); 
    }
}

// Method to refresh the list of mascots displayed in the GUI
VOID CMascotGui::RefreshList(RwInt32 iOffSet)
{
    // Retrieve the current avatar's mascot information
    SMascotInfo* MascotInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();

    // Get the current screen rectangle for layout calculations
    CRectangle rtScreen = m_pThis->GetScreenRect();

    // Set the current line offset for displaying mascots
    m_CurLine = iOffSet;

    // Calculate the starting index for the mascot data based on the offset
    int Index = (iOffSet * 4);

    // Pointer to hold the mascot grade table data
    sMASCOT_GRADE_TBLDAT* pMascotTableData;

    // Loop through the maximum number of mascot slots
    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        // Check if the current index is within the available mascot count
        if (i + Index < MascotInfo->wCount) {
            // Retrieve the mascot grade data for the current index
            pMascotTableData = (sMASCOT_GRADE_TBLDAT*)API_GetTableContainer()->GetMascotGradeTable()->FindData(MascotInfo->asMascotData[Index + i].byItemRank);

            // Set the mascot icon and its associated data
            m_MascotIcon[i].slot.SetIcon(MascotInfo->asMascotData[Index + i].itemTblidx);
            m_MascotIcon[i].byIndex = MascotInfo->asMascotData[Index + i].byIndex;
            m_MascotIcon[i].byItemRank = MascotInfo->asMascotData[Index + i].byItemRank;
            m_MascotIcon[i].dwCurExp = MascotInfo->asMascotData[Index + i].dwCurExp;
            m_MascotIcon[i].dwCurVP = MascotInfo->asMascotData[Index + i].dwCurVP;
            m_MascotIcon[i].dwMaxVP = MascotInfo->asMascotData[Index + i].dwMaxVP;
            m_MascotIcon[i].MascotTblidx = MascotInfo->asMascotData[Index + i].itemTblidx;
            m_MascotIcon[i].Tblidx = MascotInfo->asMascotData[i + Index].tblidx;
            m_MascotIcon[i].byUnknown = MascotInfo->asMascotData[i + Index].byUnknown;
            m_MascotIcon[i].wunknown = MascotInfo->asMascotData[i + Index].wunknown;
            m_MascotIcon[i].wunknown2 = MascotInfo->asMascotData[i + Index].wunknown2;
            m_MascotIcon[i].dwNeedExp = pMascotTableData->dwNeedExp;

            // Copy skill table indices for the current mascot
            memcpy(m_MascotIcon[i].skillTblidx, MascotInfo->asMascotData[Index + i].skillTblidx, DBO_MASCOT_MAX_SKILL_COUNT * sizeof(TBLIDX));
        }
        else {
            // Clear the slot if there is no corresponding mascot data
            m_MascotIcon[i].slot.Clear();
            m_MascotIcon[i].byIndex = INVALID_BYTE;
            m_MascotIcon[i].byItemRank = INVALID_BYTE;
            m_MascotIcon[i].dwCurExp = INVALID_DWORD;
            m_MascotIcon[i].dwCurVP = INVALID_DWORD;
            m_MascotIcon[i].dwMaxVP = INVALID_DWORD;
            m_MascotIcon[i].MascotTblidx = INVALID_DWORD;
            m_MascotIcon[i].Tblidx = INVALID_DWORD;
            m_MascotIcon[i].byUnknown = INVALID_DWORD;
            m_MascotIcon[i].wunknown = INVALID_DWORD;
            m_MascotIcon[i].wunknown2 = INVALID_DWORD;
            m_MascotIcon[i].dwNeedExp = INVALID_DWORD;
            memset(m_MascotIcon[i].skillTblidx, 0, DBO_MASCOT_MAX_SKILL_COUNT * sizeof(TBLIDX));
        }
    }

    bool isSelect = false;
    // Check for the currently selected mascot
    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        if (i + Index < MascotInfo->wCount) {
            // If the mascot matches the selected index, highlight it
            if (m_MascotIcon[i].byIndex == m_iSelectedIndex)
            {
                m_iSelectedSlot = m_iSelectedIndex; // Set the selected slot
                SelectEffect(true, m_iSelectedIndex); // Trigger the selection effect
                isSelect = true; // Indicate a selection was found
                break;
            }
            else {
                isSelect = false; // No selection found
            }
        }
    }
    // If no selection was found, reset the selected slot
    if (!isSelect)
    {
        m_iSelectedSlot = dMASCOT_INVALID_INDEX;
        SelectEffect(false, dMASCOT_INVALID_INDEX);
    }

    bool isSummon = false;
    // Check for the summoned mascot
    for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
    {
        if (i + Index < MascotInfo->wCount) {
            // If the mascot matches the summoned index, display its effect
            if (m_MascotIcon[i].byIndex == m_SummonMascotIndex)
            {
                CRectangle rect;
                rect.SetRectWH(m_MascotIcon[i].slot.GetX_fromParent() - 1, m_MascotIcon[i].slot.GetY_fromParent() - 1, 32, 32);
                m_apflaEffect->SetPosition(rect); // Set the position of the summon effect
                m_apflaEffect->Show(true); // Show the summon effect
                isSummon = true; // Indicate a summon was found
                break;
            }
            else {
                isSummon = false; // No summon found
            }
        }
    }
    // If no summon was found, hide the summon effect
    if (!isSummon)
    {
        m_apflaEffect->Show(false);
    }
}

// Refreshes the display of the skill slots for the selected mascot
VOID CMascotGui::RefreshSkillSlot()
{
    // Check if a valid slot is selected
    if (m_iSelectedSlot != dMASCOT_INVALID_INDEX)
    {
        // Loop through each skill slot to update the UI
        for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
        {
            // If the skill index is valid, update the skill display
            if (m_MascotIcon[m_iSelectedSlot].skillTblidx[i] != INVALID_TBLIDX)
            {
                // Check if the currently summoned mascot matches the selected mascot
                if (m_SummonMascotIndex == m_MascotIcon[m_iSelectedSlot].byIndex)
                {
                    m_NotMascotSkill[i].Show(false); // Hide the not-mascot skill indicator
                    m_pbtn_MascotSkill[i]->Enable(true); // Enable the skill button
                    SelectEffectSkill(false, i); // Deselect the skill effect
                }
                else
                {
                    m_NotMascotSkill[i].Show(false); // Hide the not-mascot skill indicator
                    m_pbtn_MascotSkill[i]->Enable(false); // Disable the skill button
                    SelectEffectSkill(true, i); // Select the skill effect
                }

                // Set the icon for the skill slot
                m_MascotSkill[i].slot.SetIcon(m_MascotIcon[m_iSelectedSlot].skillTblidx[i]);
            }
            else
            {
                // If the skill index is invalid, check if the mascot item rank allows skill display
                if (m_MascotIcon[m_iSelectedSlot].byItemRank >= i + 1)
                {
                    // Check if the currently summoned mascot matches the selected mascot
                    if (m_SummonMascotIndex == m_MascotIcon[m_iSelectedSlot].byIndex)
                    {
                        m_NotMascotSkill[i].Show(false); // Hide the not-mascot skill indicator
                        m_pbtn_MascotSkill[i]->Enable(true); // Enable the skill button
                        SelectEffectSkill(false, i); // Deselect the skill effect
                    }
                    else
                    {
                        m_NotMascotSkill[i].Show(false); // Hide the not-mascot skill indicator
                        m_pbtn_MascotSkill[i]->Enable(false); // Disable the skill button
                        SelectEffectSkill(false, i); // Deselect the skill effect
                    }
                }
                else
                {
                    // If the item rank is too low, show the not-mascot skill indicator
                    m_NotMascotSkill[i].Show(true);
                    m_pbtn_MascotSkill[i]->Enable(false); // Disable the skill button
                }

                // Clear the skill slot if no skill is assigned
                m_MascotSkill[i].slot.Clear();
            }
        }
    }
}

// Opens the mascot information UI and updates it based on the selected mascot
VOID CMascotGui::OpenMascotInfo(RwBool isShow, RwInt32 iSlotIdx)
{
    // Check if the slot index is valid and if the UI should be shown
    if (iSlotIdx != dMASCOT_INVALID_INDEX && isShow)
    {
        // Iterate over the maximum number of mascot skills to update the UI
        for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i) // Update skill UI
        {
            // Check if the skill index is valid
            if (m_MascotIcon[iSlotIdx].skillTblidx[i] != INVALID_TBLIDX)
            {
                // If the current mascot is summoned
                if (m_SummonMascotIndex == m_MascotIcon[iSlotIdx].byIndex)
                {
                    m_NotMascotSkill[i].Show(false); // Hide non-mascot skills
                    m_pbtn_MascotSkill[i]->Enable(true); // Enable skill button
                    SelectEffectSkill(false, i); // Deselect skill effect
                }
                else // If the mascot is not summoned
                {
                    m_NotMascotSkill[i].Show(false); // Hide non-mascot skills
                    m_pbtn_MascotSkill[i]->Enable(false); // Disable skill button
                    SelectEffectSkill(true, i); // Select skill effect
                }
                // Set the skill icon for the mascot
                m_MascotSkill[i].slot.SetIcon(m_MascotIcon[iSlotIdx].skillTblidx[i]);
            }
            else // If the skill index is invalid
            {
                // Check if the mascot's item rank is sufficient
                if (m_MascotIcon[iSlotIdx].byItemRank >= i + 1)
                {
                    // If the current mascot is summoned
                    if (m_SummonMascotIndex == m_MascotIcon[iSlotIdx].byIndex)
                    {
                        m_NotMascotSkill[i].Show(false); // Hide non-mascot skills
                        m_pbtn_MascotSkill[i]->Enable(true); // Enable skill button
                        SelectEffectSkill(false, i); // Deselect skill effect
                    }
                    else // If the mascot is not summoned
                    {
                        m_NotMascotSkill[i].Show(false); // Hide non-mascot skills
                        m_pbtn_MascotSkill[i]->Enable(false); // Disable skill button
                        SelectEffectSkill(false, i); // Deselect skill effect
                    }
                }
                else // If the mascot's item rank is insufficient
                {
                    m_NotMascotSkill[i].Show(true); // Show non-mascot skills
                    m_pbtn_MascotSkill[i]->Enable(false); // Disable skill button
                }
                // Clear the skill slot
                m_MascotSkill[i].slot.Clear();
            }
        }

        // Show the mascot info surface
        m_surMascot.Show(true);

        // Format and update the current VP and maximum VP display
        m_psttb_Vp->Format(L"%u / %u", m_MascotIcon[iSlotIdx].dwCurVP, m_MascotIcon[iSlotIdx].dwMaxVP);
        m_ppgb_Vp->SetRange(0, m_MascotIcon[iSlotIdx].dwMaxVP);
        m_ppgb_Vp->SetPos(m_MascotIcon[iSlotIdx].dwCurVP);

        // Get the mascot's name from the table and set it in the UI
        std::wstring MascotName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText(m_MascotIcon[iSlotIdx].slot.GetItemTable()->Name);
        m_psttb_Name->SetText(MascotName.c_str());

        // Set the class type text for the mascot
        std::string text = "DST_MASCOTEX_GRADE_" + std::to_string(m_MascotIcon[iSlotIdx].byItemRank);
        m_psttb_ClassType->SetText(GetDisplayStringManager()->GetString(text));

        // Handle special case for mascot rank 4
        if (m_MascotIcon[iSlotIdx].byItemRank == 4)
        {
            m_psttb_Cp->SetText("Currently Max Level"); // Set the CP display for rank 4
            m_ppgb_Xp->SetRange(1, 1); // Set XP progress range
            m_ppgb_Xp->SetPos(1); // Set XP progress position
        }
        else // For other ranks
        {
            m_psttb_Cp->Format(L"%u / %u", m_MascotIcon[iSlotIdx].dwCurExp, m_MascotIcon[iSlotIdx].dwNeedExp);
            m_ppgb_Xp->SetRange(0, m_MascotIcon[iSlotIdx].dwNeedExp);
            m_ppgb_Xp->SetPos(m_MascotIcon[iSlotIdx].dwCurExp);

            // Check if the current XP has reached the required XP
            if (m_MascotIcon[iSlotIdx].dwCurExp == m_MascotIcon[iSlotIdx].dwNeedExp)
                m_bCurMaxExp = true; // Mark current XP as max
            else
                m_bCurMaxExp = false; // Mark current XP as not max
        }

        // Update the summoned status display based on the current mascot
        if (m_SummonMascotIndex == m_MascotIcon[iSlotIdx].byIndex)
        {
            m_psttb_Summoned->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_STATUS_SUBTITLE_SUMMON")); // Set summoned text
        }
        else // If not summoned
        {
            m_psttb_Summoned->Clear(); // Clear summoned status
        }
    }
    else // If the slot index is invalid or UI should not be shown
    {
        // Iterate over the maximum number of mascot skills to clear the UI
        for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i) // Clear skill UI
        {
            m_MascotSkill[i].slot.Clear(); // Clear the skill slot
            m_NotMascotSkill[i].Show(true); // Show non-mascot skills
            m_pbtn_MascotSkill[i]->Enable(false); // Disable skill button
        }

        // Hide the mascot info surface
        m_surMascot.Show(false);

        // Clear the VP display
        m_psttb_Vp->Clear();
        m_ppgb_Vp->SetPos(0); // Reset VP progress

        // Set default texts for mascot name and class type
        m_psttb_Name->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_MASCOTNAME"));
        m_psttb_ClassType->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_MASCOT_GRADE"));

        // Clear other displays
        m_psttb_Cp->Clear(); // Clear CP display
        m_ppgb_Xp->SetPos(0); // Reset XP progress
        m_psttb_Summoned->Clear(); // Clear summoned status
    }
}

// Updates the flash effect's state based on the elapsed time
VOID CMascotGui::Update(RwReal fElapsed)
{
    // Check if the flash effect is currently visible
    if (m_apflaEffect->IsVisible())
    {
        // Update the flash effect with the elapsed time since the last frame
        m_apflaEffect->Update(fElapsed);
        // Retrieve the current frame of the effect and store it for later use
        m_uiEffectFrame = m_apflaEffect->GetCurrentFrame();
    }
}


VOID CMascotGui::OnMascotRenameBtnClicked(gui::CComponent* pComponent)
{
    return VOID();
}
