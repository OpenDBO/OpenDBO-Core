#include "precomp_ntlsimulation.h"
#include "InputHandler.h"

SKeyData CInputHandler::g_LastKeyData;
SMouseData CInputHandler::g_LastMouseData;

#define MOUSE_CAPTURE_MAX_DELTA		50		// tw clamps every mouse move to this many pixels

CInputHandler::CInputHandler()
{
	m_bActive = TRUE;

	m_bMouseCaptureEnable = FALSE;
	m_hMouseCaptureWnd = NULL;
	memset(&m_sMouseCapture, 0, sizeof(m_sMouseCapture));
}

CInputHandler::~CInputHandler()
{
}

CInputHandler* CInputHandler::GetInstance(void)
{
	static CInputHandler Instance;
	return &Instance;
}


void CInputHandler::UnLinkKeyDown(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkKeyDown");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyDown.begin(); it != m_listKeyDown.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listKeyDown.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkKeyUp(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkKeyUp");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyUp.begin(); it != m_listKeyUp.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listKeyUp.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseDown(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseDown");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseDown.begin(); it != m_listMouseDown.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseDown.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseUp(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseUp");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseUp.begin(); it != m_listMouseUp.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseUp.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseMove(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseMove");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseMove.begin(); it != m_listMouseMove.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseMove.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}

void CInputHandler::UnLinkMouseWheel(INPUT_HANDLE handle)
{
	NTL_FUNCTION("CInputHandler::UnLinkMouseWheel");
	
	CNtlCallbackParam1 *pWidget = reinterpret_cast<CNtlCallbackParam1*>(handle);

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseWheel.begin(); it != m_listMouseWheel.end(); ++it)
	{
		CNtlCallbackParam1 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			NTL_DELETE(pCurrWidget);
			m_listMouseWheel.erase(it);
			break;
		}
	}

	NTL_RETURNVOID();
}



void CInputHandler::KeyDownHandler(unsigned int uiChar, unsigned int uiRepCount)
{
	NTL_FUNCTION("CInputHandler::KeyDownHandler");

	if ( !m_bActive ) return;

	SKeyData Key = { uiChar, uiRepCount};
	g_LastKeyData = Key;

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyDown.begin(); it != m_listKeyDown.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<uintptr_t>(&Key) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::KeyUpHandler(unsigned int uiChar, unsigned int uiRepCount)
{
	NTL_FUNCTION("CInputHandler::KeyUpHandler");

	if ( !m_bActive ) return;

	SKeyData Key = { uiChar, uiRepCount};
	g_LastKeyData = Key;

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listKeyUp.begin(); it != m_listKeyUp.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<uintptr_t>(&Key) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseDownHandler(unsigned int uiFlags, int nPosX, int nPosY, char chBtnType, bool bDblClick)
{
	NTL_FUNCTION("CInputHandler::MouseDownHandler");

	if ( !m_bActive ) return;

	if (m_bMouseCaptureEnable)
		BeginMouseCapture(chBtnType, nPosX, nPosY, bDblClick);

	SMouseData Mouse = { uiFlags, nPosX, nPosY, 0, chBtnType, bDblClick };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseDown.begin(); it != m_listMouseDown.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<uintptr_t>(&Mouse) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseUpHandler(unsigned int uiFlags, int nPosX, int nPosY, char chBtnType, unsigned int uiDeltaTime)
{
	NTL_FUNCTION("CInputHandler::MouseUpHandler");

	// the capture ends even when the input went inactive while the button was held (loading, movie),
	// otherwise the cursor stays hidden and pinned to the middle of the window
	if (m_bMouseCaptureEnable)
		EndMouseCapture(chBtnType, nPosX, nPosY);

	if ( !m_bActive ) return;

	SMouseData Mouse = { uiFlags, nPosX, nPosY, 0, chBtnType, 0, uiDeltaTime };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseUp.begin(); it != m_listMouseUp.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<uintptr_t>(&Mouse) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseMoveHandler(unsigned int uiFlags, int nPosX, int nPosY)
{
	NTL_FUNCTION("CInputHandler::MouseMoveHandler");

	if ( !m_bActive ) return;

	// while captured the cursor stays in the middle of the window, the listeners get the virtual position
	if (m_bMouseCaptureEnable && IsMouseCaptured())
	{
		POINT ptCur;
		if (GetCursorPos(&ptCur))
		{
			int iDeltaX = max(-MOUSE_CAPTURE_MAX_DELTA, min(MOUSE_CAPTURE_MAX_DELTA, (int)(ptCur.x - m_sMouseCapture.ptLast.x)));
			int iDeltaY = max(-MOUSE_CAPTURE_MAX_DELTA, min(MOUSE_CAPTURE_MAX_DELTA, (int)(ptCur.y - m_sMouseCapture.ptLast.y)));

			m_sMouseCapture.ptVirtual.x += iDeltaX;
			m_sMouseCapture.ptVirtual.y += iDeltaY;
			m_sMouseCapture.ptLast = ptCur;
		}

		nPosX = m_sMouseCapture.ptVirtual.x;
		nPosY = m_sMouseCapture.ptVirtual.y;
	}

	SMouseData Mouse = { uiFlags, nPosX, nPosY, 0, 0 };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseMove.begin(); it != m_listMouseMove.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<uintptr_t>(&Mouse) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::MouseWheelHandler(unsigned int uiFlags, short shDelta, int nPosX, int nPosY)
{
	NTL_FUNCTION("CInputHandler::MouseWheelHandler");

	if ( !m_bActive ) return;

	if (m_bMouseCaptureEnable && IsMouseCaptured())
	{
		nPosX = m_sMouseCapture.ptVirtual.x;
		nPosY = m_sMouseCapture.ptVirtual.y;
	}

	SMouseData Mouse = { uiFlags, nPosX, nPosY, shDelta, MOUSE_LBTN, 0 };

	std::list<CNtlCallbackParam1*>::iterator it;
	for(it = m_listMouseWheel.begin(); it != m_listMouseWheel.end(); ++it)
	{
		(*it)->Call( reinterpret_cast<uintptr_t>(&Mouse) );
	}

	NTL_RETURNVOID();
}

void CInputHandler::EnableMouseCapture(HWND hWnd, RwBool bEnable)
{
	if (!bEnable)
		ReleaseMouseCapture();

	m_hMouseCaptureWnd = hWnd;
	m_bMouseCaptureEnable = bEnable;
}

bool CInputHandler::GetMouseCaptureCenter(POINT& ptCenter)
{
	if (m_hMouseCaptureWnd == NULL || ::IsIconic(m_hMouseCaptureWnd))
		return false;

	RECT rtClient;
	if (!::GetClientRect(m_hMouseCaptureWnd, &rtClient))
		return false;

	ptCenter.x = rtClient.left + (rtClient.right - rtClient.left) / 2;
	ptCenter.y = rtClient.top + (rtClient.bottom - rtClient.top) / 2;

	return ::ClientToScreen(m_hMouseCaptureWnd, &ptCenter) != FALSE;
}

void CInputHandler::BeginMouseCapture(char chBtnType, int& nPosX, int& nPosY, bool bDblClick)
{
	unsigned int uiButtonBit = 1 << chBtnType;

	// tw: a double click ends the capture of that button
	if (bDblClick)
	{
		if (m_sMouseCapture.uiButtonMask & uiButtonBit)
		{
			m_sMouseCapture.uiButtonMask &= ~uiButtonBit;

			nPosX = m_sMouseCapture.ptVirtual.x;
			nPosY = m_sMouseCapture.ptVirtual.y;

			if (m_sMouseCapture.uiButtonMask == 0)
				RestoreMouseCursor();
		}

		return;
	}

	// another button already holds the capture, keep its restore point and virtual position
	if (m_sMouseCapture.uiButtonMask != 0)
	{
		m_sMouseCapture.uiButtonMask |= uiButtonBit;

		nPosX = m_sMouseCapture.ptVirtual.x;
		nPosY = m_sMouseCapture.ptVirtual.y;
		return;
	}

	if (!IsMouseCaptureWndForeground())
		return;

	POINT ptCenter;
	if (!GetMouseCaptureCenter(ptCenter))
		return;

	if (!::GetCursorPos(&m_sMouseCapture.ptRestore))
		return;

	m_sMouseCapture.uiButtonMask = uiButtonBit;
	m_sMouseCapture.ptPress.x = m_sMouseCapture.ptVirtual.x = nPosX;
	m_sMouseCapture.ptPress.y = m_sMouseCapture.ptVirtual.y = nPosY;

	::SetCursorPos(ptCenter.x, ptCenter.y);
	m_sMouseCapture.ptLast = ptCenter;

	if (!m_sMouseCapture.bCursorHidden)
	{
		::ShowCursor(FALSE);
		m_sMouseCapture.bCursorHidden = true;
	}
}

void CInputHandler::EndMouseCapture(char chBtnType, int& nPosX, int& nPosY)
{
	unsigned int uiButtonBit = 1 << chBtnType;

	if ((m_sMouseCapture.uiButtonMask & uiButtonBit) == 0)
		return;

	m_sMouseCapture.uiButtonMask &= ~uiButtonBit;

	// tw reports the position where the button was pressed (the cursor itself is in the middle of the window)
	nPosX = m_sMouseCapture.ptPress.x;
	nPosY = m_sMouseCapture.ptPress.y;

	if (m_sMouseCapture.uiButtonMask == 0)
		RestoreMouseCursor();
}

void CInputHandler::RestoreMouseCursor(void)
{
	::SetCursorPos(m_sMouseCapture.ptRestore.x, m_sMouseCapture.ptRestore.y);

	if (m_sMouseCapture.bCursorHidden)
	{
		::ShowCursor(TRUE);
		m_sMouseCapture.bCursorHidden = false;
	}
}

void CInputHandler::UpdateMouseCapture(void)
{
	if (!m_bMouseCaptureEnable || !IsMouseCaptured())
		return;

	// the capture only lives while the game window is in front: a focus loss the WM_ACTIVATE handling missed
	// (alt+tab, minimize) would keep pulling the cursor of the other application into the game window
	if (!IsMouseCaptureWndForeground())
	{
		ReleaseMouseCapture();
		return;
	}

	POINT ptCenter;
	if (!GetMouseCaptureCenter(ptCenter))
		return;

	::SetCursorPos(ptCenter.x, ptCenter.y);
	m_sMouseCapture.ptLast = ptCenter;
}

bool CInputHandler::IsMouseCaptureWndForeground(void)
{
	if (m_hMouseCaptureWnd == NULL || ::IsIconic(m_hMouseCaptureWnd))
		return false;

	return ::GetForegroundWindow() == ::GetAncestor(m_hMouseCaptureWnd, GA_ROOT);
}

void CInputHandler::ReleaseMouseCapture(void)
{
	if (!IsMouseCaptured())
		return;

	m_sMouseCapture.uiButtonMask = 0;
	RestoreMouseCursor();
}
