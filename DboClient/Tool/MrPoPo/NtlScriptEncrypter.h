#pragma once

#include <string>

// ConfigOptions.xml of the client (DboGlobal LoadConfigOptions)
typedef struct _SConfigData
{
    CString     strAddr;                ///< op ip, the auth server
    DWORD       dwPort;                 ///< op port
    CString     strBugTrapServerIP;     ///< BUGTRAP ip
    DWORD       dwBugTrapServerPort;    ///< BUGTRAP port
    CString     strBugTrapFolder;       ///< BUGTRAP folder
    CString     strLocalize;            ///< localize folder, the language of the client
}SConfigData;

/**
 * \ingroup MrPoPo
 * \brief Loads and saves the client ConfigOptions.xml
 *
 * \date 2008-07-31
 * \author agebreak
 */
class CNtlScriptEncrypter
{
public:
    CNtlScriptEncrypter(void);
    ~CNtlScriptEncrypter(void);

    static BOOL LoadConfigOption(OUT SConfigData* pConfigData, const char* szFileName);
    static BOOL SaveConfigOption(SConfigData* pConfigData, const char* szFileName);
};
