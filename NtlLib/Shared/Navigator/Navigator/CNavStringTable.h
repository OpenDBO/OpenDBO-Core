//*****************************************************************************
// File       : CNavStringTable.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_NAV_STRING_TABLE_H___
#define ___C_NAV_STRING_TABLE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedString.h"


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
// Name       : CNavEntityVector
// Desc       : 
// Author     : 
//=============================================================================

class CNavStringTable
{
	public:

		// _

		static const CFixedString	DIRECTORY_SEPARATOR;
		static const CFixedString	EXTENSION_SEPARATOR;

		// 0

		// 1

		// 2

		// 3

		// 4

		// 5

		// 6

		// 7

		// 8

		// 9

		// A

		static const CFixedStringW	AREA;
		static const CFixedStringW	AREALIST;


		// B

		static const CFixedStringW	BOUND;


		// C

		static const CFixedStringW	COMPLEXITY;
		static const CFixedStringW	COUNT;


		// D

		static const CFixedStringW	DUNGEON;


		// E

		// F

		static const CFixedStringW	_FALSE;
		static const CFixedStringW	FIELD;
		static const CFixedStringW	FLAGS;


		// G

		// H

		static const CFixedStringW	HEADER;
		static const CFixedString	HMD;


		// I

		static const CFixedStringW	INDOOR;


		// J

		// K

		// L

		static const CFixedStringW	LINK;
		static const CFixedStringW	LOCAL;


		// M

		static const CFixedStringW	MAX;
		static const CFixedStringW	MESH;
		static const CFixedStringW	MIN;


		// N

		static const CFixedStringW	NAME;


		// O

		static const CFixedStringW	OUTDOOR;
		static const CFixedStringW	OVERLAP;


		// P

		static const CFixedStringW	PATH;
		static const CFixedStringW	PORTAL;
		static const CFixedStringW	PORTALLIST;
		static const CFixedStringW	POSTFIX;
		static const CFixedStringW	PREFIX;
		static const CFixedStringW	PRIORITY;


		// Q

		// R

		// S

		static const CFixedStringW	SECTOR;
		static const CFixedStringW	SECTORLIST;
		static const CFixedStringW	SECTION;
		static const CFixedStringW	SECTIONLIST;
		static const CFixedStringW	SEPARATOR;
		static const CFixedStringW	SESSION;
		static const CFixedStringW	SIZE;		
		static const CFixedStringW	SHAPE;


		// T

		static const CFixedStringW	TERRAIN;
		static const CFixedStringW	THREAD;
		static const CFixedStringW	TITLE;
		static const CFixedStringW	TOWN;
		static const CFixedStringW	_TRUE;
		static const CFixedStringW	TYPE;


		// U

		static const CFixedStringW	UID;


		// V

		static const CFixedStringW	VERSION;
		static const CFixedStringW	VERTEX;


		// W

		static const CFixedStringW	WORLD;
		static const CFixedStringW	WORLDLIST;


		// X

		static const CFixedStringW	X;


		// Y

		static const CFixedStringW	Y;


		// Z

		static const CFixedStringW	Z;
		static const CFixedStringW	ZONE;
		static const CFixedStringW	ZONELIST;
};

END_NAMESPACE( nav )

#endif //___C_NAV_STRING_TABLE_H___

//*****************************************************************************
//
// End of File : CNavStringTable.h
//
//*****************************************************************************