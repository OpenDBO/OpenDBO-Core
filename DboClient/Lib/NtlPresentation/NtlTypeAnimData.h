/*****************************************************************************
 *
 * File			: NtlTypeAnimData.h
 * Author		: HongHoDong
 * Copyright	: (��)NTL
 * Date			: 2006. 4. 11	
 * Abstract		: NTL NtlTypeAnimData
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_TYPE_ANIM_DATA_H__
#define __NTL_TYPE_ANIM_DATA_H__

#include "NtlAnimEventData.h"
#include "NtlDebug.h"
#include "NtlCharacterData.h"
#include "NtlSerializer.h"

/// �÷��� ���� ������
#define ANIM_FLAG_CULL_TEST_ALL_ATOMIC  0x00000001

/*!
 * \Animation
 * STypeAnimData: ������ �ʴ� Data(Read�� ����)
 * 
 */
class AnimEvent_CompareFunc
{
public:
	bool operator() (const SEventAnim* lhs, const SEventAnim* rhs) const
	{
		//if(lhs->fTime != ANIM_EVENT_NONE_TIME && rhs->fTime != ANIM_EVENT_NONE_TIME)
			return lhs->fTime < rhs->fTime;

		//return false;
	}
};

struct STypeAnimData
{
	std::string			strAnimName;							///< Animation File �̸�
	RwUInt32			uiAnimKey;								///< Animation Key
	RwReal              fPlayTime;                              ///< Animation�� PlayTime
    RwUInt32            flagAnim;                               ///< �ִϸ��̼� ���õ� ���� �÷���

	std::vector<SEventAnim *> vecAnimEvent;						///< Animation Event�� (fTime�� ������ �Ǿ� �־�� �Ѵ�.)
	RwBool						m_bReLoad;						///< Resource�� ���� �о�� �� ���
	
	STypeAnimData() : uiAnimKey(INVALID_GRAPHIC_ANIMATION_ID), m_bReLoad(FALSE), fPlayTime(0.0f), flagAnim(0)
	{
		vecAnimEvent.reserve(15);
	}

	~STypeAnimData()
	{
		if(vecAnimEvent.size())
		{
			for(RwUInt32 i = 0; i < vecAnimEvent.size(); i++)
			{
				if(vecAnimEvent[i] != NULL)
				{
                    // Hit Event���� Multi Hissidan �����Ͱ� �������� �Ҵ�Ǿ� �ֱ� ������
                    // ����ȯ�� ���Ŀ� ������� �Ѵ�.
                    if(vecAnimEvent[i]->eEventID == EVENT_ANIM_HIT)
                    {
                        SEventAnimHit* pEventAnimHit = (SEventAnimHit*)vecAnimEvent[i];
                        NTL_DELETE(pEventAnimHit);
                    }
                    else
                    {
					    NTL_DELETE (vecAnimEvent[i]);
                    }
				}
			}
		}

		vecAnimEvent.clear();
	}

	void AnimEventSort()
	{
		AnimEvent_CompareFunc cf;
		sort(vecAnimEvent.begin(), vecAnimEvent.end(), cf);
	}

    //--------- �÷��� ����
    void    SetCullTestAllAtomic(RwBool bFlag) {flagAnim |= ANIM_FLAG_CULL_TEST_ALL_ATOMIC;}
    RwBool  IsCullTestAllAtomic() {return flagAnim & ANIM_FLAG_CULL_TEST_ALL_ATOMIC;}
};


typedef	std::map<RwUInt32, STypeAnimData *>		      TYPE_ANIM_MAP;
typedef	std::map<RwUInt32, STypeAnimData *>::iterator TYPE_ANIM_MAP_ITER;

class CNtlTypeAnimTable
{
public:
	CNtlTypeAnimTable() {};
	~CNtlTypeAnimTable();

protected:
	TYPE_ANIM_MAP	m_mapTypeAnim;	
    std::string     m_strAnimPath;                                          ///< Animation Folder Path

public:
	void	Create();
	void	Destroy();
	
	STypeAnimData	*Get(RwUInt32 uiKey);									///< Get STypeAnimData
	STypeAnimData	*Add(RwUInt32 uiKey, const std::string &strAnimName);	///< Add STypeAnimData
    STypeAnimData	*Add(RwUInt32 uiKey, STypeAnimData *pTypeAnimData);     ///< Add STypeAnimData 
    void            Remove(RwUInt32 uiKey);                                 ///< �ʿ��� �����͸� �����Ѵ�. (by Agebreak. 2006.05.01)

    TYPE_ANIM_MAP	*GetTypeAnimMap();

	const std::string	*GetFileName(RwUInt32 uiKey);								///< Get AnimName

    void SetAnimPath(std::string& strAnimPath) { m_strAnimPath = strAnimPath;}
    std::string GetAnimPath() {return m_strAnimPath;}

	void	SaveSerialize( CNtlSerializer& sOut);						///< Serialize ��ü�� �����͸� �����Ѵ�.
	void	LoadSerialize( CNtlSerializer& sIn);						///< Serailize ��ü���� �����͸� �ε��Ѵ�.
};

#endif