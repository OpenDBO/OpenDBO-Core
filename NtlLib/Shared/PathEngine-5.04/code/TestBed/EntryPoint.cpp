
#include "TestBedError.h"
#include "libs/TestBed_Application/interface/ClientEntryPoint.h"
#include "libs/TestBed_Application/interface/iOSInterface.h"
#include "libs/TestBed_Interface/interface/Singletons_TestBed.h"
#include "libs/TestBed_Interface/interface/RunClient.h"
#include "common/MemoryLog.h"
#include "common/MemoryTracking.h"
#include "common/MemoryOverwriteChecking.h"
#include "common/MemoryOverwriteChecking2.h"
#include "common/interface/Error.h"

#include <stdio.h>
#include <string>
#include <windows.h>

iGraphicsInterface *gip;
controlinterface *cip;
osinterface *ip;

const char *name=0;
void ProcessArgument(const char *argument)
{
    if(strncmp(argument,"app=",4)==0)
        name=argument+4;
}

void PlatformDebugBreak()
{
    DebugBreak();
}
void Abort()
{
    _exit(1);
}

void ClientEntryPoint(osinterface *ip, int argc, char **argv)    
{        
    SetGlobalErrorHandler(&RefTestBedErrorHandler());
    ::ip=ip;
    gip=ip->getiGraphicsInterface();
    cip=ip->getcontrolinterface();

    while(argc)
    {
        ProcessArgument(*argv);
        argc--;
        argv++;
    }

    if(name==0)
        name="testapplication";
    nSingletons::init_TestBed(ip);
    RunClient(name);
    nSingletons::shutDown_TestBed();
}
