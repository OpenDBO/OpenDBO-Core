#include "stdafx.h"
#include "CharacterManager.h"
#include "QueryServer.h"


CCharacterManager::CCharacterManager()
{
	Init();
}

CCharacterManager::~CCharacterManager()
{
}


void CCharacterManager::Init()
{
	m_uiLastCharacterId = 0;

	smart_ptr<QueryResult> result = GetCharDB.Query("SELECT MAX(CharID) FROM characters");
	if (result)
	{
		Field* f = result->Fetch();

		m_uiLastCharacterId = f[0].GetUInt32();
	}

	ERR_LOG(LOG_GENERAL, "Last Character-ID %u", m_uiLastCharacterId);
}


void CCharacterManager::CreateCharacter(ACCOUNTID accountId, sPC_SUMMARY& sSum, SERVERFARMID serverFarmId, bool isGM)
{
	UNREFERENCED_PARAMETER(serverFarmId);

	GetCharDB.Execute("INSERT INTO characters (CharID,CharName,AccountID,Race,Class,Gender,Face,Hair,HairColor,SkinColor,CurLocX,CurLocY,CurLocZ,WorldID,WorldTable,MapInfoIndex,CreateTime,GameMaster)"
		"VALUES (%u,'%ls',%u,%u,%u,%u,%u,%u,%u,%u,%f,%f,%f,%u,%u,%u,%I64u,%u)",
		sSum.charId, sSum.awchName, accountId, sSum.byRace, sSum.byClass, sSum.byGender, sSum.sPcShape.byFace, sSum.sPcShape.byHair, sSum.sPcShape.byHairColor, sSum.sPcShape.bySkinColor,
		sSum.fPositionX, sSum.fPositionY, sSum.fPositionZ,
		sSum.worldId, sSum.worldTblidx, sSum.dwMapInfoIndex, time(0), isGM);
}