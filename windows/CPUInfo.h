#pragma once
#include<windows.h>
#include <vector>
#include <string>

#define CONSOLE_PRINT 1
class CPUInfo
{
private:
    int m_Tjmax;    
    int m_cores;

public:
    std::string GetCPUName();
    std::vector<float> GetCoreTemp();
    int GetCPUAverageTemp();
    float GetCPUMaxTemp();
    int GetCoreMax();
    float GetCPULoad();
    uint64_t GetMaxMemory();
    uint64_t GetUsedMemory();
};