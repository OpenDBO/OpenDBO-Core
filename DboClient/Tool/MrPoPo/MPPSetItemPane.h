#pragma once
#include "afxwin.h"

/**
 * \ingroup MrPoPo
 * \brief Buttons for the game server's GM commands (DboServer/Server/GameServer/gm.cpp)
 *
 * Every button sends its command with the values of its fields, as typed in the chat.
 * The character needs the GM level of the command on the server.
 */
class CMPPSetItemPane : public CFormView
{
	DECLARE_DYNCREATE(CMPPSetItemPane)

protected:
	CMPPSetItemPane();
	virtual ~CMPPSetItemPane();

public:
	enum { IDD = IDD_MPPSETITEMPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual void OnInitialUpdate();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnGMButton(UINT nID);

	void    AddComboItem(UINT nComboID, LPCWSTR szName, DWORD dwData);
	BOOL    GetFieldValue(UINT nFieldID, CString& strValue);     ///< the text of an edit, the item data of a combo box
	void    SendCommandOfButton(UINT nButtonID);
	void    SendTypedCommand();
	void    CollectDragonBalls();
};
