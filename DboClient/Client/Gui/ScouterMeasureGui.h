/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 9. 13
* Last Update	: 2007. 8. 28
* Abstract		: 
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

#include "NtlSLDef.h"
#include "NtlSob.h"

// dbo
#include "SurfaceGui.h"
#include "NumberGui.h"

class CNtlSobItem;
class CNtlSobActor;

class CScouterMeasureGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eMeasureType
	{
		MEASURE_POWER,								///< 전투력 측정 모드
		MEASURE_FIND_HIDDEN_OBEJCT,					///< 숨겨진 오브젝트 찾기 모드

		NUM_MEASURE
	};

	enum eNumberColor
	{
		NUMBERCOLOR_RED,
		NUMBERCOLOR_YELLOW,
		NUMBERCOLOR_WHITE,

		NUM_COLOR
	};

	struct sAvatarPower
	{
		RwBool				bShow;
		RwUInt32			uiPower;
		RwInt32				iMeasurePowerCount;

		gui::CSlot			slotOnMoiveEnd;

		gui::CFlash*		pflaAvatarDirect;			///< HP 다이얼로그를 가리키는 플래시
		CNumberGui			NumberAvatarPower;			///< 전투력 측정시 아바타의 전투력 표시		
	};

	struct sTarget;

	typedef VOID	(CScouterMeasureGui::*callback_Update)(RwReal, sTarget*);
	typedef VOID	(CScouterMeasureGui::*callback_Destroy)(sTarget*);
	typedef VOID	(CScouterMeasureGui::*callback_Paint)(sTarget*);


	struct sTarget
	{
		eMeasureType		eType;
		SERIAL_HANDLE		hTarget;			///< 측정할 타겟의 핸들
		CNtlSob*			pSobTarget;			///< 전투력 측정을 하고 있는 타겟의 Sob 포인터
		DWORD				dwDisplayFlag;		///< 화면에 표시되는 GUI 플래그
		RwReal				fTimer;		

		callback_Update		pCallback_Update;
		callback_Destroy	pCallback_Destory;
		callback_Paint		pCallback_Paint;

		sTarget()
		:hTarget(INVALID_SERIAL_ID)		
		,pSobTarget(NULL)
		,dwDisplayFlag(0)
		,fTimer(0.f)		
		,pCallback_Update(NULL)
		,pCallback_Destory(NULL)
		,pCallback_Paint(NULL)
		{}
	};

	struct sPowerData : public sTarget
	{		
		RwBool				bShowScouterBack;
		gui::CFlash*		pFlashCircle;
		gui::CFlash*		pFlashArrow;
		gui::CFlash*		pFlashDirect;

		RwUInt32			uiOffencePower;		///< 목표 전투력
		RwUInt32			uiPostPower;		///< 이전 전투력
		RwUInt32			uiAdjustPower;		///< 가감될 전투력
		RwUInt32			uiDisplayPower;		///< 실제 화면에 표시할 전투력

		RwUInt32			uiMoreMeasureCount;	///< 전투력을 갱신한 횟수
		
		RwReal				fTimeRecievePower;	///< 전투력을 서버로부터 받은 시간
		RwReal				fTimeAdjustPower;	///< 전투력 보간 시간
		RwReal				fTimeDisplayPower;	///< 전투력 수치를 보여주는 시간

		RwInt32				iAdjustCount;		///< 전투력 보간 횟수

		RwBool				bAskPower;			///< 전투력 수치를 요청했다		

		sPowerData()
		:bShowScouterBack(true)		
		,pFlashCircle(NULL)
		,pFlashArrow(NULL)
		,pFlashDirect(NULL)
		,uiOffencePower(0)
		,uiPostPower(0)
		,uiAdjustPower(0)
		,uiDisplayPower(0)
		,uiMoreMeasureCount(0)
		,fTimeRecievePower(0.f)
		,fTimeAdjustPower(0.f)
		,fTimeDisplayPower(0.f)
		,bAskPower(false)
		,iAdjustCount(0)		
		{}
	};

	struct sHiddenObjectData : public sTarget
	{
		RwBool				bShowScouterBack;	

		gui::CFlash*		pFlashCircle;
		gui::CFlash*		pFlashArrow;

		sHiddenObjectData()
		:bShowScouterBack(true)
		,pFlashCircle(NULL)
		,pFlashArrow(NULL)
		{}
	};

	typedef std::map<SERIAL_HANDLE, sTarget*>						MEASURE_MAP;
	typedef std::map<SERIAL_HANDLE, sTarget*>::iterator				MEASURE_ITER;

	typedef std::map<SERIAL_HANDLE, RwV3d>							MAP_OBJECTPOS;
	typedef std::map<SERIAL_HANDLE, RwV3d>::iterator				MAP_OBJECTPOS_ITER;

	struct sHiddenObjectVisiable
	{
		// ts에서 넘겨주는 히든 오브젝트 데이터를 보관하고 있는다.
		MAP_OBJECTPOS		mapObjectPos;
		RwBool				bRemainVisible;		///< 스카우터가 꺼지고 일정시간 뒤에 오브젝트가 사라진다
		RwReal				fElapsed;
	};
	

	CScouterMeasureGui(const RwChar* pName);
	virtual ~CScouterMeasureGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();
	VOID		Clear();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	CScouterMeasureGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	VOID		StartMeasure(eMeasureType eMode);		///< 스카우터 측정을 시작한다

	// 전투력 측정 관련
	RwBool		MeasurePower_Init();
	VOID		MeasurePower_Destroy(sTarget* pTarget);
	VOID		MeasurePower_Update(RwReal fElapsed, sTarget* pTarget);
	VOID		MeasurePower_Paint(sTarget* pTarget);
	VOID		MeasurePower_Display(sPowerData* pPowerData, RwUInt32 uiPower);			///< 전투력 측정 이펙트를 보여준다.
	VOID		MeasurePower_AskPowertoServer(sPowerData* pPowerData);					///< 전투력을 갱신한다.
	
	// 히든 오브젝트 찾기 관련
	RwBool		MeasureFindObject_Init();
	VOID		MeasureFindObject_Destroy(sTarget* pTarget);
	VOID		MeasureFindObject_Update(RwReal fElapsed, sTarget* pTarget);
	VOID		MeasureFindObject_Paint(sTarget* pTarget);

	VOID		OutOfOrder();							///< 스카우터가 고장났다

	VOID		OnMovieEnd_AvatarDirect(gui::CComponent* pComponent);

	VOID		EndDisplayImmediately(SERIAL_HANDLE hHandle);
	VOID		EndDisplay(sTarget* pTarget);

	RwBool		IsWorking(eMeasureType eType);			///< 측정 모드가 동작하는지 여부

	VOID		TargetEdgeEffect(RwBool bActive, sTarget* pTarget);

	VOID		OnPaint();

protected:	
	gui::CSlot			m_slotPaint;	

	CNumberGui			m_Number[NUM_COLOR];
	CNumberGui			m_NumberDistance;

	CSurfaceGui			m_srfDistanceMark;
	CSurfaceGui			m_srfDistanceUnit;

	RwBool				m_bOutofOrder;

	sAvatarPower		m_AvatarPower;

	MEASURE_MAP			m_mapMeasure;
	std::list<sTarget*>	m_listRemove;

	sHiddenObjectVisiable	m_HiddenObject;
};