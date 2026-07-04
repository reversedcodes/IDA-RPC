cmake_minimum_required(VERSION 3.15)
project(ida_discord_rpc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(IDA_PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ida_plugins")

find_package(IdaSDK REQUIRED)
message(STATUS "Found IDA SDK ${IdaSDK_VERSION}: ${IdaSDK_LIBRARIES}")

include(FetchContent)
include(FetchDiscordRPC)

add_library(ida_discord_rpc SHARED
    src/plugin.cpp
    src/plugin_mod.cpp

    src/core/discord/rich_presence.cpp
    src/core/discord/discord_rpc_helper.cpp

    src/core/gui/rpc_settings.cpp
    src/core/gui/settings_dialog.cpp
    src/core/gui/status_toolbar.cpp

    src/core/listener/ui_listener.cpp
    src/core/listener/idb_listener.cpp
    src/core/listener/view_listener.cpp
    src/core/listener/dbg_listener.cpp

    src/utils/ida/ida_helper.cpp

    src/utils/log/log.cpp
    src/utils/log/log_level.cpp
)

target_include_directories(ida_discord_rpc PRIVATE
    src
    ${discord-rpc_SOURCE_DIR}/include
)

target_link_libraries(ida_discord_rpc PRIVATE
    IdaSDK::IdaSDK
    discord-rpc
)

if(WIN32)
    target_compile_definitions(ida_discord_rpc PRIVATE WIN32_LEAN_AND_MEAN)
endif()

if(MSVC)
    target_compile_options(ida_discord_rpc PRIVATE /GR- /EHsc)
endif()

set_target_properties(ida_discord_rpc PROPERTIES
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    OUTPUT_NAME "ida_discord_rpc"
)
