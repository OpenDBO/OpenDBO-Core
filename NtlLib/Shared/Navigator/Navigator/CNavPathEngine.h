//*****************************************************************************
// File       : CNavPathEngine.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_PATH_ENGINE_H___
#define ___C_NAV_PATH_ENGINE_H___

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

struct SNavMemoryBufferEx;
struct SNavPosition2DInt;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#ifdef _DEBUG
	#define PE_ENGINE_NAME_DEFAULT		"PathEngineD"
#else
	#define PE_ENGINE_NAME_DEFAULT		"PathEngine"
#endif //_DEBUG

#define PE_PROCESS_NAME_DEFAULT			"_DllExport_GetIPathEngine@4"

#define g_pPathEngine					( CNavPathEngine::GetInstance() )

#define FILE_TYPE_EXT_LENGTH_MAX		( 4 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef iPathEngine *					(__stdcall * TGetInterfaceFunction)(iErrorHandler *);

#define g_pcPathEngine					( CNavPathEngine::GetInstance() )


//=============================================================================
// Name       : CNavPathEngine
// Desc       : 
// Author     : 
//=============================================================================

class CNavPathEngine
{
	protected:

		iPathEngine *					m_pcPathEngine;
		HINSTANCE						m_hDllInstance;

		DWORD							m_dwLastError;

	protected:

										CNavPathEngine( void );

										~CNavPathEngine( void );

	private:

										CNavPathEngine( const CNavPathEngine & rcDllLoader );

		const CNavPathEngine &			operator=( const CNavPathEngine & rcDllLoader );

	public:

		inline iPathEngine *			GetPathEngine( void ) const { return m_pcPathEngine; }

		bool							LoadPathEngine( const CFixedString & rcDllName, iErrorHandler * pcErrorHandler = NULL );
		bool							LoadPathEngine( iPathEngine * const pcPathEngine, iErrorHandler * pcErrorHandler = NULL );
		void							UnloadPathEngine( void );

		bool							ValidatePathEngine( void ) const;

		inline DWORD					GetLastError( void ) { DWORD dwLastError = m_dwLastError; m_dwLastError = ERROR_SUCCESS; return dwLastError; }

		iMesh *							CreateMeshFromFaceVertexMesh( const iFaceVertexMesh * const * ppcGeometryObjectPointers, long nNumberOfGeometryObjects, const char * const * ppchOptions ) const;
		iMesh *							LoadMesh( ENavFileType eFileType, const CFixedString & rcSource, UInt32 nFlags = 0 );
		iMesh *							LoadMesh( ENavMemoryType eMemoryType, void * pvData, UInt32 nFlags = 0 );
		void							UnloadMesh( iMesh ** ppcMesh );

		void							LoadPreProcess( ENavFileType eFileType, const CFixedString & rcSource, iShape * pcShape, iMesh * pcMesh, UInt32 nFlags = 0 );
		void							UnloadPreProcess( iShape * pcShape, iMesh * pcMesh, UInt32 nFlags = 0 );

		iShape *						LoadShape( ENavFileType eFileType, const CFixedString & rcSource, UInt32 nFlags = 0 );
		void							UnloadShape( iShape ** ppcShape );

	protected:

		void							Initialize( void );

		bool							ValidateEngineVersion( void ) const;
		iShape *						ParseShapeXML( const CFixedString & rcXMLFileName );

	public:

		static const CFixedStringA		KEYWORD_FILE_FORMAT[E_NAV_FILE_TYPE_COUNT];

	protected:

		static CNavPathEngine *			m_pInstance;

	public:

		static CNavPathEngine *			CreateInstance( const CFixedString & rcDllName );
		static CNavPathEngine *			CreateInstance( iPathEngine * const pcPathEngine );
		static void						DestroyInstance( void );
		inline static CNavPathEngine *	GetInstance( void ) { return m_pInstance; }
};

//=============================================================================
// End of Class : CNavPathEngine
//=============================================================================

END_NAMESPACE( nav )

#endif //___C_NAV_PATH_ENGINE_H___

//*****************************************************************************
//
// End of File : CNavPathEngine.h
//
//*****************************************************************************