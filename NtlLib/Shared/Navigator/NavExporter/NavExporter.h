//*****************************************************************************
// File       : NavExporter.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___NAV_EXPORTER_H___
#define ___NAV_EXPORTER_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

#ifdef NAVEXPORTER_EXPORTS
	#define NAVEXPORTER_API __declspec(dllexport)
#else
	#define NAVEXPORTER_API __declspec(dllimport)
#endif //NAVEXPORTER_EXPORTS

#define __COMPILE_OPTION_PLACE_OPSTACLE__
#define __COMPILE_OPTION_OPTIMIZE_MESH__		( 1 )


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"
//#include "CLogMessageManager.h"
//#include "UserDefinedAssert.h"

#include "CMemoryBuffer.h"
#include "CNavEntityVector.h"
#include "CNavigator.h"
#include "CNavPosition.h"
#include "CNavBounding.h"
#include "CNavZone.h"
#include "CNavSector.h"
#include "CNavTerrain.h"
#include "CNavHeightMap.h"
#include "CNavPathEngine.h"
#include "CFixedStringTreeNodeW.h"
#include "CXMLParserW.h"
#include "CNavFaceVertexMesh.h"
#include "CNavPosition2D.h"
#include "CNavFilePath.h"

#include "NavExporter.h"
#include "NavExStringTable.h"


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


//-----------------------------------------------------------------------------
// Macro, Inline & Template Function Table
//-----------------------------------------------------------------------------

END_NAMESPACE( navex )

#endif //___NAV_EXPORTER_H___

//*****************************************************************************
//
// End of File : NavExporter.h
//
//*****************************************************************************