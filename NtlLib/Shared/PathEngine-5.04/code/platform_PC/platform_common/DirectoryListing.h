//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON__DIRECTORY_LISTING_INCLUDED
#define PLATFORM_COMMON__DIRECTORY_LISTING_INCLUDED

#include "common/STL/string.h"
#include "common/interface/Assert.h"
#include <windows.h>

inline void
GetFilesInDirectory(const char* path, std::vector<std::string>& result)
{
    std::string pathAndPattern(path);
    if(!pathAndPattern.empty() && *pathAndPattern.rbegin() != '\\')
    {
        pathAndPattern.append("\\");
    }
    pathAndPattern.append("*.*");
    result.clear();
    WIN32_FIND_DATA data; 
    HANDLE handle;
    handle = FindFirstFile(pathAndPattern.c_str(), &data); 
    assertD(handle != INVALID_HANDLE_VALUE);
    do
    { 
        if(data.cFileName[0] == '.')
        {
            if(data.cFileName[1] == 0 || (data.cFileName[1] == '.' && data.cFileName[2] == 0))
            {
                continue;
            }
        }
        if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            continue;
        }
        result.push_back(data.cFileName);
    }
    while(FindNextFile(handle, &data));
    assertD(GetLastError() == ERROR_NO_MORE_FILES);
}

#endif
