set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

set(CLANG_FORMAT_SUFFIX "-disabled-by-ida-discord-rpc" CACHE STRING "" FORCE)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    discord-rpc
    GIT_REPOSITORY https://github.com/discord/discord-rpc.git
    GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(discord-rpc)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(discord-rpc PRIVATE -Wno-template-body)
endif()

set_target_properties(discord-rpc PROPERTIES POSITION_INDEPENDENT_CODE ON)
