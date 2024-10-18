#ifndef __INC_DBOG_GAME_GMM_H__
#define __INC_DBOG_GAME_GMM_H__

class CPlayer;
class CNtlTokenizerW;
struct sUG_SERVER_COMMAND;

#include "NtlAdmin.h"



#define ACMD(name)  void (name)(CPlayer* pPlayer, CNtlTokenizerW* pToken, int iLine)


struct command_info
{
	const WCHAR* command;
	void(*command_pointer) (CPlayer* pPlayer, CNtlTokenizerW* pToken, int iLine);
	eADMIN_LEVEL eAdminLevel;
};
extern struct command_info cmd_info[];



extern void gm_read_command(sUG_SERVER_COMMAND* sPacket, CPlayer* pPlayer);


#endif