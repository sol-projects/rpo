#include "stdafx.h"
#include "CPUInfo.h"
#include <iostream>
#include "GPUInfo2.h"
#include <cstdlib>
#include <stdio.h>
//#include "nvml.h"


int main(int argc, char* argv[])
{
    CPUInfo cpu;
    GpuInfo gpu;

    while (1)
    {
        std::cout << "-------------- CPU ---------------" << std::endl;
        std::cout<< cpu.GetCPUName()<<std::endl;
        //std::cout << "CPU Temp: " << cpu.GetCPUMaxTemp() << " C" << std::endl;
        std::cout << "CPU Load: " << cpu.GetCPULoad()*100 << " %" << std::endl;
        std::cout << "Memory used: " << cpu.GetUsedMemory() / 1000 / 1000 << " MB / " << cpu.GetMaxMemory() / 1000 / 1000 << " MB" << std::endl;
        std::cout << "-------------- GPU ---------------" << std::endl;
        std::cout<<"GPU "<< gpu.GetGPUName()<<std::endl;
        std::cout<<"GPU "<< gpu.GetGPUVRAM()<< " MB"<<std::endl;
        std::cout<<"GPU usage: "<<gpu.GetUsage()<<"% "<<std::endl;
        std::cout<<"GPU temp: "<<gpu.GetGPUTemp()<<"C "<<std::endl;
    }
    return 0;
}
