#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"

/**
 * \ingroup Client
 * \brief 넷마블 PC방 로고를 표시하는 GUI 클래스
 * 로비와 게임에서 각각 따로 생성된다. (GUI 그룹이 다르다)
 * 
 * \date 2009-05-18
 * \author agebreak
 */
class CNetMarblePCBangGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
    enum ENetMarblePCBangPos
    {
        E_POSY = 4,
        E_WIDTH = 116,
        E_LOBBY = 1024 - 889,
        E_GAME = 1024 - 720,        
    };

public:
    CNetMarblePCBangGui(void);
    CNetMarblePCBangGui(const RwChar* pName);
    virtual ~CNetMarblePCBangGui(void);

    VOID		Init();
    RwBool		Create();
    VOID		Destroy();
    RwInt32		SwitchDialog(bool bOpen);
    VOID		HandleEvents(RWS::CMsg &pMsg);

protected:
    VOID			LocateComponent(RwInt32 IWidth, RwInt32 IHeight);

};
