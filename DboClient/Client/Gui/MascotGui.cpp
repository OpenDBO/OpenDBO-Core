#include "precomp_dboclient.h"
#include "MascotGui.h"

// core
#include "NtlDebug.h"

// shared
#include "TableContainer.h"
#include "MascotStatusTable.h"
#include "ItemTable.h"
#include "SkillTable.h"
#include "GraphicDataTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLCameraRenderTexture.h"
#include "NtlPLPropertyContainer.h"

// renderware
#include "rpskin.h"
#include "rphanim.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// dbo
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "AlarmManager.h"
#include "MascotFusion.h"
#include "MascotSkillReMake.h"

namespace
{
	// layout of MascotEx.frm
	const RwInt32 MASCOT_SLOT_X			= 25;
	const RwInt32 MASCOT_SLOT_Y			= 343;
	const RwInt32 MASCOT_SLOT_GAP_X		= 75;
	const RwInt32 MASCOT_SLOT_GAP_Y		= 41;

	const RwInt32 SKILL_SLOT_X			= 236;
	const RwInt32 SKILL_SLOT_Y			= 54;
	const RwInt32 SKILL_SLOT_GAP_Y		= 62;
	const RwInt32 SKILL_BTN_OFFSET_X	= 40;
	const RwInt32 SKILL_BTN_OFFSET_Y	= 15;

	const RwInt32 PREVIEW_X				= 50;
	const RwInt32 PREVIEW_Y				= 51;
	const RwInt32 PREVIEW_WIDTH			= 132;
	const RwInt32 PREVIEW_HEIGHT		= 176;

	// render target of the preview, the same proportions as the status window avatar
	const RwInt32 PREVIEW_TEX_SIZE		= 512;
	const RwInt32 PREVIEW_REAL_WIDTH	= 386;
	const RwInt32 PREVIEW_REAL_HEIGHT	= 512;

	// the first frames skin with an unposed skeleton, the next ones measure the idle animation
	const RwInt32 PREVIEW_SKIP_FRAMES		= 2;
	const RwInt32 PREVIEW_MEASURE_FRAMES	= 10;
	// camera distance in model heights, child (rank 1) mascots stand further back on the pedestal art
	const RwReal PREVIEW_DISTANCE		= 1.2f;
	const RwReal PREVIEW_DISTANCE_CHILD	= 2.2f;
	// aims a little above the model so it sits slightly lower in the frame, in model extents
	const RwReal PREVIEW_LOOK_UP		= 0.1f;

	struct SMeshBounds
	{
		RwBBox	bbox;
		RwBool	bValid;
	};

	void AddBoundsPoint(SMeshBounds* pBounds, const RwV3d* pPos)
	{
		if (!pBounds->bValid)
		{
			pBounds->bbox.inf = *pPos;
			pBounds->bbox.sup = *pPos;
			pBounds->bValid = TRUE;
		}
		else
		{
			RwBBoxAddPoint(&pBounds->bbox, pPos);
		}
	}

	// the vertices of a rendered atomic where they are drawn: skinned by the current pose of the
	// animation (the bind pose can be far bigger than the animated mesh), then placed in the world
	RpAtomic* AddAtomicBounds(RpAtomic* pAtomic, void* pData)
	{
		SMeshBounds* pBounds = reinterpret_cast<SMeshBounds*>(pData);

		if (!(RpAtomicGetFlags(pAtomic) & rpATOMICRENDER))
			return pAtomic;

		RpGeometry* pGeometry = RpAtomicGetGeometry(pAtomic);
		if (pGeometry == NULL || RpGeometryGetNumMorphTargets(pGeometry) <= 0)
			return pAtomic;

		RwInt32 iNumVerts = RpGeometryGetNumVertices(pGeometry);
		RwV3d* pVerts = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(pGeometry, 0));
		RwFrame* pFrame = RpAtomicGetFrame(pAtomic);
		if (pVerts == NULL || iNumVerts <= 0 || pFrame == NULL)
			return pAtomic;

		RwMatrix* pLtm = RwFrameGetLTM(pFrame);

		RpSkin* pSkin = RpSkinGeometryGetSkin(pGeometry);
		RpHAnimHierarchy* pHierarchy = RpSkinAtomicGetHAnimHierarchy(pAtomic);
		RwMatrix* pBoneMatrices = pHierarchy ? RpHAnimHierarchyGetMatrixArray(pHierarchy) : NULL;

		if (pSkin == NULL || pBoneMatrices == NULL)
		{
			for (RwInt32 i = 0; i < iNumVerts; ++i)
			{
				RwV3d vPos;
				RwV3dTransformPoint(&vPos, &pVerts[i], pLtm);
				AddBoundsPoint(pBounds, &vPos);
			}

			return pAtomic;
		}

		// skin space to the current bone pose, as the skinning pipeline does
		RwUInt32 uiNumBones = min(RpSkinGetNumBones(pSkin), (RwUInt32)pHierarchy->numNodes);
		const RwMatrix* pSkinToBone = RpSkinGetSkinToBoneMatrices(pSkin);
		const RwMatrixWeights* pWeights = RpSkinGetVertexBoneWeights(pSkin);
		const RwUInt32* pIndices = RpSkinGetVertexBoneIndices(pSkin);
		if (uiNumBones == 0 || pSkinToBone == NULL || pWeights == NULL || pIndices == NULL)
			return pAtomic;

		std::vector<RwMatrix> vecSkinMatrix(uiNumBones);
		for (RwUInt32 b = 0; b < uiNumBones; ++b)
			RwMatrixMultiply(&vecSkinMatrix[b], &pSkinToBone[b], &pBoneMatrices[b]);

		// hierarchies in local space still need the atomic frame to reach the world
		RwBool bLocalSpace = (RpHAnimHierarchyGetFlags(pHierarchy) & rpHANIMHIERARCHYLOCALSPACEMATRICES) != 0;

		for (RwInt32 i = 0; i < iNumVerts; ++i)
		{
			const RwReal afWeight[4] = { pWeights[i].w0, pWeights[i].w1, pWeights[i].w2, pWeights[i].w3 };

			RwV3d vPos = { 0.0f, 0.0f, 0.0f };
			RwReal fTotal = 0.0f;

			for (RwInt32 k = 0; k < 4; ++k)
			{
				RwUInt32 uiBone = (pIndices[i] >> (8 * k)) & 0xFF;
				if (afWeight[k] <= 0.0f || uiBone >= uiNumBones)
					continue;

				RwV3d vBone;
				RwV3dTransformPoint(&vBone, &pVerts[i], &vecSkinMatrix[uiBone]);
				RwV3dIncrementScaled(&vPos, &vBone, afWeight[k]);
				fTotal += afWeight[k];
			}

			if (fTotal <= 0.0f)
				continue;

			RwV3dScale(&vPos, &vPos, 1.0f / fTotal);

			if (bLocalSpace)
			{
				RwV3d vWorld;
				RwV3dTransformPoint(&vWorld, &vPos, pLtm);
				vPos = vWorld;
			}

			AddBoundsPoint(pBounds, &vPos);
		}

		return pAtomic;
	}
}

//////////////////////////////////////////////////////////////////////////
// CMascotPreview

CMascotPreview::CMascotPreview()
: m_pCharacter(NULL)
, m_pCamera(NULL)
, m_bCameraReady(FALSE)
, m_iMeasuredFrames(0)
, m_bMeshValid(FALSE)
, m_fDistance(PREVIEW_DISTANCE)
{
}

CMascotPreview::~CMascotPreview()
{
	Destroy();
}

void CMascotPreview::SetModel(const RwChar* pModelKey)
{
	if (pModelKey && m_pCharacter && m_strModel == pModelKey)
		return;

	Destroy();

	if (pModelKey == NULL || pModelKey[0] == '\0')
		return;

	// creating a character without its property crashes the scene manager
	if (CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_CHARACTER, pModelKey) == NULL)
		return;

	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, 0.0f, 0.0f, 0.0f);

	SPLCharacterCreateParam sParam;
	sParam.pPos = &vPos;
	sParam.uiSerialId = INVALID_SERIAL_ID;
	sParam.bPcFlag = FALSE;
	sParam.uiClass = 0;
	sParam.uiRace = 0;
	sParam.uiGender = 0;
	sParam.uiFaceType = 0;
	sParam.uiHeadType = 0;
	sParam.uiHeadColorType = 0;
	sParam.uiSkinColorType = 0;
	sParam.bNotShadingFlag = FALSE;
	sParam.bIsAdult = FALSE;

	m_pCharacter = static_cast<CNtlPLCharacter*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, pModelKey, &sParam));
	if (m_pCharacter == NULL)
		return;

	// only rendered into the texture, like the status window avatar
	m_pCharacter->SetFlags(m_pCharacter->GetFlags() | NTL_PLEFLAG_DUMMY);
	m_pCharacter->SetVisible(FALSE);
	m_pCharacter->SetPicking(FALSE);
	m_pCharacter->SetPosition(&vPos);
	m_pCharacter->SetBaseAnimation(NML_IDLE_LOOP, 0.0f, TRUE);

	m_pCamera = NTL_NEW CNtlPLCameraRenderTexture;
	m_pCamera->Create(PREVIEW_TEX_SIZE, PREVIEW_TEX_SIZE, PREVIEW_REAL_WIDTH, PREVIEW_REAL_HEIGHT);

	m_strModel = pModelKey;
	m_bCameraReady = FALSE;
	m_iMeasuredFrames = 0;
	m_bMeshValid = FALSE;
}

void CMascotPreview::Destroy(void)
{
	if (m_pCharacter)
	{
		GetSceneManager()->DeleteEntity(m_pCharacter);
		m_pCharacter = NULL;
	}

	if (m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	m_strModel.clear();
	m_bCameraReady = FALSE;
	m_iMeasuredFrames = 0;
	m_bMeshValid = FALSE;
}

void CMascotPreview::SetupCamera(void)
{
	// frame the mesh as it is drawn, the property box is only a rough hit box and the bind pose of
	// some mascots is far bigger than their animated mesh (the higher grade dragons came out tiny)
	if (m_iMeasuredFrames >= PREVIEW_SKIP_FRAMES)
	{
		SMeshBounds sBounds;
		sBounds.bValid = FALSE;

		RpClump* pClump = m_pCharacter->GetClump();
		if (pClump)
			RpClumpForAllAtomics(pClump, AddAtomicBounds, &sBounds);

		// the idle animation moves a little, keep all of it in view
		if (sBounds.bValid)
		{
			if (m_bMeshValid)
			{
				RwBBoxAddPoint(&m_bboxMesh, &sBounds.bbox.inf);
				RwBBoxAddPoint(&m_bboxMesh, &sBounds.bbox.sup);
			}
			else
			{
				m_bboxMesh = sBounds.bbox;
				m_bMeshValid = TRUE;
			}
		}
	}

	RwV3d vCenter;
	RwReal fHeight, fWidth;

	if (m_bMeshValid)
	{
		// the camera looks down the z axis, the depth does not change the framed size
		fHeight = m_bboxMesh.sup.y - m_bboxMesh.inf.y;
		fWidth = m_bboxMesh.sup.x - m_bboxMesh.inf.x;
		CNtlMath::MathRwV3dAssign(&vCenter,
			(m_bboxMesh.sup.x + m_bboxMesh.inf.x) * 0.5f,
			(m_bboxMesh.sup.y + m_bboxMesh.inf.y) * 0.5f,
			(m_bboxMesh.sup.z + m_bboxMesh.inf.z) * 0.5f);
	}
	else
	{
		RwReal fScale = m_pCharacter->GetBaseScale() * m_pCharacter->GetScale();
		fHeight = m_pCharacter->GetHeight() * fScale;
		fWidth = m_pCharacter->GetWidth() * fScale;
		CNtlMath::MathRwV3dAssign(&vCenter, 0.0f, fHeight * 0.5f, 0.0f);
	}

	fHeight = max(fHeight, 0.1f);
	RwReal fExtent = max(fHeight, fWidth * (RwReal)PREVIEW_REAL_HEIGHT / (RwReal)PREVIEW_REAL_WIDTH);

	RwV3d vLookAt, vCamPos;
	vCenter.y += fExtent * PREVIEW_LOOK_UP;
	vLookAt = vCenter;
	// the camera stands far enough back for the model to sit on the pedestal art with room around it
	CNtlMath::MathRwV3dAssign(&vCamPos, vCenter.x, vCenter.y, vCenter.z + fExtent * m_fDistance);

	m_pCamera->SetCameraPosition(&vCamPos, &vLookAt);
}

void CMascotPreview::SetDistance(RwReal fDistance)
{
	if (m_fDistance == fDistance)
		return;

	m_fDistance = fDistance;

	// the same model is used by several ranks, move the camera without measuring again
	if (m_pCharacter && m_bCameraReady)
		SetupCamera();
}

RwTexture* CMascotPreview::Render(void)
{
	if (m_pCharacter == NULL || m_pCamera == NULL)
		return NULL;

	if (!m_pCharacter->GetCharScheduleResInfo()->bLoadComplete)
		return NULL;

	// measured over the first frames of the animation, then the camera stays put
	if (!m_bCameraReady)
	{
		SetupCamera();

		if (++m_iMeasuredFrames >= PREVIEW_MEASURE_FRAMES)
			m_bCameraReady = TRUE;
	}

	m_pCamera->RenderTexture(m_pCharacter);

	return m_pCamera->GetTexture();
}

//////////////////////////////////////////////////////////////////////////
// CMascotGui

CMascotGui::CMascotGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	Init();
}

CMascotGui::~CMascotGui()
{
}

VOID CMascotGui::Init()
{
	m_psttbTitle = NULL;
	m_psttbInfo = NULL;
	m_psttbSkillInfo = NULL;
	m_psttbSummoned = NULL;
	m_psttbName = NULL;
	m_psttbGrade = NULL;
	m_psttbVp = NULL;
	m_psttbExp = NULL;
	m_pbtnSummon = NULL;
	m_pbtnSummonEnd = NULL;
	m_pbtnDelete = NULL;
	m_pbtnFusion = NULL;
	m_pbtnClose = NULL;
	m_ppgbVp = NULL;
	m_ppgbExp = NULL;
	m_pscbMascot = NULL;
	m_pflaSummoned = NULL;

	for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
	{
		m_ppnlMascot[i] = NULL;
		m_abySlotIndex[i] = INVALID_BYTE;
	}

	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
	{
		m_ppnlSkill[i] = NULL;
		m_pbtnSkill[i] = NULL;
	}

	m_iCurLine = 0;
	m_bySelectedIndex = INVALID_BYTE;
	m_iFocusSlot = -1;
	m_iMouseDownMascot = -1;
	m_iMouseDownSkill = -1;
	m_iFocusSkill = -1;
	m_iInfoWndSlot = -1;
	m_fPreviewWait = 0.0f;

	m_byGradeUpItemPlace = INVALID_BYTE;
	m_byGradeUpItemPos = INVALID_BYTE;
}

RwBool CMascotGui::Create()
{
	NTL_FUNCTION("CMascotGui::Create");

	if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotEx.srf", "gui\\MascotEx.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_psttbTitle = (gui::CStaticBox*)GetComponent("Stb_Title");
	m_psttbInfo = (gui::CStaticBox*)GetComponent("Stb_Info");
	m_psttbSkillInfo = (gui::CStaticBox*)GetComponent("Stb_SkillInfo");
	m_psttbSummoned = (gui::CStaticBox*)GetComponent("Stb_Summoned");
	m_psttbName = (gui::CStaticBox*)GetComponent("Stb_Name");
	m_psttbGrade = (gui::CStaticBox*)GetComponent("Stb_ClassType");
	m_psttbVp = (gui::CStaticBox*)GetComponent("Stb_Vp");
	m_psttbExp = (gui::CStaticBox*)GetComponent("Stb_Cp");

	m_pbtnSummon = (gui::CButton*)GetComponent("Btn_Summon");
	m_pbtnSummonEnd = (gui::CButton*)GetComponent("Btn_SummonEnd");
	m_pbtnDelete = (gui::CButton*)GetComponent("Btn_MascotDel");
	m_pbtnFusion = (gui::CButton*)GetComponent("Btn_MascotFusion");
	m_pbtnClose = (gui::CButton*)GetComponent("Btn_Close");

	// some layouts carry a rename button, mascots cannot be renamed
	gui::CButton* pRenameBtn = (gui::CButton*)GetComponent("Btn_MascotRename");
	if (pRenameBtn)
		pRenameBtn->Show(false);

	m_ppgbVp = (gui::CProgressBar*)GetComponent("Pgb_Vp");
	m_ppgbExp = (gui::CProgressBar*)GetComponent("Pgb_Xp");

	m_pscbMascot = (gui::CScrollBar*)GetComponent("Scb_MascotScroll");

	gui::CPanel* pBagBack = (gui::CPanel*)GetComponent("Pn_IvnBack");
	if (pBagBack)
		pBagBack->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_BAG"));

	// texts
	m_psttbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_psttbTitle->SetText(GetDisplayStringManager()->GetString("DST_MASCOT"));
	m_psttbInfo->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_STATUS_SUBTITLE"));
	m_psttbSkillInfo->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL"));

	m_pbtnSummon->SetText(GetDisplayStringManager()->GetString("DST_RECALL"));
	m_pbtnSummonEnd->SetText(GetDisplayStringManager()->GetString("DST_RELEASE"));
	m_pbtnDelete->SetText(GetDisplayStringManager()->GetString("DST_DELETE"));
	m_pbtnFusion->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSIONBUTTON"));
	m_pbtnFusion->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_FUSION_TOOLTIP"));

	m_ppgbVp->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_VP_REGEN"));

	// mascot slots
	for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
	{
		char acName[32];
		sprintf_s(acName, "Pn_IvnIcon%d", i);
		m_ppnlMascot[i] = (gui::CPanel*)GetComponent(acName);

		m_slotMascotEnter[i] = m_ppnlMascot[i]->SigMouseEnter().Connect(this, &CMascotGui::OnMouseEnterMascot);
		m_slotMascotLeave[i] = m_ppnlMascot[i]->SigMouseLeave().Connect(this, &CMascotGui::OnMouseLeaveMascot);
		m_slotMascotDown[i] = m_ppnlMascot[i]->SigMouseDown().Connect(this, &CMascotGui::OnMouseDownMascot);
		m_slotMascotUp[i] = m_ppnlMascot[i]->SigMouseUp().Connect(this, &CMascotGui::OnMouseUpMascot);

		m_MascotSlot[i].Create(m_pThis, DIALOG_MASCOT, REGULAR_SLOT_ITEM_TABLE);
		m_MascotSlot[i].SetPosition_fromParent(MASCOT_SLOT_X + (i % dMASCOT_SLOT_ROW) * MASCOT_SLOT_GAP_X,
											   MASCOT_SLOT_Y + (i / dMASCOT_SLOT_ROW) * MASCOT_SLOT_GAP_Y);
	}

	// skill slots
	CRectangle rect;
	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
	{
		char acName[32];
		sprintf_s(acName, "Pn_SkillIcon%d", i);
		m_ppnlSkill[i] = (gui::CPanel*)GetComponent(acName);

		m_slotSkillEnter[i] = m_ppnlSkill[i]->SigMouseEnter().Connect(this, &CMascotGui::OnMouseEnterSkill);
		m_slotSkillLeave[i] = m_ppnlSkill[i]->SigMouseLeave().Connect(this, &CMascotGui::OnMouseLeaveSkill);
		m_slotSkillDown[i] = m_ppnlSkill[i]->SigMouseDown().Connect(this, &CMascotGui::OnMouseDownSkill);
		m_slotSkillUp[i] = m_ppnlSkill[i]->SigMouseUp().Connect(this, &CMascotGui::OnMouseUpSkill);

		RwInt32 iY = SKILL_SLOT_Y + i * SKILL_SLOT_GAP_Y;

		m_SkillSlot[i].Create(m_pThis, DIALOG_MASCOT, REGULAR_SLOT_SKILL_TABLE);
		m_SkillSlot[i].SetPosition_fromParent(SKILL_SLOT_X, iY);

		m_surSkillLock[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotDisableEffect"));
		m_surSkillLock[i].SetPositionfromParent(SKILL_SLOT_X, iY);

		m_SkillCoolTime[i].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect"));

		rect.SetRectWH(SKILL_SLOT_X + SKILL_BTN_OFFSET_X, iY + SKILL_BTN_OFFSET_Y, 19, 19);
		m_pbtnSkill[i] = NTL_NEW gui::CButton(rect, "",
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MascotEx.srf", "srfSkillChangeOpenUp"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MascotEx.srf", "srfSkillChangeOpenDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MascotEx.srf", "srfSkillChangeOpenDown"),
			GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MascotEx.srf", "srfSkillChangeOpenFocus"),
			NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_UP_COLOR,
			GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pbtnSkill[i]->Enable(false);
		m_pbtnSkill[i]->Raise();

		m_slotSkillBtn[i] = m_pbtnSkill[i]->SigClicked().Connect(this, &CMascotGui::OnSkillBtnClicked);
	}

	m_surFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	// srfSlotPitchEffect does not exist and drew a white box, the yellow slot frame marks the selection
	m_surSelect.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_surFocus.Show(FALSE);
	m_surSelect.Show(FALSE);

	m_surPreview.SetPositionfromParent(PREVIEW_X, PREVIEW_Y);
	m_surPreview.Show(FALSE);

	// marks the summoned mascot in the list
	rect.SetRectWH(0, 0, 32, 32);
	m_pflaSummoned = NTL_NEW gui::CFlash(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "Skill_action.swf");
	m_pflaSummoned->Show(false);
	m_pflaSummoned->Enable(false);

	// signals
	m_slotClose = m_pbtnClose->SigClicked().Connect(this, &CMascotGui::OnCloseBtnClicked);
	m_slotSummon = m_pbtnSummon->SigClicked().Connect(this, &CMascotGui::OnSummonBtnClicked);
	m_slotSummonEnd = m_pbtnSummonEnd->SigClicked().Connect(this, &CMascotGui::OnSummonEndBtnClicked);
	m_slotDelete = m_pbtnDelete->SigClicked().Connect(this, &CMascotGui::OnDeleteBtnClicked);
	m_slotFusion = m_pbtnFusion->SigClicked().Connect(this, &CMascotGui::OnFusionBtnClicked);
	m_slotScrollChanged = m_pscbMascot->SigValueChanged().Connect(this, &CMascotGui::OnScrollChanged);
	m_slotScrollMoved = m_pscbMascot->SigSliderMoved().Connect(this, &CMascotGui::OnScrollChanged);
	m_slotWheel = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CMascotGui::OnMouseWheel);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CMascotGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CMascotGui::OnMove);

	// the seal kit confirmation has no entry in the message table, use the summon box layout
	GetAlarmManager()->CloneClientAlarm("DST_MASCOTEX_MASCOTSEAL_CONFIRM_BOX", "DST_RECALL_START_MASCOT_MB_CONFIRM");

	GetNtlGuiManager()->AddUpdateFunc(this);
	GetNtlGuiManager()->AddUpdateBeforeFunc(this);

	LinkMsg(g_EventMascotRegister);
	LinkMsg(g_EventMascotDelete);
	LinkMsg(g_EventMascotSummon);
	LinkMsg(g_EventMascotUnSummon);
	LinkMsg(g_EventMascotUpdate);
	LinkMsg(g_EventMsgBoxResult);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CMascotGui::Destroy()
{
	NTL_FUNCTION("CMascotGui::Destroy");

	m_Preview.Destroy();

	for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
		m_MascotSlot[i].Destroy();

	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
		m_SkillSlot[i].Destroy();

	UnLinkMsg(g_EventMascotRegister);
	UnLinkMsg(g_EventMascotDelete);
	UnLinkMsg(g_EventMascotSummon);
	UnLinkMsg(g_EventMascotUnSummon);
	UnLinkMsg(g_EventMascotUpdate);
	UnLinkMsg(g_EventMsgBoxResult);

	GetNtlGuiManager()->RemoveUpdateFunc(this);
	GetNtlGuiManager()->RemoveUpdateBeforeFunc(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CMascotGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();

		// show the summoned mascot first, or the first one registered
		if (pInfo->Find(m_bySelectedIndex) == NULL)
		{
			if (pInfo->GetSummoned())
				m_bySelectedIndex = pInfo->bySummonIndex;
			else if (pInfo->wCount > 0)
				m_bySelectedIndex = pInfo->asMascotData[0].byIndex;
			else
				m_bySelectedIndex = INVALID_BYTE;
		}

		Show(true);

		// places the slots and the preview even if the dialog never moved
		CRectangle rtScreen = m_pThis->GetScreenRect();
		OnMove(rtScreen.left, rtScreen.top);

		RefreshList();
		RefreshInfo();
	}
	else
	{
		if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_FUSION))
			GetDialogManager()->CloseDialog(DIALOG_MASCOT_FUSION);

		if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_SKILL_REMAKE))
			GetDialogManager()->CloseDialog(DIALOG_MASCOT_SKILL_REMAKE);

		HideInfoWnd();

		m_byGradeUpItemPlace = INVALID_BYTE;
		m_byGradeUpItemPos = INVALID_BYTE;

		// the preview model only lives while the window is open
		m_Preview.Destroy();
		m_surPreview.Show(FALSE);

		Show(false);
	}

	return 1;
}

VOID CMascotGui::StartSkillGradeUp(BYTE byItemPlace, BYTE byItemPos)
{
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	if (pInfo->GetSummoned() == NULL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLGRADEUP_DONT_SUMMON");
		return;
	}

	m_byGradeUpItemPlace = byItemPlace;
	m_byGradeUpItemPos = byItemPos;
	m_bySelectedIndex = pInfo->bySummonIndex;

	if (!GetDialogManager()->IsOpenDialog(DIALOG_MASCOT))
		GetDialogManager()->OpenDialog(DIALOG_MASCOT);
	else
	{
		RefreshList();
		RefreshInfo();
	}

	GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLGRADEUP_USING");
}

VOID CMascotGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CMascotGui::HandleEvents");

	if (msg.Id == g_EventMascotRegister)
	{
		SDboEventMascotRegister* pData = reinterpret_cast<SDboEventMascotRegister*>(msg.pData);
		m_bySelectedIndex = pData->mascotdata.byIndex;

		if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT))
		{
			RefreshList();
			RefreshInfo();
		}
		else
			GetDialogManager()->OpenDialog(DIALOG_MASCOT);
	}
	else if (msg.Id == g_EventMascotDelete || msg.Id == g_EventMascotSummon || msg.Id == g_EventMascotUnSummon)
	{
		if (msg.Id == g_EventMascotSummon)
			m_bySelectedIndex = reinterpret_cast<SDboEventMascotOperate*>(msg.pData)->index;

		if (IsShow())
		{
			if (GetNtlSLGlobal()->GetAvatarMascotInfo()->Find(m_bySelectedIndex) == NULL)
				m_bySelectedIndex = INVALID_BYTE;

			RefreshList();
			RefreshInfo();
		}
	}
	else if (msg.Id == g_EventMascotUpdate)
	{
		SDboEventMascotUpdate* pData = reinterpret_cast<SDboEventMascotUpdate*>(msg.pData);

		if (pData->byType == MASCOT_UPDATE_SKILL)
		{
			m_byGradeUpItemPlace = INVALID_BYTE;
			m_byGradeUpItemPos = INVALID_BYTE;

			// the skill window reveals a learned or re-rolled skill after its gauges fill
			CMascotSkillReMake* pReMake = reinterpret_cast<CMascotSkillReMake*>(GetDialogManager()->GetDialog(DIALOG_MASCOT_SKILL_REMAKE));
			if (pData->bSuccess && pReMake && pReMake->IsRevealPending(pData->byIndex, pData->bySlot))
				NTL_RETURNVOID();
		}

		if (IsShow())
		{
			RefreshList();
			if (pData->byIndex == m_bySelectedIndex)
				RefreshInfo();
		}
	}
	else if (msg.Id == g_EventMsgBoxResult)
	{
		SDboEventMsgBoxResult* pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);
		if (pEvent->eResult != MBR_OK || pEvent->pData == NULL)
			NTL_RETURNVOID();

		RwUInt64 uiParam = pEvent->pData->uiParam;

		if (pEvent->strID == "DST_MASCOTEX_SKILL_RESCUE")
		{
			Logic_SendMascotSkill((RwUInt8)(uiParam & 0xFF));
		}
		else if (pEvent->strID == "DST_MASCOTEX_SKILL_NPC_TELEPORT")
		{
			Logic_SendMascotSkill((RwUInt8)(uiParam & 0xFF), (TBLIDX)(uiParam >> 32));
		}
		else if (pEvent->strID == "DST_MASCOTEX_SKILL_GRADEUP")
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendMascotSkillUpgradeReq((BYTE)(uiParam & 0xFF), (BYTE)((uiParam >> 8) & 0xFF), (BYTE)((uiParam >> 16) & 0xFF));
		}
		else if (pEvent->strID == "DST_MASCOTEX_MASCOTSEAL_CONFIRM_BOX")
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendMascotSealSetReq((BYTE)(uiParam & 0xFF), (BYTE)((uiParam >> 8) & 0xFF), (BYTE)((uiParam >> 16) & 0xFF));
		}
	}

	NTL_RETURNVOID();
}

VOID CMascotGui::Update(RwReal fElapsed)
{
	if (!IsShow())
		return;

	if (m_pflaSummoned->IsVisible())
		m_pflaSummoned->Update(fElapsed);

	// cooldowns of the summoned mascot
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	RwBool bSummonedShown = pInfo->bySummonIndex != INVALID_BYTE && pInfo->bySummonIndex == m_bySelectedIndex;

	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
	{
		RwUInt32 dwLeft = bSummonedShown ? pInfo->GetSkillCoolTimeLeft(m_bySelectedIndex, (RwUInt8)i) : 0;
		if (dwLeft == 0)
		{
			if (m_SkillCoolTime[i].IsWork())
				m_SkillCoolTime[i].EndProc();
			continue;
		}

		sSKILL_TBLDAT* pSkillTbl = m_SkillSlot[i].GetSkillTable();
		if (pSkillTbl == NULL || pSkillTbl->dwCoolTimeInMilliSecs == 0)
			continue;

		RwReal fMax = (RwReal)pSkillTbl->dwCoolTimeInMilliSecs / 1000.0f;
		if (!m_SkillCoolTime[i].IsWork())
			m_SkillCoolTime[i].StartProc(fMax);

		m_SkillCoolTime[i].Update(max(0.0f, fMax - (RwReal)dwLeft / 1000.0f));
	}
}

VOID CMascotGui::UpdateBeforeCamera(RwReal fElapsed)
{
	if (!IsShow())
		return;

	RwTexture* pTexture = m_Preview.Render();
	if (pTexture == NULL)
		return;

	m_texPreview.Load(pTexture);
	m_surPreview.SetTexture(&m_texPreview);

	// give the model textures a moment to stream in, a fresh model renders black
	if (!m_surPreview.IsShow())
	{
		m_fPreviewWait += fElapsed;
		if (m_fPreviewWait < 0.3f)
			return;
	}

	m_surPreview.Show(TRUE);
}

VOID CMascotGui::OnPaint()
{
	for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
		m_MascotSlot[i].Paint();

	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
	{
		m_SkillSlot[i].Paint();
		m_surSkillLock[i].Render();

		if (m_SkillCoolTime[i].IsWork())
			m_SkillCoolTime[i].Render();
	}

	m_surSelect.Render();
	m_surFocus.Render();

	// drawn in the dialog's own layer so message boxes stay on top of it
	m_surPreview.Render();
}

VOID CMascotGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
		m_MascotSlot[i].SetParentPosition(rtScreen.left, rtScreen.top);

	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
	{
		m_SkillSlot[i].SetParentPosition(rtScreen.left, rtScreen.top);
		m_surSkillLock[i].SetPositionbyParent(rtScreen.left, rtScreen.top);
		m_SkillCoolTime[i].SetPosition(rtScreen.left + m_SkillSlot[i].GetX_fromParent(), rtScreen.top + m_SkillSlot[i].GetY_fromParent());
	}

	m_surPreview.SetRectWH(rtScreen.left + PREVIEW_X, rtScreen.top + PREVIEW_Y, PREVIEW_WIDTH, PREVIEW_HEIGHT);

	RefreshList();

	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
}

VOID CMascotGui::OnScrollChanged(RwInt32 iOffset)
{
	m_iCurLine = iOffset;
	RefreshList();
}

VOID CMascotGui::OnMouseWheel(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!m_pscbMascot->IsVisible())
		return;

	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR(IsShow(), m_pThis, m_pThis, m_pscbMascot, sDelta, pos);
}

RwInt32 CMascotGui::FindComponent(gui::CComponent* pComponent, gui::CPanel** apPanel, RwInt32 iCount)
{
	for (RwInt32 i = 0; i < iCount; ++i)
	{
		if (apPanel[i] == pComponent)
			return i;
	}

	return -1;
}

VOID CMascotGui::OnMouseEnterMascot(gui::CComponent* pComponent)
{
	m_iFocusSlot = FindComponent(pComponent, m_ppnlMascot, dMAX_MASCOT_SLOT);
	if (m_iFocusSlot < 0 || GetSlotData(m_iFocusSlot) == NULL)
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_surFocus.SetRectWH(rtScreen.left + m_MascotSlot[m_iFocusSlot].GetX_fromParent(), rtScreen.top + m_MascotSlot[m_iFocusSlot].GetY_fromParent(), 32, 32);
	m_surFocus.Show(TRUE);

	ShowMascotInfoWnd(m_iFocusSlot);
}

VOID CMascotGui::OnMouseLeaveMascot(gui::CComponent* pComponent)
{
	m_iFocusSlot = -1;
	m_surFocus.Show(FALSE);
	HideInfoWnd();
}

VOID CMascotGui::OnMouseDownMascot(const CKey& key)
{
	m_iMouseDownMascot = m_iFocusSlot;
}

VOID CMascotGui::OnMouseUpMascot(const CKey& key)
{
	RwInt32 iSlot = m_iMouseDownMascot;
	m_iMouseDownMascot = -1;

	if (iSlot < 0 || iSlot != m_iFocusSlot)
		return;

	// an item carried over a mascot (seal kit)
	if (GetIconMoveManager()->IsActive())
	{
		DropOnMascot(iSlot);
		return;
	}

	sMASCOT_DATA_EX* pData = GetSlotData(iSlot);
	if (pData == NULL)
		return;

	// while fusing, right clicking another mascot offers it as the sacrifice instead of summoning it
	if (key.m_nID == UD_RIGHT_BUTTON && GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_FUSION))
	{
		CMascotFusion* pFusion = reinterpret_cast<CMascotFusion*>(GetDialogManager()->GetDialog(DIALOG_MASCOT_FUSION));
		if (pFusion)
			pFusion->SetOfferingMascot(pData->byIndex);
		return;
	}

	Select(pData->byIndex);

	if (key.m_nID == UD_RIGHT_BUTTON)
	{
		// right click summons or dismisses
		SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
		if (pInfo->bySummonIndex == pData->byIndex)
			OnSummonEndBtnClicked(NULL);
		else
			OnSummonBtnClicked(NULL);
	}
}

VOID CMascotGui::OnMouseEnterSkill(gui::CComponent* pComponent)
{
	RwInt32 iSkillSlot = FindComponent(pComponent, m_ppnlSkill, dMAX_MASCOT_SKILL);
	if (iSkillSlot < 0)
		return;

	m_iFocusSkill = iSkillSlot;

	if (m_SkillSlot[iSkillSlot].GetSkillTable() == NULL)
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_surFocus.SetRectWH(rtScreen.left + m_SkillSlot[iSkillSlot].GetX_fromParent(), rtScreen.top + m_SkillSlot[iSkillSlot].GetY_fromParent(), 32, 32);
	m_surFocus.Show(TRUE);

	ShowSkillInfoWnd(iSkillSlot);
}

VOID CMascotGui::OnMouseLeaveSkill(gui::CComponent* pComponent)
{
	m_iFocusSkill = -1;
	m_surFocus.Show(FALSE);
	HideInfoWnd();
}

VOID CMascotGui::OnMouseDownSkill(const CKey& key)
{
	m_iMouseDownSkill = m_iFocusSkill;
}

VOID CMascotGui::OnMouseUpSkill(const CKey& key)
{
	RwInt32 iSkillSlot = m_iMouseDownSkill;
	m_iMouseDownSkill = -1;

	if (iSkillSlot < 0 || iSkillSlot != m_iFocusSkill)
		return;

	// an empty open slot works like its + button
	sMASCOT_DATA_EX* pData = GetSelectedData();
	if (key.m_nID == UD_LEFT_BUTTON && pData && IsSkillSlotOpen(pData, iSkillSlot) && pData->skillTblidx[iSkillSlot] == INVALID_TBLIDX)
	{
		OnSkillBtnClicked(m_pbtnSkill[iSkillSlot]);
		return;
	}

	ClickSkill(iSkillSlot, key.m_nID == UD_RIGHT_BUTTON);
}

VOID CMascotGui::ClickSkill(RwInt32 iSkillSlot, RwBool bRightButton)
{
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	sMASCOT_DATA_EX* pData = GetSelectedData();
	if (pData == NULL || pData->skillTblidx[iSkillSlot] == INVALID_TBLIDX)
		return;

	// skills only work through the summoned mascot
	if (pInfo->bySummonIndex != pData->byIndex)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLGRADEUP_DONT_SUMMON");
		return;
	}

	if (m_byGradeUpItemPlace != INVALID_BYTE)
	{
		// the item picks one skill, the confirm box carries its place, so skills work normally
		// again whether the player confirms, cancels or picked a maxed skill
		BYTE byItemPlace = m_byGradeUpItemPlace;
		BYTE byItemPos = m_byGradeUpItemPos;
		m_byGradeUpItemPlace = INVALID_BYTE;
		m_byGradeUpItemPos = INVALID_BYTE;

		sSKILL_TBLDAT* pSkillTbl = m_SkillSlot[iSkillSlot].GetSkillTable();
		if (pSkillTbl == NULL || pSkillTbl->dwNextSkillTblidx == INVALID_TBLIDX)
		{
			GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLGRADEUP_MAX");
			return;
		}

		sMsgBoxData sData;
		sData.uiParam = (RwUInt64)iSkillSlot | ((RwUInt64)byItemPlace << 8) | ((RwUInt64)byItemPos << 16);
		GetAlarmManager()->FormattedAlarmMessage("DST_MASCOTEX_SKILL_GRADEUP", FALSE, &sData);
		return;
	}

	if (bRightButton)
		Logic_UseMascotSkill((RwUInt8)iSkillSlot);
}

VOID CMascotGui::DropOnMascot(RwInt32 iSlot)
{
	sMASCOT_DATA_EX* pData = GetSlotData(iSlot);

	if (GetIconMoveManager()->GetSrcPlace() != PLACE_BAG || pData == NULL)
	{
		GetIconMoveManager()->IconMoveEnd();
		return;
	}

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
	GetIconMoveManager()->IconMoveEnd();

	if (pItem == NULL)
		return;

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
	sITEM_TBLDAT* pItemTbl = pItemAttr->GetItemTbl();
	if (pItemTbl == NULL || pItemTbl->byItem_Type != ITEM_TYPE_EVENT_SEAL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_MASCOTSEAL_NOT_SEALITEM");
		return;
	}

	if (GetNtlSLGlobal()->GetAvatarMascotInfo()->bySummonIndex == pData->byIndex)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_MASCOTSEAL_SUMMON");
		return;
	}

	// one seal kit per mascot rank
	if (pItemAttr->GetStackNum() < pData->byItemRank)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_MASCOTSEAL_NEED_MORE_KIT");
		return;
	}

	RwUInt8 byPlace, byPos;
	if (!Logic_GetItemContainerPosFromHandle(pItem->GetSerialID(), byPlace, byPos))
		return;

	sMsgBoxData sData;
	sData.uiParam = (RwUInt64)pData->byIndex | ((RwUInt64)byPlace << 8) | ((RwUInt64)byPos << 16);
	GetAlarmManager()->FormattedAlarmMessage("DST_MASCOTEX_MASCOTSEAL_CONFIRM_BOX", FALSE, &sData);
}

VOID CMascotGui::OnCloseBtnClicked(gui::CComponent* pComponent)
{
	if (!Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_MAINMENU_MASCOT_CLOSE))
		return;

	GetDialogManager()->CloseDialog(DIALOG_MASCOT);
}

VOID CMascotGui::OnSummonBtnClicked(gui::CComponent* pComponent)
{
	sMASCOT_DATA_EX* pData = GetSelectedData();
	if (pData == NULL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_NOT_SELECT");
		return;
	}

	if (GetNtlSLGlobal()->GetAvatarMascotInfo()->bySummonIndex != INVALID_BYTE)
	{
		GetAlarmManager()->AlarmMessage("DST_WARN_ALREADY_RECALL_MASCOT");
		return;
	}

	CDboEventGenerator::SummonMascot(pData->byIndex);
}

VOID CMascotGui::OnSummonEndBtnClicked(gui::CComponent* pComponent)
{
	BYTE bySummonIndex = GetNtlSLGlobal()->GetAvatarMascotInfo()->bySummonIndex;
	if (bySummonIndex == INVALID_BYTE)
	{
		GetAlarmManager()->AlarmMessage("DST_WARN_NO_EXIST_RECALLED_MASCOT");
		return;
	}

	CDboEventGenerator::UnSummonMascot(bySummonIndex);
}

VOID CMascotGui::OnDeleteBtnClicked(gui::CComponent* pComponent)
{
	sMASCOT_DATA_EX* pData = GetSelectedData();
	if (pData == NULL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_NOT_SELECT");
		return;
	}

	if (GetNtlSLGlobal()->GetAvatarMascotInfo()->bySummonIndex == pData->byIndex)
	{
		GetAlarmManager()->AlarmMessage("DST_WARN_CANT_DEL_RECALLED_MASCOT");
		return;
	}

	CDboEventGenerator::DeleteMascot(pData->byIndex);
}

VOID CMascotGui::OnFusionBtnClicked(gui::CComponent* pComponent)
{
	sMASCOT_DATA_EX* pData = GetSelectedData();
	if (pData == NULL)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_NOT_SELECT");
		return;
	}

	// the server refuses fusions while any mascot is summoned
	if (GetNtlSLGlobal()->GetAvatarMascotInfo()->bySummonIndex != INVALID_BYTE)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_FUSION_MAINMASCOT_SUMMONING");
		return;
	}

	// only a mascot with full EXP that can still rank up is fused
	if (!Logic_CheckMascotFusion(pData))
		return;

	CMascotFusion* pFusion = reinterpret_cast<CMascotFusion*>(GetDialogManager()->GetDialog(DIALOG_MASCOT_FUSION));
	if (pFusion == NULL)
		return;

	CRectangle rect = GetPosition();
	pFusion->SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);
	pFusion->SetMainMascot(pData->byIndex);

	if (!GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_FUSION))
		GetDialogManager()->OpenDialog(DIALOG_MASCOT_FUSION);
}

VOID CMascotGui::OnSkillBtnClicked(gui::CComponent* pComponent)
{
	RwInt32 iSkillSlot = -1;
	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
	{
		if (m_pbtnSkill[i] == pComponent)
			iSkillSlot = i;
	}

	sMASCOT_DATA_EX* pData = GetSelectedData();
	if (iSkillSlot < 0 || pData == NULL)
		return;

	if (GetNtlSLGlobal()->GetAvatarMascotInfo()->bySummonIndex != pData->byIndex)
	{
		GetAlarmManager()->AlarmMessage("DST_MASCOTEX_SKILLGRADEUP_DONT_SUMMON");
		return;
	}

	CMascotSkillReMake* pReMake = reinterpret_cast<CMascotSkillReMake*>(GetDialogManager()->GetDialog(DIALOG_MASCOT_SKILL_REMAKE));
	if (pReMake == NULL)
		return;

	// the arrow toggles the window of its slot
	if (GetDialogManager()->IsOpenDialog(DIALOG_MASCOT_SKILL_REMAKE) && pReMake->GetSkillSlot() == (BYTE)iSkillSlot)
	{
		GetDialogManager()->CloseDialog(DIALOG_MASCOT_SKILL_REMAKE);
		return;
	}

	CRectangle rect = GetPosition();
	pReMake->SetPosition(rect.left + rect.GetWidth() + NTL_LINKED_DIALOG_GAP, rect.top);

	// an empty slot learns a random skill, a learned one can be re-rolled with an item
	pReMake->OpenSlot(pData->byIndex, (BYTE)iSkillSlot);
}

VOID CMascotGui::Select(BYTE byIndex)
{
	if (m_bySelectedIndex == byIndex)
		return;

	m_bySelectedIndex = byIndex;

	RefreshList();
	RefreshInfo();
}

sMASCOT_DATA_EX* CMascotGui::GetSelectedData(VOID)
{
	return GetNtlSLGlobal()->GetAvatarMascotInfo()->Find(m_bySelectedIndex);
}

sMASCOT_DATA_EX* CMascotGui::GetSlotData(RwInt32 iSlot)
{
	if (iSlot < 0 || iSlot >= dMAX_MASCOT_SLOT || m_abySlotIndex[iSlot] == INVALID_BYTE)
		return NULL;

	return GetNtlSLGlobal()->GetAvatarMascotInfo()->Find(m_abySlotIndex[iSlot]);
}

RwInt32 CMascotGui::GetMaxLine(VOID)
{
	RwInt32 iCount = GetNtlSLGlobal()->GetAvatarMascotInfo()->wCount;
	RwInt32 iLines = (iCount + dMASCOT_SLOT_ROW - 1) / dMASCOT_SLOT_ROW;

	return max(0, iLines - dMAX_MASCOT_SLOT / dMASCOT_SLOT_ROW);
}

RwBool CMascotGui::IsSkillSlotOpen(sMASCOT_DATA_EX* pData, RwInt32 iSkillSlot)
{
	// a mascot opens one skill slot per rank
	return iSkillSlot < pData->byItemRank;
}

VOID CMascotGui::RefreshList(VOID)
{
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	CRectangle rtScreen = m_pThis->GetScreenRect();

	RwInt32 iMaxLine = GetMaxLine();
	if (m_iCurLine > iMaxLine)
		m_iCurLine = iMaxLine;

	m_pscbMascot->SetMaxValue(iMaxLine);
	m_pscbMascot->SetValue(m_iCurLine, FALSE);

	m_surSelect.Show(FALSE);
	m_pflaSummoned->Show(false);

	RwInt32 iStart = m_iCurLine * dMASCOT_SLOT_ROW;
	for (RwInt32 i = 0; i < dMAX_MASCOT_SLOT; ++i)
	{
		RwInt32 iDataIdx = iStart + i;
		if (iDataIdx >= pInfo->wCount)
		{
			m_MascotSlot[i].Clear();
			m_abySlotIndex[i] = INVALID_BYTE;
			continue;
		}

		sMASCOT_DATA_EX& sData = pInfo->asMascotData[iDataIdx];
		const RwChar* pcIconName = Logic_GetMascotIconName(sData.tblidx);
		if (pcIconName)
			m_MascotSlot[i].SetIcon(pcIconName);
		else
			m_MascotSlot[i].Clear();
		m_abySlotIndex[i] = sData.byIndex;

		RwInt32 iX = rtScreen.left + m_MascotSlot[i].GetX_fromParent();
		RwInt32 iY = rtScreen.top + m_MascotSlot[i].GetY_fromParent();

		if (sData.byIndex == m_bySelectedIndex)
		{
			m_surSelect.SetRectWH(iX, iY, 32, 32);
			m_surSelect.Show(TRUE);
		}

		if (sData.byIndex == pInfo->bySummonIndex)
		{
			// same frame around the slot as the quick slot skill effect
			CRectangle rect;
			rect.SetRectWH(m_MascotSlot[i].GetX_fromParent() - 5, m_MascotSlot[i].GetY_fromParent() - 5, 42, 42);
			m_pflaSummoned->SetPosition(rect);
			if (!m_pflaSummoned->IsVisible())
			{
				m_pflaSummoned->GotoFrame(0);
				m_pflaSummoned->PlayMovie(TRUE);
			}
			m_pflaSummoned->Show(true);
		}
	}
}

VOID CMascotGui::RefreshInfo(VOID)
{
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	sMASCOT_DATA_EX* pData = GetSelectedData();

	if (pData == NULL)
	{
		m_Preview.Destroy();
		m_surPreview.Show(FALSE);

		m_psttbName->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_MASCOTNAME"));
		m_psttbGrade->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_MASCOT_GRADE"));
		m_psttbSummoned->Clear();
		m_psttbVp->Clear();
		m_psttbExp->Clear();
		m_ppgbVp->SetPos(0);
		m_ppgbExp->SetPos(0);

		RefreshSkills();
		return;
	}

	// model
	sMASCOT_STATUS_TBLDAT* pStatusTbl = (sMASCOT_STATUS_TBLDAT*)API_GetTableContainer()->GetMascotStatusTable()->FindData(pData->tblidx);
	const RwChar* pcModel = pStatusTbl ? pStatusTbl->szModel : NULL;
	m_Preview.SetDistance(pStatusTbl && pStatusTbl->byRank <= 1 ? PREVIEW_DISTANCE_CHILD : PREVIEW_DISTANCE);
	if (pcModel == NULL || m_Preview.GetModel() != pcModel)
	{
		m_Preview.SetModel(pcModel);

		// shown once the first frame is rendered, an untextured surface draws a white block
		m_surPreview.Show(FALSE);
		m_fPreviewWait = 0.0f;
	}

	// name and grade
	m_psttbName->SetText(Logic_GetMascotName(pData->tblidx));

	m_psttbGrade->SetText(Logic_GetMascotRankName(pData->byItemRank));

	if (pInfo->bySummonIndex == pData->byIndex)
		m_psttbSummoned->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_STATUS_SUBTITLE_SUMMON"));
	else
		m_psttbSummoned->Clear();

	// vp
	m_psttbVp->Format(L"%u / %u", pData->dwCurVP, pData->dwMaxVP);
	m_ppgbVp->SetRange(0, max((RwInt32)pData->dwMaxVP, 1));
	m_ppgbVp->SetPos(pData->dwCurVP);

	// exp, a mascot that cannot grow further has none to gain
	DWORD dwNeedExp = Logic_GetMascotNeedExp(pData);
	if (dwNeedExp == 0)
	{
		m_psttbExp->SetText(GetDisplayStringManager()->GetString("DST_MASCOTEX_MAX_GRADE_LIMITED"));
		m_ppgbExp->SetRange(0, 1);
		m_ppgbExp->SetPos(1);
	}
	else
	{
		m_psttbExp->Format(L"%u / %u", pData->dwCurExp, dwNeedExp);
		m_ppgbExp->SetRange(0, (RwInt32)dwNeedExp);
		m_ppgbExp->SetPos(pData->dwCurExp);
	}

	RefreshSkills();
}

VOID CMascotGui::RefreshSkills(VOID)
{
	SMascotInfo* pInfo = GetNtlSLGlobal()->GetAvatarMascotInfo();
	sMASCOT_DATA_EX* pData = GetSelectedData();
	RwBool bSummoned = pData && pInfo->bySummonIndex == pData->byIndex;

	for (RwInt32 i = 0; i < dMAX_MASCOT_SKILL; ++i)
	{
		if (m_SkillCoolTime[i].IsWork())
			m_SkillCoolTime[i].EndProc();

		if (pData == NULL || !IsSkillSlotOpen(pData, i))
		{
			m_SkillSlot[i].Clear();
			m_surSkillLock[i].Show(TRUE);
			m_pbtnSkill[i]->Enable(false);
			m_pbtnSkill[i]->SetToolTip(L"");
			continue;
		}

		m_surSkillLock[i].Show(FALSE);

		if (pData->skillTblidx[i] != INVALID_TBLIDX)
		{
			m_SkillSlot[i].SetIcon(pData->skillTblidx[i]);
			m_pbtnSkill[i]->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILLROLL_BUTTON"));
		}
		else
		{
			m_SkillSlot[i].Clear();
			m_pbtnSkill[i]->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_SKILL_CREATE_TITLE"));
		}

		// learning and re-rolling go through the summoned mascot, the click explains it otherwise
		m_pbtnSkill[i]->Enable(true);
		m_SkillSlot[i].ShowDisable(!bSummoned);
	}
}

VOID CMascotGui::ShowMascotInfoWnd(RwInt32 iSlot)
{
	sMASCOT_DATA_EX* pData = GetSlotData(iSlot);
	if (pData == NULL)
		return;

	stMACOT_INFO stMascot;
	stMascot.tblidx = pData->itemTblidx;
	stMascot.byItemRank = pData->byItemRank;
	stMascot.dwCurExp = pData->dwCurExp;
	stMascot.dwCurVP = pData->dwCurVP;
	stMascot.dwMaxVP = pData->dwMaxVP;
	stMascot.dwMaxExp = Logic_GetMascotNeedExp(pData);	// 0 shows a full bar for a mascot at its last rank

	CRectangle rtScreen = m_pThis->GetScreenRect();
	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_MASCOT_INFO,
		rtScreen.left + m_MascotSlot[iSlot].GetX_fromParent(),
		rtScreen.top + m_MascotSlot[iSlot].GetY_fromParent(),
		&stMascot, DIALOG_MASCOT);

	m_iInfoWndSlot = iSlot;
}

VOID CMascotGui::ShowSkillInfoWnd(RwInt32 iSkillSlot)
{
	sSKILL_TBLDAT* pSkillTbl = m_SkillSlot[iSkillSlot].GetSkillTable();
	if (pSkillTbl == NULL)
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_MASCOT_SKILL_INFO,
		rtScreen.left + m_SkillSlot[iSkillSlot].GetX_fromParent(),
		rtScreen.top + m_SkillSlot[iSkillSlot].GetY_fromParent(),
		pSkillTbl, DIALOG_MASCOT);

	m_iInfoWndSlot = dMAX_MASCOT_SLOT + iSkillSlot;
}

VOID CMascotGui::HideInfoWnd(VOID)
{
	if (GetInfoWndManager()->GetRequestGui() == DIALOG_MASCOT)
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}
