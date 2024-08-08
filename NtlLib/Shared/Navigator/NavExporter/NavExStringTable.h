//*****************************************************************************
// File       : NavExStringTable.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___NAV_EX_STRING_TABLE_H___
#define ___NAV_EX_STRING_TABLE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------


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

static const CFixedStringW	KEY_CONFIG( L"config" );
static const CFixedStringW	KEY_REFERENCE( L"reference" );
static const CFixedStringW	KEY_HEIGHT_MAP( L"heightmap" );
static const CFixedStringW	KEY_ATTRIBUTE( L"attribute" );
static const CFixedStringW	KEY_OBJECT( L"object" );
static const CFixedStringW	KEY_MODELING( L"modeling" );
static const CFixedStringW	KEY_STRUCTURE( L"structure" );
static const CFixedStringW	KEY_OUTPUT( L"output" );

static const CFixedStringW	KEY_PATH( L"path" );
static const CFixedStringW	KEY_LOCAL( L"local" );
static const CFixedStringW	KEY_NAME( L"name" );
static const CFixedStringW	KEY__NAME( L"NAME" );
static const CFixedStringW	KEY_EXT( L"ext" );

static const CFixedStringW	KEY_TRUE( L"true" );
static const CFixedStringW	KEY_FALSE( L"false" );

static const CFixedStringW	KEY__OBJECT_PROPERTY( L"OBJECT_PROPERTY" );
static const CFixedStringW	KEY__OBJECT_ELEMENT( L"OBJECT_ELEMENT" );
static const CFixedStringW	KEY__ELEMENT( L"ELEMENT" );

static const CFixedStringW	KEY__OBJECT_SCRIPT( L"OBJECT_SCRIPT" );
static const CFixedStringW	KEY__BODY( L"BODY" );
static const CFixedStringW	KEY__BBOX( L"BBOX" );
static const CFixedStringW	KEY__COLLISION( L"COLLISION" );
static const CFixedStringW	KEY__PATH_COLLISION( L"PATH_COLLISION" );
static const CFixedStringW	KEY__COLLISION_MESH( L"COLLISON_MESH" );
static const CFixedStringW	KEY__HEADER( L"HEADER" );

static const CFixedString	KEY_ASE( _T( "ase" ) );
static const CFixedString	KEY__GEOMOBJECT( _T( "*GEOMOBJECT" ) );
static const CFixedString	KEY__NODE_TM( _T( "*NODE_TM" ) );
static const CFixedString	KEY__TM_POS( _T( "*TM_POS" ) );
static const CFixedString	KEY__MESH( _T( "*MESH" ) );
static const CFixedString	KEY__MESH_NUMVERTEX( _T( "*MESH_NUMVERTEX" ) );
static const CFixedString	KEY__MESH_NUMFACES( _T( "*MESH_NUMFACES" ) );
static const CFixedString	KEY__MESH_VERTEX_LIST( _T( "*MESH_VERTEX_LIST" ) );
static const CFixedString	KEY__MESH_VERTEX( _T( "*MESH_VERTEX" ) );
static const CFixedString	KEY__MESH_FACE_LIST( _T( "*MESH_FACE_LIST" ) );
static const CFixedString	KEY__MESH_FACE( _T( "*MESH_FACE" ) );
static const CFixedString	KEY__A_( _T( "A:" ) );
static const CFixedString	KEY__B_( _T( "B:" ) );
static const CFixedString	KEY__C_( _T( "C:" ) );
static const CFixedString	KEY__AB_( _T( "AB:" ) );
static const CFixedString	KEY__BC_( _T( "BC:" ) );
static const CFixedString	KEY__CA_( _T( "CA:" ) );

static const CFixedString	COLLISION_PREFIX( _T( "c_" ) );


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro, Inline & Template Function Table
//-----------------------------------------------------------------------------

END_NAMESPACE( navex )

#endif //___NAV_EX_STRING_TABLE_H___

//*****************************************************************************
//
// End of File : NavExStringTable.h
//
//*****************************************************************************