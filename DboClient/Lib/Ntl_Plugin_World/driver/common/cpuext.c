/* 
 * Querying processor features
 *
 * Copyright (c) 2001 Criterion Software Ltd.
 */

#include <string.h>
#include <intrin.h>

#include "batypes.h"
#include "barwtyp.h"
#include "badebug.h"

#include "cpuext.h"

#ifdef WIN32
#include <windows.h>
#endif

#define CPUID_MMX   (1 << 23)
#define CPUID_SSE   (1 << 25)
#define CPUID_SSE2  (1 << 26)

#define CPUID_EXT_3DNOW (1 << 31)

static RwBool
CPUHaveCPUID(void)
{
    RwUInt32  cpuidOkay = 1;
    RWFUNCTION( RWSTRING( "CPUHaveCPUID" ) );
    RWRETURN( 0 != cpuidOkay );
}

static RwUInt32
CPUGetFeatures(void)
{
    int cpuInfo[4] = { 0 };
    RWFUNCTION(RWSTRING("CPUGetFeatures"));
    __cpuid(cpuInfo, 1);
    RWRETURN((RwUInt32)cpuInfo[3]);
}

static RwBool
CPUHaveExtFeatures(void)
{
    int cpuInfo[4] = { 0 };
    RWFUNCTION(RWSTRING("CPUHaveExtFeatures"));
    __cpuid(cpuInfo, 0x80000000);
    RWRETURN(cpuInfo[0] >= 0x80000001);
}

static RwUInt32
CPUGetExtFeatures(void)
{
    int cpuInfo[4] = { 0 };
    RWFUNCTION(RWSTRING("CPUGetExtFeatures"));
    __cpuid(cpuInfo, 0x80000001);
    RWRETURN((RwUInt32)cpuInfo[3]);
}

#if (!defined(__GNUC__))
static void
CPUGetProcessorFamilyModel(RwUInt32 *family, RwUInt32 *model)
{
    RWFUNCTION(RWSTRING("CPUGetProcessorFamilyModel"));

    if (CPUHaveCPUID())
    {
        int cpuInfo[4] = { 0 };
        __cpuid(cpuInfo, 0x80000000);
        if (cpuInfo[0] >= 0x80000001)
        {
            __cpuid(cpuInfo, 0x80000001);
        }
        else
        {
            __cpuid(cpuInfo, 1);
        }
        *family = (cpuInfo[0] >> 8) & 0xf;
        *model = (cpuInfo[0] >> 4) & 0xf;
    }

    RWRETURNVOID();
}
#endif /* (!defined(__GNUC__)) */

void
_rwCPUGetCPUName(RwChar *cpuName)
{
    RWFUNCTION(RWSTRING("_rwCPUGetCPUName"));

    cpuName[0] = 0;

    if (CPUHaveCPUID())
    {
#if (!defined(__GNUC__))
        RwBool  fakeCPUName = FALSE;
        {
            int cpuInfo[4] = { 0 };

            /* Check for support of extended functions. */
            __cpuid(cpuInfo, 0x80000000);
            if (cpuInfo[0] >= 0x80000004)
            {
                /* Query and save the CPU name string */
                __cpuid(cpuInfo, 0x80000002);
                memcpy(cpuName + 0, cpuInfo, 16);
                __cpuid(cpuInfo, 0x80000003);
                memcpy(cpuName + 16, cpuInfo, 16);
                __cpuid(cpuInfo, 0x80000004);
                memcpy(cpuName + 32, cpuInfo, 16);
                cpuName[48] = 0;

                /* remove leading spaces */
                {
                    RwChar *p = cpuName;
                    while (*p == ' ') p++;
                    if (p != cpuName)
                    {
                        memmove(cpuName, p, 49 - (p - cpuName));
                    }
                }
            }
            else
            {
                __cpuid(cpuInfo, 0);
                memcpy(cpuName, cpuInfo + 1, 4);
                memcpy(cpuName + 4, cpuInfo + 3, 4);
                memcpy(cpuName + 8, cpuInfo + 2, 4);
                cpuName[12] = 0;

                fakeCPUName = TRUE;
            }
        }

        if (fakeCPUName)
        {
            RwUInt32 family;
            RwUInt32 model;

            CPUGetProcessorFamilyModel(&family, &model);

            if (memcmp(cpuName, "GenuineIntel", 12) == 0)
            {
                switch (family)
                {
                    case 4:
                        strcpy(cpuName, "Intel 486");
                        break;

                    case 5:
                        switch (model)
                        {
                            case 4:
                            case 8:
                                strcpy(cpuName, "Intel Pentium MMX");
                                break;

                            default:
                                strcpy(cpuName, "Intel Pentium");
                                break;
                        }
                        break;

                    case 6:
                        switch (model)
                        {
                            case 0:
                            case 1:
                                strcpy(cpuName, "Intel Pentium Pro");
                                break;

                            case 3:
                            case 5:
                                strcpy(cpuName, "Intel Pentium II");
                                break;

                            case 6:
                                strcpy(cpuName, "Intel Celeron");
                                break;

                            case 7:
                            case 8:
                                strcpy(cpuName, "Intel Pentium III");
                                break;

                            case 0xa:
                                strcpy(cpuName, "Intel Pentium III Xeon");
                                break;

                            default:
                                break;
                        }
                        break;

                    case 0xf:
                        strcpy(cpuName, "Intel Pentium 4");
                        break;

                    default:
                        break;
                }
            }
            else if (memcmp(cpuName, "AuthenticAMD", 12) == 0 ||
                    memcmp(cpuName, "AMD ISBETTER", 12) == 0 )
            {
                switch (family)
                {
                    case 4:
                        strcpy(cpuName, "AMD 486");
                        break;

                    case 5:
                        switch (model)
                        {
                            case 0:
                            case 1:
                            case 2:
                            case 3:
                                strcpy(cpuName, "AMD K5");
                                break;

                            case 4:
                            case 5:
                            case 6:
                            case 7:
                                strcpy(cpuName, "AMD K6");
                                break;

                            case 8:
                                strcpy(cpuName, "AMD K6 2");
                                break;

                            case 9:
                            case 0xd:
                                strcpy(cpuName, "AMD K6 III");
                                break;

                            default:
                                break;
                        }
                        break;

                    case 6: /* K7 Athlon */
                        switch (model)
                        {
                            case 1:
                            case 2:
                                strcpy(cpuName, "AMD Athlon");
                                break;

                            case 3:
                                strcpy(cpuName, "AMD Duron");
                                break;

                            case 4:
                                strcpy(cpuName, "AMD Athlon");
                                break;

                            default:
                                strcpy(cpuName, "AMD Athlon");
                                break;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
#endif /* (!defined(__GNUC__)) */
    }

    RWRETURNVOID();
}

RwBool
_rwCPUHaveMMX(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHaveMMX"));

    if (CPUHaveCPUID())
    {
        result = CPUGetFeatures();

        result = ((result & CPUID_MMX) != 0);
    }

    RWRETURN(result);
}

RwBool
_rwCPUHaveSSE(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHaveSSE"));

    if (CPUHaveCPUID())
    {
        result = CPUGetFeatures();

        result = ((result & (CPUID_SSE | CPUID_SSE2)) != 0);
    }

    RWRETURN(result);
}

RwBool
_rwCPUHaveSSE2(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHaveSSE2"));

    if (CPUHaveCPUID())
    {
        result = CPUGetFeatures();

        result = ((result & CPUID_SSE2) != 0);
    }

    RWRETURN(result);
}

RwBool
_rwCPUHave3DNow(void)
{
    RwBool  result = FALSE;

    RWFUNCTION(RWSTRING("_rwCPUHave3DNow"));

    if (CPUHaveCPUID())
    {
        if (CPUHaveExtFeatures())
        {
            result = CPUGetExtFeatures();

            result = ((result & CPUID_EXT_3DNOW) != 0);
        }
    }

    RWRETURN(result);
}

#if defined(WIN32)
RwBool
_rwCPUGetOSName(RwChar *osName)
{
    OSVERSIONINFOEX osvi;
    RwBool osVersionInfoEx;

    RWFUNCTION(RWSTRING("_rwCPUGetWindowsVersion"));

    osName[0] = 0;

    memset(&osvi, 0, sizeof(OSVERSIONINFOEX));

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&osvi);
    if (osVersionInfoEx == FALSE)
    {
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (GetVersionEx((OSVERSIONINFO *)&osvi) == FALSE)
        {
            RWRETURN(FALSE);
        }
    }

    switch (osvi.dwPlatformId)
    {
        case VER_PLATFORM_WIN32_NT:
            if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
            {
                osName += rwsprintf(osName, "Microsoft Windows .NET Server 2003 family, ");
            }
            else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
            {
                osName += rwsprintf(osName, "Microsoft Windows XP ");
            }
            else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
            {
                osName += rwsprintf(osName, "Microsoft Windows 2000 ");
            }
            else if ( osvi.dwMajorVersion <= 4 )
            {
                osName += rwsprintf(osName, "Microsoft Windows NT ");
            }

            #if defined(VER_NT_WORKSTATION) && defined(VER_NT_SERVER)
            if (osVersionInfoEx)
            {
                if ( osvi.wProductType == VER_NT_WORKSTATION )
                {
                    if ( osvi.dwMajorVersion == 4 )
                    {
                        osName += rwsprintf(osName, "Workstation 4.0 " );
                    }
                    else if ( osvi.wSuiteMask & VER_SUITE_PERSONAL )
                    {
                        osName += rwsprintf(osName, "Home Edition " );
                    }
                    else
                    {
                        osName += rwsprintf(osName, "Professional " );
                    }
                }
                else if ( osvi.wProductType == VER_NT_SERVER )
                {
                    if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
                    {
                        if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                        {
                            osName += rwsprintf(osName, "Datacenter Edition " );
                        }
                        else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        {
                            osName += rwsprintf(osName, "Enterprise Edition " );
                        }
#if (!defined(__GNUC__))       
                        else if ( osvi.wSuiteMask == VER_SUITE_BLADE )
                        {
                            osName += rwsprintf(osName, "Web Edition " );
                        }
#endif
                        else
                        {
                            osName += rwsprintf(osName, "Standard Edition " );
                        }
                    }
                    else if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
                    {
                        if ( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                        {
                            osName += rwsprintf(osName, "Datacenter Server " );
                        }
                        else if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        {
                            osName += rwsprintf(osName, "Advanced Server " );
                        }
                        else
                        {
                            osName += rwsprintf(osName, "Server " );
                        }
                    }
                    else
                    {
                        if ( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                        {
                            osName += rwsprintf(osName, "Server 4.0, Enterprise Edition " );
                        }
                        else
                        {
                            osName += rwsprintf(osName, "Server 4.0 " );
                        }
                    }
                }
            }
            else
            #endif
            {
                if ( osvi.dwMajorVersion <= 4 )
                {
                    osName += rwsprintf(osName, "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion);
                }
            }

            if (osVersionInfoEx)
            {
                if (osvi.wServicePackMajor)
                {
                    if (osvi.wServicePackMinor)
                    {
                        osName += rwsprintf(osName, "Service Pack %d.%d", osvi.wServicePackMajor, osvi.wServicePackMinor);
                    }
                    else
                    {
                        osName += rwsprintf(osName, "Service Pack %d", osvi.wServicePackMajor);
                    }
                }
            }
            break;

        case VER_PLATFORM_WIN32_WINDOWS:
            if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
            {
                osName += rwsprintf(osName, "Microsoft Windows 95 ");
                if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
                {
                    osName += rwsprintf(osName, "OSR2 " );
                }
            }
            else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
            {
                osName += rwsprintf(osName, "Microsoft Windows 98 ");
                if ( osvi.szCSDVersion[1] == 'A' )
                {
                    osName += rwsprintf(osName, "SE " );
                }
            }
            else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
            {
                osName += rwsprintf(osName, "Microsoft Windows Millennium Edition");
            } 
            break;
    }

    RWRETURN(TRUE);
}
#endif /* defined(WIN32) */
