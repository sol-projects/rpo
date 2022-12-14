#include <cstdlib>
#include <string>

class GpuInfo{
private:
    std::string name;

public:
    std::string GetGPUName();
    int GetGPUVRAM();
    int GetGPUTemp();
    int GetUsage(); // https://stackoverflow.com/questions/25376080/nvcplgetthermalsettings-call-to-nvcpl-dll-returns-false-c/26009138#26009138?newreg=fadeb5cdf9ce4cfea4c3662ab6621630
};