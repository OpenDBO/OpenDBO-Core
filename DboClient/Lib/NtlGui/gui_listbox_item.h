#ifndef __GUI_LISTBOX_ITEM_H__
#define __GUI_LISTBOX_ITEM_H__

#include "gui_define.h"
#include "gui_unibuffer.h"

START_GUI

class CListBox_Item
{
public:
	CListBox_Item() : m_bSelected(false) {}

	CListBox_Item( const WCHAR* pString, const CGuiFont* pFont, DWORD dwData = 0, bool bSelected = false )
	{
		m_pBuffer = NTL_NEW CUniBuffer( 0 );
		m_pBuffer->SetSizeCalcEnable();
		m_pBuffer->SetFont( pFont );
		m_pBuffer->SetText( pString );		
		m_dwData	= dwData;
		m_bSelected = bSelected;
	}

	~CListBox_Item()
	{
		NTL_DELETE( m_pBuffer );
	}

	bool operator< ( const CListBox_Item &item ) const
	{
		std::wstring str1, str2;
		str1 = m_pBuffer->GetBuffer();
		str2 = item.m_pBuffer->GetBuffer();

		return str1 < str2;
	}

	bool operator== ( const CListBox_Item &item ) const
	{
		std::wstring str1, str2;
		str1 = m_pBuffer->GetBuffer();
		str2 = item.m_pBuffer->GetBuffer();

		return str1 == str2;
	}

	CUniBuffer* m_pBuffer;
	DWORD		m_dwData;		
	bool		m_bSelected;	
};

END_GUI

#endif
