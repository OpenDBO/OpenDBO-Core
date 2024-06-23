//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/interface/Error.h"
#include "i_pathengine.h"
#include "common/STL/sstream.h"

static iErrorHandler* g_handler=0;
void SetGlobalErrorHandler(iErrorHandler* value)
{
    g_handler=value;
}
iErrorHandler* GetGlobalErrorHandler()
{
    return g_handler;
}

const char* gAttributeEnd=0;
void Error(const char* error_type, const char* error_string, const char *const* attributes)
{
    if(!g_handler)
        Abort();
    g_handler->handle(error_type,error_string,attributes);
}

bool
HandleAssertion(const char *file, long line, const char *comment)
{
    if(!g_handler)
    {
        PlatformDebugBreak();
        Abort();
    }
    std::ostringstream oss;
    oss << file << '(' << line << "): Assert(" << comment << ").";
    iErrorHandler::eAction action = g_handler->handle("Assertion", oss.str().c_str(), 0);
    if(action == iErrorHandler::BREAK)
    {
        PlatformDebugBreak();
    }
    return action != iErrorHandler::CONTINUE_IGNORE_IN_FUTURE;
}
bool&
FullAssertsOn()
{
    static bool value = false;
    return value;
}

bool ReportInterfaceCheckFailure(const char *file, long line, const char *comment)
{
    if(!g_handler)
        Abort();
    std::ostringstream oss;
    oss << file << '(' << line << "): Interface check failed <" << comment << ">.";
    iErrorHandler::eAction action=g_handler->handle("Assertion", oss.str().c_str(), 0);
    if(action == iErrorHandler::BREAK)
    {
        PlatformDebugBreak();
    }
    return action!=iErrorHandler::CONTINUE_IGNORE_IN_FUTURE;
}

