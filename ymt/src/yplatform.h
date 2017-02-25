/*
 * Copyright 2016 Peng Wan <phylame@163.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _YMT_PLATFORM_H_
#define _YMT_PLATFORM_H_

/*  detect the operating system type using compiler predefined macros */
#if defined(_WIN32)
  #define Y_OS_WINDOWS
  #if defined(_WIN64)
    #define Y_OS_WINDOWS_64
  #else
    #define Y_OS_WINDOWS_32
  #endif
#elif defined(__linux__)
  #define Y_OS_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
  #define Y_OS_DARWIN
  #include <TargetConditionals.h>
  #if TARGET_IPHONE_SIMULATOR == 1
    #define Y_OS_IOS_SIMULATOR
  #elif TARGET_OS_IPHONE == 1
    #define Y_OS_IOS
  #elif TARGET_OS_MAC == 1
    #define Y_OS_OSX
  #endif
#elif defined(__CYGWIN__)
  #define Y_OS_CYGWIN
#elif defined(_AIX)
  #define Y_OS_AIX
#elif defined(__hpux)
  #define Y_OS_HPUX
#elif defined(__sun) && defined(__SVR4)
  #define Y_OS_SOLARIS
#endif

#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
  #define Y_OS_UNIX
  #include <unistd.h>
  #if defined(_POSIX_VERSION)
    #define Y_OS_POSIX
  #endif
#endif

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
  #include <sys/param.h>
  #if defined(BSD)
    #define Y_OS_BSD
  #endif
#endif

#if defined(__STDC_VERSION__)
  #if __STDC_VERSION__ >= 201112L
    #define Y_USE_C11
  #elif __STDC_VERSION__ >= 199901L
    #define Y_USE_C99
  #elif __STDC_VERSION__ >= 199401L
    #define Y_USE_C90
  #endif
#endif

#if !defined(Y_USE_C89) && defined(Y_OS_WINDOWS)
  #define Y_USE_C89 /* broadly, Windows is C89 */
#endif

#ifndef __has_attribute
  #define __has_attribute(x) 0
#endif
#if (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4) && (__GNUC_MINOR__ > 2))) || __has_attribute(visibility)
  #define Y_EXPORT __attribute__((visibility("default")))
  #define Y_IMPORT __attribute__((visibility("default")))
#elif defined(Y_OS_WINDOWS)
  #define Y_EXPORT __declspec(dllexport)
  #define Y_IMPORT __declspec(dllimport)
#else
  #define Y_EXPORT
  #define Y_IMPORT extern
#endif

#if defined(Y_OS_WINDOWS)
  #if defined(Y_STATIC)
    #define Y_SHARED extern
  // To build DLL, Y_BUILD must be defined.
  #elif defined(Y_BUILD)
    #define Y_SHARED Y_EXPORT
  #else
    #define Y_SHARED Y_IMPORT
  #endif
#else
  #define Y_SHARED extern
#endif

#endif
