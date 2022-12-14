cmake_minimum_required(VERSION 3.20)

project(magnus_libris
        DESCRIPTION "magnus libris nice"
        VERSION 1.0.0
        LANGUAGES CXX
)
#set (CMAKE_CXX_CPPCHECK "cppcheck")

#set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.*;-checks=cppcoreguidelines-*, performance-*, bugprone-*, readability-*, hicpp-*") 


add_executable(${PROJECT_NAME}
	main.cpp
	CPUInfo.cpp
	stdafx.cpp
    GPUInfo2.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Ofast)
endif()