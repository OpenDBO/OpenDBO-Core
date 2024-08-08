//*****************************************************************************
// File       : CWeManager.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NTL_WE_FILE_CONVERTER_H___
#define ___C_NTL_WE_FILE_CONVERTER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavExporter.h"
#include "NtlWeBase.h"


//-----------------------------------------------------------------------------
// Type Forward Definition Table (external)
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CWeManager
// Desc       : 
// Author     : 
//=============================================================================

class NAVEXPORTER_API CWeManager
{
	protected:

		CFixedString 					m_cProjectPathName;

	protected:

		inline							CWeManager( void ) { Initialize(); }

		inline virtual					~CWeManager( void ) { Destroy(); }

	private:

										CWeManager( const CWeManager & rcLoader );

		const CWeManager &				operator =( const CWeManager & rcLoader );

	public:

		virtual bool					ConvertWorld( const CFixedString & rcConfigFileName );
//		virtual bool					ConvertSector( const CFixedString & rcInputSectorFileName, const CFixedString & rcOutputSectorFile, const SNtlWeSectorExportReference & rsReference );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

		bool							ParsePathNode( CNavFilePath & rsPath, const CFixedStringTreeNodeW * pcNode );

	protected:

		static CFixedString				m_cPathName;

	public:

		inline static const CFixedString &	GetPathName( void ) { return m_cPathName; }
		inline static const void		SetPathName( CFixedString & rcPathName ) { m_cPathName = rcPathName; }

		static CWeManager *				CreateFileConverter( void ) { return new CWeManager; }
		static void						DestroyFileConverter( CWeManager ** ppcConverter ) { delete *ppcConverter; *ppcConverter = NULL; }
};

//=============================================================================
// End of Class : CWeManager
//=============================================================================

END_NAMESPACE( navex )

#endif //___C_NTL_WE_FILE_CONVERTER_H___

//*****************************************************************************
//
// End of File : CWeManager.h
//
//*****************************************************************************