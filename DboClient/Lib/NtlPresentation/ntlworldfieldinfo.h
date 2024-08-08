#pragma once

#include "ntlworldcommon.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

//FieldInfo
// : 화일 또는 화일 메모리에 접근하는 모든 코드는 이곳에 정의 한다.
// : 화일 접근 시에는 dGET_WORLD_PARAM()의 WorldLoadVer, WorldSaveVer을 구분하여 작성한다.
// : 화일 메모리 접근 시에는 dGET_WORLD_PARAM()의 WorldVer으로 작성한다. (단 예외의 경우가 발생 할 수 있다)
class CNtlWorldFieldInfo
{
public:
	CNtlWorldFieldInfo();
	virtual ~CNtlWorldFieldInfo();

public:
	static CNtlWorldFieldInfo*	CreateInstance();
	static void					DestroyInstance();

	static CNtlWorldFieldInfo*	GetInstance();

protected:
	static CNtlWorldFieldInfo*	s_pInstance;

public:
	// Name
	RwBool	FieldNameLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldNameSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldNameSkipToFile(FILE* pFile);

	BYTE*	FieldNameSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldNameSkipToFileMem(BYTE* pFileMem);

	// BitProp
	RwBool	FieldBitPropLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField);
	RwBool	FieldBitPropSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField);
	RwInt32	FieldBitPropSkipToFile(FILE* pFile, RwInt32 iTileNumInField);

	BYTE*	FieldBitPropSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iTileNumInField);
	BYTE*	FieldBitPropSkipToFileMem(BYTE* pFileMem, RwInt32 iTileNumInField);

	// Fog
	RwBool	FieldFogLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldFogSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldFogSkipToFile(FILE* pFile);

	BYTE*	FieldFogSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldFogSkipToFileMem(BYTE* pFileMem);

	// Sky
	RwBool	FieldSkyLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldSkySaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldSkySkipToFile(FILE* pFile);

	BYTE*	FieldSkySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldSkySkipToFileMem(BYTE* pFileMem);


	// RealLight
	RwBool	FieldRealLightLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldRealLightSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldRealLightSkipToFile(FILE* pFile);

	BYTE*	FieldRealLightSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldRealLightSkipToFileMem(BYTE* pFileMem);

	// BGM
	RwBool	FieldBGMLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldBGMSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldBGMSkipToFile(FILE* pFile);

	BYTE*	FieldBGMSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldBGMSkipToFileMem(BYTE* pFileMem);

	// Bloom
	RwBool	FieldBloomLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldBloomSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldBloomSkipToFile(FILE* pFile);

	BYTE*	FieldBloomSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldBloomSkipToFileMem(BYTE* pFileMem);

	// Planet
	RwBool	FieldPlanetLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldPlanetSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldPlanetSkipToFile(FILE* pFile);

	BYTE*	FieldPlanetSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldPlanetSkipToFileMem(BYTE* pFileMem);

	// Specular
	RwBool	FieldSpecularLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldSpecularSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldSpecularSkipToFile(FILE* pFile);

	BYTE*	FieldSpecularSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldSpecularSkipToFileMem(BYTE* pFileMem);

	// WorldMaterial
	RwBool	FieldWorldMaterialLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField);
	RwBool	FieldWorldMaterialSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp, RwInt32 iTileNumInField);
	RwInt32	FieldWorldMaterialSkipToFile(FILE* pFile, RwInt32 iTileNumInField);

	BYTE*	FieldWorldMaterialSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iTileNumInField);
	BYTE*	FieldWorldMaterialSkipToFileMem(BYTE* pFileMem, RwInt32 iTileNumInField);

	// FieldColor
	RwBool	FieldColorLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldColorSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldColorSkipToFile(FILE* pFile);

	BYTE*	FieldColorSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldColorSkipToFileMem(BYTE* pFileMem);

	// Weather
	RwBool	FieldWeatherLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldWeatherSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldWeatherSkipToFile(FILE* pFile);

	BYTE*	FieldWeatherSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldWeatherSkipToFileMem(BYTE* pFileMem);

	// HeatHaze
	RwBool	FieldHeatHazeLoadFromFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwBool	FieldHeatHazeSaveIntoFile(FILE* pFile, sNTL_FIELD_PROP* pNtlFieldProp);
	RwInt32	FieldHeatHazeSkipToFile(FILE* pFile);

	BYTE*	FieldHeatHazeSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	BYTE*	FieldHeatHazeSkipToFileMem(BYTE* pFileMem);
};

static CNtlWorldFieldInfo* GetNtlWorldFieldInfo()
{
	return CNtlWorldFieldInfo::GetInstance();
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif