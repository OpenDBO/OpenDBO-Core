//*****************************************************************************
// File       : CXMLParserW.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

#ifndef ___C_XML_PARSER_W_H___
#define ___C_XML_PARSER_W_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringTreeNodeW;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedString.h"
#include "CFixedStringList.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CXMLParser
// Desc       : 
// Author     : 
//=============================================================================

class CXMLParserW
{
	protected:

		CFixedStringW					m_cFileName;
		CFixedStringTreeNodeW *			m_pcRootNode;

	public:

		inline							CXMLParserW( void ) { Initialize(); }
		inline							CXMLParserW( const CFixedStringW & rcFileName ) { Initialize(); Create( rcFileName ); }

		inline							~CXMLParserW( void ) { Destroy(); }

	private:

										CXMLParserW( const CXMLParserW & rcParser );

		const CXMLParserW &				operator =( const CXMLParserW & rcParser );

	public:

		bool							Create( const CFixedStringW & rcFileName );

		CFixedStringTreeNodeW *			GetRoot( void ) const { return m_pcRootNode; }

		CFixedStringTreeNodeW *			FindChild( UInt32 nOrder = 0 ) const;
		CFixedStringTreeNodeW *			FindChild( const CFixedStringW & rcKeyword ) const;
		CFixedStringTreeNodeW *			FindChild( const CFixedStringW & rcKeyword, UInt32 nId ) const;

		bool							Report( const CFixedStringW & rcReportFileName ) const;

	protected:

		void							Initialize( void );
		void							Destroy( void );

		bool							GenerateTree( const CFixedStringListW & rcList );

		UInt32							SkipEncodingTag( const CFixedStringListW & rcList );
		UInt32							GenerateList( CFixedStringTreeNodeW * pcParent, const CFixedStringListW & rcList, UInt32 nPosition );
		void							GeneratePair( CFixedStringTreeNodeW * pcParent, const CFixedStringW & rcPair );

		bool							ReportNode( CFixedStringListW & rcList, CFixedStringTreeNodeW * pcNode, UInt32 nDepth ) const;

	public:

		static const SVersionInfo		VERSION;
};

//=============================================================================
// End of Class : CXMLParserW
//=============================================================================

#endif //___C_XML_PARSER_W_H___

//*****************************************************************************
//
// End of File : CXMLParserW.h
//
//*****************************************************************************