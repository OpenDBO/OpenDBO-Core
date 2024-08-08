//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "DecodeRecord.h"
#include "libs/ParseXML/interface/StandardXMLOutputStream.h"
#include "platform_common/FileOutputStream.h"
#include "platform_common/LoadSaveBinary.h"
#include "platform_common/DirectErrorHandling.h"
#include "common/interface/Error.h"
#include "common/STL/sstream.h"
#include <windows.h>

void PlatformDebugBreak()
{
    DebugBreak();
}
void Abort()
{
    _exit(1);
}

int
main(int argc, char* argv[])
{
    std::vector<char> buffer;
    LoadBinary("queryLog.bin", buffer);
    if(buffer.empty())
    {
        Error("NonFatal", "Binary query log ('queryLog.bin') not found in current directory, or empty. Aborting.");
        return 1;
    }
    cFileOutputStream os("queryLog.xml");
    cStandardXMLOutputStream xos(os);
    xos.openElement("queryLog");
    const char* recordStart = &buffer.front();
    unsigned long bufferRemaining = static_cast<unsigned long>(buffer.size());
    while(bufferRemaining)
    {
        bool decodedOK = DecodeRecord(recordStart, bufferRemaining, xos);
        if(!decodedOK)
        {
            xos.openElement("decodingFailedForPreviousEntry");
        }
    }
    xos.closeElement("queryLog");
	return 0;
}
