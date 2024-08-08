//*****************************************************************************
// File       : CNavFilePath.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_FILE_PATH_H___
#define ___C_NAV_FILE_PATH_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavFilePath
// Desc       : 
// Author     : 
//=============================================================================

class CNavFilePath
{
	protected:

		CFixedString					m_cPath;
		CFixedString					m_cName;
		CFixedString					m_cExt;

	public:

		inline							CNavFilePath( void ) {}
		inline							CNavFilePath( const CNavFilePath & rcFilePath ) { Set( rcFilePath ); }

		inline virtual					~CNavFilePath( void ) {}

		inline const CNavFilePath &		operator =( const CNavFilePath & rcFilePath ) { Set( rcFilePath ); return *this; }

	public:

		inline const CFixedString &		GetName( void ) const { return m_cName; }
		inline const CFixedString &		GetPath( void ) const { return m_cPath; }
		inline const CFixedString &		GetExt( void ) const { return m_cExt; }

		inline const bool				IsGlobal( void ) const { return ( m_cPath.GetLength() > 2 && m_cPath.GetAt( 1 ) == ':' ); } 
		inline const bool				HasExt( void ) const { return ( m_cExt.GetLength() > 0 ); }

		inline void						Set( const CNavFilePath & rcFilePath )
		{
			SetPath( rcFilePath.GetPath() );
			SetName( rcFilePath.GetName() );
			SetExt( rcFilePath.GetExt() );
		}

		inline void						Set( const CFixedString & rcPath, const CFixedString & rcName, const CFixedString & rcExt )
		{
			SetPath( rcPath );
			SetName( rcName );
			SetExt( rcExt );
		}

		inline void						SetPath( const CFixedString & rcPath ) { m_cPath = rcPath; }
		inline void						SetName( const CFixedString & rcName ) { m_cName = rcName; }
		inline void						SetExt( const CFixedString & rcExt ) { m_cExt = rcExt; }

		inline void						GetFullPathName( CFixedString & rcFullPath ) const
		{
			if( m_cPath.GetLength() > 0 )
			{
				if( IsGlobal() == false && rcFullPath.GetLength() > 0 )
				{
					rcFullPath.Append( CNavStringTable::DIRECTORY_SEPARATOR );
					rcFullPath.Append( m_cPath );
				}
				else
				{
					rcFullPath.Assign( m_cPath );
				}
			}

			if( m_cName.GetLength() > 0 )
			{
				if( rcFullPath.GetLength() > 0 )
				{
					rcFullPath.Append( CNavStringTable::DIRECTORY_SEPARATOR );
				}

				rcFullPath.Append( m_cName );
			}

			if( m_cExt.GetLength() > 0 )
			{
				if( rcFullPath.GetLength() > 0 )
				{
					rcFullPath.Append( CNavStringTable::EXTENSION_SEPARATOR );
				}

				rcFullPath.Append( m_cExt );
			}
		}

		inline void						GetFullPathName( CFixedString & rcFullPath, const CFixedString & rcIdentifier ) const
		{
			if( m_cPath.GetLength() > 0 )
			{
				if( IsGlobal() == false && rcFullPath.GetLength() > 0 )
				{
					rcFullPath.Append( CNavStringTable::DIRECTORY_SEPARATOR );
					rcFullPath.Append( m_cPath );
				}
				else
				{
					rcFullPath.Assign( m_cPath );
				}
			}

			if( m_cName.GetLength() > 0 )
			{
				if( rcFullPath.GetLength() > 0 )
				{
					rcFullPath.Append( CNavStringTable::DIRECTORY_SEPARATOR );
				}

				rcFullPath.Append( m_cName );
			}

			if( rcIdentifier.GetLength() > 0 )
			{
				rcFullPath.Append( rcIdentifier );
			}

			if( m_cExt.GetLength() > 0 )
			{
				if( rcFullPath.GetLength() > 0 )
				{
					rcFullPath.Append( CNavStringTable::EXTENSION_SEPARATOR );
				}

				rcFullPath.Append( m_cExt );
			}
		}
};

//=============================================================================
// End of Class : CNavFilePath
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_FILE_PATH_H___

//*****************************************************************************
//
// End of File : CNavFilePath.h
//
//*****************************************************************************