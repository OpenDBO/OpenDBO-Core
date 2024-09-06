#ifndef __OPENDBO_WPSHOPCONTAINER__
#define __OPENDBO_WPSHOPCONTAINER__


#include "NtlItem.h"


const int NTL_MAX_WP_TABS = 4;
const wchar_t* NTL_WP_TAB_NAMES[NTL_MAX_WP_TABS] = { L"Misc.", L"Utility", L"Upgrade", L"Fashion" };

// The original TW server did not store this information in any RDF file. To ensure the original RDF files are fully compatible
// with this server, I chose to hardcode the values here instead of adding them to a merchant table. An alternative approach 
// would be to store this information in the database or a similar structure.
static const sSELL_ITEM_INFO NTL_WP_SELL_INFO_CONTAINER[NTL_MAX_WP_TABS][NTL_MAX_SHOPPING_CART + 1] =
{
	// Misc.
	{
		{303006, 5},
		{403040, 10},
		{403041, 20},
		{11130003, 10},
		{11130002, 5},
		{11120031, 5},
		{403000, 5},
		{11120032, 1},
		{11120033, 1},
		{11120092, 10},
		{INVALID_TBLIDX, 0}
	},
	// Utility
	{
		{304018, 2},
		{304022, 2},
		{18111, 5},
		{18115, 5},
		{304014, 5},
		{304015, 5},
		{304008, 10},
		{304010, 10},
		{304011, 10},
		{304013, 10},
		{18180, 15},
		{18183, 15},
		{150031, 20},
		{18169, 15},
		{11120251, 10},
		{11120252, 10},
		{11120253, 10},
		{6005011, 5},
		{6005012, 10},
		{INVALID_TBLIDX, 0}
	},
	// Upgrade
	{
		{160005, 5},
		{160006, 5},
		{160008, 10},
		{160010, 10},
		{160011, 15},
		{160013, 15},
		{160205, 5},
		{160206, 5},
		{160208, 10},
		{160210, 10},
		{160211, 15},
		{160213, 15},
		{160605, 10},
		{160606, 10},
		{160608, 20},
		{160610, 20},
		{160611, 30},
		{160613, 30},
		{160405, 10},
		{160406, 10},
		{160408, 20},
		{160410, 20},
		{160411, 30},
		{160413, 30},
		{160417, 100},
		{160617, 100},
		{11120219, 150},
		{INVALID_TBLIDX, 0}
	},
	// Fashion
	{
		{90039, 5},
		{90005, 50},
		{90078, 100},
		{11170026, 160},
		{11200061, 200},
		{11170108, 200},
		{INVALID_TBLIDX, 0}
	}
};


#endif // __OPENDBO_WPSHOPCONTAINER__
