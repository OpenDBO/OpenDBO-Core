/******************************************************************************
* File			: PortalGui.h
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 10. 22
* Abstract		: 
*****************************************************************************
* Desc			: CPortalGui
*                 포탈 NPC를 클릭하면 나오는 Portal GUI
*                 포탈맵에서 가고 싶은 장소를 클릭하면 순간이동 된다.
*				  
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"
#include "surfaceGui.h"

// presetation
#include "NtlPLGui.h"

class SNtlEventPortalInfoRes;


// 포탈 UI에 사용될 아이콘의 타입
enum EPortalIconType
{
    E_PORTAL_ICON_START_POINT,                      ///< 현재 포인트를 표시할 아이콘
    E_PORTAL_ICON_START_POINT_FOCUS,
    E_PORTAL_ICON_ENABLE,                           ///< 현재 이동 가능한 아이콘    
    E_PORTAL_ICON_ENABLE_FOCUS,

    MAX_ICON_TYPE_CNT,

    E_PORTAL_ICON_NOT,
};

/// 포탈 툴팁을 표시할때 넘겨주기 위한 구조체
struct SPortalTooltip
{
    RwUInt32 nPortalID;
    RwInt32  ePortalIconType;
    DWORD    nZenny;
};

// 포탈별 상태 (포탈의 ID는 맵의 Key값이 된다.)
struct SPortalInfo
{
    RwInt32             m_nIndex;           ///< 포탈의 ID
    CSurfaceGui         m_surIcon;          ///< ICon을 표시할 서페이스
    EPortalIconType     m_eIconType;        ///< 현재 포탈의 상태이자 아이콘 타입
    DWORD               m_nZenny;           ///< 소비 제니 (갈수 없는 포탈이면 0)    
    RwBool              m_bRegist;          ///< 등록 여부
};

class CPortalGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
    CPortalGui(const RwChar* pName);
    ~CPortalGui();

    RwBool      Create();
    VOID        Destroy();
    RwInt32     SwitchDialog(bool bOpen);                   ///< DialogManager에서의 Open/Close

protected:
    VOID        HandleEvents(RWS::CMsg &pMsg);
    VOID        Init();    
    
    VOID        SetupPortalInfo();                          ///< 포탈들의 정보들을 세팅한다 (최초에 한번만 호출)
    VOID        UpdatePortalInfo();                         ///< 포탈들의 정보들을 업데이트한다.
    RwBool      CheckRegist(RwUInt32 hNPCSerialID);         ///< 현재 포탈 포인트가 등록되어 있는 곳인지를 체크하고, 등록되어 있지않으면 등록한다.    
    RwBool      IsConnect(RwUInt32 nPortalID, DWORD& nZenny);    ///< 현재 포인트와 연결되어 있는 포인트인지 확인한다. (등록되어 있으면 True와 필요 Zenny를 반환한다)
    VOID        SetPortalInfo(SNtlEventPortalInfoRes* pInfo);    ///< 등록된 포탈들의 정보를 세팅한다.
    RwInt32     FindPointIndex(RwUInt32 nCurrPortalID, RwUInt32 nTargetPortalID); ///< 타겟 포탈의 현재포인트에서의 인덱스를 찾아서 반환한다. 

    inline RwBool      IsRegist(RwUInt32 nPortalID);               ///< 현재 등록되어 있는지 확인한다.

    // CallBack
    VOID        OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
    VOID        OnMouseUp(const CKey& key);    
    VOID	    OnPaint();
    VOID	    OnMove(RwInt32 iOldX, RwInt32 iOldY);
    VOID	    OnClickExitBtn(gui::CComponent* pComponent);    

protected:
    typedef     std::map<RwUInt32, SPortalInfo*>  MAPPortalInfo;    
    typedef     std::pair<RwUInt32, SPortalInfo*> PAIRPortalInfo;

    MAPPortalInfo m_mapPortalInfo;                          ///< 포탈들의 정보를 가지고 있는 맵

    RwInt32     m_nCurrIdx;                                 ///< 현재 위치하고 있는 포탈의 인덱스
    RwInt32     m_nSelectIdx;                               ///< 현재 선택중(커서가 위에있는)인 포탈의 인덱스    
    RwUInt32    m_hNPCSerialID;                             ///< NPC의 Serail ID    
    RwBool      m_bTeleportSuccess;                         ///< 텔레포트 성공 여부
    
    gui::CSurface m_aSurIcon[MAX_ICON_TYPE_CNT];        ///< 아이콘에 사용될 서페이스들

    // Control
    gui::CButton* m_pBtnExit; 

    // 아이콘 범례 텍스트들
    gui::CStaticBox* m_pSttStartPoint;                      
    gui::CStaticBox* m_pSttEnablePoint;    

    // slot
    gui::CSlot  m_slotMouseMove;
    gui::CSlot  m_slotMouseUp;
    gui::CSlot  m_slotPaint;
    gui::CSlot  m_slotBtnExit;
    gui::CSlot  m_slotMove;
};
