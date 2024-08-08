//*****************************************************************************
// File       : CNavPreProcessThread.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_PRE_PROCESS_THREAD_H___
#define ___C_NAV_PRE_PROCESS_THREAD_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CBaseThread.h"
#include "NavEntityBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( nav )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CNavTerrain;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CNavPreProcessThread
// Desc       : 
// Author     : 
//=============================================================================

class CNavPreProcessThread : public CBaseThread
{
	protected:

		SNavEntityPreProcessInfo 		m_sPreProcessInfo;
		CFixedString					m_cEntityName;

	public:

		inline							CNavPreProcessThread( void ) { Initialize(); }

		inline							~CNavPreProcessThread( void ) { Destroy(); }

	private:

										CNavPreProcessThread( const CNavPreProcessThread & rcThread );

		const CNavPreProcessThread &	operator=( const CNavPreProcessThread & rcThread );

	public:

		// Common

		// Exclusive

		void							SetPreProcessInfo( const SNavEntityPreProcessInfo & rsPreProcessInfo );
		inline const SNavEntityPreProcessInfo &		GetPreProcessInfo( void ) const { return m_sPreProcessInfo; }

		inline void						SetEntityName( const CFixedString & rcEntityName ) { m_cEntityName = rcEntityName; }
		inline const CFixedString &		GetEntityName( void ) const { return m_cEntityName; }

	protected:

		void							Initialize( void );
		void							Destroy( void );

		UInt32							Begin( void );
		UInt32							Run( void );
		UInt32							End( void );
};

//=============================================================================
// End of Class : CNavPreProcessThread
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_PRE_PROCESS_THREAD_H___

//*****************************************************************************
//
// End of File : CNavPreProcessThread.h
//
//*****************************************************************************