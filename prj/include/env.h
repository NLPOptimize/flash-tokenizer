//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                            C++ Environment Check Module
//
//               Copyright (C) 2025, Kim Bomm, all rights reserved.
//
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
#pragma once
#ifndef EUUECTTEQNEEWWFLVRPCDLSEWYMHQFCVVLPKHJECCYQSMWNSCYYKVXHRHYZDEA
#define EUUECTTEQNEEWWFLVRPCDLSEWYMHQFCVVLPKHJECCYQSMWNSCYYKVXHRHYZDEA

#include<vector>
#include<string>
#include<sstream>
#include <string>

#if defined(_WIN32)
#  include <intrin.h>
#elif defined(__APPLE__)
#  include <sys/types.h>
#  include <sys/sysctl.h>
#  include <unistd.h>
#elif defined(__linux__)
#  include <fstream>
#  include <sstream>
#endif

static std::string get_cpu_name() {
#if defined(_WIN32)
    int info[4] = {0};
    __cpuid(info, 0x80000000);
    unsigned int maxExt = info[0];
    if (maxExt >= 0x80000004) {
        char brand[49] = {};
        __cpuid(reinterpret_cast<int*>(info), 0x80000002);
        std::memcpy(brand +  0, info, sizeof(info));
        __cpuid(reinterpret_cast<int*>(info), 0x80000003);
        std::memcpy(brand + 16, info, sizeof(info));
        __cpuid(reinterpret_cast<int*>(info), 0x80000004);
        std::memcpy(brand + 32, info, sizeof(info));
        return std::string(brand);
    }

#elif defined(__APPLE__)
    char brand[256];
    size_t size = sizeof(brand);
    if (sysctlbyname("machdep.cpu.brand_string", brand, &size, nullptr, 0) == 0)
        return std::string(brand, size - 1);

#elif defined(__linux__)
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.rfind("model name", 0) == 0) {
            auto pos = line.find(':');
            if (pos != std::string::npos)
                return line.substr(pos + 2);
        }
    }
#endif

    return "Unknown CPU";
}


static std::string GetOS() {
#if defined(_WIN32) || defined(_WIN64)
    std::string os = "Windows";
#elif defined(__linux__)
    std::string os = "Linux";
#elif defined(__APPLE__)
    std::string os = "MacOS";
#endif
    return os;
}

static std::string GetCompiler() {
    std::ostringstream compiler_version;
#ifdef __clang__
    compiler_version << "clang++(" << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << ")";
#elif defined(__GNUC__)
    compiler_version << "g++(" << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << ")";
#elif defined(_MSC_VER)
    compiler_version << "MSVC(" << _MSC_VER << ")";
#elif defined(__INTEL_COMPILER)
    std::cout << "Intel(" << __INTEL_COMPILER << ")";
#endif
    return compiler_version.str();
}

static std::string GetParallelImpl() {
#ifdef _OPENMP
    return "OpenMP";
#else
    return "std::thread";
#endif
}

static std::string cpp_env(const std::string &version = "dev") {
    std::ostringstream oss;
    oss << "[" << version << "] ";
#if defined(_WIN32) || defined(_WIN64)
    const char *os = "Windows";
#elif defined(__linux__)
    const char* os = "Linux";
#elif defined(__APPLE__)
    const char *os = "MacOS";
#endif

#if defined(__aarch64__)
    const char *arch = "ARM64";
#elif defined(__arm__)
    const char* arch = "ARM32";
#elif defined(__x86_64__)
    const char* arch = "x86_64";
#elif defined(_WIN64)
    const char *arch = "Win64";
#elif defined(__ppc64__)
    const char* arch = "PowerPC64";
#else
    const char* arch = "Unknown architecture";
#endif

    std::ostringstream compiler_version;
#ifdef __clang__
    compiler_version << "clang++(" << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__ << ")";
#elif defined(__GNUC__)
    compiler_version << "g++(" << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << ")";
#elif defined(_MSC_VER)
    compiler_version << "MSVC(" << _MSC_VER << ")";
#elif defined(__INTEL_COMPILER)
    std::cout << "Intel(" << __INTEL_COMPILER << ")";
#endif
    oss << os << ", " << compiler_version.str() << " - " << arch;

#ifdef _DEBUG
    oss << "(Debug)";
#else
    oss << "(Release)";
#endif

    std::stringstream cpp_version;
#if __cplusplus > 202302L
    cpp_version<< "C++26";
#elif __cplusplus > 202002L
    cpp_version << "C++23";
#elif __cplusplus > 201703L
    cpp_version << "C++20";
#elif __cplusplus > 201402L
    cpp_version << "C++17";
#elif __cplusplus > 201103L
    cpp_version << "C++14";
#elif __cplusplus > 199711L
        cpp_version << "C++11";
#else
        cpp_version << "C++98/03";
#endif
    oss << " - " << cpp_version.str() << "[" << __cplusplus << "]";
#ifdef _OPENMP
    oss << "(OPENMP)";
#else
    oss << "(std::thread)";
#endif

    return oss.str();
}

// Parallel STL
#if defined(__clang__) || defined(_MSC_VER)
#define PARALLEL_STL 0
#else
#define PARALLEL_STL 1
#endif


#endif
