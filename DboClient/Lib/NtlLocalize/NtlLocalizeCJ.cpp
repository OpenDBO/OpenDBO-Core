#include "stdafx.h"
#include "NtlLocalizeCJ.h"
#include "NMCrypt.h"


char* StrTok(const char* str, const char sep)
{
	// strtok()와 같은 기능의 함수이지만 separator를 1개만 받고
	// 인자가 비어있는 경우도 인자로 인정함

	static const int TOK_BUFSIZE = 1024*40;

	static char* pnow = NULL;
	static char buf[TOK_BUFSIZE];
	static char token[TOK_BUFSIZE];

	if(str) 
	{
		int slen = (int)strlen(str);
		strcpy_s(buf, TOK_BUFSIZE, str);
		pnow = buf;
	}

	if(!pnow || *pnow == 0)
		return NULL;

	int tlen = 0;
	char* porg = pnow;
	while(*pnow != 0)
	{
		if(*pnow == sep) {
			++pnow;
			break;
		}

		++tlen;
		++pnow;
	}

	if(tlen > 0) {
		strncpy_s(token, TOK_BUFSIZE, porg, tlen);
		token[tlen] = 0;
	}
	else {
		token[0] = 0;
	}

	return token;
}

bool DetectLocalConfig(SLocalConfig *pConfig, const char *pKey)
{
	//
	// # key는 명령행(Command Line)인자로 넘어오는 값으로 클립보드에 저장된 
	//   게임 실행인자를 복호화하기 위한 키값이다
	//
	// # 클립보드 인자 전달 규칙(각각의 인자는 컴마(,)로 구분되며 인자 사이의 공백은 없다)
	//   전달 인자의 순서 : ServerIP,Port,AuthCookie,DataCookie,CpCookie,SpareParam (인자는 암호화되어 있다)
	//   - 넷마블 게임에서는 CpCookie를 전달하지 않는다
	//   - CP 게임에서는 AuthCookie, DataCookie를 전달하지 않는다(몇몇 CP게임은 예외)
	//   - Extra 인자는 생략되기도 한다
	//

	char buf[1024*20] = {0,};	// 버퍼는 충분히 크게 잡는다

	// 클립보드에서 인자 데이터를 읽어온다(한번 얻어오면 삭제됨)
	if(!GetNMClipData(buf, sizeof(buf)-1, pKey, true))
		return false;

	if(strlen(buf)==0)
		return false;

	char arg[6][1024*2] = {0,};

	int argcnt = 0;
	char* token = StrTok(buf, ',');
	while(token && argcnt < 6)
	{
		strcpy(arg[argcnt], token);
		argcnt++;
		token = StrTok(NULL, ',');
	}

	// 인자가 최소한 5개는 되어야 한다
	if(argcnt < 5)
		return false;

	// IP주소가 올바른지 검사
	if(strlen(arg[0]) > 16)
		return false;

	// CP 게임은 아래부분을 고쳐 쓸것
	strcpy_s(pConfig->chAddr, MAX_LOCAL_ADDR, arg[0]);  // 서버 IP 
	pConfig->uiPort  = (unsigned int)::atoi(arg[1]);	// 서버 Port

	// CP쿠키 값 저장...
	strcpy(pConfig->uCP.chCJData, arg[4]);

//	g_AuthCookie = arg[2];					// 인증 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
//	g_DataCookie = arg[3];					// 데이터 쿠키	- 버퍼 크기를1024*2 정도로 잡아야 한다
//	g_CpCookie   = arg[4];					// CP쿠키		- 버퍼 크기를1024*2 정도로 잡아야 한다
//	g_SpareParam = arg[5];					// 여분의 인자(CP게임등에서 사용)
	
	return true;
}