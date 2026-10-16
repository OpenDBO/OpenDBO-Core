////////////////////////////////////////////////////////////////////////////////
// File: ItemUpgradeCardGui.h
// Desc: Card selection window shown while upgrading an item (retail ItemUpgradeSubExGui).
//		 Three cards are dealt when the upgrade can fail, a single one when it
//		 is guaranteed. The result is decided by the server, the cards are cosmetic.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEM_UPGRADE_CARD_GUI_H__
#define __ITEM_UPGRADE_CARD_GUI_H__

#include "ceventhandler.h"
#include "NtlPLGui.h"

class CItemUpgradeGui;
class CBackboardGui;

class CItemUpgradeCardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	// card button index == the result its disabled surface shows (see ItemUpgradeSubEx.frm)
	enum eCARD { CARD_SUCCESS, CARD_FAIL, CARD_BROKEN, NUM_CARD };
	enum eSTATE { STATE_NONE, STATE_DROP, STATE_SELECT, STATE_PACKETWAIT, STATE_RESULT, STATE_REVEALED, STATE_FINISH };

	//! Constructor & Destructor
	CItemUpgradeCardGui( const RwChar* pName );
	~CItemUpgradeCardGui(VOID);

	//! Operation
	RwBool	Create( CItemUpgradeGui* pOwner );
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	VOID	Open( RwBool bThreeCards );
	VOID	Close(VOID);
	RwBool	IsOpen(VOID) const { return m_eState != STATE_NONE; }

	VOID	SetResult( WORD wResultMessageCode, BYTE byPrevGrade, BYTE byGrade, const WCHAR* pwszItemName, RwBool bGuardStoneUsed );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	VOID	ResetCards(VOID);

	VOID	PlayFlash( gui::CFlash* pFlash, const RwChar* pFileName, RwInt32 nIndex, RwInt32 nX, RwInt32 nY );
	VOID	StopFlash( gui::CFlash* pFlash );

	VOID	OnDropEnd( RwInt32 nCardIdx );
	VOID	OnResultEnd( RwInt32 nResultCard );
	VOID	SelectCard( RwInt32 nSlot );
	VOID	ShowResultMessage(VOID);

	RwInt32	GetCardOnSlot( RwInt32 nSlot );

	//! Callback
	VOID	OnClickCard1( gui::CComponent* pComponent );
	VOID	OnClickCard2( gui::CComponent* pComponent );
	VOID	OnClickCard3( gui::CComponent* pComponent );
	VOID	OnClickCancel( gui::CComponent* pComponent );
	VOID	OnClickReSelect( gui::CComponent* pComponent );

	VOID	OnDropEnd1( gui::CComponent* pComponent );
	VOID	OnDropEnd2( gui::CComponent* pComponent );
	VOID	OnDropEnd3( gui::CComponent* pComponent );
	VOID	OnSuccessEnd( gui::CComponent* pComponent );
	VOID	OnFailEnd( gui::CComponent* pComponent );
	VOID	OnBrokenEnd( gui::CComponent* pComponent );

	gui::CSlot			m_slotClickCard[NUM_CARD];
	gui::CSlot			m_slotClickCancel;
	gui::CSlot			m_slotClickReSelect;
	gui::CSlot			m_slotDropEnd[NUM_CARD];
	gui::CSlot			m_slotResultEnd[NUM_CARD];

	//! Variable
	CItemUpgradeGui*	m_pOwner;
	CBackboardGui*		m_pBackboard;				// darkens the screen behind the cards

	gui::CButton*		m_pbtnCard[NUM_CARD];
	gui::CButton*		m_pbtnCancel;
	gui::CButton*		m_pbtnReSelect;
	gui::CPanel*		m_ppnlResult[NUM_CARD];
	gui::CPanel*		m_ppnlSelectEffect;
	gui::CFlash*		m_pflsDrop[NUM_CARD];
	gui::CFlash*		m_pflsResult[NUM_CARD];

	gui::CFlash*		m_pflsSkipped;				// flash that failed to load, ended on the next update
	RwInt32				m_nSkippedIdx;

	eSTATE				m_eState;
	RwBool				m_bThreeCards;
	RwInt32				m_nSelectedSlot;
	RwInt32				m_anCardSlot[NUM_CARD];		// slot each card button sits on

	RwInt32				m_nResultCard;
	WORD				m_wResultMessageCode;
	BYTE				m_byPrevGrade;
	BYTE				m_byGrade;
	std::wstring		m_wstrItemName;
	RwBool				m_bGuardStoneUsed;

	RwReal				m_fFinishTime;
};

#endif//__ITEM_UPGRADE_CARD_GUI_H__
