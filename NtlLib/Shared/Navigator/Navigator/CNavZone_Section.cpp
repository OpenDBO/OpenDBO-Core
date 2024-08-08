//*****************************************************************************
// File       : CNavZone_Section.cpp
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CNavSection.h"

#include "CNavZone.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

USE_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name			: GetSection
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CNavSection * const CNavZone::GetSection( UInt32 nSectionOrder ) const
{
	return down_cast<CNavSection *>(m_cSections.GetAt( nSectionOrder ));
}


//-----------------------------------------------------------------------------
// Name			: LinkSection
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::LinkSection( CNavSection * const pcSection )
{
	return m_cSections.Insert( pcSection );
}


//-----------------------------------------------------------------------------
// Name			: UnlinkSection
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

bool CNavZone::UnlinkSection( CNavSection * const pcSection )
{
	return m_cSections.Remove( pcSection );
}


//*****************************************************************************
//
// End of File : CNavZone_Section.cpp
//
//*****************************************************************************