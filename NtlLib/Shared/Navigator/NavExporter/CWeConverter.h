//*****************************************************************************
// File       : CWeConverter.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_WE_CONVERTER_H___
#define ___C_WE_CONVERTER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NavExporter.h"
#include "CNavConverter.h"
#include "NtlWeBase.h"
#include "NavExporterBase.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( navex )


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
// Name       : CWeConverter
// Desc       : 
// Author     : 
//=============================================================================

class NAVEXPORTER_API CWeConverter : public CNavConverter
{
	protected:

		CNavFilePath					m_cReferencePath;
		CNavFilePath					m_cHeightMapPath;
		CNavFilePath					m_cAttributePath;
		CNavFilePath					m_cObjectPath;
		CNavFilePath					m_cModelPath;
		CNavFilePath					m_cStructurePath;
		CNavFilePath					m_cOutputPath;

		bool							m_bHasReference;

		CNavPosition					m_cTileSize;
		CNavPosition					m_cSectorSize;
		UInt32							m_nSectorsInField;
		UInt32							m_nSectorsInWorld;

		SWeModelInfoNode *				m_psModels;
		UInt32							m_nModelCount;

	public:

		inline							CWeConverter( void ) : m_bHasReference( false ), m_nSectorsInField( 0 ), m_nSectorsInWorld( 0 ), m_psModels( NULL ), m_nModelCount( 0 ) { Initialize(); }

		inline							~CWeConverter( void ) { Destroy(); }

	private:

										CWeConverter( const CWeConverter & rcConverter );

		const CWeConverter &			operator =( const CWeConverter & rcConverter );

	public:

		// Common

		virtual bool					ImportHeightMap( void * pvData = NULL );
		virtual bool					ExportHeightMap( void * pvData = NULL );

		virtual bool					ImportTerrain( void * pvData = NULL );
		virtual bool					ExportTerrain( void * pvData = NULL );

		// Exclusive

		void							SetReferencePath( const CNavFilePath & rsPath ) { m_cReferencePath.Set( rsPath ); }
		void							SetHeightMapPath( const CNavFilePath & rsPath ) { m_cHeightMapPath.Set( rsPath ); }
		void							SetAttributePath( const CNavFilePath & rsPath ) { m_cAttributePath.Set( rsPath ); }
		void							SetObjectPath( const CNavFilePath & rsPath ) { m_cObjectPath.Set( rsPath ); }
		void							SetModelPath( const CNavFilePath & rsPath ) { m_cModelPath.Set( rsPath ); }
		void							SetStructurePath( const CNavFilePath & rsPath ) { m_cStructurePath.Set( rsPath ); }
		void							SetOutputPath( const CNavFilePath & rsPath ) { m_cOutputPath.Set( rsPath ); }

		const CNavFilePath &			GetReferencePath( void ) const { return m_cReferencePath; }
		const CNavFilePath &			GetHeightMapPath( void ) const { return m_cHeightMapPath; }
		const CNavFilePath &			GetAttributePath( void ) const { return m_cAttributePath; }
		const CNavFilePath &			GetObjectPath( void ) const { return m_cObjectPath; }
		const CNavFilePath &			GetModelPath( void ) const { return m_cModelPath; }
		const CNavFilePath &			GetStructurePath( void ) const { return m_cStructurePath; }
		const CNavFilePath &			GetOutputPath( void ) const { return m_cOutputPath; }

	protected:

		// Common

		inline void						Initialize( void ) {}
		inline void						Destroy( void ) { DestroyModelInfoes(); }

		// Exclusive

		bool							LoadReference( const CFixedString & rcFileName );
		bool							ConvertWeHeightMapFile( const CFixedString & rcHeightFileName, CNavHeightMap * pcHeightMap );
		bool							ConvertWeHeightMapFile( const CFixedString & rcHeightFileName, const CFixedString & rcAttributeFileName, CNavHeightMap * pcHeightMap );

		iMesh *							CreateMeshFromZone( const SNavEntityCreateInfo & rsCreateInfo ) const;
		bool							GetMeshBound( CNavBounding & rcMeshBounding, const SNavEntityCreateInfo & rsCreateInfo ) const;

		iFaceVertexMesh *				CreateFaceVertexMeshFromHeightMap( CNavHeightMap * pcHeightMap ) const;

		UInt32							ConvertObstacles( iMesh * pcMesh, const CNavEntityVector * const pcSectors ) const;
		bool							ConvertPosition( cPosition * pcResultPosition, CNavPosition * pcPosition, iMesh * pcMesh ) const;

		bool							CreateModelInfoes( CFixedString & rcModelListFileName );
		void							DestroyModelInfoes( void );

		bool							LoadModelInfo( SWeModelInfo & rcModelInfo, const CFixedStringW & rcModelName ) const;
		bool							LoadCollisionInfo( SWeModelInfo & rcModelInfo, const CFixedStringW & rcCollisionName ) const;
		UInt32							ParsePolygon( SWePolygonNode ** ppsPolygon, const CFixedStringList & rcStringList, UInt32 nPosition ) const;
		const SWeModelInfo &			RegisterModelInfo( const CFixedStringW & rcModelName );
		const SWeModelInfo &			GetModelInfo( const CFixedStringW & rcModelName ) const;
};

//=============================================================================
// End of Class : CWeConverter
//=============================================================================

END_NAMESPACE( navex )

#endif //___C_WE_CONVERTER_H___

//*****************************************************************************
//
// End of File : CWeConverter.h
//
//*****************************************************************************