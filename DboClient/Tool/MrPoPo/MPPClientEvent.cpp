#include "StdAfx.h"
#include "MPPClientEvent.h"
#include "MPPLogView.h"

// window class of the client main window (NtlApplication.h kMainWindowClassName)
#define DBO_CLIENT_WINDOW_CLASS		L"DragonBall"

CMPPClientEvent::CMPPClientEvent(void)
{
    m_hDBOClient = NULL;
}

CMPPClientEvent::~CMPPClientEvent(void)
{
}

BOOL CMPPClientEvent::CheckClientConnect()
{
    // found by its class only, the window title changed over time
    m_hDBOClient = FindWindow(DBO_CLIENT_WINDOW_CLASS, NULL);
    if(!m_hDBOClient)
    {
        MessageBox(NULL, L"Can't Connect DBO Client", NULL, MB_OK);
        AddLogView(L"Can't Connect DBO Client");
        return FALSE;
    }

    return TRUE;
}

void CMPPClientEvent::SendEvent( int nMsgCode, DWORD param )
{
    if(!CheckClientConnect())
        return;

    SendMessage(m_hDBOClient, MSG_FROM_MRPOPO, nMsgCode, param);
}

void CMPPClientEvent::SendServerCommand( const CString& strCommand )
{
    if(!CheckClientConnect())
        return;

    // WM_COPYDATA copies the text into the client process
    COPYDATASTRUCT cds;
    cds.dwData = MPP_COPYDATA_SERVER_COMMAND;
    cds.cbData = (strCommand.GetLength() + 1) * sizeof(WCHAR);
    cds.lpData = (PVOID)(LPCWSTR)strCommand;

    if(SendMessage(m_hDBOClient, WM_COPYDATA, (WPARAM)AfxGetMainWnd()->GetSafeHwnd(), (LPARAM)&cds))
        AddLogView(L"Sent: " + strCommand);
    else
        AddLogView(L"Not sent (needs a dev user in the game, and a command under 256 characters): " + strCommand);
}
