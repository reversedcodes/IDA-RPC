cmake_minimum_required(VERSION 3.15)

# Let find_package(IdaSDK) honour the IDASDK_ROOT variable/environment hint.
if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()

project(ida_discord_rpc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# The plugin binary is written here, ready to be copied into an IDA plugins directory.
set(IDA_PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ida_plugins")

find_package(IdaSDK REQUIRED)
message(STATUS "Found IDA SDK ${IdaSDK_VERSION}: ${IdaSDK_LIBRARIES}")

include(FetchContent)
include(FetchDiscordRPC)

add_subdirectory(src/ida_discord_rpc)
