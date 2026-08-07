#include "stdafx.h"
#include "Discord.h"

CDiscord* CDiscord::m_pInstance = NULL;

CDiscord::CDiscord() {}
CDiscord::~CDiscord() {}
bool CDiscord::CreateInstance() { return true; }
void CDiscord::DestroyInstance() {}
void CDiscord::UpdateDiscordConnection() {}
void CDiscord::SetUseDiscord(bool bFlag) {}
void CDiscord::SetHideName(bool bFlag, bool bUpdatePresence) {}
void CDiscord::LoginAuthServer() {}
void CDiscord::LoginCharServer() {}
void CDiscord::LoginChannel(unsigned char channelId, char* name, unsigned char level, char* className, unsigned char classID) {}
void CDiscord::SetConnected(bool bFlag) {}
void CDiscord::UpdateLevel(unsigned char byLevel) {}
void CDiscord::InitDiscord() {}
void CDiscord::UpdatePresence() {}
void CDiscord::ClearPresence() {}
