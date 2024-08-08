/*****************************************************************************
 *
 * File			: DboEvenetGenerator.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 27	
 * Abstract		: DBO event generator
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_EVENT_GENERATOR_H__
#define __DBO_EVENT_GENERATOR_H__

// shared
#include "NtlBattle.h"

// dbo
#include "DboEvent.h"

class CDboEventGenerator
{
public:

	static void LogInServerConnect(RwUInt8 byConnectType);
	static void LobbyServerConnect(RwUInt8 byConnectType);
	static void GameServerConnect(RwUInt8 byConnectType);
	
	static void StageCreate(const RwChar *pStageName);
	static void StageDelete(const RwChar *pStageName);

	static void Debug_for_Developer(RwUInt32 uiType);
	static void NetConnectBoxShow(const WCHAR *pString, RwInt32 eWorkId = MBW_NONE, RwUInt32 flagStyle = MBTF_NONE, RwReal fShowTime = 0.0f, sMsgBoxData* pData = NULL, std::list<sMsgBoxCustomBtn>* plistCustomBtn = NULL, RwBool bAcceptDuplicate = TRUE, RwInt32 eStringID = -1, RwBool bUpdate = FALSE );
	static void LobbyPacketHelper(RwUInt32 uiCord);
	static void LogInStageStateEnter(RwUInt8 byState);
	static void LogInStageStateExit(RwUInt8 byState);
	static void LogInStageTimeOut(RwUInt8 byState);

	static void CharStageStateEnter(RwUInt8 byState);
	static void CharStageStateExit(RwUInt8 byState);
	static void CharMaking(RwBool bSetDefaultCamera, RwUInt8 byRace, RwUInt8	byClass, RwUInt8 byHair, RwUInt8 byHairColor, RwUInt8 byFace, RwUInt8 bySkinColor, RwUInt8 byGender);
	static void LoginEvent(RwUInt8 byMessage, RwReal fValue = 0.f, RwUInt32 uiExData = 0);
	static void LoginPostEvent(RwUInt8 byMessage, RwReal fValue = 0.f, RwUInt32 uiExData = 0);
	static void LobbyEvent(RwUInt8 byMessage, RwReal fValue = 0.f, RwUInt32 uiExData = 0);
	
	static void LoginGuiEnable(void);
	static void CharSelectGuiEnable(void);
	static void MsgBoxShow( const WCHAR *pString, RwInt32 eWorkId, RwUInt32 flagStyle, RwBool bPopup = FALSE, RwReal fShowTime = 0.0f, sMsgBoxData* pData = NULL, std::list<sMsgBoxCustomBtn>* plistCustomBtn = NULL, RwBool bAcceptDuplicate = TRUE, RwInt32 eStringID = -1, RwBool bUpdate = FALSE );
	static void MsgBoxResult( RwInt32 eResult, RwUInt32 uiWorkId, sMsgBoxData* pData);
	static void ChatGuiMode(RwBool bMode);
	static void SayMessage(RwUInt8 byMsgType, const WCHAR *pUserName, RwUInt16 wMsgLen, const WCHAR *pMessage, RwUInt32 hObject = 0xffffffff);
	static void SetTextToInputChat(WCHAR* pString);
	static void TimeFixedNotifyMessage(RwBool bActive, RwUInt32 uiStringID, const WCHAR* pString);
	static void CautionSideNotify(RwBool bActive, RwUInt32 uiStringID, const WCHAR* pString, RwReal fLifeTime);
	static void NotifyMessage( RwInt32 eType, const WCHAR* pMessage );
	static void FlashNotify(RwUInt8 byProgressType = 0xff, RwUInt8 byMessageValue = 0xff, RwUInt8 byMessageType = 0xff);
	static void FlashNotifyString(const RwChar* pcFlashFile, const RwChar* pcSecondFlashFile = NULL, RwBool bFinshNotify = TRUE);
	static void FlashFinishNotify(RwUInt8 byProgressType = 0xff, RwUInt8 byMessageValue = 0xff, RwUInt8 byMessageType = 0xff, char* pcText = NULL, char* pcText2 = NULL);
	static void GroupNotify(RwUInt8 byType, WCHAR* pwcText);
	static void NameNotify(WCHAR* pwcText);
	
	static void IconMoveClick( RwBool bPickUp );

	static void IconPopupShow( RwBool bShow, RwUInt32 uiSerial, RwInt32 nSrcPlace, RwInt32 nSrcSlotIdx = 0, RwInt32 nXPos = 0, RwInt32 nYPos = 0 );
	static void IconPopupResult( RwUInt32 uiSerial, RwInt32 nSrcPlace, RwInt32 nSrcSlotIdx, RwInt32 nWorkId );
	static void CalcPopupShow( RwBool bShow, RwUInt32 uiSerial, RwInt32 nSrcPlace, RwInt32 nXPos, RwInt32 nYPos, RwUInt32 uiMaxValue );
	static void CalcPopupResult( RwUInt32 uiSerial, RwInt32 nSrcPlace, RwUInt32 uiValue );
		
	static void PickedUpHide(RwInt32 nSrcPlace);
	
	static void ShopEvent(RwInt32 iType, RwUInt32 uiSerial, unsigned long ulPrice = 0,
							wchar_t* pcName = NULL, int iPlace = 0, int iPosition= 0 ,
							int iOverlapCount = 0);

	static void PartyCharm(RwUInt32 iType, RwInt32 iValue = 0, RwUInt32 uiValue2 = 0);
	static void PartyVictim(RwUInt32 iType, RwUInt32 uiValue = 0, RwUInt32 uiValue2 = 0, RwUInt32 uiValue3 = 0, RwUInt32 uiValue4 = 0, RwBool bByMyself = false);
	static void PartyInventory(RwUInt32 iType, RwUInt8 byPos, RwUInt32 uiSerial = INVALID_SERIAL_ID, RwUInt32 uiCount = 0 , RwUInt8 byRank = 0 ,
							   RwUInt32 uiOptionSerial1 = INVALID_TBLIDX , RwUInt32 uiOptionSerial2 = INVALID_TBLIDX , RwBool bTrue = false , RwUInt8 byGrade = 0,
							   RwUInt8 byBattleAttribute = BATTLE_ATTRIBUTE_UNKNOWN);

	static void DialogEvent(RwInt32 iType, RwInt32 iSrcPlace = 0, RwInt32 iDestPlace = 0,
							RwInt32 iUserData = 0, RwInt32 iUserData2 = 0, RwUInt32 uiUserData3 = 0xffffffff, RwUInt32 uiUserData4 = 0xffffffff);
	static void DialogPostEvent(RwInt32 iType, RwInt32 iSrcPlace = 0, RwInt32 iDestPlace = 0,
								RwInt32 iUserData = 0, RwInt32 iUserData2 = 0, RwUInt32 uiUserData3 = 0xffffffff, RwUInt32 uiUserData4 = 0xffffffff);
	
	static void UpdateRP( RwUInt32 hSerial, RwInt32 nRP, RwInt32 nMaxRP );
    static void UpdateRPStock(RwUInt32 hSerial, RwInt32 nRpStock, RwInt32 nMaxRpStock, bool bDropByTime = false);
	static void UpdateExp( RwUInt32 uiIncreasedExp, RwUInt32 uiAcquisitionExp, RwUInt32 uiBonusExp );    
	static void CharObjDelete( RwUInt32 uiSerial );

	static void ItemUpgradeResult( SDboItemUpgradeResult::RESULT eResult, RwUInt8 byBattleAttribute = 0, RwInt32 nGrade = 0 );
	static void RegistItemUpgrade( RwUInt32 hSerialID, RwUInt32 eSrcPlace, RwUInt32 uiSlotIdx );		

	static void ScouterUpgradePredict( RwUInt8 byPosNum, RwUInt8 byRetRate );
	static void ESCinNarrationMode(void);

	static void QuickSlotInfo(void);
	static void QuickSlotDelNfy( RwUInt8 bySlotIdx );

	static void NPCDialogOpen( RwUInt32 hSerialID, RwInt32 eDialog );

	static void EnterWorld();

	static void MapEvent(RwInt32 iMessage, RwUInt32 uiValue = 0);

	static void UserTrade(RwUInt8 byEventType, RwUInt32 uiTarget = 0xffffffff, RwUInt32 uiHandle = 0xffffffff, RwUInt8 byCount = 0, VOID* pData = NULL);

	static void ServerCharDialog(RwUInt32 uiSerial, RwUInt8 byDialogType, RwUInt32 uiTextTblIdx, RwBool bIsRefSpeechTable, RwUInt16 wTextLen, WCHAR* pTextData);

	static void OpenHelpWindow(RwUInt32 uiTableIndex);

	static void OpenHelpContent(RwUInt32 uiDialogType);

	static void	EnableItemIcon( RwBool bEnable, RwInt32 ePlace, RwUInt32 uiSlotIdx, RwInt32 nPlaceIdx = -1 ); // peessi: nPlaceIdx는 WareHouse, Bag에만 쓰임.
	static void CapsuleLockItem( RwBool bEnable, RwInt32 ePlace, RwUInt32 uiSlotIdx, RwInt32 nPlaceIdx = -1 );
	static void CapsuleLockItemWithoutBag( RwBool bEnable, SERIAL_HANDLE hItem );

	static void DirectMoveIcon(RwUInt32 uiSrcSerial, RwUInt32 eSrcPlace, RwUInt32 eDestPlace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiSrcStackCount);

	// 개인상점
	static void PrivateShopSelectNfy(RwUInt8 uiPrivateShopPos, RwUInt8 uiItemState);
	static void RegPrivateShopCartItem(RwUInt32 uiSerial, RwUInt32 uiPlace, RwUInt32 uiPos, RwUInt32 uiStackCount, RwUInt32 uiPrice, void* pData);
	static void RegPrivateShopItem(RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos);

	// 개인상점 흥정
	static void PrivateShopBusinessStart(RwUInt8 uiType, RwUInt32 uiShopOwner, WCHAR* pwcTargetName, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData, void* pGui);
	static void PrivateShopBusinessFluctuations(RwUInt8 uiType, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData, RwUInt32 uiStartPrice);
	static void PrivateShopBusinessCloseNfy(RwUInt32 uiShopOwner);

	static void BackboardEvent(RwUInt32 uiMessage, RwUInt8 byValue = 0, RwUInt8 byValue2 = 0, RwUInt8 byValue3 = 0);

	static void SummonPet(RwBool bCreate, RwUInt32 uiSerialID);

	static void OpenSendMail(const WCHAR* pwcTargetName);

	static void ReadingMail(RwUInt32 uiMailID, RwBool bReading);

	static void MailSlotUpdate(RwUInt8 byType, RwUInt32 uiParam1 = 0, RwUInt32 uiParam2 = 0);

	static void QuestMessage(RwUInt32 uiValue, RwUInt32 uiValue2);
	static void QuestMessage(sQUEST_INFO* pQuestInfo, RwUInt32 uiEventGenType, RwUInt32 uiID);

	// Rp Bonus
	static void RpBonusSetup(RwInt32 iXPos, RwInt32 iYPos, VOID* pData );
	static void RpBonusSelect(VOID* pData);
	static void RpBonusAuto(RwUInt8 byRpBonusType);
	
	static void TargetStatusRefresh(VOID);

	// Ranking Board
	static void RankBattleRankListRes(RwUInt16 wResultCode, RwUInt32 dwPage, RwUInt8 byCompareDay,
		RwUInt8 byRankInfoCount, RwUInt16 wRankInfo, void* sData);
	static void RankBattleRankFindCharacterRes(RwUInt16 wResultCode, RwUInt32 dwPage, RwUInt8 byCompareDay,
		WCHAR* pwsCharName, RwUInt8 byRankInfoCount, RwUInt16 wRankInfo, void* sData);
	static void RankBattleRankCompareDayRes(RwUInt16 wResultCode, RwUInt32 dwPage, RwUInt8 byCompareDay,
		RwBool bIsFullList, RwUInt8 byInfoCount, RwUInt16 wRankInfo, RwUInt16 wCompareInfo, void* sData);
	static void RankBoardDisable();

	// Looting, Getting Effect
	static void ZennyLootingEffect( RwUInt32 uiZenny );
	static void ItemCreatedEffect( RwUInt32 hSerial, RwUInt8 ucBagIdx, RwUInt8 ucSlotIdx );

	static void SideDialogEvent(RwUInt8 byMessage, RwUInt32 uiSrcSDialog, RwUInt32 uiDestSDialog, RwUInt32 uiExData = 0);

	// Ranking Board ( TMQ )
	static void TMQRecordListRes(RwUInt16 wResultCode, RwUInt32 tmqTblidx, RwUInt8 byDifficult, RwUInt8 byTeamCount, void* paData );
	static void TMQMemberListRes(RwUInt16 wResultCode, RwUInt32 tmqTblidx, RwUInt8 byDifficult, RwUInt8 byRank, void* pData );

	// Tenkaichi Budokai
	static void BudokaiNews(RwBool bShow, RwUInt8 byType = 0xFF);
	static void BudokaiPrizeWinnerName( BYTE byMatchType, WORD wWinner, WORD wSecondWinner, void* pData );

	static void BudokaiTournamentIndividualList(RwUInt16 wEntryTeam_var, RwUInt8 byMatchDataCount, RwUInt16 wMatchData_var, sVARIABLE_DATA* pData);
	static void BudokaiTournamentIndividualInfo(RwUInt16 wTeamType[2], sVARIABLE_DATA* pData);
	static void BudokaiTournamentTeamList(RwUInt16 wEntryTeam_var, RwUInt8 byMatchDataCount, RwUInt16 wMatchData_var, sVARIABLE_DATA* pData);
	static void BudokaiTournamentTeamInfo(RwUInt16 wTeamType[2], sVARIABLE_DATA* pData);

	// Link Script
	static void ScriptLinkNotify(RwUInt8 byType, RwUInt8 byAction, WCHAR* pwcValue );

	// 주사위
	static void DiceStart(RwUInt8 byWorkID, void* pExData = NULL);
	static void DiceResult(SERIAL_HANDLE hHandle, RwUInt8 byWorkID, RwUInt8 byResultNum);
	static void DiceResultShow(SERIAL_HANDLE hHandle, RwUInt8 byWorkID, RwUInt8 byResultNum);
	static void DiceRequestResulttoServer(RwUInt8 byWorkID);	

	static void ShopGambleBuyRes( RwUInt16 wResultCode, RwUInt32 hHandle, RwUInt32 hItem );
	static void GambleUseEffect( RwUInt8	ucBagIdx, RwUInt8 ucSlotIdx, RwUInt32 hCreateSerial, RwChar* pcEffectFile );

	// Mail System
	static void RegMailAttatchItem( SERIAL_HANDLE hHandle, RwUInt32 uiPlace, RwUInt32 uiIndex, RwUInt32 uiPos );

	// Petition
	static void PetitionEvent(RwUInt32 uiEventType);
	static void PetitionPostEvent(RwUInt32 uiEventType);

	// TeleportProposal
	static void TeleportProposalNfy( SDboEventTeleportProposalNfy* pNotify );
	static void TeleportConfirmRes( SDboEventTeleportConfirmRes* pResult );

	// GM Menu
	static void ObserverMenu(RwUInt8 byEventType, SERIAL_HANDLE hHandle = INVALID_SERIAL_ID, RwUInt32 uiValue = 0xffffffff, void* pExdata = NULL);
	static void ObserverMenuPost(RwUInt8 byEventType, SERIAL_HANDLE hHandle = INVALID_SERIAL_ID, RwUInt32 uiValue = 0xffffffff);

	// RPCharge
	static void RPCharge( RwBool bIncrease, RwBool bStop );

	// Item Identify Effect
	static void ItemIdentifyEffect( RwBool bHandle, SERIAL_HANDLE hHandle = INVALID_SERIAL_ID, RwUInt8 byPlace = 0xFF, RwUInt8 byPos = 0xFF );

	// Hoipoi Mix
	static void HoipoiMixItemCheck( HOBJECT hObject, TBLIDX recipeTblIdx );
	static void HoipoiMixItemMake( RwUInt16 wResultCode, RwUInt8 bySuccessResult, HOBJECT hObject, TBLIDX idxRecipe, SERIAL_HANDLE hItem );
	static void HoipoiMixItemMakeEp();
	static void HoipoiMixStartCreate( TBLIDX idxRecipeTbl );

	static void HintSideViewClosed();

	// Loading
	static void LoadingEvent(RwUInt8 byMessage);

	static void NetPyShopEvent(RwUInt8 byType, RwUInt32 uiSerial, unsigned long ulPrice = 0,
		wchar_t* pcName = NULL, int iPlace = 0, int iPosition= 0 ,
		int iOverlapCount = 0);
	// CommercialExtend command Event
	static void StartProcessBuyDurItemNPC( RwUInt32 uiItemIdx,  RwUInt32 uiNPCSerial, sSHOP_BUY_CART& ShopBuyCart );
	static void StartProcessBuyDurItemNetpy( RwUInt32 uiItemIdx, RwUInt32 uiMerchantTblidx, BYTE byMerchanSlotPos );
	static void StartProcessMoveDurItem( RwUInt32 uiItemIdx, RwUInt32 uiProductId );
	static void SetDurItemOfCommercialExtend( RwUInt32 uiItemIdx );
	static void SetDurExtendTargetItemOfCommercialExtend( void* pNtlSobItem );
	static void ChangeCommercialExtendmode( RwInt32 iMode );
	static void OpenCommercialExtendGui( RwInt32 iMode = -1, RwUInt32 uiItemIdx = 0xFFFFFFFF );
	static void NetSendBuyDurItem();

	// open BagGui, ScouterBackgroundGui, ...
	static void OpenBagGui();
	static void OpenScouterBackgroundGui();
    //////////////////////////////////////////////////////////////////////////
    // 한국 상용화 관련
    static void UpdateNetPy(NETP netPy, NETP dwAccumulationNetP, DWORD timeNextGainTime);       
    static void NetMarbleMemberShipNfy();

	static void GmtInfo( RwUInt8 byGMTResetCount, sGAME_MANIA_TIME_DATA& sCurrent, sGAME_MANIA_TIME_DATA& sNext );
	static void GmtUpdate( sGAME_MANIA_TIME& sNext );
	static void GmtChanged( sGAME_MANIA_TIME& sCurrent );
	static void GmtResetOpen( RwBool bOpen, RwInt32 nLeft, RwInt32 nTop );
};

#endif