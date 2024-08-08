
////////////////////////////////////////////////////////////////////////////////
// Name: class gui::CRadarEffect
// Desc: 시간에 따라 시계 혹은 반시계방향으로 돌아가면서 표시되는 Effect
//			 
// 2006.06.15 Peessi@hitel.net   
//
// To Be Desired: 
//	1. 시간 부족으로 전체적인 구조는 생각하지 않았다. 
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_RADAR_EFFECT__
#define __GUI_RADAR_EFFECT__

START_GUI

class CRadarEffect
{
public:
	typedef VOID (CRadarEffect::*CalcProc)( FLOAT fGradient, BOOL bFirst );
	enum { NUM_TRIANGLE = 8 };

//! Constructor & Destructor
	CRadarEffect(VOID);
	~CRadarEffect(VOID);

//! Attribute
	BOOL		IsWork(VOID) { return m_bShow; }

//! Opration
	VOID		SetPosition( INT nPosX, INT nPosY );
	VOID		SetSurface( const CSurface& surface );

	// bDirClock : TRUE 시계방향으로 회전. bFill : TRUE -> 진행에 따라 채워짐.
	BOOL		StartProc( FLOAT fEffectTime, RwBool bDirClock = TRUE, RwBool bFill = FALSE ); 
	VOID		EndProc(VOID);

	VOID		Update( FLOAT fCurrentTime );
	VOID		Render(VOID);

	INT			GetWidth() { return m_nWidth; }
	INT			GetHeight() { return m_nHeight; }

	VOID		SetClippingMode( BOOL bClipping );
	VOID		SetClippingRect( CRectangle& rtRect );

private:
//! Implementation
	VOID		CalcProc0( FLOAT fGradient, BOOL bFirst );
	VOID		CalcProc1( FLOAT fGradient, BOOL bFirst );
	VOID		CalcProc2( FLOAT fGradient, BOOL bFirst );
	VOID		CalcProc3( FLOAT fGradient, BOOL bFirst );
	VOID		CalcProc4( FLOAT fGradient, BOOL bFirst );
	VOID		CalcProc5( FLOAT fGradient, BOOL bFirst );
	VOID		CalcProc6( FLOAT fGradient, BOOL bFirst );
	VOID		CalcProc7( FLOAT fGradient, BOOL bFirst );
	
	FLOAT		GetRelativeValue( FLOAT fValue, FLOAT fComparison );

	VOID		SetAttribute( RwBool bDirClock, RwBool bFill );

//! Viriables
	TriSnapShot m_triOriginal[NUM_TRIANGLE];
	TriSnapShot	m_triSnapShot[NUM_TRIANGLE];
	CTexture*	m_pTexture;

	CalcProc	m_fpCalcProc[NUM_TRIANGLE];

	FLOAT		m_fUpdateElapsedTime;
	FLOAT		m_fEffectTime;
	INT			m_nState;

	INT			m_nWidth;
	INT			m_nHeight;
	FLOAT		m_fUVWidth;
	FLOAT		m_fUVHeight;

	CRectangle	m_rtClipping;
	RwBool		m_bClipping;

	BOOL		m_bShow;
	BOOL		m_bRotDirClock;		// 회전방향.
	BOOL		m_bClockDirFill;	// 시계방향 기준으로 TRUE : 텍스쳐가 펼쳐진다. FALSE : 텍스쳐가 사라진다.	
};

END_GUI

#endif//__GUI_RADAR_EFFECT__
