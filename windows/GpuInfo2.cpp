#include "GPUInfo2.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <windows.h>

#define NVAPI_MAX_PHYSICAL_GPUS   64
#define NVAPI_MAX_USAGES_PER_GPU  34

// function pointer types
typedef int *(*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int (*NvAPI_Initialize_t)();
typedef int (*NvAPI_EnumPhysicalGPUs_t)(int **handles, int *count);
typedef int (*NvAPI_GPU_GetUsages_t)(int *handle, unsigned int *usages);



std::string GpuInfo::GetGPUName(){
    std::string out;
    std::string fullQuery = "wmic path win32_VideoController get name";
        FILE* pFile = _popen(fullQuery.c_str(), "r");
        char buffer[256];
         while (!feof(pFile)) {
            if (fgets(buffer, 128, pFile) != NULL)
                out += buffer;
        }
        _pclose(pFile);
        out.erase(std::remove(out.begin(), out.end(), '\n'), out.cend());
        return std::string(out);
}
int GpuInfo::GetGPUVRAM(){
    std::string out;
    std::string test;
    std::string stevilo;
    std::string fullQuery = "wmic path win32_VideoController get AdapterRam";
        FILE* pFile = _popen(fullQuery.c_str(), "r");
        char buffer[128];
         while (!feof(pFile)) {
            if (fgets(buffer, 128, pFile) != NULL){
                if (isdigit(buffer[0])){
                    out += buffer;
                }
            }
        }
        _pclose(pFile);
        unsigned long int  rez = stoul(out);
        //out.erase(std::remove(out.begin(), out.end(), '\n'), out.cend());
        return (rez/1024/1024);
}

int GpuInfo::GetUsage(){
    HMODULE hmod = LoadLibraryA("nvapi.dll");
    if (hmod == NULL)
    {
        std::cerr << "Couldn't find nvapi.dll" << std::endl;
        return 1;
    }

    // nvapi.dll internal function pointers
    NvAPI_QueryInterface_t      NvAPI_QueryInterface     = NULL;
    NvAPI_Initialize_t          NvAPI_Initialize         = NULL;
    NvAPI_EnumPhysicalGPUs_t    NvAPI_EnumPhysicalGPUs   = NULL;
    NvAPI_GPU_GetUsages_t       NvAPI_GPU_GetUsages      = NULL;

    // nvapi_QueryInterface is a function used to retrieve other internal functions in nvapi.dll
    NvAPI_QueryInterface = (NvAPI_QueryInterface_t) GetProcAddress(hmod, "nvapi_QueryInterface");

    // some useful internal functions that aren't exported by nvapi.dll
    NvAPI_Initialize = (NvAPI_Initialize_t) (*NvAPI_QueryInterface)(0x0150E828);
    NvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t) (*NvAPI_QueryInterface)(0xE5AC921F);
    NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t) (*NvAPI_QueryInterface)(0x189A1FDF);

    if (NvAPI_Initialize == NULL || NvAPI_EnumPhysicalGPUs == NULL ||
        NvAPI_EnumPhysicalGPUs == NULL || NvAPI_GPU_GetUsages == NULL)
    {
        std::cerr << "Couldn't get functions in nvapi.dll" << std::endl;
        return 2;
    }

    // inicializacija funkcije preden jo poklicemo
    (*NvAPI_Initialize)();

    int          gpuCount = 0;
    int         *gpuHandles[NVAPI_MAX_PHYSICAL_GPUS] = { NULL };
    unsigned int gpuUsages[NVAPI_MAX_USAGES_PER_GPU] = { 0 };

    // gpuUsages[0] v njega shranimo podatke
    gpuUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;

    (*NvAPI_EnumPhysicalGPUs)(gpuHandles, &gpuCount);

        (*NvAPI_GPU_GetUsages)(gpuHandles[0], gpuUsages);
        int usage = gpuUsages[3]; 
        return usage;
}

int GpuInfo::GetGPUTemp(){
     std::string out;
    std::string test;
    std::string stevilo;
    std::string fullQuery = "nvidia-smi --query-gpu=temperature.gpu --format=csv";
        FILE* pFile = _popen(fullQuery.c_str(), "r");
        char buffer[128];
         while (!feof(pFile)) {
            if (fgets(buffer, 128, pFile) != NULL){
                if (isdigit(buffer[0])){
                    out += buffer;
                }
            }
        }
        _pclose(pFile);
        unsigned long int  rez = stoul(out);
        //out.erase(std::remove(out.begin(), out.end(), '\n'), out.cend());
        return (rez);
}