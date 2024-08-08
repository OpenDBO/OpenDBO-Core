#ifndef _DBO_TSQCTRL_H_
#define _DBO_TSQCTRL_H_


#include "DboTSTCtrl.h"


/**
	Quest controller
	퀘스트에 대한 서버, 클라이언트 공유 데이터 및 조작 루틴을 처리할 책임이 있음
*/


class CDboTSQCtrl : public CDboTSTCtrl
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	struct sQUEST_STOC_EVT_DATA
	{
		NTL_TS_TC_ID					tcId;
		NTL_TS_TA_ID					taId;

		eSTOC_EVT_DATA_TYPE				eSToCEvtDataType;
		uSTOC_EVT_DATA					uSToCEvtData;

		eSTOC_EVT_COND_DATA_TYPE		eSToCEvtCondDataType;
		uSTOC_EVT_COND_DATA				uSToCEvtCondData;

		unsigned int					uiSToCEvtSubCondDataType;	// eSTOC_EVT_SUB_COND_DATA_FLAG 이 Bit flag로 등록됨
		sSTOC_EVT_SUB_COND_DATA			sSToCEvtSubCondData;

		void Init( void )
		{
			tcId = NTL_TS_TC_ID_INVALID;
			taId = NTL_TS_TA_ID_INVALID;

			eSToCEvtDataType = eSTOC_EVT_DATA_TYPE_INVALID;
			memset( &uSToCEvtData, 0xffffffff, sizeof( uSToCEvtData ) );

			eSToCEvtCondDataType = eSTOC_EVT_COND_DATA_TYPE_INVALID;
			memset( &uSToCEvtCondData, 0xffffffff, sizeof( uSToCEvtCondData ) );

			uiSToCEvtSubCondDataType = eSTOC_EVT_SUB_COND_DATA_FLAG_INVALID;
			memset( &sSToCEvtSubCondData, 0, sizeof( sSToCEvtSubCondData ) );
		}
	};

// Member variables
protected:
	CNtlTSCont*							m_pSavedCurTSP;			// 저장된 현재 매인 그룹의 TSP
	CNtlTSCont*							m_pSavedPreTSP;			// 저장된 이전 매인 그룹의 TSP
	sQUEST_STOC_EVT_DATA				m_sSToCEvtData;			// 서버 이벤트 정보
	NTL_TS_TG_ID						m_tgExcCGroup;			// 실행할 클라이언트 그룹

	NTL_TS_TC_ID						m_tcQuestInfo;			// 퀘스트 정보 컨테이너 아이디
	NTL_TS_TA_ID						m_taQuestInfo;			// 퀘스트 정보 액션 아이디

// Constructions
public:
	CDboTSQCtrl( void );
	virtual ~CDboTSQCtrl( void );

// Methods
public:
	// 저장된 TSP
	CNtlTSCont*							GetSavedCurTSP( void ) const;
	CNtlTSCont*							GetSavedPreTSP( void ) const;

	// 서버 이벤트
	NTL_TS_TC_ID						GetServerEvtContainerID( void ) const;
	NTL_TS_TA_ID						GetServerEvtActionID( void ) const;

	eSTOC_EVT_DATA_TYPE					GetServerEvtDataType( void ) const;
	uSTOC_EVT_DATA&						GetServerEvtData( void );

	eSTOC_EVT_COND_DATA_TYPE			GetServerEvtCondDataType( void ) const;
	uSTOC_EVT_COND_DATA&				GetServerEvtCondData( void );

	unsigned int						GetServerEvtSubCondDataType( void ) const;
	sSTOC_EVT_SUB_COND_DATA&			GetServerEvtSubCondData( void );

	void								GetSToCEvtDBData( sSTOC_EVT_DB_DATA& sSvrEvtData );
	void								SetSToCEvtDBData( const sSTOC_EVT_DB_DATA& sSvrEvtData, bool bIgnoreUserDataField );

	// 실행할 클라이언트 그룹
	NTL_TS_TG_ID						GetExcCGroup( void ) const;
	void								SetExcCGroup( NTL_TS_TG_ID tgId );

	// Quest info
	void								GetQuestInfo( NTL_TS_TC_ID& tcID, NTL_TS_TA_ID& taID );
	void								SetQuestInfo( NTL_TS_TC_ID tcID, NTL_TS_TA_ID taID );

	// 퀘스트 컨트롤러 정보 설정
	virtual void						LoadQuestProgressInfo_V0( const sPROGRESS_QUEST_INFO::uDATA& uData );
	void								SaveQuestProgressInfo_V0( sPROGRESS_QUEST_INFO::uDATA& uData );

// Implementations
protected:
	// 퀘스트 단계 진행 처리 함수
	virtual void						MoveTSP( CNtlTSCont* pCurCont, CNtlTSCont* pNextCont, bool bSave );
};


inline CNtlTSCont* CDboTSQCtrl::GetSavedCurTSP( void ) const
{
	return m_pSavedCurTSP;
}

inline CNtlTSCont* CDboTSQCtrl::GetSavedPreTSP( void ) const
{
	return m_pSavedPreTSP;
}

inline NTL_TS_TC_ID CDboTSQCtrl::GetServerEvtContainerID( void ) const
{
	return m_sSToCEvtData.tcId;
}

inline NTL_TS_TA_ID CDboTSQCtrl::GetServerEvtActionID( void ) const
{
	return m_sSToCEvtData.taId;
}

inline eSTOC_EVT_DATA_TYPE CDboTSQCtrl::GetServerEvtDataType( void ) const
{
	if ( NTL_TS_TC_ID_INVALID == m_sSToCEvtData.tcId ||
		 NTL_TS_TA_ID_INVALID == m_sSToCEvtData.taId )
	{
		return eSTOC_EVT_DATA_TYPE_INVALID;
	}

	return m_sSToCEvtData.eSToCEvtDataType;
}

inline uSTOC_EVT_DATA& CDboTSQCtrl::GetServerEvtData( void )
{
	return m_sSToCEvtData.uSToCEvtData;
}

inline eSTOC_EVT_COND_DATA_TYPE CDboTSQCtrl::GetServerEvtCondDataType( void ) const
{
	return m_sSToCEvtData.eSToCEvtCondDataType;
}

inline uSTOC_EVT_COND_DATA& CDboTSQCtrl::GetServerEvtCondData( void )
{
	return m_sSToCEvtData.uSToCEvtCondData;
}

inline unsigned int CDboTSQCtrl::GetServerEvtSubCondDataType( void ) const
{
	return m_sSToCEvtData.uiSToCEvtSubCondDataType;
}

inline sSTOC_EVT_SUB_COND_DATA& CDboTSQCtrl::GetServerEvtSubCondData( void )
{
	return m_sSToCEvtData.sSToCEvtSubCondData;
}

inline NTL_TS_TC_ID CDboTSQCtrl::GetExcCGroup( void ) const
{
	return m_tgExcCGroup;
}

inline void CDboTSQCtrl::GetQuestInfo( NTL_TS_TC_ID& tcID, NTL_TS_TA_ID& taID )
{
	tcID = m_tcQuestInfo;
	taID = m_taQuestInfo;
}


#endif