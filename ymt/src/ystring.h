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

#ifndef _YMT_STRING_H_
#define _YMT_STRING_H_

#include "yobject.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ystring_meta {
    Y_OBJECT_META
};

struct ymt_string {
    Y_OBJECT
    ychar* chars;
    ysize length;
    yuint hash;
};

Y_SHARED yclass ycls_for_ystring(void);

Y_SHARED ystring ystr_new(void);

Y_SHARED ystring ystr_for_bytes(const char*, yint);

Y_SHARED ystring ystr_for_chars(const ychar*, yint);

Y_SHARED ystring ystr_copy_of(ystring);

Y_SHARED ysize ystr_length(ystring);

#define ystr_is_empty(self) (yobj_is_nil((yobject)self) || ystr_length(self) == 0)

Y_SHARED ychar ystr_char_at(ystring, yuint);

Y_SHARED const ychar* ystr_chars(ystring);

Y_SHARED ysize ystr_dump(ystring, ychar*, yint);

Y_SHARED yint ystr_compare(ystring, ystring);

Y_SHARED ystring ystr_concat(ystring, ystring);

Y_SHARED ystring ystr_concat_chars(ystring, ychar*, ysize);

Y_SHARED ystring ystr_of_bool(ybool);

Y_SHARED ystring ystr_of_char(ychar);

#define ystr_of_byte(value) ystr_of_int(value)

#define ystr_of_ubyte(value) ystr_of_uint(value)

#define ystr_of_short(value) ystr_of_int(value)

#define ystr_of_ushort(value) ystr_of_uint(value)

Y_SHARED ystring ystr_of_int(yint);

Y_SHARED ystring ystr_of_uint(yuint);

Y_SHARED ystring ystr_of_long(ylong);

Y_SHARED ystring ystr_of_ulong(yulong);

Y_SHARED ystring ystr_of_llong(yllong);

Y_SHARED ystring ystr_of_ullong(yullong);

#define ystr_of_float(value) ystr_of_double(value)

Y_SHARED ystring ystr_of_double(ydouble);

Y_SHARED ystring ystr_of_ldouble(yldouble);

Y_SHARED ystring ystr_of_object(yobject);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
