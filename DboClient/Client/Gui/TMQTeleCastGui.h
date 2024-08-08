/******************************************************************************
* File			: TMQTeleCastGui.h
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 11. 15
* Abstract		: 
*****************************************************************************
* Desc			: TMQ의 방송 시스템의 메인 GUI
*				  
*****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlSLEvent.h"
#include "NtlPLCameraRenderTexture.h"
#include "NtlPLCharacter.h"
#include "SurfaceGui.h"
#include "DBOUIConfig.h"
#include "NtlPLAccelController.h"
    
class CTMQTeleCastGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
    /// 연출 상태 플래그
    enum ETeleCastState
    {
        TELECAST_SLIDING_IN,        ///< 슬라이딩 인 연출 상태
        TELECAST_OPENING,           ///< 열리는 연출 상태
        TELECAST_OPEN,              ///< 일반 열린 상태        
        TELECAST_CLOSING,           ///< 닫히는 연출 상태
        TELECAST_SLIDING_OUT,       ///< 슬라이딩 아웃 연출 상태
        TELECAST_CLOSED,            ///< 닫힌 상태
    };

public:
    CTMQTeleCastGui(const RwChar* pName);
    ~CTMQTeleCastGui(void);

    // 오버 라이딩
    RwBool      Create();
    VOID        Destroy();
    VOID		Update(RwReal fElapsed);
    RwInt32     SwitchDialog(bool bOpen);                   ///< DialogManager에서의 Open/Close
    void	    Show(bool bShow);				
    VOID	    UpdateBeforeCamera(RwReal fElapsed);
    
protected:
    // 오버 라이딩
    VOID        HandleEvents(RWS::CMsg &pMsg);
    VOID        Init();    

    // CallBack
    VOID	    OnPaint();
    VOID		OnOpeningMovieEnd(gui::CComponent* pComponent);
    VOID		OnClosingMovieEnd(gui::CComponent* pComponent);
    VOID        OnFadeMovieEnd(gui::CComponent* pComponent);
    VOID	    OnMove(RwInt32 iOldX, RwInt32 iOldY);

    // 자체 메소드
    VOID        LoadFlash(RwInt32 nWidth, RwInt32 nHeight);///< 플래시 리소스들을 로드한다.
    VOID        OnSlidingIn();                             ///< 슬라이딩인 연출을 시작한다.
    VOID        Opening();                                 ///< Opening 연출을 한다
    VOID        Closing();                                 ///< Closing 연출을 한다.
    VOID        UpdateShake(RwReal fElapsed);              ///< Shake 연출을 업데이트 한다.
    VOID        UpdateSliding(RwReal fElapsed);            ///< 슬라이딩 연출을 업데이트 한다.
    VOID        CreateNPCModel();                          ///< NPC 모델을 생성한다.
    VOID        CreateDialogBalloon();                     ///< 말풍선을 생성한다.
    VOID        SetDialogBalloon();                        ///< 말풍선을 설정한다.
    VOID        AdjustDialogBalloon();                     ///< 말풍선의 위치를 설정한다.
    VOID        CloseDialogBalloon();                      ///< 말풍선을 끈다.

protected:
    // 컨트롤    
    gui::CPanel*            m_pPnlBack;
    gui::CFlash*            m_pFlashFade;                   ///< Fade 연출 플래시
    gui::CFlash*            m_pFlashOpening;                ///< 열리는 연출 플래시
    gui::CFlash*            m_pFlashClosing;                ///< 닫히는 연출 플래시
    gui::CFlash*            m_pFlashNoraml;                 ///< 평상시 전경 플래시
    gui::CFlash*            m_pFlashWarn;                   ///< 경고시 전경 플래시
    gui::CFlash*            m_pFlashBalloon;                ///< 현재 말풍선 플래시
    std::map<RwInt32, gui::CFlash*> m_mapFlashBalloon;      ///< 말풍선 플래시 맵
    gui::CStaticBox*        m_pSttBalloon;                  ///< 말풍선 텍스트

    // 슬롯
    gui::CSlot  m_slotPaint;
    gui::CSlot  m_slotOpeningMovieEnd;
    gui::CSlot  m_slotClosingMovieEnd;
    gui::CSlot  m_slotFadeMovieEnd;
    gui::CSlot  m_slotMove;

    // 자체 멤버 변수
    ETeleCastState          m_eTeleCastState;               ///< 현재 연출 상태 플래그
    SNtlEventTeleCastBegNfy m_eventInfo;                    ///< 이벤트 정보를 담고 있는 구조체
    DWORD                   m_dwDisplayTime;                ///< 표시 시간
    RwInt32                 m_nPrevBalloonType;             ///< 이전 말풍선 타입
    SUIConfigTeleCast*      m_pUIConfigTeleCast;            ///< Config 설정 구조체의 포인터        

    // NPC 렌더 관련
    CNtlPLCameraRenderTexture m_NPCCamera;                  ///< NPC를 렌더링할 카메라    
    CNtlPLCharacter*          m_pNPCModel;                  ///< NPC 모델 객체
    CSurfaceGui               m_surNPC;                     ///< NPC를 렌더링할 서페이스
    gui::CTexture             m_NPCTexture;                 ///< NPC를 렌더링할 텍스쳐 객체
    RwUInt32                  m_uiPrevNPCIdx;               ///< NPC 캐싱을 위해서 저장하고 있는 NPC 인덱스

    // Shake 및 슬라이딩 관련
    RwReal                    m_fFreqTime;                  ///< 진동수를 계산하기 위한 타임 변수
    RwBool                    m_bShakeLeft;                 ///< 진동 방향을 결정 하기 위한 변수        
    CNtlPLAccelController     m_AccelController;            ///< 슬라이등 연출을 위한 가속도 컨트롤러
};
