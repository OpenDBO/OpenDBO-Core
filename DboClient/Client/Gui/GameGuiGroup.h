/*****************************************************************************
 *
 * File			: GameGuiGroup.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 20	
 * Abstract		: DBO Game stage gui group.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __GAME_GUI_GROUP_H__
#define __GAME_GUI_GROUP_H__

#include "ceventhandler.h"
#include "gui_precomp.h"

class CHpGui; 
class CChatGui;
class CInventoryGui;
class CQuickSlotGui;
class CExpGui;
class CMainMenuGroup;
class CESCMenuGui;
class CMinimapGui;
class CTargetHpGui;
class CNpcHelpGui;
class CMsgBoxGui;
class CStatusWindowGui;
class CBagSlotGui;
//class CSkillWindowGui;
class CSkillCustomizeGui;
class CSkillMasterGui;
class CPopupMenuGui;
class CCalcPopupGui;
class CTradeCart;
class CNPCShop;
class CScouterSlotGui;
class CScouterGui;
class CScouterBodyGui;
class CScouterMeasureGui;
class CScouterMenuGui;
class CScouterBackgroundGui;
class CScouterPCInfoViewerGui;
class CSubGaugeGui;
class CCommunityGui;
class CQuestGui;
class CItemUpgradeGui;
class CWarehouseBarGui;
class CWarehouseGui;
class CWarehouseCommonGui;
class CCautionNotifyGui;
class CTerritoryNotifyGui;
class CPublicNotifyGui;
class CActionNotifyGui;
class CGroupNotifyGui;
class CWorldMapGui;
class COptionWindowGui;
class CHelpWindowGui;
class CDropItemInfoGui;
class CSideIconGui;
class CPrivateShopGui;
class CPrivateShopCartGui;
class CPrivateShopTradeGui;
class CPrivateShopTradeBoxGui;
class CRBBoardGui;
//class CRBChallengeGui;
class CTMQBoardGui;
class CMailSystemGui;
class CMailSystemReadGui;
class CCinematicGui;
class CBackboardGui;
class CPortalGui;
class CObjectClickGui;
class CTMQTeleCastGui;
class CFlashNotifyGui;
class CFriendListGui;
class CInterfaceShakeGui;
class CRankBoardGui;
class CCommuTargetGui;
class CTimeNotifyGui;
class CSideDialogControlGui;
class CTutorialDialogFocus;
class CTutorialExampleGui;
class CTutorialMessageGui;
class CTutorialConfirmGui;
class CTBGambleResultGui;
class CChannelChangeGui;
class CGroupChatManager;
class CGMChatGui;
class CPetitionGui;
class CPetitionSatisfaction;
class CBroadCastGui;
class CPartyMenu;
class CNameNotifyGui;
class CSkillAbilitySetGui;
class CSkillAbilityUseGui;
class CSkillHTBRpResult;
class CSkillHtbRpUse;
class CVehicleGui;
class CHoipoiMixCapsuleGui;
class CDeveloperPreviewGui;
class CDojoInfoGui;
class CDojoAccountGui;
class CDogiGui;
class CDojoUpradeGui;
class CScrambleStatusGui;
class CScrambleNotifyGui;
class CNetMarblePCBangGui;
class CNetPyShopGui;
class CNetPyShopCartGui;
class CDeliberationRankContainer;
class CCommercialExtendGui;
class CYardratStoreGui;
class CGMTResetGui;

class CGMGuiGroup;

class CGameGuiGroup : public RWS::CEventHandler
{
private:

	CHpGui*						m_pHp;
	CChatGui*					m_pChat;
	CQuickSlotGui*				m_pQuickSlot;
	CExpGui*					m_pExp;
	CMainMenuGroup*				m_pMainMenu;
	CESCMenuGui*				m_pESCMenu;
	CMinimapGui*				m_pMinimap;
	CTargetHpGui*				m_pTargetHp;
	CNpcHelpGui*				m_pNpcHelp;
	CStatusWindowGui*			m_pStatusWindow;
	CBagSlotGui*				m_pBagSlot;
	CSkillCustomizeGui*			m_pSkillWindow;
	//CSkillWindowGui*			m_pSkillWindow;
	CSkillMasterGui*			m_pSkillMasterWindow;
	CPopupMenuGui*				m_pPopupMenu;
	CCalcPopupGui*				m_pCalcPopup;
	CTradeCart*					m_pTradeCart;
	CNPCShop*					m_pNPCShop;
	CScouterSlotGui*			m_pScouterSlot;
	CScouterGui*				m_pScouter;
	CScouterBodyGui*			m_pScouterBody;
	CScouterMeasureGui*			m_pScouterMeasure;
	CScouterMenuGui*			m_pScouterMenu;
	CScouterBackgroundGui*		m_pScouterBackground;
	CScouterPCInfoViewerGui*	m_pScouterPCInfoViewer;
	CSubGaugeGui*				m_pSubGauge;
	CCommunityGui*				m_pCommunity;
	CQuestGui*					m_pQuest;
	CItemUpgradeGui*			m_pItemUpgrade;
	CWarehouseBarGui*			m_pWarehouseBar;
	CWarehouseGui*				m_pWarehouse_1;
	CWarehouseGui*				m_pWarehouse_2;
	CWarehouseGui*				m_pWarehouse_3;
	CWarehouseCommonGui*		m_pWarehouseCommon;
	CCautionNotifyGui*			m_pCautionNotify;
	CTerritoryNotifyGui*		m_pTerritoryNotify;
	CPublicNotifyGui*			m_pPublicNotify;
	CActionNotifyGui*			m_pActionNotify;
	CGroupNotifyGui*			m_pGroupNotify;
	CWorldMapGui*				m_pWorldMap;
	COptionWindowGui*			m_pOptionWindowGui;
	CHelpWindowGui*				m_pHelpWindowGui;
	CDropItemInfoGui*			m_pDropItemInfoGui;
	CSideIconGui*				m_pSideIconGui;
	CPrivateShopGui*			m_pPrivateShopGui;
	CPrivateShopCartGui*		m_pPrivateShopCartGui;
	CPrivateShopTradeGui*		m_pPrivateShopTradeGui;
	CPrivateShopTradeBoxGui*	m_pPrivateShopTradeBoxGui;	
	CRBBoardGui*				m_pRBBoardGui;
	/*CRBChallengeGui*			m_pRBChallengeGui;*/
	CTMQBoardGui*				m_pTMQBoard;
	CMailSystemGui*				m_pMailSystemGui;
	CMailSystemReadGui*			m_pMailSystemReadGui;
	CCinematicGui*				m_pCinematicGui;
	CBackboardGui*				m_pBackboardGui;
    CPortalGui*                 m_pPortalGui;
	CObjectClickGui*			m_pObjectClickGui;
    CTMQTeleCastGui*            m_pTMQTeleCastGui;
	CFlashNotifyGui*			m_pFlashNotify;
    CFriendListGui*             m_pFriendListGui;
	CInterfaceShakeGui*			m_pInterfaceShakeGui;
	CRankBoardGui*				m_pRankBoardGui;
    CCommuTargetGui*            m_pCommuTargetGui;
	CTimeNotifyGui*				m_pTimeNotifyGui;
	CSideDialogControlGui*		m_pSideDialogControl;
	CTutorialDialogFocus*		m_pTutorialDialogFocus;
	CTutorialExampleGui*		m_pTutorialExample;
	CTutorialMessageGui*		m_pTutorialMessage;
	CTutorialConfirmGui*		m_pToturialConfirm;
	CTBGambleResultGui*			m_pTBGambleResult;
	CChannelChangeGui*			m_pChannelChange;
	CGroupChatManager*			m_pGroupChatManager;
	CGMChatGui*					m_pGMChatting;
	CPetitionGui*				m_pPetition;
	CPetitionSatisfaction*		m_pPetitionSatisfaction;
	CBroadCastGui*				m_pBroadCastGui;
	CPartyMenu*					m_pPartyMenu;	
	CNameNotifyGui*				m_pNameNotify;
	CSkillAbilitySetGui*		m_pSkillAbilitySetGui;
	CSkillAbilityUseGui*		m_pSkillAbilityUseGui;
	CSkillHTBRpResult*			m_pHTBRpResult;
	CSkillHtbRpUse*				m_pHTBRpUse;
	CVehicleGui*				m_pVehicle;
	CHoipoiMixCapsuleGui*		m_pHoipoiMixGui;
	CDeveloperPreviewGui*		m_pDeveloperPreviewGui;
	CDojoInfoGui*				m_pDojoInfoGui;
	CDojoAccountGui*			m_pDojoAccountGui;
	CDogiGui*					m_pDogiGui;
	CDojoUpradeGui*				m_pDojoUpgrade;
	CScrambleStatusGui*			m_pScrambleStatusGui;
	CScrambleNotifyGui*			m_pScrambleNotify;
    CNetMarblePCBangGui*        m_pNetMablePCBangGui;
	CNetPyShopGui*				m_pNetPyShopGui;
	CNetPyShopCartGui*			m_pNetPyShopCartGui;
	CDeliberationRankContainer*		m_pDeliverationRankContainer;
	CCommercialExtendGui*		m_pCommercialExtendGui;
	CYardratStoreGui*			m_pYardratStoreGui;
	CGMTResetGui*				m_pGMTResetGui;

	CGMGuiGroup*				m_pGMGuiGroup;

	gui::CSlot m_slotPaint;
		
public:

	CGameGuiGroup();
	~CGameGuiGroup();

	RwBool		Create(void);
	void		Destroy(void);

	void		OnPaint(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

protected:
	void		LoadCommonScript();
	void		UnloadCommonScript();
};


#endif