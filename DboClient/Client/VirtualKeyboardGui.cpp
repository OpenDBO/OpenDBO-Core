#include "precomp_dboclient.h"
#include "VirtualKeyboardGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

#include "DisplayStringManager.h"
#include "DboEventGenerator.h"


CVirtualKeyboardGui::CVirtualKeyboardGui(const RwChar* pName)
{
	m_bIsCapLock = false;
}

CVirtualKeyboardGui::~CVirtualKeyboardGui()
{

}

RwBool CVirtualKeyboardGui::Create()
{
	NTL_FUNCTION("CVirtualKeyboardGui::Create");

	if (!CNtlPLGui::Create("gui\\VirtualKeyboard.rsr", "gui\\VirtualKeyboard.srf", "gui\\VirtualKeyboard.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pCapsLockButton = (gui::CButton*)GetComponent("CapsLockButton");
	m_pCapsLockButton->SetText(GetDisplayStringManager()->GetString("DST_VIRTUALKEYCAPSLOCK"));
	m_slotCapLockBtn = m_pCapsLockButton->SigClicked().Connect(this,&CVirtualKeyboardGui::OnClickCapsLockBtn);

	m_pBackSpaceButton = (gui::CButton*)GetComponent("BackSpaceButton");
	m_pBackSpaceButton->SetText(GetDisplayStringManager()->GetString("DST_VIRTUALKEYBACKSPACE"));
	m_slotBackSpaceBtn = m_pBackSpaceButton->SigClicked().Connect(this,&CVirtualKeyboardGui::OnClickBackSpace);

	m_pAllDeleteButton = (gui::CButton*)GetComponent("AllDeleteButton");
	m_pAllDeleteButton->SetText(GetDisplayStringManager()->GetString("DST_VIRTUALKEYALLDELETE"));
	m_slotAllDeleteBtn = m_pAllDeleteButton->SigClicked().Connect(this,&CVirtualKeyboardGui::OnClickAllDelete);

	m_pCloseButton = (gui::CButton*)GetComponent("CloseButton");
	m_pCloseButton->SetText(GetDisplayStringManager()->GetString("DST_VIRTUALKEYCLOSE"));
	m_slotCloseButton = m_pCloseButton->SigClicked().Connect(this,&CVirtualKeyboardGui::OnClickCloseBtn);

	CRectangle rect;

	rect.SetRect(0, 0, 22, 22);
	int x = 10;
	int y = 29;
	for (int i = 0;i < 50 ; i++)
	{
		m_pBtnKey[i] = NTL_NEW gui::CButton(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		m_pBtnKey[i]->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("VirtualKeyboard.srf", "srfVirtualKeyBtnUp"));
		m_pBtnKey[i]->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("VirtualKeyboard.srf", "srfVirtualKeyBtnFoc"));
		m_pBtnKey[i]->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("VirtualKeyboard.srf", "srfVirtualKeyBtnDown"));
		m_pBtnKey[i]->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("VirtualKeyboard.srf", "srfVirtualKeyBtnDis"));
		m_pBtnKey[i]->SetPosition(x,y);
		m_pBtnKey[i]->SetText(GetVirtualKeyboardString(i,false));
		m_slotClickKeyBtn[i] = m_pBtnKey[i]->SigClicked().Connect(this,&CVirtualKeyboardGui::OnClickKey);
		if ((i+1) % 10 == 0)
		{
			y += 23;
			x = 10;
		}
		else
			x += 23;

	}

	Show(true);
	NTL_RETURN(TRUE);
}

VOID CVirtualKeyboardGui::Destroy()
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}


VOID CVirtualKeyboardGui::OnClickCloseBtn(gui::CComponent* pComponent)
{
	Show(false);
}

VOID CVirtualKeyboardGui::OnClickCapsLockBtn(gui::CComponent* pComponent)
{
	if (m_bIsCapLock)
		m_bIsCapLock = false;
	else
		m_bIsCapLock = true;

	for (int i = 0; i < 50; i++)
	{
		m_pBtnKey[i]->SetText(GetVirtualKeyboardString(i, m_bIsCapLock));
	}
}

VOID CVirtualKeyboardGui::OnClickBackSpace(gui::CComponent* pComponent)
{
	CDboEventGenerator::SendVirtualKeyboard("51");
}

VOID CVirtualKeyboardGui::OnClickAllDelete(gui::CComponent* pComponent)
{
	CDboEventGenerator::SendVirtualKeyboard("52");
}

VOID CVirtualKeyboardGui::OnClickKey(gui::CComponent* pComponent)
{
	for (int i = 0; i < 50; i++)
	{
		if (m_pBtnKey[i] == pComponent)
		{
			CDboEventGenerator::SendVirtualKeyboard(GetVirtualKeyboardString(i, m_bIsCapLock));
			break;
		}
	}
}

std::string CVirtualKeyboardGui::GetVirtualKeyboardString(int index,RwBool isCapsLock)
{
	switch (index)
	{
		case 0: return "0"; break;
		case 1: return "1"; break;
		case 2: return "2"; break;
		case 3: return "3"; break;
		case 4: return "4"; break;
		case 5: return "5"; break;
		case 6: return "6"; break;
		case 7: return "7"; break;
		case 8: return "8"; break;
		case 9: return "9"; break;
		case 10:
			if (isCapsLock)
				return "Q";
			else
				return "q";
			break;
		case 11:
			if (isCapsLock)
				return "W";
			else
				return "w";
			break;
		case 12:
			if (isCapsLock)
				return "E";
			else
				return "e";
			break;
		case 13:
			if (isCapsLock)
				return "R";
			else
				return "r";
			break;
		case 14:
			if (isCapsLock)
				return "T";
			else
				return "t";
			break;
		case 15:
			if (isCapsLock)
				return "Y";
			else
				return "y";
			break;
		case 16:
			if (isCapsLock)
				return "U";
			else
				return "u";
			break;
		case 17:
			if (isCapsLock)
				return "I";
			else
				return "i";
			break;
		case 18:
			if (isCapsLock)
				return "O";
			else
				return "o";
			break;
		case 19:
			if (isCapsLock)
				return "P";
			else
				return "p";
			break;
		case 20:
			if (isCapsLock)
				return "A";
			else
				return "a";
			break;
		case 21:
			if (isCapsLock)
				return "S";
			else
				return "s";
			break;
		case 22:
			if (isCapsLock)
				return "D";
			else
				return "d";
			break;
		case 23:
			if (isCapsLock)
				return "F";
			else
				return "f";
			break;
		case 24:
			if (isCapsLock)
				return "G";
			else
				return "g";
			break;
		case 25:
			if (isCapsLock)
				return "H";
			else
				return "h";
			break;
		case 26:
			if (isCapsLock)
				return "J";
			else
				return "j";
			break;
		case 27:
			if (isCapsLock)
				return "K";
			else
				return "k";
			break;
		case 28:
			if (isCapsLock)
				return "L";
			else
				return "l";
			break;
		case 29:
			if (isCapsLock)
				return "Z";
			else
				return "z";
			break;
		case 30:
			if (isCapsLock)
				return "X";
			else
				return "x";
			break;
		case 31:
			if (isCapsLock)
				return "C";
			else
				return "c";
			break;
		case 32:
			if (isCapsLock)
				return "V";
			else
				return "v";
			break;
		case 33:
			if (isCapsLock)
				return "B";
			else
				return "b";
			break;
		case 34:
			if (isCapsLock)
				return "N";
			else
				return "n";
			break;
		case 35:
			if (isCapsLock)
				return "M";
			else
				return "m";
			break;
		case 36: return "!"; break;
		case 37: return "@"; break;
		case 38: return "#"; break;
		case 39: return "$"; break;
		case 40: return "%"; break;
		case 41: return "^"; break;
		case 42: return "&"; break;
		case 43: return "*"; break;
		case 44: return "("; break;
		case 45: return ")"; break;
		case 46: return "~"; break;
		case 47: return "?"; break;
		case 48: return ":"; break;
		case 49: return "/"; break;
	}

	return "";
}
