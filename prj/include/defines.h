//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                               DS,CONFIG Macros
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
#ifndef FTNACWLRUCDTKQHXMLSYPEJZLUZBRVCVUMSUUPTWEMUVFVKHXLBQEYZYDMCPWT
#define FTNACWLRUCDTKQHXMLSYPEJZLUZBRVCVUMSUUPTWEMUVFVKHXLBQEYZYDMCPWT

#include <vector>
#include <string>

//****************************** SELECT_ANY ******************************

#if defined(_WIN32) || defined(_WIN64)
#define SELECT_ANY  __declspec(selectany)
#elif defined(__linux__)
#define SELECT_ANY  __attribute__((weak))
#elif defined(__APPLE__)
#define SELECT_ANY  __attribute__((weak))
#endif

//****************************** OPENMP ******************************
#if defined(_WIN32) || defined(_WIN64)
#if defined(__GNUC__)
#ifdef _OPENMP
#include "omp.h"
#else
#endif
#endif
#endif

//****************************** std::execution ******************************
#if defined(_MSC_VER) || defined(__clang__)
#define EXECUTION
#else
#include <execution>
#define EXECUTION std::execution::par
#endif

//****************************** INLINE ******************************
#ifdef DO_FORCE_INLINE
#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif
#else
#define FORCE_INLINE static
#endif

#if defined(_MSC_VER)
#include<memory.h>
#define ALLOCA _alloca
#else
#include<alloca.h>
#define ALLOCA alloca
#endif

#define _4MB_ 4194304 // 1,048,576 int
#define _1MB_ 1048576

#endif
