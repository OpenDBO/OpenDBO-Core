#ifndef __INC_DBOG_CHARATTRIBUTE_PC_H__
#define __INC_DBOG_CHARATTRIBUTE_PC_H__


#include "CharacterAtt.h"

class CPlayer;

class CCharacterAttPC : public CCharacterAtt
{

public:
	CCharacterAttPC();
	virtual	~CCharacterAttPC();

public:

	bool				Create(CPlayer* pPlayer);


	virtual void		CalculateAtt();


public:

	virtual float		GetWalkSpeed();

public:

	virtual void		CalculateStr(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void		CalculateCon(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void		CalculateFoc(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void		CalculateDex(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void		CalculateSol(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void		CalculateEng(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void		CalculateBattleAttribute(float fValue, BYTE byApplyType, bool bIsPlus);

	DWORD				GetTotalWeight() { return m_dwTotalWeight; }
	BYTE				GetOverweightLevel() { return m_byOverweightLevel; }


private:

	void				CalcSecondWeaponOffence();
	void				ApplyOverweightEffects();


private:

	CPlayer*			m_pPlayerRef;

	DWORD				m_dwTotalWeight;
	BYTE				m_byOverweightLevel;

};

#endif