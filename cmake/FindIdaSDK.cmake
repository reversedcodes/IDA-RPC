
if(WIN32)
    set(_idasdk_libdir "x64_win_vc_64")
    set(_idasdk_platform_def "__NT__")
elseif(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
        set(_idasdk_libdir "arm64_mac_clang_64")
    else()
        set(_idasdk_libdir "x64_mac_clang_64")
    endif()
    set(_idasdk_platform_def "__MAC__")
else()
    set(_idasdk_libdir "x64_linux_gcc_64")
    set(_idasdk_platform_def "__LINUX__")
endif()

set(_idasdk_hints "")
if(IDASDK_ROOT)
    list(APPEND _idasdk_hints "${IDASDK_ROOT}")
endif()
if(DEFINED ENV{IDASDK})
    list(APPEND _idasdk_hints "$ENV{IDASDK}")
endif()
if(DEFINED ENV{IDASDK_ROOT})
    list(APPEND _idasdk_hints "$ENV{IDASDK_ROOT}")
endif()
list(APPEND _idasdk_hints
    "${CMAKE_SOURCE_DIR}/lib/ida-sdk"
    "${CMAKE_SOURCE_DIR}/lib/ida-sdk/src")

find_path(IdaSDK_INCLUDE_DIR
    NAMES ida.hpp
    HINTS ${_idasdk_hints}
    PATH_SUFFIXES include src/include
    DOC "IDA SDK include directory (contains ida.hpp)")

find_library(IdaSDK_LIBRARY
    NAMES ida
    HINTS ${_idasdk_hints}
    PATH_SUFFIXES
        "lib/${_idasdk_libdir}"
        "src/lib/${_idasdk_libdir}"
    DOC "IDA kernel import/shared library")

if(IdaSDK_INCLUDE_DIR AND EXISTS "${IdaSDK_INCLUDE_DIR}/pro.h")
    file(STRINGS "${IdaSDK_INCLUDE_DIR}/pro.h" _idasdk_ver_line
        REGEX "^#define[ \t]+IDA_SDK_VERSION[ \t]+[0-9]+")
    if(_idasdk_ver_line MATCHES "([0-9]+)")
        set(_idasdk_ver_raw "${CMAKE_MATCH_1}")
        math(EXPR _idasdk_major "${_idasdk_ver_raw} / 100")
        math(EXPR _idasdk_minor "(${_idasdk_ver_raw} / 10) % 10")
        math(EXPR _idasdk_patch "${_idasdk_ver_raw} % 10")
        set(IdaSDK_VERSION "${_idasdk_major}.${_idasdk_minor}.${_idasdk_patch}")
    endif()
    unset(_idasdk_ver_line)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IdaSDK
    REQUIRED_VARS IdaSDK_LIBRARY IdaSDK_INCLUDE_DIR
    VERSION_VAR IdaSDK_VERSION)

if(IdaSDK_FOUND)
    set(IdaSDK_INCLUDE_DIRS "${IdaSDK_INCLUDE_DIR}")
    set(IdaSDK_LIBRARIES "${IdaSDK_LIBRARY}")

    if(NOT TARGET IdaSDK::IdaSDK)
        add_library(IdaSDK::IdaSDK UNKNOWN IMPORTED)
        set_target_properties(IdaSDK::IdaSDK PROPERTIES
            IMPORTED_LOCATION "${IdaSDK_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${IdaSDK_INCLUDE_DIR}"
            INTERFACE_COMPILE_DEFINITIONS "__EA64__;${_idasdk_platform_def}")
    endif()
endif()

mark_as_advanced(IdaSDK_INCLUDE_DIR IdaSDK_LIBRARY)

unset(_idasdk_libdir)
unset(_idasdk_platform_def)
unset(_idasdk_hints)
