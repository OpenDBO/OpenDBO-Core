/******************************************************************************
* File			: DialogManager.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 8. 11
* Abstract		: 
*****************************************************************************
* Desc			: Dialog Manager
*****************************************************************************/

#pragma once

#include <map>
#include <set>

// core
#include "NtlDebug.h"
#include "ceventhandler.h"
#include "NtlCallbackWidget.h"

// gui
#include "gui_guimanager.h"

// simulation
#include "NtlSLDef.h"

// Dbo
#include "DialogDefine.h"
#include "DialogPriority.h"

class CNtlPLGui;
class CNtlSob;
class CNtlSobItem;

#define dREGULARDIALOG_FIRST_X		37
#define dREGULARDIALOG_GAP			10
#define dCHARM_Y					450
#define dNEED_SIZE_MORE_REGULAR		379	///< Regular Dialog�� �ϳ��� �� �� �ֱ� ���� �ʿ��� ������
#define dDAFAULT_REGULAR_COUNT		2	///< �⺻������ �� �� �ִ� ���ַ� ���̾�α��� ����



class CDialogManager : public RWS::CEventHandler
{
public:
	static CDialogManager*	GetInstance();

	struct DialogInfo
	{
		CNtlPLGui*				pDialog;		///< ���̾�α׷��� ������
		CNtlCallbackParam1bool* pCallSwitch;	///< ���̾�α� ���� �ݴ� �Լ����� �ݹ�

		DialogInfo()
		{
			pDialog = NULL;
			pCallSwitch = NULL;
		}
	};

	struct sOpenTarget
	{
		CNtlSob*				pOpenedTarget;	///< NPC Pointer to the target that opened the window
		RwV3d					v3Pos;
	};

	struct sRequestedOpenDialog
	{
		RwInt32					eRequestedDialogType;
		SERIAL_HANDLE			hSerial;
		RwBool					bSound;
	};

	typedef std::list<eDialogType>						LIST_OPENED_REGULAR_DIALOG;
	typedef std::list<eDialogType>::iterator			LIST_OPENED_REGULAR_ITER;

	typedef std::map<int, DialogInfo>					DIALOG_MAP;
	typedef std::map<int, DialogInfo>::iterator			DIALOG_MAP_ITER;
	typedef std::map<int, DialogInfo>::value_type		DIALOG_MAP_VALUE;

	typedef std::map<eDialogType, RwUInt32>				REGULAR_MAP;
	typedef std::map<eDialogType, RwUInt32>::iterator	REGULAR_MAP_ITER;

	typedef std::list<sRequestedOpenDialog>				LIST_REQUESTED_OPENDIALOG_IN_NARRATION;
	typedef std::list<sRequestedOpenDialog>::iterator	LIST_REQUESTED_OPENDIALOG_IN_NARRATION_ITER;

	// Entire Focusing Dialog
	typedef REGULAR_MAP									FE_MAP;
	typedef REGULAR_MAP_ITER							FE_MAP_ITER;

	typedef std::list<int>								DIALOG_RAISE;

	static CDialogManager* m_pInstance;

	virtual ~CDialogManager();
	
	static VOID	CreateInstance(gui::CGUIManager* pGuiManager);								///< �Ŵ��� �ʱ�ȭ	
	static VOID	DestoryInstance();

	VOID		Update(RwReal fElapsed);

	template <class Callbackclass>
	VOID		RegistDialog(int iDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen));	///< ���̾�α׸� ����Ѵ�.	
	VOID		UnRegistDialog(int iDialog);		///< �ش� ���̾�α׸� �����Ѵ�.	

	VOID		SwitchBag(RwBool bOpen);			///< ���� �����ֱ� On/Off
	VOID		SwitchBagByIndex(RwInt32 nIndex);
	RwBool		SwitchDialog(int iDialog);			///< ���̾�α׸� ���ų� �ݴ´�.	

	VOID		OpenGMDialog();						///< GM ���̾�α׸� ���� ����
	VOID		OpenDefaultDialog();				///< �⺻ ���̾�α׸� ����
	RwBool		OpenDialog(int iDialog, SERIAL_HANDLE hSerial = INVALID_SERIAL_ID, RwBool bPlaySound = TRUE); ///< ���̾�α׸� ����, uiNPCSerial : NPC ���̾�α��� ���

	RwBool		CloseDialog(int iDialog, RwBool bPlaySound = TRUE);				///< ���̾�α׸� �ݴ´�.
	VOID		CloseGMDialog();					///< GM ���̾�α׸� ���� �ݴ´�
	VOID		CloseNotDefaultDialog();			///< �⺻ ���̾�α׸� ������ ��� ���̾�α׸� �ݴ´�.
	VOID		CloseAll();							///< ��� ���̾�α׸� �ݴ´�.


	VOID		RaiseDialogbyUser(RwInt32 iDialog);	///< ������ ���� ���̾�αװ� ȭ�鿡 ��Ÿ���� ���� ���� ���� �׷����� ������ �˸���.
	VOID		HideDialogbyUser(RwInt32 iDialog);	///< ������ ���� ���̾�αװ� ȭ�鿡�� ������� ���� ȭ�鿡 �׷����� ������ �˸���.
		
	RwInt32		GetOpendNPCDIalog();				///< �����ִ� NPC ���̾�α׸� �˻��Ѵ�.	
	CNtlSob*	GetOpenedTarget();

	DialogInfo*	GetDialogInfo(int iDialog);			///< �ش� ���̾�α��� ������ ��ȯ�Ѵ�.
	CNtlPLGui*	GetDialog(int iDialog);			///< �ش� ���̾�α��� �����͸� ��ȯ�Ѵ�.
	RwInt32		GetTopDialog();						///< ���� ������ �׷����� �ִ� ���̾�α��� �ε����� ��ȯ�Ѵ�.

		int HandleDialogAction(uintptr_t iAction); ///< �׼��� ó���Ѵ�. by Kell
	VOID		HandleMouse(const CKey& key);		///< ���콺�� ó���Ѵ�

	VOID		OnMode(eDialogMode eMode);			///< ��� ����
	VOID		OffMode();							///< ��带 �����Ѵ�
	eDialogMode	GetMode();
	RwInt32		HowManyOpenMaxRegular();			///< ���� Ŭ���̾�Ʈ�� ���̿��� �� �� �ִ� �ִ� ���ַ� ���̾�α��� ������ ��ȯ�Ѵ�
	RwInt32		GetPosY_of_HalfRegular();			///< ���̰� ���������� ���ַ� ���̾�α��� Y ��ǥ�� ��ȯ�Ѵ�
	VOID		Disable_OnObserver(RwBool bVisible);	///< Observer ��忡�� �Ϻ� ���̾�α׸� �������� ���θ� ���Ѵ�

	VOID		LocateBag(RwInt32 iWidth, RwInt32 iHeight, RwBool bForce = FALSE);///< ������ �˸��� ��ġ�� �����Ѵ�
	VOID		LocationDialogs(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_MovableDefaultPosition();	///< ���̾�αװ� ������ �� ���� ���� �⺻ ��ġ�� ����

	RwBool		IsExistDialog(RwInt32 iDialog);		///< ���̾�αװ� �����ϴ��� ����
	RwBool		CanMoveItem_in_Capsule();			///< ĸ������ �������� ������ �� �ִ��� �Ǵ�
	RwBool		CanPickupItem_in_Capsule();			///< ĸ������ �������� ���� �� �ִ��� �Ǵ�
	RwBool		IsOpenDialog(int iDialog);			///< ���̾�αװ� �����ִ��� �˻��Ѵ�.
	RwBool		IsBagOpen();						///< t : ������ �ϳ� �̻� �����ִ�.
	RwBool		IsOpenNPCDialog();					///< NPC ���� ���̾�αװ� �����ִ��� �˻��Ѵ�.
	RwBool		IsNPCDialog(RwInt32 iDialog);		///< NPC ���� ���̾�α����� �˻��Ѵ�.
	RwBool		IsTriggerSystemDialogOpen();		///< Quest, Trigger���� â�� �����ִ��� �˻��Ѵ�. by peessi
	RwBool		IsMode(eDialogMode eMode);			///< ���̾�α� ���� ��带 �˾ƺ���
	RwBool		IsRegularDialog(eDialogType eType);
	RwBool		IsEFDialog(eDialogType eType);
	RwBool		IsOpenEFDialog();
	RwBool		IsHaveAttribute(eDialogType eType, RwUInt32 uiAttr);
	RwBool		CanOpenRegularDialog();
	RwBool		IsDialogMovable();
	//// Test Func				/// woosungs_test 20090723
	CNtlPLGui* const GetpDialogTEST(const char* szFrmFileName);
	RwBool		ShowDialogTEST(RwInt32 iDialog, bool bOpen = true);
	VOID		ShowAllDialogTEST(bool bOpen = true);

	void			SetClickedItem(CNtlSobItem* pItem) { m_pClickedItem = pItem; }
	CNtlSobItem*	GetClickedItem() { return m_pClickedItem; }

protected:
	CDialogManager();
	CDialogManager(const CDialogManager& dialog) {};

	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID		RegisteRegularDialog();
	VOID		RegisteEntireFocusingDialog();
	
	VOID		ProcessESC();						///< ESCŰ�� ������ �� ó��	
	VOID		ProcessOpenRegular(eDialogType eDialog);
	VOID		ProcessLayer(eDialogType eDialog);		///< ���̾�α��� ���̾ �����Ѵ�
	VOID		ProcessCloseRegular(eDialogType eDialog);
	VOID		ProcessMovableDialogs(RwBool bMovable);

	VOID		Locate_AutoPosition(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_UserPosition(RwInt32 iWidth, RwInt32 iHeight);
	VOID		Locate_UserDefaultPosition(RwInt32 iWidth, RwInt32 iHeight);	

	VOID		PlayOpenSound(RwInt32 iDialog);		///< ������ ���̾�α׸� �� ���� ȿ����
	VOID		PlayCloseSound(RwInt32 iDialog);	///< ������ ���̾�α׸� ���� ���� ȿ����

protected:
	gui::CSlot		m_slotMouse;

	DIALOG_MAP		m_mapDialog;					///< ��ϵ� ���̾�α׵��� ������ ����
	sOpenTarget		m_OpenedTarget;
	
	eDialogMode		m_eMode;						///< ���
	RwInt32			m_iRegularStrartY;
	RwInt32			m_iOldWidth;
	RwInt32			m_iOldHeight;

	RwBool			m_bExpectingCloseNPCDialog;
	RwBool			m_bESCSkip;
	RwBool			m_bDialogMovable;
	RwBool			m_bCanVisible_OnOberserver;

	LIST_OPENED_REGULAR_DIALOG m_listOpenedRegularDilaog;
	LIST_REQUESTED_OPENDIALOG_IN_NARRATION m_listRequestedOpenDialogInNarration;	
	
	DIALOG_RAISE	m_listRaiseDialogbyUser;		///< ������ ���ۿ� ���� ȭ�鿡 ��Ÿ���� GUI�� ����
													///< 가장 나중에 그리는 다이얼로그 순으로 오름차순 정렬

	REGULAR_MAP		m_mapRegular;					///< ȭ���� ������ ��ġ�� ������ ���̾�α��� ����
	FE_MAP			m_mapEntireFocuing;				///< ȭ���� ��ü ������ �̿��ϸ� ������ ������ ���߽�Ű�� ���� ���̾�α�

	// If we clicked an item (like dogi ball)
	CNtlSobItem*	m_pClickedItem;
};

static CDialogManager* GetDialogManager(VOID)
{
	return CDialogManager::GetInstance();
}

template <class Callbackclass>
VOID CDialogManager::RegistDialog(int iDialog, CNtlPLGui* pDialog, int (Callbackclass::*callback)(bool bOpen))
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);
	if (pDialogInfo)
	{
		return;
	}

	DialogInfo info;

	pDialog->SetMovable(m_bDialogMovable);

	info.pDialog = pDialog;
	info.pCallSwitch = NTL_NEW CNtlCallbackWidget1bool<Callbackclass>(reinterpret_cast<Callbackclass*>(pDialog), callback);

	m_mapDialog[iDialog] = info;

	// EFDialog 우선순위 결정
	if( IsEFDialog((eDialogType)iDialog) )
		info.pDialog->GetDialog()->SetPriority(dDIALOGPRIORITY_EFDIALOG);

	// Regular Dialog, 가방, 사이드 다이얼로그는 dDIALOGPRIORITY_EFDIALOG 를 쓴다
}