#ifndef _DBO_TSEMCOLRGN_H_
#define _DBO_TSEMCOLRGN_H_


/** 
	Event mapper collision region
	특정 영역에서 발동할 수 있는 Trigger id를 외부에 제공할 책임을 가진다
*/


class CDboTSEMColRgn : public CNtlTSEvtMapper
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::map< std::string, vecdef_TID_LIST > mapdef_RGN_MAPPER;

// Member variables
protected:
	mapdef_RGN_MAPPER						m_defRgnMapper;

// Constructions and Destructions
public:
	CDboTSEMColRgn( void );
	virtual ~CDboTSEMColRgn( void );

// Methods
public:
	virtual const vecdef_TID_LIST*			FindTSList( unsigned int uiId, const std::string& strKey ) const;

	virtual void							Clear( void );

	virtual bool							Load( const std::string& strFileName );
	virtual bool							Load( const char* pData, int nDataSize );

	virtual	bool							Save( const std::string& strFileName );

	virtual bool							AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter );
	virtual bool							DelBuildData( const std::string& strKey );
};

#endif