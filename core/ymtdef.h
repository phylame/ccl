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

#ifndef _YMT_DEF_H_
#define _YMT_DEF_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "yplatform.h"

#ifdef __cplusplus
extern "C" {
#endif

#define YMT_VERSION_STR "1.0.0"

// YMT_VERSION is (major << 16) + (minor << 8) + patch.
#define YMT_VERSION 0x010000

// can be used like #if (YMT_VERSION >= YMT_VERSION_CHECK(4, 4, 0))
#define YMT_VERSION_CHECK(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

// base primitive types
typedef int8_t yint8;
typedef uint8_t yuint8;
typedef int16_t yint16;
typedef uint16_t yuint16;
typedef int32_t yint32;
typedef uint32_t yuint32;
typedef int64_t yint64;
typedef uint64_t yuint64;

// platform types
typedef bool ybool;
typedef wchar_t ychar;
typedef char ybyte;
typedef unsigned char yubyte;
typedef short yshort;
typedef unsigned short yushort;
typedef int yint;
typedef unsigned int yuint;
typedef long ylong;
typedef unsigned long yulong;
typedef long long yllong;
typedef unsigned long long yullong;
typedef float yfloat;
typedef double ydouble;
typedef long double yldouble;
typedef void* yany;

typedef size_t ysize;
typedef yint ystate;

/* core ymt object types */
typedef struct ymt_object* yobject;
typedef struct ymt_class* yclass;
typedef struct ymt_string* ystring;

/* constants */
enum yconstant { Y_LT = -1, Y_EQ = 0, Y_GT = 1, Y_NOT_FOUND = -1 };

/* represent no object to ymt reference */
#define nil NULL

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // _YMT_DEF_H_
