//*****************************************************************************
// File       : CXMLParserW.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "VersionLog.h"

#include "CFixedStringTreeNodeW.h"
#include "CFixedStringTreeW.h"

#include "CXMLParserW.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const SVersionInfo CXMLParserW::VERSION( _VERSION_MAJOR_, _VERSION_MINOR_, _VERSION_BUILD_, _VERSION_LABEL_ );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CXMLParserW::Initialize( void )
{
	m_pcRootNode = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CXMLParserW::Destroy( void )
{
	if( m_pcRootNode != NULL )
	{
		delete m_pcRootNode;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CXMLParserW::Create( const CFixedStringW & rcFileName )
{
	Destroy();

	CFixedStringListW	cStringList;
	if( cStringList.CreateFromFile( rcFileName ) == 0 )
	{
		Assert( !"CXMLParserW::Create() : CreateFromFile() Failure" );

		return false;
	}

	if( GenerateTree( cStringList ) == false )
	{
		Assert( !"CXMLParserW::Create() : GenerateTree() Failure" );

		return false;
	}

	//Report( CFixedStringW( L"Test.txt" ) );
	m_cFileName.Assign( rcFileName );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: FindChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CXMLParserW::FindChild( UInt32 nOrder ) const
{
	if( m_pcRootNode == NULL )
	{
		return NULL;
	}

	return m_pcRootNode->FindChild( nOrder );
}


//-----------------------------------------------------------------------------
// Name			: FindChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CXMLParserW::FindChild( const CFixedStringW & rcKeyword ) const
{
	if( m_pcRootNode == NULL )
	{
		return NULL;
	}

	return m_pcRootNode->FindChild( rcKeyword );
}


//-----------------------------------------------------------------------------
// Name			: FindChild
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CFixedStringTreeNodeW * CXMLParserW::FindChild( const CFixedStringW & rcKeyword, UInt32 nId ) const
{
	if( m_pcRootNode == NULL )
	{
		return NULL;
	}

	return m_pcRootNode->FindChild( rcKeyword, nId );
}


//-----------------------------------------------------------------------------
// Name			: GenerateTree
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CXMLParserW::GenerateTree( const CFixedStringListW & rcList )
{
	m_pcRootNode = new CFixedStringTreeNodeW( NULL );

	UInt32	nIndex = SkipEncodingTag( rcList );

	if( GenerateList( m_pcRootNode, rcList, nIndex ) != rcList.GetCount() )
	{
		Assert( !"CXMLParserW::GenerateTree() : GenerateList() Failure" );

		if( m_pcRootNode != NULL )
		{
			delete m_pcRootNode;
			m_pcRootNode = NULL;
		}

		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: SkipEncodingTag
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CXMLParserW::SkipEncodingTag( const CFixedStringListW & rcList )
{
	UInt32	nPosition = 0;
	bool	bFoundTag = false;

	CFixedStringW *		pcString = rcList.GetAt( nPosition );

	while( pcString != NULL )
	{
		if( pcString->GetFirst() == L'<' &&
			pcString->GetAt( 1 ) == L'?' )
		{
			bFoundTag = true;
		}

		if( bFoundTag == true &&
			pcString->GetLast() == L'>' &&
			pcString->GetLength() >= 2 &&
			pcString->GetAt( pcString->GetLength() - 2 ) == L'?' )
		{
			return ++nPosition;
		}

		pcString = rcList.GetAt( ++nPosition );
	}

	if( bFoundTag == false )
	{
		return 0;
	}
	else
	{
		return rcList.GetCount();
	}
}


//-----------------------------------------------------------------------------
// Name			: GenerateList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

UInt32 CXMLParserW::GenerateList( CFixedStringTreeNodeW * pcParent, const CFixedStringListW & rcList, UInt32 nPosition )
{
	CFixedStringW *		pcString = rcList.GetAt( nPosition );
	CFixedStringTreeNodeW *		pcNode = NULL;

	while( pcString != NULL )
	{
		++nPosition;

		if( pcString->GetFirst() == L'<' )
		{
			if( pcString->GetAt( 1 ) == L'/' )
			{
				return nPosition;
			}

			CFixedStringW *		pcKeyword;

			if( pcString->GetLast() == L'>' )
			{
				if( pcString->GetAt( pcString->GetLength() - 2 ) == L'/' )
				{
					pcString = rcList.GetAt( nPosition );

					continue;
				}
				else
				{
					pcKeyword = pcString->CreateSubString( 1, Max( static_cast<UInt32>(pcString->GetLength()), static_cast<UInt32>(2) ) - 2 );
				}
			}
			else
			{
				pcKeyword = pcString->CreateSubString( 1, Max( static_cast<UInt32>(pcString->GetLength()), static_cast<UInt32>(1) ) - 1 );
			}

			pcNode = new CFixedStringTreeNodeW( *pcKeyword, pcParent );
			Assert( pcNode != NULL );

			delete pcKeyword;

			pcParent->PushBackChild( pcNode );

			nPosition = GenerateList( pcNode, rcList, nPosition );
		}
		else if( pcString->GetLast() == L'>' &&
				pcString->GetLength() >= 2 &&
				pcString->GetAt( pcString->GetLength() - 2 ) == L'/' )
		{
			return nPosition;
		}
		else if( pcString->GetFirst() == L'=' )
		{
			if( pcString->GetLength() == 1 )
			{
				GeneratePair( pcNode, *(rcList.GetAt( nPosition )) );

				++nPosition;
			}
			else
			{
				CFixedStringW *		pcPair = pcString->CreateSubString( 1, static_cast<UInt32>(pcString->GetLength() - 1) );

				GeneratePair( pcNode, *pcPair );

				delete pcPair;
			}
		}
		else if( pcString->GetLast() == L'=' )
		{
			CFixedStringW *		pcPair = pcString->CreateSubString( 0, static_cast<UInt32>(pcString->GetLength() - 2) );

			pcNode = new CFixedStringTreeNodeW( *pcPair, pcParent );
			Assert( pcNode != NULL );

			delete pcPair;

			pcParent->PushBackChild( pcNode );

			GeneratePair( pcNode, *(rcList.GetAt( nPosition )) );

			++nPosition;
		}
		else
		{
			pcNode = new CFixedStringTreeNodeW( *pcString, pcParent );
			Assert( pcNode != NULL );

			pcParent->PushBackChild( pcNode );
		}
		
		pcString = rcList.GetAt( nPosition );
	}

	return nPosition;
}


//-----------------------------------------------------------------------------
// Name			: GenerateList
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CXMLParserW::GeneratePair( CFixedStringTreeNodeW * pcKey, const CFixedStringW & rcPair )
{
	static const CFixedStringW		KEYWORD_ID( L"id" );

	CFixedStringTreeNodeW *	pcNode = new CFixedStringTreeNodeW( rcPair, pcKey );
	Assert( pcNode != NULL );

	pcKey->PushFrontChild( pcNode );

	if( pcKey->GetString().IsEqual( KEYWORD_ID ) &&
		pcKey->GetParent() != NULL )
	{
		pcKey->GetParent()->SetId( rcPair.ConvertToInt32() );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Report
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CXMLParserW::Report( const CFixedStringW & rcReportFileName ) const
{
	CFixedStringListW		cList;

	ReportNode( cList, m_pcRootNode, 0 );

	HANDLE		hFile = CreateFileSafeW( rcReportFileName.GetBuffer(), GENERIC_WRITE );

	cList.Serialize( hFile );

	CloseHandle( hFile );

	return true;
}


//-----------------------------------------------------------------------------
// Name			: ReportNode
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CXMLParserW::ReportNode( CFixedStringListW & rcList, CFixedStringTreeNodeW * pcNode, UInt32 nDepth ) const
{
	CFixedStringW		cReport;

	for( UInt32 nTab = 0; nTab < nDepth; ++nTab )
	{
		cReport.AppendFormat( L"\t" );
	}

	cReport.AppendFormat( L"[Id]=%d\t[String]=\"%s\"\r\n",
						pcNode->GetId(), pcNode->GetString().GetBuffer() );
	rcList.Push( &cReport );

	if( pcNode->GetChildCount() == 0 )
	{
		return true;
	}

	CFixedStringTreeNodeW *	pcChildNode = pcNode->GetChildList()->GetFirst();

	while( pcChildNode->GetNext() != pcChildNode )
	{
		ReportNode( rcList, pcChildNode, nDepth + 1 );

		pcChildNode = pcChildNode->GetNext();
	}

	return true;
}


//*****************************************************************************
//
// End of File : CXMLParserW.cpp
//
//*****************************************************************************