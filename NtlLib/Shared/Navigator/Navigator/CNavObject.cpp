//*****************************************************************************
// File       : CNavObject.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavPathEngine.h"
#include "CNavRenderer.h"

#include "CNavObject.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CFixedString	CNavObject::m_cPathName			= CFixedString::NULLSTRING;


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavObject::Initialize( void )
{
	m_pcShape		= NULL;
	m_cPosition.Reset();

	m_pcAgent		= NULL;

	m_nVertexCount	= 0L;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CNavObject::Destroy( void )
{
	if( m_pcAgent != NULL )
	{
		delete m_pcAgent;
	}

	if( m_pcShape != NULL )
	{
		delete m_pcShape;
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

bool CNavObject::Create( SNavEntityCreateInfo & rsCreateInfo )
{
	if( CNavEntity::Create( rsCreateInfo ) == false )
	{
		return false;
	}

	switch( rsCreateInfo.eCreateType )
	{
		case E_NAV_CREATE_TYPE_FILE:
		{
			CFixedString	cFullFilePathName;
			cFullFilePathName.AppendFormat( _T( "%s\\%s.xml" ),
											CNavObject::m_cPathName.GetBuffer(),
											rsCreateInfo.cName.GetBuffer() );

			m_pcShape = g_pPathEngine->LoadShape( E_NAV_FILE_TYPE_XML, cFullFilePathName );

			break;
		}

		case E_NAV_CREATE_TYPE_INSTANCE:
		{
			Assert( rsCreateInfo.pvUserData != NULL );

			m_pcShape = reinterpret_cast<iShape *>(rsCreateInfo.pvUserData);

			break;
		}

		default:
		{
			m_pcShape = NULL;
		}
	}

	if( m_pcShape == NULL )
	{
		return false;
	}

	m_nVertexCount = m_pcShape->size();

	m_cPosition.Set( rsCreateInfo.cPosition );

	CalculateBoundingFromShape();

	return true;
}


//-----------------------------------------------------------------------------
// Name			: Render
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

Int32 CNavObject::Render( CNavRenderer * const pcRenderer ) const
{
	Assert( pcRenderer != NULL );

	CNavPosition	cPolygonVertex[256];

	for( UInt32 nVertexIndex = 0L; nVertexIndex < m_nVertexCount; ++nVertexIndex )
	{
		GetVertex( nVertexIndex, cPolygonVertex[nVertexIndex] );
	}

	for( UInt32 nVertexIndex = 0L; nVertexIndex < m_nVertexCount; ++nVertexIndex )
	{
		pcRenderer->RenderLine( cPolygonVertex[nVertexIndex], cPolygonVertex[(nVertexIndex + 1) % m_nVertexCount], 0x000000FF, 3 );
	}

	return 1L;
}


//*****************************************************************************
//
// End of File : CNavObject.cpp
//
//*****************************************************************************