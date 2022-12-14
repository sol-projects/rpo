#include "stdafx.h"
#include <Windows.h>
#include <processthreadsapi.h>
#include <sysinfoapi.h>
#include <Intrin.h>
#include <atomic>
#include "OlsApiInit.h"
#include "CPUInfo.h"


static std::atomic<int> s_count = 0;


static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
   static unsigned long long _previousTotalTicks = 0;
   static unsigned long long _previousIdleTicks = 0;

   unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
   unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;

   float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);

   _previousTotalTicks = totalTicks;
   _previousIdleTicks  = idleTicks;
   return ret;
}
static unsigned long long FileTimeToInt64(const FILETIME & ft) {return (((unsigned long long)(ft.dwHighDateTime))<<32)|((unsigned long long)ft.dwLowDateTime);}


float CPUInfo::GetCPULoad()
{
   FILETIME idleTime, kernelTime, userTime;
   return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime)+FileTimeToInt64(userTime)) : -1.0f;
}

uint64_t CPUInfo::GetMaxMemory()
{
    MEMORYSTATUSEX memstat = { sizeof(memstat) };
    if (!GlobalMemoryStatusEx(&memstat))
        return 0;
    return memstat.ullTotalPhys;
}

uint64_t CPUInfo::GetUsedMemory()
{
    MEMORYSTATUSEX memstat = { sizeof(memstat) };
    if (!GlobalMemoryStatusEx(&memstat))
        return 0;
    return memstat.ullTotalPhys - memstat.ullAvailPhys;
}

std::string CPUInfo::GetCPUName()
{
    std::string out;
    std::string fullQuery = "wmic cpu get name";
        FILE* pFile = _popen(fullQuery.c_str(), "r");
        char buffer[256];
         while (!feof(pFile)) {
            if (fgets(buffer, 128, pFile) != NULL)
                out += buffer;
        }
        out.erase(std::remove(out.begin(), out.end(), '\n'), out.cend());
        _pclose(pFile);
        return std::string(out);
}