// MPPSetItemPane.cpp : the GM command pane
//

#include "stdafx.h"
#include "MrPoPo.h"
#include "MPPSetItemPane.h"
#include "MPPClientEvent.h"

namespace
{
    // the client sends less than NTL_MAX_LENGTH_OF_CHAT_MESSAGE (256) characters
    const int MAX_COMMAND_LENGTH = 255;

    /// a button and the fields whose values follow its command, in order
    struct SGMButton
    {
        UINT    nButtonID;
        LPCWSTR szCommand;
        UINT    anFieldID[3];       ///< 0 when unused
        UINT    nOptionalMask;      ///< bit i: anFieldID[i] may stay empty
    };

    const SGMButton s_aGMButton[] =
    {
        // character
        { IDC_GM_BTN_LEVEL,             L"@setlevel",       { IDC_GM_LEVEL } },
        { IDC_GM_BTN_CLASS,             L"@setclass",       { IDC_GM_CLASS } },
        { IDC_GM_BTN_SPEED,             L"@setspeed",       { IDC_GM_SPEED } },
        { IDC_GM_BTN_ZENI,              L"@setzenny",       { IDC_GM_ZENI } },
        { IDC_GM_BTN_INVINCIBLE,        L"@invincible",     { IDC_GM_INVINCIBLE }, 0x1 },
        { IDC_GM_BTN_GOD,               L"@god" },
        { IDC_GM_BTN_HIDE,              L"@hide" },
        { IDC_GM_BTN_RESTORE,           L"@r" },
        { IDC_GM_BTN_COOLDOWN,          L"@removeskillscd" },
        { IDC_GM_BTN_EXP_ON,            L"@exp on" },
        { IDC_GM_BTN_EXP_OFF,           L"@exp off" },
        { IDC_GM_BTN_PETEXP,            L"@petexp" },
        { IDC_GM_BTN_PURGE,             L"@purge" },

        // items and skills
        { IDC_GM_BTN_ITEM,              L"@additem",        { IDC_GM_ITEM, IDC_GM_ITEM_COUNT }, 0x2 },
        { IDC_GM_BTN_GRADE,             L"@upgrade",        { IDC_GM_GRADE } },
        { IDC_GM_BTN_RANK,              L"@setitemrank",    { IDC_GM_RANK } },
        { IDC_GM_BTN_SKILL,             L"@addskill",       { IDC_GM_SKILL } },
        { IDC_GM_BTN_HTB,               L"@addhtb",         { IDC_GM_HTB } },
        { IDC_GM_BTN_MASTER_PASSIVE,    L"@addskill2" },
        { IDC_GM_BTN_TITLE,             L"@addtitle",       { IDC_GM_TITLE } },
        { IDC_GM_BTN_MUDOSA,            L"@addmudosa",      { IDC_GM_MUDOSA } },

        // movement
        { IDC_GM_BTN_PLACE,             L"@teleport",       { IDC_GM_PLACE } },
        { IDC_GM_BTN_GO,                L"@go",             { IDC_GM_X, IDC_GM_Y, IDC_GM_Z } },
        { IDC_GM_BTN_WORLD,             L"@world",          { IDC_GM_WORLD } },
        { IDC_GM_BTN_WARP,              L"@warp",           { IDC_GM_NAME } },
        { IDC_GM_BTN_CALL,              L"@call",           { IDC_GM_NAME } },
        { IDC_GM_BTN_BIND,              L"@bind" },
        { IDC_GM_BTN_WARFOG,            L"@warfog" },

        // spawn
        { IDC_GM_BTN_MOB,               L"@addmob",         { IDC_GM_MOB } },
        { IDC_GM_BTN_NPC,               L"@addnpc",         { IDC_GM_NPC } },

        // server
        { IDC_GM_BTN_NOTICE,            L"@notice",         { IDC_GM_NOTICE_TYPE, IDC_GM_NOTICE } },
    };
}

// CMPPSetItemPane

IMPLEMENT_DYNCREATE(CMPPSetItemPane, CFormView)

CMPPSetItemPane::CMPPSetItemPane()
: CFormView(CMPPSetItemPane::IDD)
{
}

CMPPSetItemPane::~CMPPSetItemPane()
{
}

BEGIN_MESSAGE_MAP(CMPPSetItemPane, CFormView)
    ON_COMMAND_RANGE(IDC_GM_BTN_FIRST, IDC_GM_BTN_LAST, &CMPPSetItemPane::OnGMButton)
END_MESSAGE_MAP()


#ifdef _DEBUG
void CMPPSetItemPane::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMPPSetItemPane::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CMPPSetItemPane::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // class ids of the server (ePC_CLASS)
    static const LPCWSTR s_aszClass[] =
    {
        L"Human Fighter", L"Human Mystic", L"Human Engineer", L"Namekian Warrior", L"Namekian Dragon Clan",
        L"Mighty Majin", L"Wonder Majin", L"Street Fighter", L"Swordsman", L"Crane Hermit", L"Turtle Hermit",
        L"Gun Mania", L"Mech Mania", L"Dark Warrior", L"Shadow Knight", L"Dende Priest", L"Poko Priest",
        L"Ultimate Majin", L"Grand Chef Majin", L"Plasma Majin", L"Karma Majin",
    };
    for(int i = 0; i < _countof(s_aszClass); ++i)
        AddComboItem(IDC_GM_CLASS, s_aszClass[i], i);

    AddComboItem(IDC_GM_RANK, L"Normal", 1);
    AddComboItem(IDC_GM_RANK, L"Superior", 2);
    AddComboItem(IDC_GM_RANK, L"Excellent", 3);
    AddComboItem(IDC_GM_RANK, L"Rare", 4);
    AddComboItem(IDC_GM_RANK, L"Legendary", 5);

    // eDRAGON_BALL_TYPE, the client finds the balls in its dragon ball table
    AddComboItem(IDC_GM_DRAGONBALL, L"Basic", 1);
    AddComboItem(IDC_GM_DRAGONBALL, L"Normal", 2);
    AddComboItem(IDC_GM_DRAGONBALL, L"Scramble", 5);

    // the places of @teleport (EMPPTeleport)
    AddComboItem(IDC_GM_PLACE, L"Yahoi Village", MPP_TELE_YAHOI);
    AddComboItem(IDC_GM_PLACE, L"Yureka Park", MPP_TELE_YUREKA);
    AddComboItem(IDC_GM_PLACE, L"Dalpang Village", MPP_TELE_DALPANG);
    AddComboItem(IDC_GM_PLACE, L"Dragon's Snore", MPP_TELE_DRAGON);
    AddComboItem(IDC_GM_PLACE, L"Baee Village", MPP_TELE_BAEE);
    AddComboItem(IDC_GM_PLACE, L"Ajirang Spring", MPP_TELE_AJIRANG);
    AddComboItem(IDC_GM_PLACE, L"Karinga Village 1", MPP_TELE_KARINGA_1);
    AddComboItem(IDC_GM_PLACE, L"Karinga Village 2", MPP_TELE_KARINGA_2);
    AddComboItem(IDC_GM_PLACE, L"Great Tree", MPP_TELE_GREAT_TREE);
    AddComboItem(IDC_GM_PLACE, L"Karinga Fishing Village", MPP_TELE_KARINGA_3);
    AddComboItem(IDC_GM_PLACE, L"Mermaid Statue", MPP_TELE_MERMAID);
    AddComboItem(IDC_GM_PLACE, L"Gannet Observatory", MPP_TELE_GANNET);
    AddComboItem(IDC_GM_PLACE, L"Emerald Beach", MPP_TELE_EMERALD);
    AddComboItem(IDC_GM_PLACE, L"Tembarin Camp", MPP_TELE_TEMBARIN);
    AddComboItem(IDC_GM_PLACE, L"Cell Ring", MPP_TELE_CELL);
    AddComboItem(IDC_GM_PLACE, L"Buu Memorial", MPP_TELE_BUU);
    AddComboItem(IDC_GM_PLACE, L"CC History Museum", MPP_TELE_CC);
    AddComboItem(IDC_GM_PLACE, L"Mushroom Village", MPP_TELE_MUSHROOM);
    AddComboItem(IDC_GM_PLACE, L"Papaya Island", MPP_TELE_PAPAYA);

    AddComboItem(IDC_GM_NOTICE_TYPE, L"Chat", 0);
    AddComboItem(IDC_GM_NOTICE_TYPE, L"Screen", 1);
    AddComboItem(IDC_GM_NOTICE_TYPE, L"Chat and screen", 2);
    AddComboItem(IDC_GM_NOTICE_TYPE, L"Tori NPC", 3);
    AddComboItem(IDC_GM_NOTICE_TYPE, L"Caution", 4);
    AddComboItem(IDC_GM_NOTICE_TYPE, L"Territory", 5);

    static const UINT s_anComboID[] = { IDC_GM_CLASS, IDC_GM_RANK, IDC_GM_DRAGONBALL, IDC_GM_PLACE, IDC_GM_NOTICE_TYPE };
    for(int i = 0; i < _countof(s_anComboID); ++i)
    {
        CComboBox* pCombo = (CComboBox*)GetDlgItem(s_anComboID[i]);
        pCombo->SetCurSel(0);

        // the list opens as wide as its longest name
        CClientDC dc(pCombo);
        CFont* pOldFont = dc.SelectObject(pCombo->GetFont());
        int nWidth = 0;
        for(int nItem = 0; nItem < pCombo->GetCount(); ++nItem)
        {
            CString strItem;
            pCombo->GetLBText(nItem, strItem);
            nWidth = max(nWidth, (int)dc.GetTextExtent(strItem).cx);
        }
        dc.SelectObject(pOldFont);
        pCombo->SetDroppedWidth(nWidth + ::GetSystemMetrics(SM_CXVSCROLL) + 8);
    }

    ((CEdit*)GetDlgItem(IDC_GM_COMMAND))->SetLimitText(MAX_COMMAND_LENGTH);
    ((CEdit*)GetDlgItem(IDC_GM_NOTICE))->SetLimitText(MAX_COMMAND_LENGTH - 16);
}

void CMPPSetItemPane::AddComboItem( UINT nComboID, LPCWSTR szName, DWORD dwData )
{
    CComboBox* pCombo = (CComboBox*)GetDlgItem(nComboID);
    int nIndex = pCombo->AddString(szName);
    pCombo->SetItemData(nIndex, dwData);
}

BOOL CMPPSetItemPane::GetFieldValue( UINT nFieldID, CString& strValue )
{
    CWnd* pField = GetDlgItem(nFieldID);
    if(pField == NULL)
        return FALSE;

    WCHAR szClass[32] = {0,};
    ::GetClassName(pField->GetSafeHwnd(), szClass, _countof(szClass));
    if(_wcsicmp(szClass, L"ComboBox") == 0)
    {
        CComboBox* pCombo = (CComboBox*)pField;
        int nSel = pCombo->GetCurSel();
        if(nSel == CB_ERR)
            return FALSE;

        strValue.Format(L"%u", (DWORD)pCombo->GetItemData(nSel));
        return TRUE;
    }

    pField->GetWindowText(strValue);
    strValue.Trim();
    return !strValue.IsEmpty();
}

void CMPPSetItemPane::OnGMButton( UINT nID )
{
    switch(nID)
    {
    case IDC_GM_BTN_DRAGONBALL:
        CollectDragonBalls();
        break;
    case IDC_GM_BTN_LEARN_ALL:
        // the client knows the skills of the class
        GetMPPClientEvent()->SendEvent(MSG_MRPOPO_LEARN_ALL_SKILL, 0);
        break;
    case IDC_GM_BTN_COMMAND:
        SendTypedCommand();
        break;
    default:
        SendCommandOfButton(nID);
        break;
    }
}

void CMPPSetItemPane::SendCommandOfButton( UINT nButtonID )
{
    for(int i = 0; i < _countof(s_aGMButton); ++i)
    {
        const SGMButton& button = s_aGMButton[i];
        if(button.nButtonID != nButtonID)
            continue;

        CString strCommand = button.szCommand;
        for(int nField = 0; nField < _countof(button.anFieldID) && button.anFieldID[nField]; ++nField)
        {
            CString strValue;
            if(GetFieldValue(button.anFieldID[nField], strValue))
            {
                strCommand += L" ";
                strCommand += strValue;
            }
            else if(!(button.nOptionalMask & (1 << nField)))
            {
                MessageBox(L"Fill in the fields of this command first.", NULL, MB_ICONWARNING);
                GetDlgItem(button.anFieldID[nField])->SetFocus();
                return;
            }
            else
            {
                // the later fields would shift into the place of the empty one
                break;
            }
        }

        GetMPPClientEvent()->SendServerCommand(strCommand);
        return;
    }
}

void CMPPSetItemPane::SendTypedCommand()
{
    CString strCommand;
    GetDlgItem(IDC_GM_COMMAND)->GetWindowText(strCommand);
    strCommand.Trim();
    if(strCommand.IsEmpty())
        return;

    if(strCommand[0] != L'@')
        strCommand.Insert(0, L'@');

    GetMPPClientEvent()->SendServerCommand(strCommand);
}

void CMPPSetItemPane::CollectDragonBalls()
{
    CString strType;
    if(GetFieldValue(IDC_GM_DRAGONBALL, strType))
        GetMPPClientEvent()->SendEvent(MSG_MRPOPO_COLLECT_DRAGONBALL, (DWORD)_wtoi(strType));
}
