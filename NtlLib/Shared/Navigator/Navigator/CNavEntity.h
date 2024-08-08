//*****************************************************************************
// File       : CNavEntity.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_ENTITY_H___
#define ___C_NAV_ENTITY_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavEntityBase.h"

#include "CNavEntityVector.h"
#include "CNavEntityFactory.h"

#include "CNavEntityHierarchy.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavOutputStream;
class CNavRenderer;
class CNavPath;
class CNavEntityVector;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavEntity
// Desc       : 
// Author     : 
//=============================================================================

class CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		CNavBounding					m_cBounding;
		CFixedString					m_cName;
		CFixedString					m_cTitle;
		ENavEntityType					m_eType;
		bool							m_bExport;

		UInt32							m_nChildCount;
		UInt32							m_nDescendantCount;

	private:

		UId32							m_nUniqueId;
		CNavEntity *					m_pcParent;

	protected:

		inline							CNavEntity( UId32 nUniqueId ) : m_nUniqueId( nUniqueId ), m_pcParent( NULL ),
																		m_eType( E_NAV_ENTITY_TYPE_NONE ), m_bExport( false ),
																		m_nChildCount( 0 ), m_nDescendantCount( 0 ) { Initialize(); }

		inline virtual					~CNavEntity( void ) { PreDestroy(); Destroy(); }

	private:

										CNavEntity( void );
										CNavEntity( const CNavEntity & rcEntity );

		const CNavEntity &				operator =( const CNavEntity & rcEntity );

	public:

		// Common

		inline virtual bool				Create( SNavEntityCreateInfo & rsCreateInfo )
										{
											Destroy();
											Initialize();

											m_bExport	= rsCreateInfo.bExport;
											m_cName		= rsCreateInfo.cName;
											m_cTitle	= rsCreateInfo.cTitle;
											m_pcParent	= rsCreateInfo.pcParent;
											m_cBounding.Set( rsCreateInfo.cBounding );

											return true;
										}

		inline virtual bool				PostCreate( SNavEntityCreateInfo & rsCreateInfo )
										{
											if( rsCreateInfo.pcParent != NULL )
											{
												rsCreateInfo.pcParent->InsertChild( this );
												m_pcParent = rsCreateInfo.pcParent;
											}

											return true;
										}

		inline ENavEntityType			GetType( void ) const { return m_eType; }

		inline const CFixedString &		GetName( void ) const { return m_cName; }
		inline const CFixedString &		GetTitle( void ) const { return m_cTitle; }

		inline virtual bool				InsertChild( CNavEntity * const pcChild ) { UNREFERENCED_PARAMETER( pcChild ); return false; }
		inline virtual bool				RemoveChild( CNavEntity * const pcChild ) { UNREFERENCED_PARAMETER( pcChild ); return false; }

		inline virtual CNavEntity * const	GetChildByUniqueId( UId32 nUniqueId ) const { UNREFERENCED_PARAMETER( nUniqueId ); return NULL; }
		inline virtual CNavEntity * const	GetChildByPosition( const CNavPosition & rcPosition1 ) const { UNREFERENCED_PARAMETER( rcPosition1 ); return NULL; }
		inline virtual CNavEntity * const	GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const { UNREFERENCED_PARAMETER( rcPosition1 ); (void)rcPosition2; return NULL; }
		inline virtual UInt32			GetChildrenByIntersection( const CNavBounding & rcBounding, CNavEntityVector * pcEntityVector ) const { UNREFERENCED_PARAMETER( rcBounding ); (void)pcEntityVector; return NULL; }

		inline virtual bool				GetHeight( CNavPosition & rcPosition ) const { UNREFERENCED_PARAMETER( rcPosition ); return false; }
		inline virtual DWORD			GetAttribute( CNavPosition & rcPosition ) const { UNREFERENCED_PARAMETER( rcPosition ); return E_NAV_TILE_ATTRIBUTE_NONE; }

		inline virtual Int32			Export( CNavOutputStream * pcOutputStream ) { UNREFERENCED_PARAMETER( pcOutputStream ); return 0; }
		inline virtual Int32			Render( CNavRenderer * const pcRenderer ) const { UNREFERENCED_PARAMETER( pcRenderer ); return 0; }
		inline virtual Int32			Process( SNavEntityProcessInfo * psProcessInfo ) { UNREFERENCED_PARAMETER( psProcessInfo ); return 0; }

		inline virtual bool				PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo ) { UNREFERENCED_PARAMETER( rsPreProcessInfo ); return true; }
		inline virtual bool				PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo ) { UNREFERENCED_PARAMETER( rsPostProcessInfo ); return true; }

		inline virtual bool				DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData ) { return pfnToDo( this, pvData ); }
		inline virtual bool				DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData ) { return pfnToDo( this, pvData ); }

		inline virtual bool				IsValid( void ) const { return true; }

		inline virtual bool				GetHierarchy( CNavEntityHierarchy & rcHierarchy, const CNavPosition & rcPosition ) const { UNREFERENCED_PARAMETER( rcHierarchy ); UNREFERENCED_PARAMETER( rcPosition ); return false; }

		// Exclusive

		inline UId32					GetUniqueId( void ) const { return m_nUniqueId; }

		inline const CNavBounding &		GetBounding( void ) const { return m_cBounding; }
		inline Int32					GetW( void ) const { return m_cBounding.GetW(); }

		inline bool						IsInBound( const CNavPosition & rcPosition ) const { return m_cBounding.IsContain( rcPosition ); }
		inline bool						IsIntersect( const CNavBounding & rcBounding ) const { return m_cBounding.IsIntersect( rcBounding ); }

		inline CNavEntity *	const		GetParent( void ) const { return m_pcParent; }
		inline CNavEntity *	const		GetRoot( void ) const { if( m_pcParent == NULL ) return const_cast<CNavEntity * const>(this); else return m_pcParent->GetRoot(); }

		inline CNavEntity * const		GetChildByBounding( const CNavBounding & rcBounding ) const { UNREFERENCED_PARAMETER( rcBounding ); return GetChildByPosition( GetBounding().GetMin(), GetBounding().GetMax() ); }

		inline UInt32					GetChildCount( void ) const { return m_nChildCount; }
		inline UInt32					GetDescendantCount( void ) const { return m_nDescendantCount; }

	protected:

		// Common

		inline virtual void				Initialize( void ) { m_cBounding.Invalidate(); m_cName.Clear(); }
		inline virtual void				Destroy( void ) { m_cName.Clear(); }
		inline virtual void				PreDestroy( void )
		{
			if( m_pcParent != NULL )
			{
			//	m_pcParent->RemoveChild( this );
				m_pcParent = NULL;
			}
		}

		inline virtual bool				CreateFromFile( const CFixedString & rcFileName, UInt32 nFlags = 0 ) { (void)rcFileName; (void)nFlags; return 0; }
		inline virtual bool				DestroyChild( void ) { return 0; }

		// Exclusive

		inline void						SetParent( CNavEntity * const pcParent ) { m_pcParent = pcParent; }

	public:

		static const CNavEntity			NULL_ENTITY;
};

//=============================================================================
// End of Class : CNavEntity
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_ENTITY_H___

//*****************************************************************************
//
// End of File : CNavEntity.h
//
//*****************************************************************************