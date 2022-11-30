#pragma once
#include <shared/config/config.hpp>
#include <DOCTEST/doctest.h>
#include <filesystem>

TEST_CASE("Creating and testing default config.")
{
    config::init();

    SUBCASE("Testing option names.")
    {
        CHECK(config::get("cpu_linux_temperature_measurement_mode").name == "cpu_linux_temperature_measurement_mode");
    }
    
    SUBCASE("Testing setting and getting option values.")
    {
        auto option = config::get("cpu_linux_temperature_measurement_mode");
        CHECK(option.value == "any");

        option.value = "acpitz";
        config::set(option);
        option = config::get("cpu_linux_temperature_measurement_mode");
        CHECK(option.value == "acpitz");
    }

    SUBCASE("Testing if the config file is generated correctly.")
    {
        CHECK(!config::saved());
        config::save();
        CHECK(config::saved());
    }
}

