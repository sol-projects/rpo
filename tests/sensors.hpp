#pragma once
#include "sensors/sensors.hpp"
#include <DOCTEST/doctest.h>
#include <LLOG/llog.hpp>
#include <cassert>
#include <chrono>
#include <thread>

TEST_CASE("Printing current temperature/usage information")
{
    using namespace std::chrono_literals;

    std::vector<sensors::Device> devices;

    SUBCASE("Gathering info from all devices.")
    {
        devices = sensors::getDevices(sensors::Device::Type::Any);
        CHECK(!devices.empty());
    }

    // SUBCASE("Checking temperature and load.")
    {
        for (auto& device : devices)
        {
            switch (device.type)
            {
                case sensors::Device::Type::CPU:
                    {
                        CHECK(!device.name.empty());
                        llog::Print("Testing CPU:", device.name);

                        auto temp = sensors::getTemp(device);
                        if (temp == 0)
                        {
                            llog::Print(llog::pt::warning, "Temperature not supported.");
                        }
                        else
                        {
                            llog::Print("Temperature:", temp);
                        }

                        std::this_thread::sleep_for(200ms);
                        auto load = sensors::getLoad(device);
                        if (load == 0)
                        {
                            llog::Print(llog::pt::warning, "Load not supported.");
                        }
                        else
                        {
                            auto loadf = std::to_string(load);
                            llog::Print("Load: ", std::string(loadf.substr(0, loadf.size() - 1) + "." + loadf.back() + "%"));
                        }
                    }
                    break;

                case sensors::Device::Type::RAM:
                    {
                        CHECK(!device.name.empty());
                        llog::Print("Testing RAM:", device.name);

                        auto temp = sensors::getTemp(device);
                        if (temp == 0)
                        {
                            llog::Print(llog::pt::warning, "Temperature not supported.");
                        }
                        else
                        {
                            llog::Print("Temperature:", temp);
                        }

                        auto load = sensors::getLoad(device);
                        if (load == 0)
                        {
                            llog::Print(llog::pt::warning, "Load not supported.");
                        }
                        else
                        {
                            auto loadf = std::to_string(load);
                            llog::Print("Load:", std::string(loadf.substr(0, loadf.size() - 1) + "." + loadf.back() + " GB"));
                        }
                    }
                    break;

                case sensors::Device::Type::GPU:
                    {
                        CHECK(!device.name.empty());
                        llog::Print("Testing GPU:", device.name);

                        auto temp = sensors::getTemp(device);
                        if (temp == 0)
                        {
                            llog::Print(llog::pt::warning, "Temperature not supported.");
                        }
                        else
                        {
                            llog::Print("Temperature:", temp);
                        }

                        auto load = sensors::getLoad(device);
                        if (load == 0)
                        {
                            llog::Print(llog::pt::warning, "Load not supported.");
                        }
                        else
                        {
                            auto loadf = std::to_string(sensors::getLoad(device));
                            llog::Print("Load:", std::to_string(sensors::getLoad(device)) + "%");
                        }
                    }
                    break;

                case sensors::Device::Type::VRAM:
                    {
                        CHECK(!device.name.empty());
                        llog::Print("Testing VRAM:", device.name);

                        auto temp = sensors::getTemp(device);
                        if (temp == 0)
                        {
                            llog::Print(llog::pt::warning, "Temperature not supported.");
                        }
                        else
                        {
                            llog::Print("Temperature:", temp);
                        }

                        auto load = sensors::getLoad(device);
                        if (load == 0)
                        {
                            llog::Print(llog::pt::warning, "Load not supported.");
                        }
                        else
                        {
                            auto loadf = std::to_string(load);
                            llog::Print("Load:", std::string(loadf.substr(0, loadf.size() - 1) + "." + loadf.back() + " GB"));
                        }
                    }
                    break;
                case sensors::Device::Type::Any:
                    {
                        REQUIRE(0);
                        llog::Print(llog::pt::error, "Unknown device type", device.name);
                    }
            }
        }
    }
}
