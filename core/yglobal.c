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

#include "yglobal.h"
#include "stdio.h"
#include <string.h>
#include <stdarg.h>

#define _MARK L'%'
#define _MAX_BUF 24

int y_log(const ychar* restrict format, ...)
{
    va_list ap;
    va_start(ap, format);

    ysize count = 0;

    while(*format) {
        if(*format != _MARK) {
            putwchar(*format++);
            continue;
        }
        switch(*++format) {
        case L's':
            break;
        case L'd':
            break;
        }
    }

    va_end(ap);
    return count;
}

void y_array_copy(const void* src, yuint srcbeg, void* dest, yuint destbeg, ysize size, ysize length)
{
    memcpy(dest + size * destbeg, src + size * srcbeg, size * length);
}

void* y_array_dump(const void* src, yuint begin, ysize size, ysize length, ysize new_length)
{
    void* copy = calloc(new_length, size);
    if(nil == copy) {
        return nil;
    }
    y_array_copy(src, begin, copy, 0, size, length);
    return copy;
}
