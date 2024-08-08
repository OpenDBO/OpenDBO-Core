//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include <vector>
#include <windows.h>

bool GetBufferFromResource(HMODULE module, const char* type, unsigned long id, std::vector<char>& result);
