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

#include "yfcore.h"
#include "ylist.h"
#include "ystring.h"
#include <string.h>
#include <stdio.h>

static yuint _yobj_hash_(yobject self)
{
    return (uintptr_t)self;
}

static ybool _yobj_equals_(yobject self, yobject other)
{
    return self == other;
}

static ystring _yobj_string_(yobject self)
{
    char buf[Y_MAX_CLASS_NAME + 1 + sizeof(yuint)];
    sprintf(buf, "%s@%u", ycls_name(self->clazz), yobj_hash(self));
    return ystr_with_bytes(buf, -1);
}

yclass ycls_for_yobject(void)
{
    return _y_class_of_yobject_();
}

yobject_def
y_object_mkdef(yany data, yclass clazz, yf_hash hash, yf_equals equals, yf_string string, yf_destructor destructor)
{
    yobject_def definition = {
        .data = data, .clazz = clazz, .hash = hash, .equals = equals, .string = string, .destructor = destructor
    };
    return definition;
}

yobject yobj_new(yobject_def definition, yobject parent)
{
    if(yobj_is_nil(definition.clazz)) {
        y_die("class of object cannot be nil");
    }
    yobject self = _y_alloc_object_(&definition, parent);
    if(self != nil && self->parent != nil) {
        if(nil == parent->children) {
            parent->children = ylst_new();
        }
        ylst_append(parent->children, self);
    }
    return self;
}

yclass yobj_class(yobject self)
{
    yobj_not_nil(self);
    return self->clazz;
}

yany yobj_data(yobject self)
{
    yobj_not_nil(self);
    return self->data;
}

yuint yobj_hash(yobject self)
{
    yobj_not_nil(self);
    return self->hash != nil ? self->hash(self) : _yobj_hash_(self);
}

ybool yobj_equals(yobject self, yobject other)
{
    yobj_not_nil(self);
    if(nil == other) {
        return false;
    }
    return self->equals != nil ? self->equals(self, other) : _yobj_equals_(self, other);
}

ystring yobj_string(yobject self)
{
    yobj_not_nil(self);
    return self->string != nil ? self->string(self) : _yobj_string_(self);
}

yobject yobj_retain(yobject self)
{
    yobj_not_nil(self);
    ++self->count;
    return self;
}

static void yobj_delete(yobject);

yobject yobj_release(yobject self)
{
    if(yobj_is_nil(self)) {
        return nil;
    } else if(1 == self->count) {
        yobj_delete(self);
        return nil;
    } else {
        --self->count;
        return self;
    }
}

yuint yobj_count(yobject self)
{
    yobj_not_nil(self);
    return self->count;
}

void yobj_print(yobject self)
{
    yobj_not_nil(self);
    wprintf(L"%s", ystr_chars(yobj_string(self)));
}

void yobj_println(yobject self)
{
    yobj_not_nil(self);
    wprintf(L"%s\n", ystr_chars(yobj_string(self)));
}

void yobj_delete(yobject self)
{
    if(self->destructor != nil) {
        self->destructor(self);
    }
    ylist list = self->children;
    if(list != nil) {
        yobj_release(list);
    }
    if(self->parent != nil) {
        list = self->parent->children;
        if(nil == list) {
            y_die("BUG: object has parent, but parent's children is nil");
        }
        ylst_remove(list, self);
    }
    free(self);
}
