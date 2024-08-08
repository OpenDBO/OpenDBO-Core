//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "GetBufferFromResource.h"

bool
GetBufferFromResource(HMODULE module, const char* type, unsigned long id, std::vector<char>& result)
{
    HRSRC handle = FindResource(module, MAKEINTRESOURCE(id), type);
    if(handle == NULL)
    {
        return false;
    }

    DWORD length = SizeofResource(module, handle);
    result.resize(length);

    HGLOBAL global = LoadResource(module, handle);
    void* ptr = LockResource(global);

    memcpy(&result.front(), ptr, length);
    return true;
}
