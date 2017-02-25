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

#ifndef _YMT_VALUE_H_
#define _YMT_VALUE_H_

#include "yobject.h"

#ifdef __cplusplus
extern "C" {
#endif

struct yvalue_meta {
    YOBJ_META
};

typedef enum {
    Bool = 1,
    Char,
    Byte,
    UByte,
    Short,
    UShort,
    Int,
    UInt,
    Long,
    ULong,
    LLong,
    ULLong,
    Float,
    Double,
    LDouble,
    Object,
    Min = Bool,
    Max = Object
} ytype;

YD_DEC_DATA(yvalue)
ytype type;
union {
    ybool z;
    ychar c;
    ybyte b;
    yubyte B;
    yshort s;
    yushort S;
    yint i;
    yuint I;
    ylong j;
    yulong J;
    yllong q;
    yullong Q;
    yfloat f;
    ydouble d;
    yldouble D;
    yobject l;
    yany p;
} value;
YD_END_DATA(yvalue)

Y_SHARED yclass ycls_for_yvalue(void);

Y_SHARED yvalue yval_of_bool(ybool);

Y_SHARED yvalue yval_of_char(ychar);

Y_SHARED yvalue yval_of_byte(ybyte);

Y_SHARED yvalue yval_of_ubyte(yubyte);

Y_SHARED yvalue yval_of_short(yshort);

Y_SHARED yvalue yval_of_ushort(yushort);

Y_SHARED yvalue yval_of_int(yint);

Y_SHARED yvalue yval_of_uint(yuint);

Y_SHARED yvalue yval_of_long(ylong);

Y_SHARED yvalue yval_of_ulong(yulong);

Y_SHARED yvalue yval_of_llong(yllong);

Y_SHARED yvalue yval_of_ullong(yullong);

Y_SHARED yvalue yval_of_float(yfloat);

Y_SHARED yvalue yval_of_double(ydouble);

Y_SHARED yvalue yval_of_ldouble(yldouble);

Y_SHARED yvalue yval_of_object(yobject);

Y_SHARED yvalue yval_of_any(const yany, yuint);

Y_SHARED ytype yval_type(yvalue);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
