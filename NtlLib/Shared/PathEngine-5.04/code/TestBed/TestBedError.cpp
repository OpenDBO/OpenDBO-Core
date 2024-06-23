
#include "platform_common/Header.h"
#include "TestBedError.h"
#include "i_pathengine.h"
#include "common/Containers/BitVector.h"
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <string>

class cTestBedError : public iErrorHandler
{
    cBitVector ignore_flags;
    bool testIgnore(long code)
    {
        if(code >= SizeL(ignore_flags))
        {
            return false;
        }
        return ignore_flags[code];
    }
    void setIgnore(long code)
    {
        if(code >= SizeL(ignore_flags))
        {
            ignore_flags.resize(code + 1, false);
        }
        ignore_flags.setBit(code, true);
    }
    cTestBedError() {}
public:
    eAction handle(const char* error_type, const char* error_string, const char *const* attributes)
    {
        long error_code=-1;
        if(attributes && *attributes)
        {
            const char *const* ptr=attributes;
            do
            {
                if(strcmpi("errorcode",ptr[0])==0)
                    error_code=strtol(ptr[1],0,10);
                ptr+=2;
            }
            while(*ptr);
        }
        if(error_code!=-1 && testIgnore(error_code))
            return CONTINUE;

        std::string display_string(error_string);
        if(attributes && *attributes)
        {
            display_string+="\n\nAttributes:\n";
            do
            {
                display_string+="attribute \"";
                display_string+=*attributes;
                display_string+="\"=\"";
                attributes++;
                display_string+=*attributes;
                display_string+="\"\n";
                attributes++;
            }
            while(*attributes);
        }

        char title[1000];
        sprintf(title,"Error, type: %s",error_type);

        int choice;

        if(error_code!=-1 || strcmpi(error_type,"Assertion")==0)
        {
            display_string+="\nSelect 'Yes' to continue to report this error in future,\n"
                "'No' to ignore this error in future (by checking 'errorcode'),\n"
                " or 'Cancel' to trigger a debug break.";
            choice=MessageBox(0,display_string.c_str(),title,MB_YESNOCANCEL);
            if(choice==IDNO)
            {
                if(strcmpi(error_type,"Assertion")==0)
                    return CONTINUE_IGNORE_IN_FUTURE;
                setIgnore(error_code);
                return CONTINUE;
            }
        }
        else
        {
            display_string+="\n\nSelect 'Ok' to continue execution,\n"
                " or 'Cancel' to trigger a debug break.";
            choice=MessageBox(0,display_string.c_str(),title,MB_OKCANCEL);
        }

        if(choice==IDCANCEL)
        {
//..... in fact assertions currently ignore BREAK
//            if(strcmpi(error_type,"Assertion")==0)
//                return BREAK;
            DebugBreak();
            return CONTINUE;
        }
        return CONTINUE;
    }
    static cTestBedError &refInstance()
    {
        static cTestBedError the_instance;
        return the_instance;
    }
};
iErrorHandler& RefTestBedErrorHandler()
{
    return cTestBedError::refInstance();
}
