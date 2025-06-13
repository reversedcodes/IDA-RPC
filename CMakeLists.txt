cmake_minimum_required(VERSION 3.15)
project(ida_discord_rpc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(IDASDK_PATH "${CMAKE_SOURCE_DIR}/sdk/ida_home_9.1_sdk")
set(IDASDK_LIB "${IDASDK_PATH}/lib/x64_win_vc_64_home/ida.lib")
set(IDA_PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ida_plugins")

include(FetchContent)
include(cmake/fetch_discord_rpc.cmake) 

include_directories(${IDASDK_PATH}/include)

add_library(ida_discord_rpc SHARED
    src/plugin.cpp
    src/plugin_mod.cpp
    src/helper.cpp
    
    src/log/log.cpp
    src/log/log_level.cpp

    src/discord/rich_presence.cpp

    src/listener/ui_listener.cpp
    src/listener/idb_listener.cpp
    src/listener/view_listener.cpp

)

target_include_directories(ida_discord_rpc PRIVATE
    ${discord-rpc_SOURCE_DIR}/include
    src
)

target_link_libraries(ida_discord_rpc PRIVATE
    "${IDASDK_LIB}"
    discord-rpc
)

if(MSVC)
    add_definitions(-D__NT__ -DWIN32_LEAN_AND_MEAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR- /EHsc")
endif()

set_target_properties(ida_discord_rpc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    OUTPUT_NAME "ida_discord_rpc"
)
