/*****************************************************************************
 *
 * File			: NtlPLSceneManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (占쏙옙)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation scene manager base class 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLVISUALMANAGER_H__
#define __NTL_PLVISUALMANAGER_H__

#include <map>
#include <list>
#include <utility>
#include "rpcollis.h"
#include "ceventhandler.h"
#include "NtlPLSceneManager.h"
#include "NtlPLRenderGroup.h"
#include "NtlPLCollision.h"
#include "NtlPLCullingScheduling.h"

class CNtlPLRenderGroup;
class CNtlPLDistanceFilterGroup;
class CNtlPLAtomicSorterContainter;
class CNtlPLDojoContainer;

class CNtlPLDummyWorld;
class CNtlPLWorldEntity;
class CNtlPLSky;
class CNtlPLFog;
class CNtlPLWater;
class CNtlPLPlant;
class CNtlPicking;


/**
 * \ingroup NtlPresentation
 * client application占쏙옙 visual 占쏙옙체占쏙옙 占쏙옙占쏙옙占싹댐옙 class占싱댐옙.
 * PL entity占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 layer占쏙옙 占쏙옙占쏙옙占쏙옙 占쌍곤옙, layer 占쏙옙占쏙옙占실뤄옙 update 占쏙옙 rendering 占싼댐옙.
 * renderware占쏙옙占쏙옙占쏙옙 alpha sorting占쏙옙 占싫되곤옙 占쏙옙占쏙옙占실뤄옙 , object 占쏙옙占쏙옙占쏙옙 layer占쏙옙 占싱울옙占싹울옙 alpha sorting占쏙옙 占싼댐옙.
 * 또한 PL entity를 생성 및 소멸의 interface를 제공하고 있으며, 생성/소멸 관리를 은폐시킬수 있으며, 
 * factory 방식으로 생성/소멸을 관리한다.
 * PL entity를 추가/삭제 interface 함수를 사용함으로써, PL entity 객체 관리를 은폐시킬 수 있으며, 
 * stl ���� �迭 map�� ����Ѵ�.
 *
 */

class CNtlPLVisualManager : public CNtlPLSceneManager//, public RWS::CEventHandler
{
protected:

	EActiveWorldType m_ActiveWorldType;

	// active
	RwBool m_bThreadLoad;

	// sorting占쏙옙 占쏙옙占쏙옙 container占쏙옙 占쌔븝옙占싼댐옙.
	CNtlPLAtomicSorterContainter *m_pSortContainer;

	// FXAA deferred text: name tags / damage text collected during Render()
	// and drawn after the composite pass (sorted far -> near).
	std::list<std::pair<RwReal, CNtlPLEntity*>> m_listDeferredText;
	// Dojo Data
	CNtlPLDojoContainer	*m_pDojoContainer;
	
	CNtlPLDummyWorld	*m_pRWWorldEntity;		
	CNtlPLWorldEntity	*m_pWorldEntity;		/** 占쌈듸옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싹깍옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙. */
	CNtlPLSky			*m_pSkyEntity;			/** 占쌈듸옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싹깍옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙. */
	CNtlPLFog			*m_pFogEntity;			/** 占쌈듸옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싹깍옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙. */	
	CNtlPLPlant			*m_pPlantEntity;		/** 占쌈듸옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싹깍옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙. */	
	CNtlPLWater			*m_pWaterEntity;
	CNtlPicking			*m_pPickingObj;			/** 占쏙옙占쏙옙占쏙옙 Picking **/
    CNtlPLEntity        *m_pSoundListener;      /** Sound를 듣는 위치가 되는 리스너 객체 */

	typedef std::map< RwInt32, CNtlPLRenderGroup*, std::less<RwUInt32> > MapRenderGroup;
	MapRenderGroup m_mapInstanceGroup;	/** instance占쏙옙 占쏙옙占쏙옙占싼댐옙. */
	MapRenderGroup m_mapUpdateGroup;	/** update 占싹댐옙 entity占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙. */
	MapRenderGroup m_mapRenderGroup;	/** rendering 占싹댐옙 entity占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占싼댐옙. */

	typedef std::map<CNtlPLEntity*, CNtlPLEntity*> MapReservedUpdate;
	MapReservedUpdate m_mapAddUpdate;
	MapReservedUpdate m_mapRemoveUpdate;
	
	// distance filtering
	RwBool m_bDistFiter;							/** object distance filtering on/off flag */    
	RwReal m_fDistFilterTime;						/** object distance filter time */
	RwReal m_fDistFilterDetectTime;					/** object distance filter detecting time */
	CNtlPLDistanceFilterGroup *m_pDistFiterGroup;	/** object distance filter group */

	// auto delete call
	RwUInt32	m_uiAutoDelAlarmFlags;

    // 占쏙옙占쏙옙 占쏙옙占싱억옙 Visible 占쌓쏙옙트占쏙옙 
    std::map<RwUInt32, RwBool> m_mapRenderVisible;

	// Culling Scheduling
	CNtlPLCullingScheduling	m_CullScheduler;

protected:
		
	void RegisterInstanceGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup);

	void RegisterUpdateGroup(const RwInt32 uiClassType, CNtlPLRenderGroup *pRenderGroup);

	void RegisterRenderingGroup(const RwInt32 uiRenderLayer, CNtlPLRenderGroup *pRenderGroup);


	RwBool GetRWWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen);

	RwBool GetHeightFieldWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen);

	RwBool GetRWTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);

	RwBool GetHeightFieldTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);


	void UpdateDistanceFilter(RwReal fElapsed);		

	void UpdateSound(RwReal fElapsed);

	void ProcReservedAdd(void);

	void ProcReservedRemove(void);

	void AddReservedUpdate(CNtlPLEntity *pEntity);

	void RemoveReservedUpdate(CNtlPLEntity *pEntity);

	// auto delete marking flag
	void MarkAutoDeleteFlag(ENtlPLEntityType eType);

protected:

	virtual void AlarmAutoDelete(CNtlPLEntity *pEntity) {}

public:

	CNtlPLRenderGroup* FindInstanceGroup(const RwInt32 uiClassType);

	CNtlPLRenderGroup* FindUpdateGroup(const RwInt32 uiClassType);

	CNtlPLRenderGroup* FindRenderingGroup(const RwInt32 uiRenderLayer);
	
public:

	/**
    *  Default constructor for Presentation visual manager 
    *
    */
	CNtlPLVisualManager();

	/**
    *  Virtual destructor for Presentation vitual manager 
    *
    */
	~CNtlPLVisualManager();

	/**
	*  visual manager 占쏙옙체占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 호占쏙옙占싹댐옙 占쌉쇽옙.
	*  \see Destroy
	*/
	virtual RwBool Create(void);
	
	/**
	*  visual manager 占쏙옙占쏙옙占실깍옙 占쏙옙占쏙옙 호占쏙옙占싹댐옙 占쌉쇽옙.
	*  \see Create
	*/
	virtual void Destroy(void);

	/**
	*  vitual manager update interface 占쌉쇽옙
	*  \param fElapsed update delta time
	*/
	virtual void Update(RwReal fElapsed);

	/**
    *  presentation scene manager update 占쏙옙占쏙옙 ui占쏙옙 entity占쏙옙 rendering 占싹깍옙 占쏙옙占쏙옙 占쌉쇽옙.
	* render ware占쏙옙 camera update占쏙옙占쏙옙 占쏙옙치占쏙옙 占싫된댐옙.
	* UI도 camera texture를 사용하므로, main scene camera와 update가 겹치면 안된다.
    *
    */
	virtual void UpdateBeforeCamera(RwReal fElapsed);

	/**
	*  vitual manager render interface 占쌉쇽옙
	*/
	virtual void Render(void);
	virtual void Render4RWWorld(void);

	// Renders name tags / damage text deferred after the composite pass so
	// post-process filters (FXAA) do not affect them.
	void RenderDeferredText(void);

	/**
    *  visual manager postrender 占쌉쇽옙.
    *
    */
	virtual void PostRender(void);

	/**
    *  presentation scene active on/off flag.
    *
    */
	virtual void SetThreadLoad(RwBool bThreadLoad);

	/**
    *  reset gui manager
    *
    */
	virtual void ResetGuiManager(RwInt32 iScreenWidth, RwInt32 iScreenHeight);

	/**
	*  PL entity 占쏙옙占쏙옙 interface 占쌉쇽옙.
	*  \param pEntityName占쏙옙 entity name.
	*  \param pPropertyName는 entity에 적용될 property name.
	*  \return 占쏙옙占쏙옙占쏙옙 entity pointer
	*  \see DeleteEntity
	*/
	virtual CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const RwChar *pKey, const SPLEntityCreateParam *pParam = NULL);

	/**
	*  PL entity 占쌀몌옙 interface 占쌉쇽옙.
	*  \param pEntity占쏙옙 占쏙옙효占쏙옙 entity instance pointer.
	*  \see CreateEntity
	*/
	virtual void DeleteEntity(CNtlPLEntity *pEntity);

	/**
	*  PL entity占쏙옙 entity containter占쏙옙 add占싹댐옙 interface 占쌉쇽옙.
	*  \param pEntity占쏙옙 占쏙옙효占쏙옙 entity instance pointer.
	*  \return 占쏙옙占쏙옙占싹몌옙 TRUE, 占쏙옙占쏙옙占싹몌옙 FALSE
	*  \see RemovePLEntity
	*/
	virtual RwBool AddPLEntity(CNtlPLEntity *pEntity);

	/**
	*  PL entity占쏙옙 entity containter占쏙옙占쏙옙 remove占싹댐옙 interface 占쌉쇽옙.
	*  \param pEntity占쏙옙 占쏙옙효占쏙옙 entity instance pointer.
	*  \see RemovePLEntity
	*/
	virtual void RemovePLEntity(CNtlPLEntity *pEntity);

	/**
    *  entity占쏙옙 scene manager占쏙옙 update占쏙옙 add占싹댐옙 interface 占쌉쇽옙.
	*  \param pEntity scene manager占쏙옙 add占쏙옙 entity pointer
	*  \see RemoveUpdate
    *
    */
	virtual void AddUpdate(CNtlPLEntity *pEntity);

	/**
    *  entity占쏙옙 scene manager占쏙옙 update占쏙옙 remove占싹댐옙 interface 占쌉쇽옙.
	*  \param pEntity scene manager占쏙옙 remove占쏙옙 entity pointer
	*  \see AddUpdate
    *
    */
	virtual void RemoveUpdate(CNtlPLEntity *pEntity);

	/**
    *  entity占쏙옙 distance filtering manager占쏙옙 占쌩곤옙占싼댐옙.
	*  \param pEntity distance filtering manager占쏙옙 add占쏙옙 entity pointer
	*  \see RemoveDistanceFilter
    *
    */
	virtual void AddDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *  entity占쏙옙 distance filtering manager占쏙옙 占쏙옙占쏙옙占싼댐옙.
	*  \param pEntity distance filtering manager占쏙옙 remove占쏙옙 entity pointer
	*  \see AddDistanceFilter
    *
    */
	virtual void RemoveDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *  占쏙옙占쏙옙 active 占실억옙 占쌍댐옙 world type(renderware world or height field占싸곤옙?)
    *
    */
	virtual EActiveWorldType GetActiveWorldType(void);

	/**
	*  world 占쏙옙 占쌔븝옙 占실억옙 占쌍는곤옙?
	*  \return ready is TRUE or FALSE
	*
	*/
	virtual RwBool IsWorldReady(void);

	/**
    *  world position占쏙옙 占쌔댐옙占싹댐옙 terrain占쏙옙 height占쏙옙 占쏙옙占싹댐옙 interface 占쌉쇽옙.
	*  \return terrain height value占쏙옙 占쏙옙占쏙옙占싼댐옙.
	*  \param pWorldPos world position占쏙옙 占쌔댐옙占싹댐옙 RwV3d pointer
	*
    */
	virtual RwBool GetWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen = 1000.0f);

	/**
    *  world position占쏙옙 占쌔댐옙占싹댐옙 terrain 占쏙옙占쏙옙占쏙옙 占쌔댐옙占싹댐옙 height占쏙옙 占쏙옙占싹댐옙 interface 占쌉쇽옙.
	*  \return terrain height value占쏙옙 占쏙옙占쏙옙占싼댐옙.
	*  \param pWorldPos world position占쏙옙 占쌔댐옙占싹댐옙 RwV3d pointer
	*
    */
	virtual RwBool GetTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);


	/**
    *  world占쏙옙 pick占쏙옙 polygon占쏙옙 찾占승댐옙.
	*  \return �浿�� polygon�� ��ǥ.
    *
    */

	virtual RwBool PickWorld(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);

	//RwBool PickWorld_Old(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);
	RwBool PickWorld_New(RwInt32 iPosX, RwInt32 iPosY, SWorldPickInfo& sPickInfo, RwReal fPickTerrainLimit, RwReal fPickObjLimit);

	virtual RwBool PickTerrian(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo);
	virtual RwBool PickTerrianAndWater(RwV3d& vStartPos, RwV3d& vEndPos, SWorldPickInfo& sPickInfo);

	/**
    *  camera �浹 �˻縦 �Ͽ�, ���ο� �浹 camera ��ġ�� ���Ѵ�.
	*  \param1 pCameraPos 占쏙옙占쏙옙 camera占쏙옙 position
	*  \param2 pCameraDir 占쏙옙占쏙옙 camera占쏙옙 direction
	*  \param2 占쏙옙占싸울옙 camera占쏙옙 position
	*
    */
	virtual RwBool GetCameraCollision(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos);

	/**
	*  RpWorld pointer占쏙옙 占쏙옙占쏙옙占쏙옙 interface 占쌉쇽옙.
	*
	*/
	virtual RpWorld* GetWorldPtr(void);

	// woody1019
// 	RwBool CTChar2Poly(sNPE_COLLISION_PARAM& sNPECollisionParam);
// 	RwBool CTLine2Sphere(RwLine* _pLine, sNPE_COLLISION_PARAM& sNPECollisionParam);

	/**
	*  visual manager占쏙옙 event handler
	*  \param pMsg는 event가 발생할 경우 넘어오는 message.
	*/
	//virtual void HandleEvents(RWS::CMsg &pMsg);
	
    // Object占쏙옙 Fade 占쏙옙占쏙옙 占쌉쇽옙
	virtual void	SetDistanceFilter(RwBool bEnable);        ///< Object의 Fade기능을 끄거나 켠다. (Map Tool에서 사용)
    virtual RwBool  GetDistanceFilterEnable() {return m_bDistFiter;}                        ///< 占쏙옙占쏙옙 Fade On/Off 占쏙옙占승몌옙 占쏙옙환占싼댐옙.

	/**
	* world attribute
	* \return world占쏙옙 attribute value占쏙옙 占쏙옙占쏙옙占싼댐옙.
	* \see GetWorldNormalAttribute
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldAttribute(RwV3d vPos);

	/**
	* world normal attribute
	* \return world占쏙옙 normal attribute value占쏙옙 占쏙옙占쏙옙占싼댐옙.
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldNormalAttribute(RwV3d vPos);

	/**
	* world special attribute(占싹반속쇽옙 占쏙옙 특占쏙옙占쌈쇽옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙)
	* \return world占쏙옙 special attribute value占쏙옙 占쏙옙占쏙옙占싼댐옙.
	* \see GetWorldNormalAttribute
	*/
	virtual DWORD GetWorldSpecialAttribute(RwV3d vPos);

	/**
	* world material attribute
	* \return world占쏙옙 material attribute value占쏙옙 占쏙옙占쏙옙占싼댐옙.
	*/
	virtual BYTE GetWorldMaterialAttribute(RwV3d vPos);

	/**
	* �Ϲ������� world�� normal attribute�� üũ�Ͽ� ���� ��쿡�� ����Ѵ�.
	* \return world占쏙옙 water 占쏙옙占쏙옙.
	*/
	virtual RwReal GetWorldWaterHeight(RwV3d vPos);

	/**
	* 占쏙옙占쏙옙 占쏙옙치占쏙옙 Bloom effect power factor占쏙옙 占쏙옙占쌔온댐옙.
	* \return if TRUE 占싱몌옙 占쏙옙占쏙옙 占쏙옙효占싹곤옙, FALSE占싱몌옙 占쏙옙효占쏙옙占쏙옙 占십댐옙.
	*/
	virtual RwReal					GetActiveBloomFactor(void); 

	virtual VOID					SetActiveSky(CNtlPLSky* _pNtlPLSky) { m_pSkyEntity = _pNtlPLSky; }

	virtual CNtlPLDojoContainer*	GetDojoContainer(void)		{ return m_pDojoContainer; } 

	virtual CNtlPLDummyWorld*		GetRwWorld(void)			{ return m_pRWWorldEntity; }
	virtual CNtlPLWorldEntity*		GetWorld(void)				{ return m_pWorldEntity; }
	virtual CNtlPLSky*				GetSky(void)				{ return m_pSkyEntity; }
	virtual CNtlPLFog*				GetFog(void)				{ return m_pFogEntity; }
	virtual CNtlPLPlant*			GetPlant(void)				{ return m_pPlantEntity; }
	virtual CNtlPLWater*			GetWater(void)				{ return m_pWaterEntity; }
	virtual CNtlPicking*			GetPickingObject( void )	{ return m_pPickingObj; }

	virtual RwBool GetAvailablePos(RwV3d& _CurPos);

	virtual VOID SetWorldPVSActivation(RwBool _Flag);

	virtual RwBool GetWorldTerrainRotate(RwMatrix* pmatOut, RwV3d* pvPos, RwBool bPosHeightChange, RwReal fLineLen = 1000.0f);

    virtual VOID SetSoundListener(CNtlPLEntity* pEntity);

	virtual RwBool GetWorldShadowColor(RwV3d& vPos, RwRGBA* pOutColor);

	virtual RwBool GetWorldLightColor(RwV3d& vPos, RwRGBA* pOutColor, RwBool bCalcShadow);

    virtual void   SetEntityVisible(ENtlPLEntityType eType, RwBool bVisible);             ///< 占쏙옙占쏙옙占쏙옙占싱억옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 On/Off 占싼댐옙. (占쌓쏙옙트占쏙옙)

	virtual void	SetVisiblePlanet(RwBool bVisible);

	virtual void	RunableRainBloomLight(RwReal fBloomShaderFadeInTime, RwReal fBloomShaderFadeOutTime);

	virtual void	RunableRainSkyLight(RwReal fSkyFadeInTime, RwReal fSkyFadeOutTime);
};

#endif
