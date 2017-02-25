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

#include "ystring.h"
#include "yobject.h"
#include "yclass.h"
#include <string.h>
#include <wchar.h>
#include <inttypes.h>

static ychar* _empty_chars_ = L"";

static int _compare_chars_(ychar* s1, ychar* s2, yuint size, ybool ignore_case);

static yclass _ystr_meta_ = nil;

static yuint _ystr_hash_(yobject);
static ybool _ystr_equal_(yobject, yobject);
static ystring _ystr_string_(yobject);
static void _ystr_destroy_(yobject);

yclass ycls_for_ystring(void)
{
    if(nil == _ystr_meta_) {
        struct ystring_meta* meta = ycls_new(struct ystring_meta, "ystring", -1, ycls_for_yobject());
        if(nil == meta) {
            y_core_oom();
        }
        meta->_hash_ = _ystr_hash_;
        meta->_equal_ = _ystr_equal_;
        meta->_string_ = _ystr_string_;
        meta->_destroy_ = _ystr_destroy_;
        _ystr_meta_ = (yclass)meta;
    }
    return _ystr_meta_;
}

static ystring _make_ystring_(ychar* chars, ysize length);

#define _ystr_check_range_(self, index)                               \
    if(index >= self->length) {                                       \
        y_die("index(%u) out of range[0, %Iu)", index, self->length); \
    }

ystring ystr_new(void)
{
    return _make_ystring_(_empty_chars_, 0);
}

ystring ystr_for_bytes(const char* chars, yint size)
{
    if(nil == chars) {
        return ystr_new();
    } else if(size < 0) {
        size = strlen(chars);
    }
    ychar* cs = y_new_array(ychar, size + 1);
    if(nil == cs) {
        return nil;
    }
    size = mbstowcs(cs, chars, size);
    ychar* ncs = y_resize_array(ychar, cs, size);
    if(nil == ncs) {
        free(cs);
    }
    ystring self = _make_ystring_(ncs, size);
    if(nil == self) {
        free(cs);
    }
    return self;
}

ystring ystr_for_chars(const ychar* chars, yint length)
{
    if(nil == chars) {
        return ystr_new();
    } else if(length < 0) {
        length = wcslen(chars);
    }
    ychar* cs = y_array_dump(chars, 0, sizeof(ychar), length, length + 1);
    if(nil == cs) {
        return nil;
    }
    return _make_ystring_(cs, length);
}

ystring ystr_copy_of(ystring other)
{
    ystring self = ystr_for_chars(other->chars, other->length);
    if(self != nil) {
        self->hash = other->hash;
    }
    return self;
}

ysize ystr_length(ystring self)
{
    yobj_check((yobject)self, ycls_for_ystring());
    return self->length;
}

ychar ystr_char_at(ystring self, yuint index)
{
    yobj_check((yobject)self, ycls_for_ystring());
    _ystr_check_range_(self, index) return *(self->chars + index);
}

const ychar* ystr_chars(ystring self)
{
    yobj_check((yobject)self, ycls_for_ystring());
    return self->chars;
}

ysize ystr_dump(ystring self, ychar* chars, yint count)
{
    yobj_check((yobject)self, ycls_for_ystring());
    if(nil == chars) {
        y_die("chars cannot be nil");
    }
    if(count < 0) {
        count = self->length;
    } else {
        count = count < self->length ? count : self->length;
    }
    y_array_copy(self->chars, 0, chars, 0, sizeof(ychar), count);
    return count;
}

yint ystr_compare(ystring self, ystring other)
{
    yobj_check((yobject)self, ycls_for_ystring());
    if(yobj_is_nil(other)) {
        return Y_GT;
    }
    if(self->length > other->length) {
        return Y_GT;
    } else if(self->length < other->length) {
        return Y_LT;
    }
    return _compare_chars_(self->chars, other->chars, self->length, false);
}

ystring ystr_concat(ystring self, ystring other)
{
    yobj_check((yobject)self, ycls_for_ystring());
    if(ystr_is_empty(other)) {
        return self;
    }
    ychar* chars = y_array_dump(self->chars, 0, sizeof(ychar), self->length, self->length + other->length + 1);
    if(nil == chars) {
        return nil;
    }
    y_array_copy(other->chars, 0, chars, self->length, sizeof(ychar), other->length);
    ystring str = ystr_for_chars(chars, self->length + other->length);
    if(nil == str) {
        free(chars);
    }
    return str;
}

ystring ystr_of_bool(ybool value)
{
    ychar* cs = value ? L"true" : L"false";
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_char(ychar value)
{
    ychar cs[] = { value };
    return ystr_for_chars(cs, 1);
}

ystring ystr_of_int(yint value)
{
    ychar cs[11] = { 0 };
    swprintf(cs, 10, L"%d", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_uint(yuint value)
{
    ychar cs[11] = { 0 };
    swprintf(cs, 10, L"%u", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_long(ylong value)
{
    ychar cs[21] = { 0 };
    swprintf(cs, 20, L"%ld", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_ulong(yulong value)
{
    ychar cs[21] = { 0 };
    swprintf(cs, 20, L"%lu", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_llong(yllong value)
{
    ychar cs[49] = { 0 };
    swprintf(cs, 48, L"%lld", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_ullong(yullong value)
{
    ychar cs[49] = { 0 };
    swprintf(cs, 48, L"%llu", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_double(ydouble value)
{
    ychar cs[65] = { 0 };
    swprintf(cs, 64, L"%f", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_ldouble(yldouble value)
{
    ychar cs[65] = { 0 };
    swprintf(cs, 64, L"%Lf", value);
    return ystr_for_chars(cs, -1);
}

ystring ystr_of_object(yobject value)
{
    return !yobj_is_nil(value) ? yobj_string(value) : ystr_for_chars(L"nil", 4);
}

yuint _ystr_hash_(yobject self)
{
    return ((ystring)self)->hash;
}

ybool _ystr_equal_(yobject self, yobject other)
{
    if(!ycls_is_instance(ycls_for_ystring(), other)) {
        return false;
    }
    ystring my = (ystring)self, it = (ystring)other;
    if(my->length != it->length) {
        return false;
    }
    return _compare_chars_(my->chars, it->chars, my->length, false);
}

ystring _ystr_string_(yobject self)
{
    return (ystring)self;
}

void _ystr_destroy_(yobject self)
{
    ystring my = (ystring)self;
    if(my->chars != nil && my->chars != _empty_chars_) {
        free(my->chars);
    }
}

ystring _make_ystring_(ychar* chars, ysize length)
{
    ystring self = yobj_new(struct ymt_string, ycls_for_ystring());
    if(self != nil) {
        self->hash = 0;
        self->chars = chars;
        self->length = length;
    }
    return self;
}

yint _compare_chars_(ychar* s1, ychar* s2, yuint size, ybool ignore_case)
{
    return ignore_case ? 0 : wcscmp(s1, s2);
}

#undef _ystr_check_range_
