#pragma once
#include <cargs.h>
#include <string>

namespace nogui
{
    struct OptionFlags
    {
        int interval = 0;
        int accuracy = 0;
        bool temperature = false;
        bool load = false;
        std::string devices;
        bool file = false;
        std::string path;
    };

    // clang-format off
    constexpr struct cag_option options[]
    {

        {
            .identifier = 'i',
            .access_letters = "i",
            .access_name = "interval",
            .value_name = "ms",
            .description = "Set the interval in milliseconds at which measurements are performed."
        },

        {
            .identifier = 't',
            .access_letters = "t",
            .access_name = "temperature",
            .value_name = nullptr,
            .description = "Measure temperature if the device has temperature sensors."
        },

        {
            .identifier = 'l',
            .access_letters = "l",
            .access_name = "load",
            .value_name = nullptr,
            .description = "Measure load."
        },

        {
            .identifier = 'd',
            .access_letters = "d",
            .access_name = "devices",
            .value_name = "[cpu|cput|ram|gpu|vram|all]",
            .description = "Choose which device types to measure."
        },

        {
            .identifier = 'f',
            .access_letters = "f",
            .access_name = "file",
            .value_name = nullptr,
            .description = "Save measurements to an automatically generated CSV file."
        },

        {
            .identifier = 'p',
            .access_letters = "p",
            .access_name = "path",
            .value_name = "path",
            .description = "Set the path for measurement csv files."
        },

        {
            .identifier = 'a',
            .access_letters = "a",
            .access_name = "accuracy",
            .value_name = "accuracy",
            .description = "Set the measurement accuracy (precision) in terms of decimals."
        },

        {
            .identifier = 'h',
            .access_letters = "h",
            .access_name = "help",
            .value_name = nullptr,
            .description = "Displays information about all available commands."
        }
    };
    // clang-format on
}
