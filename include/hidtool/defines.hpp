#ifndef HIDTOOL_DEFINES_HPP
#define HIDTOOL_DEFINES_HPP

#include <hidtool/config.hpp>

#include <cstddef>
#include <cstdint>

#ifdef _WIN32
    #define HIDTOOL_WIN
#elif defined(__APPLE__)
    #define HIDTOOL_APPLE
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC && !TARGET_OS_IPHONE
        #define HIDTOOL_MACOS
    #else
        #error "Unsupported platform"
    #endif // TARGET_OS_MAC && !TARGET_OS_IPHONE
#elif defined(__linux__)
    #define HIDTOOL_LINUX
#else
    #error "Unsupported platform"
#endif // _WIN32

#ifdef HIDTOOL_WIN
    #ifdef HIDTOOL_BUILD_SHARED
        #define HIDTOOL_API __declspec(dllexport)
    #elif defined(HIDTOOL_SHARED)
        #define HIDTOOL_API __declspec(dllimport)
    #else
        #define HIDTOOL_API
    #endif // HIDTOOL_BUILD_SHARED
#else
    #define HIDTOOL_API __attribute__((visibility("default")))
#endif // HIDTOOL_WIN

#endif // !HIDTOOL_DEFINES_HPP
