#include "precomp_dboclient.h"
#include "DisplayStringManager.h"


CDisplayStringManager* CDisplayStringManager::GetInstance(void)
{
	static CDisplayStringManager Instance;
	return &Instance;
}

void CDisplayStringManager::AddString(RwUInt32 eType, const std::string& wKey, const std::wstring& wstr )
{
	if (eType == CLIENT_ONLY_MSG)
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapClientString.find(wKey);
		if (it != m_mapClientString.end())
		{
			NTL_ASSERT(false, "DisplayString Error: Duplicate ID - " << wKey.c_str());
			return;
		}

		m_mapClientString[wKey] = wstr;
	}
	else
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapServerString.find(wKey);
		if (it != m_mapServerString.end())
		{
			NTL_ASSERT(false, "DisplayString Error: Duplicate ID - " << wKey.c_str());
			return;
		}

		m_mapServerString[wKey] = wstr;
	}

}

const WCHAR * CDisplayStringManager::GetString(std::string key, RwUInt32 eType)
{
	if (eType == CLIENT_ONLY_MSG)
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapClientString.find(key);
		if (it != m_mapClientString.end())
			return it->second.c_str();
	}
	else
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapServerString.find(key);
		if (it != m_mapServerString.end())
			return it->second.c_str();
	}

	// Provide default values for specific keys (hardcoded)
	if (key == "DST_OPTION_FILTER_RARITY") return L"Rarity";
	if (key == "DST_OPTION_TOOLTIP_FILTER_NORMAL") return L"Normal items";
	if (key == "DST_OPTION_FILTER_RARITY_NORMAL") return L"Normal";
	if (key == "DST_OPTION_TOOLTIP_FILTER_SUPERIOR") return L"Superior items";
	if (key == "DST_OPTION_FILTER_RARITY_SUPERIOR") return L"Superior";
	if (key == "DST_OPTION_TOOLTIP_FILTER_EXCELLENT") return L"Excellent items";
	if (key == "DST_OPTION_FILTER_RARITY_EXCELLENT") return L"Excellent";
	if (key == "DST_OPTION_TOOLTIP_FILTER_RARE") return L"Rare items";
	if (key == "DST_OPTION_FILTER_RARITY_RARE") return L"Rare";
	if (key == "DST_OPTION_TOOLTIP_FILTER_LEGENDARY") return L"Legendary items";
	if (key == "DST_OPTION_FILTER_RARITY_LEGENDARY") return L"Legendary";
	if (key == "DST_OPTION_FILTER_STONES") return L"Stones";
	if (key == "DST_OPTION_TAB_EXTRA") return L"Extra";
	if (key == "DST_OPTION_TOOLTIP_TAB_EXTRA") return L"Extra stuff added";
	if (key == "DST_OPTION_TAB_EXTRA_FILTER") return L"Drop Filter";
	if (key == "DST_OPTION_TOOLTIP_TAB_EXTRA_FILTER") return L"Option for Drop Filter";
	if (key == "DST_OPTION_FILTER_STONE_BLUE") return L"Blue Stone";
	if (key == "DST_OPTION_TOOLTIP_FILTER_STONE_BLUE") return L"Armor Normal Stone";
	if (key == "DST_OPTION_FILTER_STONE_RED") return L"Red Stone";
	if (key == "DST_OPTION_TOOLTIP_FILTER_STONE_RED") return L"Weapon Normal Stone";
	if (key == "DST_OPTION_FILTER_STONE_GREEN") return L"Green Stone";
	if (key == "DST_OPTION_TOOLTIP_FILTER_STONE_GREEN") return L"Armor Special Stone";
	if (key == "DST_OPTION_FILTER_STONE_PURPLE") return L"Purple Stone";
	if (key == "DST_OPTION_TOOLTIP_FILTER_STONE_PURPLE") return L"Weapon Special Stone";
	if (key == "DST_OPTION_FILTER_STONE_WHITE") return L"White Stone";
	if (key == "DST_OPTION_TOOLTIP_FILTER_STONE_WHITE") return L"Stone that protects upgrade";
	if (key == "DST_OPTION_FILTER_STONE_ADOWN") return L"A. Grey Stone";
	if (key == "DST_OPTION_TOOLTIP_FILTER_STONE_ADOWN") return L"Armor Downgrade Stone";
	if (key == "DST_OPTION_FILTER_STONE_WDOWN") return L"W. Grey Stone";
	if (key == "DST_OPTION_TOOLTIP_FILTER_STONE_WDOWN") return L"Weapon Downgrade Stone";
	if (key == "DST_OPTION_FILTER_MYSTERYITEM") return L"Mystery Item";
	if (key == "DST_OPTION_TOOLTIP_FILTER_MYSTERYITEM") return L"The capsules that need to be identified";
	if (key == "DST_OPTION_FILTER_MISC") return L"Misc Items";
	if (key == "DST_OPTION_FILTER_CONSUMABLE") return L"Consumables";
	if (key == "DST_OPTION_TOOLTIP_FILTER_CONSUMABLE") return L"LP, EP restoration and treatment pills (poison, abdominal, etc...)";

	DBO_WARNING_MESSAGE("Missing string with key: " << key);
	static WCHAR awcBuffer[] = L"String not found";

	return awcBuffer;
}

void CDisplayStringManager::InsertOptions(const CDisplayParser& Parser, RwUInt32 eType /*= CLIENT_ONLY_MSG*/)
{
	NTL_FUNCTION("CDisplayStringManager::InsertOptions");


	const CDisplayParser::MapDataOption& DataOption = Parser.GetDataOptions();
	CDisplayParser::MapDataOption::const_iterator it;

	for(it = DataOption.begin(); it != DataOption.end(); ++it)
	{
		AddString(eType, it->first, it->second);
	}

	NTL_RETURNVOID();
}
