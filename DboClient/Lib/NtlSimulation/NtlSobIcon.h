/*****************************************************************************
 *
 * File			: NtlSobIcon.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 3. 07	
 * Abstract		: Simulation icon object
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_ICON_H__
#define __NTL_SOB_ICON_H__

#include "NtlSob.h"

class CNtlSobIcon : public CNtlSob
{
public:
	enum 
	{
		ICON_STATE_NOTUSEABLE,
		ICON_STATE_USEABLE,
		ICON_STATE_COOLING
	};

protected:

	CNtlSob* m_pSobObj;		/** icon 을 소유하는 객체의 pointer => CNtlSobSkill, CNtlSobItem 등등*/ 
	void*    m_pImage;

	CNtlSobIcon();

	RwUInt8 m_byIconState;
	RwReal  m_fCoolTime;
	RwReal  m_fMaxCoolTime;
    RwUInt32    m_uiErrorMsg;           ///< 에러 메시지 변수

	static RwReal m_fCollingUpdateFactor;
	
public:

	virtual ~CNtlSobIcon();

	void SetSobObj(CNtlSob *pSobObj);
	CNtlSob* GetSobObj(void) const;
	void CreateImage(const RwChar *pFileName);
	void* GetImage(void) { return m_pImage; }

	virtual RwBool IsDragAndDropPossible(void) { return FALSE; }
	virtual RwBool IsUsePossible(void) { return FALSE; }
	
	virtual void	Update(RwReal fElapsed);
	virtual RwBool	Use(SERIAL_HANDLE hTargetSerialId,RwUInt32 param1=0) { return FALSE; }
	virtual RwBool	CanUse(SERIAL_HANDLE hTargetSerialId)	{ return FALSE; }

	virtual void SetIconState(RwUInt8 byIconState, RwReal fCoolTIme = 0.0f);
	virtual void SetIconState(RwUInt8 byIconState, RwReal fMaxCoolTime, RwReal fCoolTime);
	virtual RwUInt8 GetIconState(void) const { return m_byIconState; }
	
	virtual RwReal GetCoolingTime(void) const { return m_fCoolTime; }
	virtual RwReal GetMaxCoolingTime(void) const { return m_fMaxCoolTime; }

	static void	SetCoolingUpdateFactor(RwReal fFactor);

    RwUInt32    GetErrorMsg() {return m_uiErrorMsg;}
};

inline CNtlSob* CNtlSobIcon::GetSobObj(void) const
{
	return m_pSobObj;
}

#endif
