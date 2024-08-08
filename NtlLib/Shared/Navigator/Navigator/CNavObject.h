//*****************************************************************************
// File       : CNavObject.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2006. 12. 5
//*****************************************************************************

#ifndef ___C_NAV_OBJECT_H___
#define ___C_NAV_OBJECT_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CNavEntity.h"


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

#define FACE_ID_INVALID					( 0xFFFFFFFF )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavObject
// Desc       : 
// Author     : 
//=============================================================================

class CNavObject : public CNavEntity
{
	friend class						CNavEntityFactory;

	protected:

		iShape *						m_pcShape;
		iAgent *						m_pcAgent;

		CNavPosition					m_cPosition;

		UInt32							m_nVertexCount;

	protected:

		inline							CNavObject( UInt32 nUniqueId ) : CNavEntity( nUniqueId ) { Initialize(); m_eType = E_NAV_ENTITY_TYPE_OBJECT; }

		inline virtual					~CNavObject( void ) { Destroy(); }

	private:

										CNavObject( void );
										CNavObject( const CNavObject & rcWorld );

		const CNavObject &				operator =( const CNavObject & rcWorld );

	public:

		// Common

		bool							Create( SNavEntityCreateInfo & rsCreateInfo );

		Int32							Render( CNavRenderer * const pcRenderer ) const;

		// Exclusive

		void							SetAgent( iAgent * pcAgent ) { m_pcAgent = pcAgent; }

		inline iShape * const			GetShape( void ) const { return m_pcShape; }
		inline const CNavPosition &		GetPosition( void ) const { return m_cPosition; }

		bool							GetVertex( UInt32 nVertexId, CNavPosition & rcPosition ) const;

	protected:

		// Common

		virtual void					Initialize( void );
		virtual void					Destroy( void );

		// Exclusive

		void							CalculateBoundingFromShape( void );

	protected:

		static CFixedString				m_cPathName;

	public:

		inline static void				SetPathName( CFixedString & rcPathName ) { m_cPathName.Assign( rcPathName ); }
		inline static const CFixedString &	GetPathName( void ) { return m_cPathName; }
};

//=============================================================================
// End of Class : CNavObject
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_OBJECT_H___

//*****************************************************************************
//
// End of File : CNavObject.h
//
//*****************************************************************************