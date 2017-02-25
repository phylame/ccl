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

#include "yvalue.h"
#include "yobject.h"
#include "yclass.h"
#include "ystring.h"
#include <string.h>

static yclass _yval_meta_ = nil;

static yuint _yval_hash_(yobject self);
static ystring _yval_string_(yobject self);
static ybool _yval_equal_(yobject self, yobject other);

yclass ycls_for_yvalue(void)
{
    if(nil == _yval_meta_) {
        struct yvalue_meta* meta = ycls_new(struct yvalue_meta, "yvalue", -1, ycls_for_yobject());
        if(nil == meta) {
            y_core_oom();
        }
        meta->_hash_ = _yval_hash_;
        meta->_equal_ = _yval_equal_;
        meta->_string_ = _yval_string_;
        _yval_meta_ = (yclass)meta;
    }
    return _yval_meta_;
}

#define _make_yvalue_(_type, field, _value)                 \
    yvalue self = YD_ALLOC_DATA(yvalue, ycls_for_yvalue()); \
    if(self != nil) {                                       \
        self->type = _type;                                 \
        self->value.field = _value;                         \
    }                                                       \
    return self;

yvalue yval_of_bool(ybool value)
{
    _make_yvalue_(Bool, z, value);
}

yvalue yval_of_char(ychar value)
{
    _make_yvalue_(Char, c, value);
}

yvalue yval_of_byte(ybyte value)
{
    _make_yvalue_(Byte, b, value);
}

yvalue yval_of_ubyte(yubyte value)
{
    _make_yvalue_(UByte, B, value);
}

yvalue yval_of_short(yshort value)
{
    _make_yvalue_(Short, s, value);
}

yvalue yval_of_ushort(yushort value)
{
    _make_yvalue_(UShort, S, value);
}

yvalue yval_of_int(yint value)
{
    _make_yvalue_(Int, i, value);
}

yvalue yval_of_uint(yuint value)
{
    _make_yvalue_(UInt, I, value);
}
yvalue yval_of_long(ylong value)
{
    _make_yvalue_(Long, j, value);
}

yvalue yval_of_ulong(yulong value)
{
    _make_yvalue_(ULong, J, value);
}

yvalue yval_of_llong(yllong value)
{
    _make_yvalue_(LLong, q, value);
}

yvalue yval_of_ullong(yullong value)
{
    _make_yvalue_(ULLong, Q, value);
}

yvalue yval_of_float(yfloat value)
{
    _make_yvalue_(Float, f, value);
}

yvalue yval_of_double(ydouble value)
{
    _make_yvalue_(Double, d, value);
}

yvalue yval_of_ldouble(yldouble value)
{
    _make_yvalue_(LDouble, D, value);
}

yvalue yval_of_object(yobject value)
{
    _make_yvalue_(Object, l, value);
}

yvalue yval_of_any(const yany value, yuint type)
{
    _make_yvalue_(type, p, value);
}

#undef _make_yvalue_

ytype yval_type(yvalue self)
{
    yobj_check((yobject)self, ycls_for_yvalue());
    return self->type;
}

yuint _yval_hash_(yobject self)
{
    yvalue my = (yvalue)self;
    switch(my->type) {
    case Bool:
        return 0;
    case Char:
        return 0;
    case Byte:
        return 0;
    case UByte:
        return 0;
    case Short:
        return 0;
    case UShort:
        return 0;
    case Int:
        return 0;
    case UInt:
        return my->value.i;
    case Long:
        return 0;
    case ULong:
        return 0;
    case LLong:
        return 0;
    case ULLong:
        return 0;
    case Float:
        return 0;
    case Double:
        return 0;
    case LDouble:
        return 0;
    case Object:
        return yobj_hash(my->value.l);
    default:
        return (uintptr_t)my->value.p;
    }
}

ystring _yval_string_(yobject self)
{
    yvalue my = (yvalue)self;
    switch(my->type) {
    case Bool:
        return ystr_of_bool(my->value.z);
    case Char:
        return ystr_of_char(my->value.c);
    case Byte:
        return ystr_of_byte(my->value.b);
    case UByte:
        return ystr_of_ubyte(my->value.B);
    case Short:
        return ystr_of_short(my->value.s);
    case UShort:
        return ystr_of_ushort(my->value.S);
    case Int:
        return ystr_of_int(my->value.i);
    case UInt:
        return ystr_of_uint(my->value.I);
    case Long:
        return ystr_of_long(my->value.j);
    case ULong:
        return ystr_of_ulong(my->value.J);
    case LLong:
        return ystr_of_llong(my->value.q);
    case ULLong:
        return ystr_of_ullong(my->value.Q);
    case Float:
        return ystr_of_float(my->value.f);
    case Double:
        return ystr_of_double(my->value.d);
    case LDouble:
        return ystr_of_ldouble(my->value.D);
    case Object:
        return yobj_string(my->value.l);
    default: {
        ychar cs[64];
        swprintf(cs, 64, L"user[%d]@%p", my->type, my->value.p);
        return ystr_for_chars(cs, -1);
    }
    }
}

ybool _yval_equal_(yobject self, yobject other)
{
    yvalue my = (yvalue)self, it = (yvalue)other;
    if(my->type != it->type) {
        return false;
    }
    switch(my->type) {
    case Bool:
        return my->value.z == it->value.z;
    case Char:
        return my->value.c == it->value.c;
    case Byte:
        return my->value.b == it->value.b;
    case UByte:
        return my->value.B == it->value.B;
    case Short:
        return my->value.s == it->value.s;
    case UShort:
        return my->value.S == it->value.S;
    case Int:
        return my->value.i == it->value.i;
    case UInt:
        return my->value.I == it->value.I;
    case Long:
        return my->value.j == it->value.j;
    case ULong:
        return my->value.J == it->value.J;
    case LLong:
        return my->value.q == it->value.q;
    case ULLong:
        return my->value.Q == it->value.Q;
    case Float:
        return my->value.f == it->value.f;
    case Double:
        return my->value.d == it->value.d;
    case LDouble:
        return my->value.D == it->value.D;
    case Object:
        return yobj_equals(my->value.l, it->value.l);
    default:
        return my->value.p == it->value.p;
    }
}
