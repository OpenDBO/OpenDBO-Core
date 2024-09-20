#ifndef __OPENDBO_WPSHOPCONTAINER__
#define __OPENDBO_WPSHOPCONTAINER__


#include "NtlItem.h"


const int NTL_MAX_WP_TABS = 4;
const wchar_t* NTL_WP_TAB_NAMES[NTL_MAX_WP_TABS] = { L"Material", L"Weapon", L"Armor", L"Dogi" };

// The original TW server did not store this information in any RDF file. To ensure the original RDF files are fully compatible
// with this server, I chose to hardcode the values here instead of adding them to a merchant table. An alternative approach 
// would be to store this information in the database or a similar structure.
static const sSELL_ITEM_INFO NTL_WP_SELL_INFO_CONTAINER[NTL_MAX_WP_TABS][NTL_MAX_SHOPPING_CART + 1] =
{
	// Material
	{
		{310010, 20},
		{310020, 25},
		{310030, 30},
		{310040, 35},
		{310050, 40},
		{310060, 45},
		{310070, 50},
		{INVALID_TBLIDX, 0}
	},
	// Weapon
	{
		{1276101, 50},
		{1276102, 50},
		{1276103, 50},
		{1276201, 50},
		{1276202, 50},
		{1276203, 50},
		{1276204, 50},
		{1276205, 50},
		{1276206, 50},
		{1276207, 50},
		{1276208, 50},
		{1276209, 50},
		{1276210, 50},
		{1276211, 50},
		{1276212, 50},
		{INVALID_TBLIDX, 0}
	},
	// Armor
	{
		{1276311, 30},
		{1276312, 30},
		{1276313, 30},
		{1276321, 30},
		{1276322, 30},
		{1276323, 30},
		{1276331, 30},
		{1276332, 30},
		{1276333, 30},
		{INVALID_TBLIDX, 0}
	},
	// Dogi
	{
		{11150011, 100},
		{11150012, 100},
		{11150013, 100},
		{11150014, 100},
		{11150015, 100},
		{11150016, 100},
		{11150017, 100},
		{11150018, 100},
		{11150019, 50},
		{11150020, 50},
		{11150021, 50},
		{11150022, 50},
		{11150006, 10},
		{11150007, 10},
		{11150008, 10},
		{11150009, 10},
		{11150010, 15},
		{11160010, 20},
		{11160011, 20},
		{11160012, 20},
		{11160013, 20},
		{INVALID_TBLIDX, 0}
	}
};


#endif // __OPENDBO_WPSHOPCONTAINER__
