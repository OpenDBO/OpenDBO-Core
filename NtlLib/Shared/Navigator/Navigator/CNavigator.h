//*****************************************************************************
// File       : CNavigator.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAVIGATOR_H___
#define ___C_NAVIGATOR_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CNavShapeManager.h"

#include "CNavEntity.h"


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringTreeNodeW;


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavRenderer;
class CNavConverter;
class CNavEntityVector;
class CNavWorld;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define g_pcNavigator								( CNavigator::GetInstance() )

#define NAVIGATOR_DATA_PATH_DEFAULT					( _T( "NavData" ) )
#define WORLD_LIST_SIZE_DEFAULT						( 1L )
#define MAX_PATH_LENGTH								( 1024 )

#define FIND_UNBLOCKED_POSITION_DISTANCE_DEFAULT	( 1000 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavigator
// Desc       : 
// Author     : 
//=============================================================================

class CNavigator : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		CNavEntityVector *				m_pcWorlds;
		UInt32							m_nWorldCount;
		Fixed32							m_nFindUnblockedPositionDistance;

		CNavShapeManager				m_cShapeManager;

		CNavRenderer *					m_pcRenderer;
		CNavConverter *					m_pcImporter;
		CNavConverter *					m_pcExporter;

		UInt32							m_nThreadSize;

	protected:

		inline							CNavigator( UId32 nUniqueId ) :
																		CNavEntity( nUniqueId ), m_nFindUnblockedPositionDistance( FIND_UNBLOCKED_POSITION_DISTANCE_DEFAULT ),
																		m_pcRenderer( NULL ), m_pcImporter( NULL ), m_pcExporter( NULL ), m_nThreadSize( 0 )
																		{ Initialize(); m_eType = E_NAV_ENTITY_TYPE_NAVIGATOR; }

		inline virtual					~CNavigator( void ) { Destroy(); }

	private:

										CNavigator( void );
										CNavigator( const CNavigator & rcNavigator );

		const CNavigator &				operator =( const CNavigator & rcNavigator );

	public:

		// Common

		bool							Create( SNavEntityCreateInfo & rsCreateInfo );

		bool							InsertChild( CNavEntity * const pcChild );
		bool							RemoveChild( CNavEntity * const pcChild );

		CNavEntity * const				GetChildByUniqueId( UId32 nUniqueId ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1 ) const;
		CNavEntity * const				GetChildByPosition( const CNavPosition & rcPosition1, const CNavPosition & rcPosition2 ) const;

		bool							GetHeight( CNavPosition & rcPosition ) const;
		DWORD							GetAttribute( CNavPosition & rcPosition ) const;

		bool							PreProcess( SNavEntityPreProcessInfo & rsPreProcessInfo );
		bool							PostProcess( SNavEntityPreProcessInfo & rsPostProcessInfo );

		bool							DoIt( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );
		bool							DoItAfter( bool (*pfnToDo)( CNavEntity * pcThis, void * pvData ), void * pvData = NULL );

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		bool							GetHierarchy( CNavEntityHierarchy & rcHierarchy, const CNavPosition & rcPosition ) const;

		// Exclusive

		// 검토 필요! - 메쉬가 없는 경우는 보장, 하지만 장애물에 경우는 보장 확인 안해봄
		inline bool						FindUnblockedPosition( CNavPosition & rcSearchPosition, UId32 nShapeId = 0 ) const { return FindUnblockedPositionInternal( rcSearchPosition, m_nFindUnblockedPositionDistance, nShapeId ); }
		inline bool						FindUnblockedPosition( CNavPosition & rcSearchPosition, Float32 fDistance, UId32 nShapeId = 0 ) const { return FindUnblockedPositionInternal( rcSearchPosition, Float32ToFixed32( fDistance ), nShapeId ); }

		bool							IsBlockedPosition( CNavPosition & rcSearchPosition, UId32 nShapeId = 0 ) const;

		// 검토 필요! - 제약이 풀렸다라는 얘기를 본것 같은데 확인은 안해봄.
		inline void						SetFindUnblockedPositionDistance( Float32 fDistance ) { m_nFindUnblockedPositionDistance = Float32ToFixed32( fDistance ); }
		inline Float32					GetFindUnblockedPositionDistance( void ) const { return Fixed32ToFloat32( m_nFindUnblockedPositionDistance ); }

		CNavEntity * const				CreateEntity( SNavEntityCreateInfo & rsCreateInfo, bool bInsertChild = false );
		void							DestroyEntity( CNavEntity * pcEntity );

		inline UId32					RegisterShape( const CFixedString & rcShapeName, UInt32 nFlags = 0 ) { return m_cShapeManager.Register( rcShapeName, nFlags ); }
		UId32							RegisterShape( const TCHAR * pchShapeName, UInt32 nFlags = 0 );
		inline void						UnregisterShape( UId32 nShapeId, UInt32 nFlags = 0 ) { m_cShapeManager.Unregister( nShapeId, nFlags ); }
		inline iShape * const			GetShape( UId32 nShapeId = 0 ) const { return m_cShapeManager.GetByShapeId( nShapeId ); }

		inline void						SetRenderer( CNavRenderer * pcRenderer ) { m_pcRenderer = pcRenderer; }
		inline CNavRenderer *			GetRenderer( void ) const { return m_pcRenderer; }

		inline void						SetImporter( CNavConverter * pcImporter ) { m_pcImporter = pcImporter; }
		inline CNavConverter *			GetImporter( void ) const { return m_pcImporter; }

		inline void						SetExporter( CNavConverter * pcExporter ) { m_pcExporter = pcExporter; }
		inline CNavConverter *			GetExporter( void ) const { return m_pcExporter; }

	protected:

		// Common

		void							Initialize( void );
		void							Destroy( void );

		bool							CreateFromFile( SNavEntityCreateInfo & rsCreateInfo );

		// Exclusive

		CNavWorld *	const				GetWorld( UInt32 nOrder = 0 ) const;

		bool							LinkWorld( CNavWorld * const pcWorld );
		bool							UnlinkWorld( CNavWorld * const pcWorld );

		bool							ParseHeader( CFixedStringTreeNodeW * pcHeader );
		bool							ParseShape( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcShape );
		bool							ParseThread( CFixedStringTreeNodeW * pcThread );
		bool							ParseWorldList( SNavEntityCreateInfo & rsCreateInfo, CFixedStringTreeNodeW * pcWorldList );

		bool							FindUnblockedPositionInternal( CNavPosition & rcSearchPosition, Fixed32 nDistance, UId32 nShapeId ) const;

	public:

		static const SVersionInfo		VERSION;

	protected:

		static CFixedString				m_cPathName;
		static bool						m_bAbsolutePath;

	private:

		static iPathEngine *			m_pcPathEngine;
		static CNavConverter *			m_pcDefaultConverter;

		static CNavigator *				m_pcInstance;

	public:

		static void						GetVersionInfo( TCHAR * pchBuffer, size_t nBufferLength );

		static CNavigator *				CreateInstance( const CFixedString & rcEngineName, const CFixedString & rcSessionName, SNavEntityExternalProcess * const psExternalProcess = NULL, UInt32 nFlag = E_NAV_ENTITY_CREATE_INFO_FLAG_DEFAULT );
		static CNavigator *				CreateInstance( const TCHAR * const pchPathName, const TCHAR * const pszSessionName, SNavEntityExternalProcess * const psExternalProcess = NULL, UInt32 nFlag = E_NAV_ENTITY_CREATE_INFO_FLAG_DEFAULT );
		static CNavigator *				CreateInstance( iPathEngine * const pcPathEngine, const CFixedString & rcSessionName, SNavEntityExternalProcess * const psExternalProcess = NULL, UInt32 nFlag = E_NAV_ENTITY_CREATE_INFO_FLAG_DEFAULT );
		static void						DestroyInstance( void );
		inline static CNavigator *		GetInstance( void ) { return m_pcInstance; }

		inline static CNavConverter *	GetDefaultConverter( void ) { return m_pcDefaultConverter; }

		inline static void				SetPathName( CFixedString & rcPathName, bool bAbsolutePath = true ) { m_cPathName.Assign( rcPathName ); m_bAbsolutePath = bAbsolutePath; }
		inline static void				SetPathName( const TCHAR * pchPathName, bool bAbsolutePath = true ) { m_cPathName.Assign( pchPathName ); m_bAbsolutePath = bAbsolutePath; }
		inline static CFixedString &	GetPathName( void ) { return m_cPathName; }
};

//=============================================================================
// End of Class : CNavigator
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAVIGATOR_H___

//*****************************************************************************
//
// End of File : CNavigator.h
//
//*****************************************************************************