add_library(ida_discord_rpc SHARED
    plugin.cpp
    plugin_mod.cpp

    discord/discord_rpc_helper.cpp
    discord/rich_presence.cpp

    gui/rpc_settings.cpp
    gui/settings_dialog.cpp
    gui/status_toolbar.cpp

    listener/dbg_listener.cpp
    listener/idb_listener.cpp
    listener/ui_listener.cpp
    listener/view_listener.cpp

    util/ida_helper.cpp
    util/log.cpp
    util/log_level.cpp
)

# Sources include their own headers as "ida_discord_rpc/<module>/<header>".
target_include_directories(ida_discord_rpc PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
    "${discord-rpc_SOURCE_DIR}/include"
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

# The file name must match "entryPoint" in ida-plugin.json.
set_target_properties(ida_discord_rpc PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ida-discord-rpc"
    RUNTIME_OUTPUT_DIRECTORY "${IDA_PLUGIN_OUTPUT_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${IDA_PLUGIN_OUTPUT_DIR}"
)

if(APPLE)
    set_target_properties(ida_discord_rpc PROPERTIES SUFFIX ".dylib")
endif()
