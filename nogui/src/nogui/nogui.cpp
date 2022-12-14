#include "nogui/nogui.hpp"
#include "sensors/sensors.hpp"
#include "shared/csv/csv.hpp"
#include <LLOG/llog.hpp>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <string>
#include <thread>

namespace nogui
{
    namespace
    {

        std::vector<sensors::Device> getDevices(std::string& devicesToMeasure)
        {
            std::transform(std::begin(devicesToMeasure), std::end(devicesToMeasure), std::begin(devicesToMeasure), ::tolower);

            if (devicesToMeasure == "all")
            {
                return sensors::getDevices(sensors::Device::Type::Any);
            }

            std::vector<sensors::Device> devices;

            auto addToDevicesIfExists = [&devicesToMeasure, &devices](const std::string& deviceName, sensors::Device::Type type) {
                if (auto found = devicesToMeasure.find(deviceName); found != std::string::npos)
                {
                    auto newDevices = sensors::getDevices(type);
                    devices.insert(std::end(devices), std::begin(newDevices), std::end(newDevices));
                    devicesToMeasure.erase(found, deviceName.size());
                }
            };

            addToDevicesIfExists("cput", sensors::Device::Type::CPUThread);
            addToDevicesIfExists("cpu", sensors::Device::Type::CPU);
            addToDevicesIfExists("vram", sensors::Device::Type::VRAM);
            addToDevicesIfExists("ram", sensors::Device::Type::RAM);
            addToDevicesIfExists("gpu", sensors::Device::Type::GPU);

            return devices;
        }

        void runProgramAtInterval(bool temperature, bool load, int interval, std::vector<sensors::Device> devices, bool fileFlag, const std::string& filepath, int accuracy)
        {
            using namespace std::chrono_literals;

            if (devices.empty())
            {
                llog::Print(llog::pt::error, "No devices found.");
                return;
            }

            csv::MeasurementType measurementType;
            if (temperature && load)
            {
                measurementType = csv::MeasurementType::Both;
            }
            else if (temperature)
            {
                measurementType = csv::MeasurementType::Temperature;
            }
            else if (load)
            {
                measurementType = csv::MeasurementType::Load;
            }
            else
            {
                llog::Print(llog::pt::error, "Must use flags -t or -l to choose which temperatures to measure.");
                return;
            }

            std::vector<csv::Csv> files;
            if (fileFlag)
            {
                for (const auto& device : devices)
                {
                    files.emplace_back(device, interval, measurementType, filepath);
                }
            }
            else if (std::size(devices) > 1 || (load && temperature))
            {
                llog::Print(llog::pt::error, "You can only use a maximum of 1 device and 1 type of measurement if you're not combining with the -f option.");
                return;
            }

            for (;;)
            {
                auto intervalStartTime = std::chrono::high_resolution_clock::now();
                for (auto i = 0ULL; i < std::size(devices); i++)
                {
                    auto& device = devices.at(i);

                    if (temperature)
                    {
                        device.temperature = sensors::getTemp(device, accuracy);
                    }

                    if (load)
                    {
                        device.load = sensors::getLoad(device, accuracy);
                    }

                    if (fileFlag)
                    {
                        auto& file = files.at(i);
                        file.add(device);
                    }
                    else
                    {
                        if (temperature)
                        {
                            std::cout << device.temperature << '\n';
                        }
                        else if (load)
                        {
                            std::cout << device.load << '\n';
                        }
                    }
                }

                auto intervalEndTime = std::chrono::high_resolution_clock::now();

                auto intervalCycleTime = std::chrono::duration_cast<std::chrono::microseconds>(intervalEndTime - intervalStartTime);

                if (intervalCycleTime.count() < 0)
                {
                    llog::Print(llog::pt::error, "Improper interval, must be at least" - 1000 * intervalCycleTime.count(), "longer");
                    return;
                }

                std::this_thread::sleep_for(std::chrono::microseconds(interval * 1000 - intervalCycleTime.count()));
            }
        }
    }

    void run(int argc, char* argv[])
    {
        cag_option_context context;
        OptionFlags optionFlags {};
        cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
        while (cag_option_fetch(&context))
        {
            auto identifier = cag_option_get(&context);
            switch (identifier)
            {
                case 'a':
                    optionFlags.accuracy = std::stoi(cag_option_get_value(&context));
                    break;
                case 'i':
                    optionFlags.interval = std::stoi(cag_option_get_value(&context));
                    break;
                case 't':
                    optionFlags.temperature = true;
                    break;
                case 'l':
                    optionFlags.load = true;
                    break;
                case 'd':
                    optionFlags.devices = cag_option_get_value(&context);
                    break;
                case 'f':
                    optionFlags.file = true;
                    break;
                case 'p':
                    optionFlags.path = cag_option_get_value(&context);
                    break;
                case 'h':
                    cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
                    return;
            }
        }

        if (optionFlags.interval != 0)
        {
            runProgramAtInterval(optionFlags.temperature, optionFlags.load, optionFlags.interval, getDevices(optionFlags.devices), optionFlags.file, optionFlags.path, optionFlags.accuracy);
        }
    }
}
