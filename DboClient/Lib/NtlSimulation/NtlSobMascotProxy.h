/*****************************************************************************
 *
 * File			: NtlSobMascotProxy.h
 * Author		: Alejandro-HUB
 * Copyright	: (C)NTL
 * Date			: 2024. 11. 10
 * Abstract		: Simulation object mascot proxy class
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/

#ifndef _NTL_SOB_MASCOT_PROXY_H_
#define _NTL_SOB_MASCOT_PROXY_H_

// Core
#include "NtlMath.h"

// shared
#include "NtlMascot.h"

// presentation - add animation includes
#include "NtlInstanceAnimData.h"

// SL
#include "NtlSobProxy.h"

class CNtlPLCharacter;
class CNtlPLCameraRenderTexture;
class CNtlSobMascot;

class CNtlSobMascotProxy : public CNtlSobProxy
{
public:
	typedef struct _SUICharacter
	{
		CNtlPLCharacter				*pPLCharacter;
		CNtlPLCameraRenderTexture	*pUICamera;				/** UI render camera */
	}SUICharacter;

private:
	struct SUIMascotStatusRotate
	{
		RwBool	bLeftRot;
		RwBool	bRightRot;
		RwReal  fAngle;        
	};

	CNtlPLCharacter				*m_pCharacter;				/** character resource */
	RwBool						m_bResLoaded;
	RwUInt8						m_byAnimBlendNoneOneTime;	/** animation blending을 한번만 실행하지 않는다 */

	// Animation system members (following character proxy pattern)
	CNtlInstanceAnimTable		m_InstanceAnimTable;		/** Instance animation table for loading animations */
	RpHAnimHierarchy			*m_pBaseHierarchy;			/** Base animation hierarchy */
	RwUInt32					m_uiCurrentAnimKey;			/** Current playing animation key */

	SUICharacter				*m_pUIMascotStatusWnd;		/** UI mascot character */
	SUIMascotStatusRotate		*m_pUIMascotStatusRotate;
	CNtlPLCameraRenderTexture	*m_pUIMascotStatusBarCamera;
	RwBool						m_bUIMascotStatusWndInitialized;	/** UI character initialization flag */

//-------------------------------------------------------------------------------
// event handler
//-------------------------------------------------------------------------------

public:

	typedef void (CNtlSobMascotProxy::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	static MapFuncEventHandler m_mapEventFunc;

	static void RegisterEventHandler(void);

	// event handler function
	void SobCreateEventHandler(RWS::CMsg &pMsg);
	void SobDestroyEventHandler(RWS::CMsg &pMsg);
	void SobGotFocusEventHandler(RWS::CMsg &pMsg);
	void SobLostFocusEventHandler(RWS::CMsg &pMsg);

protected:

	/** 
	* mascot entity 생성 (following expert guidance)
	*/
	CNtlPLCharacter* CreateMascotEntity(RwBool bNotShading = FALSE);



	/**
	* Update
	*/
	void UpdateResourceLoadCheck(RwReal fElapsed);
	void UpdateGUIUpdateCheck(RwReal fElapsed);
	void UpdateMascotFollowing(RwReal fElapsed);
	
	/**
	* resource
	*/
	void ResourceLoadComplete(RwBool bVisible);
	RwBool IsResourceLoaded(void);

public:

	CNtlSobMascotProxy();
	virtual ~CNtlSobMascotProxy();

	virtual RwBool Create(RwUInt32 uiCompType);
	virtual void Destroy(void);
	virtual void Update(RwReal fElapsed);
	virtual void HandleEvents(RWS::CMsg &pMsg);

	//---------------------------------------------------
	// proxy system flag 관련된 함수들.
	//---------------------------------------------------
	void EnableProxySystemVisible(RwBool bVisible);

	//---------------------------------------------------
	// Inventory UI 에 관련된 함수들.
	//---------------------------------------------------
	RwTexture* UIPcStatusBarRender(void);
	CNtlPLCharacter* GetUIMascot() {return m_pUIMascotStatusWnd ? m_pUIMascotStatusWnd->pPLCharacter : NULL;}		///< UI화면의 캐릭터 객체를 반환한다.
	void RecreateUIMascotStatusWndCharacter(void);

	/**
	* UI mascot status window rendering (made public for temporary UI mascot creation)
	*/
	void CreateUIMascotStatusWndCharacter(void);
	void DeleteUIMascotStatusWndCharacter(void);
	void CreateUIMascotStatusWndCamera(void);
	void DeleteUIMascotStatusWndCamera(void);

	//---------------------------------------------------
	// owner simulation object  상태 관련 함수들.
	//---------------------------------------------------
	void	NotifyStateEnter(RwUInt32 uiStateId);	
	void	NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState);

	//---------------------------------------------------
	// presentation entity 생성 함수들.                                                                      
	//---------------------------------------------------

	// bone attach하면서 effect 생성.
	virtual CNtlPLEntity* CreatePLChildEffect(const RwChar *pKey, const RwChar *pAttachBoneName, RwV3d vOffset = ZeroAxis, RwBool bIgnoreVisible = FALSE);

	// offset position에 attach하면서 effect 생성.
	virtual CNtlPLEntity*	CreatePLChildEffect(const RwChar *pKey, RwV3d vOffset, RwBool bIgnoreVisible = FALSE, RwBool bApplyRotate = FALSE);

	//---------------------------------------------------
	// main presentation entity에 관련된 함수들 
	//---------------------------------------------------

	/**
	* world에 add 와 remove 관련된 함수들.
	*/
	virtual void			AddWorld(void);		
	virtual void			RemoveWorld(void);
	void					DeleteMascotEntity(void);		// Made public for proper cleanup

	/**
	* transform에 관련된 함수들.
	*/
	virtual void SetPosition(const RwV3d *pPos);
	virtual void SetDirection(const RwV3d *pDir);
	virtual void SetAngleY(RwReal fAngle);
	virtual void SetDeltaAngleY(RwReal fAngle);
	virtual void SetAngleX(RwReal fAngle);
	virtual void SetDeltaAngleX(RwReal fAngle);
	virtual void SetScale(RwReal fScale);

	/**
	* material에 관련된 함수들.
	*/
	void SetAlpha(RwUInt8 byAlpha);
	void SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	void SetInkThicknessWeight(RwReal fWeightValue = 1.0f); 
	void SetInkThickness(RwReal fValue = NTL_INK_THICKNESS);
	void SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);

	/**
	* animation에 관련된 함수들.
	*/
	// animation
	void		SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop = TRUE, RwReal fStartTime = 0.0f);
	RwBool		IsExistBaseAnimation(RwUInt32 uiAnimKey);
	RwUInt32	GetBaseAnimationKey(void);
	RwBool		IsBaseAnimationEnd(void);
	void		SetAnimSpeed(RwReal fAniSpeed);
	RwReal      GetAnimSpeed();
	void		SetAnimBlendNoneOneTime(void);

	/**
	* entity 관련된 함수들.
	*/
	CNtlPLEntity* GetPLMainEntity(void);    // presentation main entity
	virtual RwV3d GetBonePosition(const RwChar *pBoneName); 	// bone position을 반환한다.
	virtual RwReal GetPLEntityHeight(void); // pl entity의 높이를 반환한다.
	virtual RwReal GetPLEntityWidth(void);  // pl entity의 너비를 반환한다.
	virtual RwReal GetPLEntityDepth(void);  // pl entity의 깊이를 반환한다.
	virtual RwReal GetPLEntityBaseScale(void);  // pl entity의 base scale을 반환한다.
	virtual RwReal GetScale();
	void           ChangeModel(CNtlPLCharacter* pCharacter, RwBool bDeleteOrgModel = FALSE);        ///< 새로운 Entity 모델을 교체한다.
	void           ChangeUIModel(CNtlPLCharacter* pCharacter);                         ///< UI창의 모델을 교체한다.

	/**
	* UI rotation 관련 함수들
	*/
	void	MascotStatusRotateLeft(void);
	void	MascotStatusRotateRight(void);
	void	MascotStatusRotateStop(void);
	void	MascotStatusRotateReset(void);
	void	MascotStatusRotate(RwReal fDelta);

	/**
	* visibility 관련 함수들
	*/
	virtual void EnableVisible(RwBool bEnable);    
	virtual void EnablePicking(RwBool bEnable);
	virtual void EnableShadowRendering(RwBool bEnable);
	virtual void EnableHighlight(RwBool bEnable);
	virtual void CreateElapsedController(RwReal fCtrlTime, RwReal fWeightValue);
	virtual void DeleteElapsedController(void);

	//---------------------------------------------------
	// focus 관련된 함수들.
	//---------------------------------------------------
	virtual void SetFocus(void);
	virtual void ReleaseFocus(void);

	//---------------------------------------------------
	// Static functions
	//---------------------------------------------------
	// Note: Static CreateMascotEntity removed to avoid conflict with instance method
};

#endif 