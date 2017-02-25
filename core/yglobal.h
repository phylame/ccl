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

#ifndef _YMT_GLOABL_
#define _YMT_GLOABL_

#include "ymtdef.h"
#include "yplatform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define y_die(...)                                                                  \
    do {                                                                            \
        fprintf(stderr, "fatal error in %s:%s:%d: ", __FILE__, __func__, __LINE__); \
        fprintf(stderr, ##__VA_ARGS__);                                             \
        fprintf(stderr, "\n");                                                      \
        abort();                                                                    \
    } while(0)

Y_SHARED int y_log(const ychar* restrict fmt, ...);

#define y_core_oom() y_die("no memory for creating core objects")

#define y_name_of(o) #o

#define y_first_arg(_0, ...) _0

#define y_escond_arg(_0, _1, ...) _1

#define y_third_arg(_0, _1, _2, ...) _2

#define y_new_item(type) calloc(1, sizeof(type))

#define y_new_array(type, count) calloc(count, sizeof(type))

#define y_resize_array(type, array, count) realloc(array, count * sizeof(type))

#define y_release_objects(objects, size)  \
    do {                                  \
        for(yuint i = 0; i < size; ++i) { \
            yobj_release(*(objects + i)); \
        }                                 \
    } while(0)

Y_SHARED void y_array_copy(const void* src, yuint srcbeg, void* dest, yuint destbeg, ysize size, ysize length);

Y_SHARED void* y_array_dump(const void* src, yuint begin, ysize size, ysize length, ysize new_length);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif
