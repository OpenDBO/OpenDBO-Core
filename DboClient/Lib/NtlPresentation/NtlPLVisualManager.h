/*****************************************************************************
 *
 * File			: NtlPLSceneManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (��)NTL
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
 * client application�� visual ��ü�� �����ϴ� class�̴�.
 * PL entity���� ������ layer�� ������ �ְ�, layer �����Ƿ� update �� rendering �Ѵ�.
 * renderware������ alpha sorting�� �ȵǰ� �����Ƿ� , object ������ layer�� �̿��Ͽ� alpha sorting�� �Ѵ�.
 * ���� PL entity�� ���� �� �Ҹ��� interface�� �����ϰ� ������, ����/�Ҹ� ������ �����ų�� ������, 
 * factory ������� ����/�Ҹ��� �����Ѵ�.
 * PL entity�� �߰�/���� interface �Լ��� ��������ν�, PL entity ��ü ������ �����ų �� ������, 
 * stl ���� �迭 map�� ����Ѵ�.
 *
 */

class CNtlPLVisualManager : public CNtlPLSceneManager//, public RWS::CEventHandler
{
protected:

	EActiveWorldType m_ActiveWorldType;

	// active
	RwBool m_bThreadLoad;

	// sorting�� ���� container�� �غ��Ѵ�.
	CNtlPLAtomicSorterContainter *m_pSortContainer;

	// FXAA deferred text: name tags / damage text collected during Render()
	// and drawn after the composite pass (sorted far -> near).
	std::list<std::pair<RwReal, CNtlPLEntity*>> m_listDeferredText;
	// Dojo Data
	CNtlPLDojoContainer	*m_pDojoContainer;
	
	CNtlPLDummyWorld	*m_pRWWorldEntity;		
	CNtlPLWorldEntity	*m_pWorldEntity;		/** �ӵ��� ������ �ϱ� ���� ���� ����. */
	CNtlPLSky			*m_pSkyEntity;			/** �ӵ��� ������ �ϱ� ���� ���� ����. */
	CNtlPLFog			*m_pFogEntity;			/** �ӵ��� ������ �ϱ� ���� ���� ����. */	
	CNtlPLPlant			*m_pPlantEntity;		/** �ӵ��� ������ �ϱ� ���� ���� ����. */	
	CNtlPLWater			*m_pWaterEntity;
	CNtlPicking			*m_pPickingObj;			/** ������ Picking **/
    CNtlPLEntity        *m_pSoundListener;      /** Sound�� ��� ��ġ�� �Ǵ� ������ ��ü */

	typedef std::map< RwInt32, CNtlPLRenderGroup*, std::less<RwUInt32> > MapRenderGroup;
	MapRenderGroup m_mapInstanceGroup;	/** instance�� �����Ѵ�. */
	MapRenderGroup m_mapUpdateGroup;	/** update �ϴ� entity�� ���� �����Ѵ�. */
	MapRenderGroup m_mapRenderGroup;	/** rendering �ϴ� entity�� ���� �����Ѵ�. */

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

    // ���� ���̾� Visible �׽�Ʈ�� 
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
	*  visual manager ��ü�� ������ ���� ȣ���ϴ� �Լ�.
	*  \see Destroy
	*/
	virtual RwBool Create(void);
	
	/**
	*  visual manager �����Ǳ� ���� ȣ���ϴ� �Լ�.
	*  \see Create
	*/
	virtual void Destroy(void);

	/**
	*  vitual manager update interface �Լ�
	*  \param fElapsed update delta time
	*/
	virtual void Update(RwReal fElapsed);

	/**
    *  presentation scene manager update ���� ui�� entity�� rendering �ϱ� ���� �Լ�.
	* render ware�� camera update���� ��ġ�� �ȵȴ�.
	* UI�� camera texture�� ����ϹǷ�, main scene camera�� update�� ��ġ�� �ȵȴ�.
    *
    */
	virtual void UpdateBeforeCamera(RwReal fElapsed);

	/**
	*  vitual manager render interface �Լ�
	*/
	virtual void Render(void);
	virtual void Render4RWWorld(void);

	// Renders name tags / damage text deferred after the composite pass so
	// post-process filters (FXAA) do not affect them.
	void RenderDeferredText(void);

	/**
    *  visual manager postrender �Լ�.
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
	*  PL entity ���� interface �Լ�.
	*  \param pEntityName�� entity name.
	*  \param pPropertyName�� entity�� ����� property name.
	*  \return ������ entity pointer
	*  \see DeleteEntity
	*/
	virtual CNtlPLEntity* CreateEntity(ENtlPLEntityType eType, const RwChar *pKey, const SPLEntityCreateParam *pParam = NULL);

	/**
	*  PL entity �Ҹ� interface �Լ�.
	*  \param pEntity�� ��ȿ�� entity instance pointer.
	*  \see CreateEntity
	*/
	virtual void DeleteEntity(CNtlPLEntity *pEntity);

	/**
	*  PL entity�� entity containter�� add�ϴ� interface �Լ�.
	*  \param pEntity�� ��ȿ�� entity instance pointer.
	*  \return �����ϸ� TRUE, �����ϸ� FALSE
	*  \see RemovePLEntity
	*/
	virtual RwBool AddPLEntity(CNtlPLEntity *pEntity);

	/**
	*  PL entity�� entity containter���� remove�ϴ� interface �Լ�.
	*  \param pEntity�� ��ȿ�� entity instance pointer.
	*  \see RemovePLEntity
	*/
	virtual void RemovePLEntity(CNtlPLEntity *pEntity);

	/**
    *  entity�� scene manager�� update�� add�ϴ� interface �Լ�.
	*  \param pEntity scene manager�� add�� entity pointer
	*  \see RemoveUpdate
    *
    */
	virtual void AddUpdate(CNtlPLEntity *pEntity);

	/**
    *  entity�� scene manager�� update�� remove�ϴ� interface �Լ�.
	*  \param pEntity scene manager�� remove�� entity pointer
	*  \see AddUpdate
    *
    */
	virtual void RemoveUpdate(CNtlPLEntity *pEntity);

	/**
    *  entity�� distance filtering manager�� �߰��Ѵ�.
	*  \param pEntity distance filtering manager�� add�� entity pointer
	*  \see RemoveDistanceFilter
    *
    */
	virtual void AddDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *  entity�� distance filtering manager�� �����Ѵ�.
	*  \param pEntity distance filtering manager�� remove�� entity pointer
	*  \see AddDistanceFilter
    *
    */
	virtual void RemoveDistanceFilter(CNtlPLEntity *pEntity);

	/**
    *  ���� active �Ǿ� �ִ� world type(renderware world or height field�ΰ�?)
    *
    */
	virtual EActiveWorldType GetActiveWorldType(void);

	/**
	*  world �� �غ� �Ǿ� �ִ°�?
	*  \return ready is TRUE or FALSE
	*
	*/
	virtual RwBool IsWorldReady(void);

	/**
    *  world position�� �ش��ϴ� terrain�� height�� ���ϴ� interface �Լ�.
	*  \return terrain height value�� �����Ѵ�.
	*  \param pWorldPos world position�� �ش��ϴ� RwV3d pointer
	*
    */
	virtual RwBool GetWorldHeight(const RwV3d *pWorldPos, RwReal& fHeight, RwV3d *pNormal, RwReal fLineLen = 1000.0f);

	/**
    *  world position�� �ش��ϴ� terrain ������ �ش��ϴ� height�� ���ϴ� interface �Լ�.
	*  \return terrain height value�� �����Ѵ�.
	*  \param pWorldPos world position�� �ش��ϴ� RwV3d pointer
	*
    */
	virtual RwBool GetTerrainHeight(const RwV3d *pWorldPos, RwReal& fHeight);


	/**
    *  world�� pick�� polygon�� ã�´�.
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
	*  \param1 pCameraPos ���� camera�� position
	*  \param2 pCameraDir ���� camera�� direction
	*  \param2 ���ο� camera�� position
	*
    */
	virtual RwBool GetCameraCollision(const RwV3d *pCameraPos, const RwV3d *pLookAt, RwReal fRadius, RwV3d& vNewPos);

	/**
	*  RpWorld pointer�� ������ interface �Լ�.
	*
	*/
	virtual RpWorld* GetWorldPtr(void);

	// woody1019
// 	RwBool CTChar2Poly(sNPE_COLLISION_PARAM& sNPECollisionParam);
// 	RwBool CTLine2Sphere(RwLine* _pLine, sNPE_COLLISION_PARAM& sNPECollisionParam);

	/**
	*  visual manager�� event handler
	*  \param pMsg�� event�� �߻��� ��� �Ѿ���� message.
	*/
	//virtual void HandleEvents(RWS::CMsg &pMsg);
	
    // Object�� Fade ���� �Լ�
	virtual void	SetDistanceFilter(RwBool bEnable);        ///< Object�� Fade����� ���ų� �Ҵ�. (Map Tool���� ���)
    virtual RwBool  GetDistanceFilterEnable() {return m_bDistFiter;}                        ///< ���� Fade On/Off ���¸� ��ȯ�Ѵ�.

	/**
	* world attribute
	* \return world�� attribute value�� �����Ѵ�.
	* \see GetWorldNormalAttribute
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldAttribute(RwV3d vPos);

	/**
	* world normal attribute
	* \return world�� normal attribute value�� �����Ѵ�.
	* \see GetWorldSpecialAttribute
	*/
	virtual DWORD GetWorldNormalAttribute(RwV3d vPos);

	/**
	* world special attribute(�ϹݼӼ� �� Ư���Ӽ����� ������)
	* \return world�� special attribute value�� �����Ѵ�.
	* \see GetWorldNormalAttribute
	*/
	virtual DWORD GetWorldSpecialAttribute(RwV3d vPos);

	/**
	* world material attribute
	* \return world�� material attribute value�� �����Ѵ�.
	*/
	virtual BYTE GetWorldMaterialAttribute(RwV3d vPos);

	/**
	* �Ϲ������� world�� normal attribute�� üũ�Ͽ� ���� ��쿡�� ����Ѵ�.
	* \return world�� water ����.
	*/
	virtual RwReal GetWorldWaterHeight(RwV3d vPos);

	/**
	* ���� ��ġ�� Bloom effect power factor�� ���ؿ´�.
	* \return if TRUE �̸� ���� ��ȿ�ϰ�, FALSE�̸� ��ȿ���� �ʴ�.
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

    virtual void   SetEntityVisible(ENtlPLEntityType eType, RwBool bVisible);             ///< �������̾��� �������� On/Off �Ѵ�. (�׽�Ʈ��)

	virtual void	SetVisiblePlanet(RwBool bVisible);

	virtual void	RunableRainBloomLight(RwReal fBloomShaderFadeInTime, RwReal fBloomShaderFadeOutTime);

	virtual void	RunableRainSkyLight(RwReal fSkyFadeInTime, RwReal fSkyFadeOutTime);
};

#endif