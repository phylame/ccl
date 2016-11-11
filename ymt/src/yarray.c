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

#include "yarray.h"
#include "yclass.h"
#include <string.h>

#define _CHECK_RANGE_(self, index)                                  \
    if(index >= self->size) {                                       \
        y_die("index(%u) out of range[0, %Iu)", index, self->size); \
    }

#define _CHECK_ITEM_(item)                          \
    if(yobj_is_nil(item)) {                         \
        y_die("yarray does not accept nil object"); \
    }

static yclass _yary_meta_ = nil;

static void _yary_destroy_(yobject self);

yclass ycls_for_yarray(void)
{
    if(nil == _yary_meta_) {
        struct yarray_meta* meta = ycls_new(struct yarray_meta, "yarray", -1, ycls_for_yobject());
        if(nil == meta) {
            y_core_oom();
        }
        meta->_destroy_ = _yary_destroy_;
        _yary_meta_ = (yclass)meta;
    }
    return _yary_meta_;
}

static yarray _make_yarray_(void);

yarray yary_for_capacity(ysize capacity)
{
    yarray self = _make_yarray_();
    if(self != nil) {
        if(0 == capacity) {
            self->items = nil;
        } else {
            self->items = y_new_array(yobject, capacity);
            if(nil == self->items) {
                yobj_release((yobject)self);
                return nil;
            }
        }
        self->size = 0;
        self->capacity = capacity;
    }
    return self;
}

yarray yary_for_iterator(yiterator iterator)
{
    yarray self = yary_new();
    if(self != nil) {
        yary_extend(self, iterator);
    }
    return self;
}

yarray yary_for_objects(const yobject* objects, ysize size)
{
    if(nil == objects || 0 == size) {
        return yary_new();
    }
    yarray self = yary_for_capacity(size + 1);
    if(self != nil) {
        for(yuint i = 0; i < size; ++i) {
            self->items[i] = yobj_retain(objects[i]);
        }
    }
    return self;
}

static ybool _grow_items_(yarray self, ysize capacity);

void yary_resize(yarray self, ysize capacity)
{
    yary_check(self);
    ysize expand = (self->items != nil) ? 0 : YARY_INIT_CAPACITY;
    if(capacity > expand && capacity > self->capacity) {
        _grow_items_(self, capacity);
    }
}

ysize yary_size(yarray self)
{
    yary_check(self);
    return self->size;
}

yint yary_find(yarray self, yobject object)
{
    yary_check(self);
    if(yobj_is_nil(object)) {
        return Y_NOT_FOUND;
    }
    for(yuint i = 0; i < self->size; ++i) {
        if(yobj_equals(object, self->items[i])) {
            return i;
        }
    }
    return Y_NOT_FOUND;
}

yint yary_rfind(yarray self, yobject object)
{
    yary_check(self);
    if(yobj_is_nil(object)) {
        return Y_NOT_FOUND;
    }
    for(yuint i = self->size - 1; i >= 0; --i) {
        if(yobj_equals(object, self->items[i])) {
            return i;
        }
    }
    return Y_NOT_FOUND;
}

ysize yary_dump(yarray self, yobject* objects, yint count)
{
    yary_check(self);
    if(nil == objects || 0 == count) {
        return 0;
    }
    if(count < 0) {
        count = self->size;
    } else {
        count = count < self->size ? count : self->size;
    }
    for(yuint i = 0; i < count; ++i) {
        objects[i] = yobj_retain(self->items[i]);
    }
    return count;
}

yobject yary_first(yarray self)
{
    yary_check(self);
    return self->size != 0 ? self->items[0] : nil;
}

yobject yary_last(yarray self)
{
    yary_check(self);
    return self->size != 0 ? self->items[self->size - 1] : nil;
}

yobject yary_get(yarray self, yuint index)
{
    yary_check(self);
    _CHECK_RANGE_(self, index)
    return self->items[index];
}

ysize yary_count(yarray self, yobject object)
{
    yary_check(self);
    if(yobj_is_nil(object)) {
        return 0;
    }
    ysize num = 0;
    for(yuint i = 0; i < self->size; ++i) {
        if(yobj_equals(object, self->items[i])) {
            ++num;
        }
    }
    return num;
}

void yary_set(yarray self, yuint index, yobject object)
{
    yary_check(self);
    _CHECK_RANGE_(self, index)
    _CHECK_ITEM_(object)
    yobj_release(self->items[index]);
    self->items[index] = yobj_retain(object);
}

static void _ensure_capacity_(yarray self, ysize capacity);

void yary_append(yarray self, yobject obj)
{
    yary_check(self);
    _CHECK_ITEM_(obj)
    _ensure_capacity_(self, self->size + 1);
    self->items[self->size++] = yobj_retain(obj);
}

void yary_insert(yarray self, yuint index, yobject obj)
{
    yary_check(self);
    if(index > self->size) {
        y_die("index(%u) out of range[0, %Iu]", index, self->size);
    }
    _CHECK_ITEM_(obj)
    _ensure_capacity_(self, self->size + 1);
    yobject* items = self->items;
    for(yuint i = self->size; --i >= index;) {
        items[i + 1] = items[i];
    }
    items[index] = yobj_retain(obj);
    ++self->size;
}

void yary_extend(yarray self, yiterator iterator)
{
    yary_check(self);
    yitr_check(iterator);
    while(yitr_has(iterator)) {
        yary_append(self, yobj_retain(yitr_next(iterator)));
    }
}

static void _fast_remove_(yarray, yuint);

void yary_remove_at(yarray self, yuint index)
{
    yary_check(self);
    _CHECK_RANGE_(self, index)
    yobj_release(self->items[index]);
    _fast_remove_(self, index);
}

ybool yary_remove(yarray self, yobject object)
{
    yary_check(self);
    if(yobj_is_nil(object)) {
        return false;
    }
    for(yuint i = 0; i < self->size; ++i) {
        if(yobj_equals(object, self->items[i])) {
            _fast_remove_(self, i);
            yobj_release(object);
            return true;
        }
    }
    return false;
}

void yary_clear(yarray self)
{
    yary_check(self);
    y_release_objects(self->items, self->size);
    self->size = 0;
}

static ybool _batch_remove_(yarray, yiterator, ybool);

void yary_remove_in(yarray self, yiterator i)
{
    yary_check(self);
    _batch_remove_(self, i, false);
}

void yary_retain_in(yarray self, yiterator i)
{
    yary_check(self);
    _batch_remove_(self, i, true);
}

static yiterator _make_iterator_(yarray, yuint);

yiterator yary_iterator_from(yarray self, yuint index)
{
    yary_check(self);
    _CHECK_RANGE_(self, index)
    return _make_iterator_(self, index);
}

void _fast_remove_(yarray self, yuint index)
{
    const int moved = self->size - index - 1;
    if(moved > 0) {
        y_array_copy(self->items, index + 1, self->items, index, sizeof(yobject), moved);
    }
    --self->size;
}

ybool _batch_remove_(yarray self, yiterator iterator, ybool complement)
{
    yitr_check(iterator);
    const yarray objects = yary_for_iterator(iterator);
    ybool modified = false;
    yuint r = 0, w = 0;
    for(; r < self->size; ++r) {
        if(yary_has(objects, self->items[r]) == complement) {
            self->items[w++] = self->items[r];
        }
    }
    if(w != self->size) {
        y_release_objects(self->items + w, self->size);
        self->size = w;
        modified = true;
    }
    yobj_release((yobject)objects);
    return modified;
}

struct _yary_iterator_ {
    yarray array;
    yuint cursor;
    yint last;
};

static ybool _ai_test_(yany data)
{
    struct _yary_iterator_* ip = data;
    return ip->cursor < ip->array->size;
}

static yobject _ai_next_(yany data)
{
    struct _yary_iterator_* ip = data;
    if(ip->cursor >= ip->array->size) {
        yitr_error_stopped();
    }
    ip->last = ip->cursor++;
    return ip->array->items[ip->last];
}

static void _ai_remove_(yany data)
{
    struct _yary_iterator_* ip = data;
    if(ip->last < 0) {
        yitr_error_nostart();
    }
    yary_remove_at(ip->array, ip->last);
    ip->cursor = ip->last;
    ip->last = -1;
}

static void _ai_cleanup_(yany data)
{
    struct _yary_iterator_* ip = data;
    yobj_release((yobject)ip->array);
    free(ip);
}

yiterator _make_iterator_(yarray self, yuint from)
{
    struct _yary_iterator_* yi = y_new_item(struct _yary_iterator_);
    if(nil == yi) {
        return nil;
    }
    yi->array = (yarray)yobj_retain((yobject)self);
    yi->cursor = from;
    yi->last = -1;
    yitr_data data = {
        .data = yi, .cleanup = _ai_cleanup_, .test = _ai_test_, .next = _ai_next_, .remove = _ai_remove_
    };
    yiterator i = yitr_alloc(data);
    if(nil == i) {
        _ai_cleanup_(yi);
    }
    return i;
}

/* Empty array reuse scheme to save calls to malloc and free */
#ifndef YARY_POOL_SIZE
#define YARY_POOL_SIZE 80
#endif

static yarray _array_pool_[YARY_POOL_SIZE];
static ysize _free_number_ = 0;

void _yary_destroy_(yobject self)
{
    yarray my = (yarray)self;
    if(my->items != nil) {
        y_release_objects(my->items, my->size);
        free(my->items);
    }
    if(_free_number_ < YARY_POOL_SIZE) {
        _array_pool_[_free_number_++] = (yarray)yobj_retain(self);
    }
}

yarray _make_yarray_(void)
{
    yarray self;
    if(_free_number_ != 0) {
        self = _array_pool_[--_free_number_];
    } else {
        self = Y_ALLOC_OBJECT(yarray, ycls_for_yarray());
        if(nil == self) {
            return nil;
        }
    }
    return self;
}

ybool _grow_items_(yarray self, ysize capacity)
{
    const ysize old_capacity = self->capacity;
    ysize new_capacity = old_capacity + (old_capacity >> 1);
    if(new_capacity < capacity) {
        new_capacity = capacity;
    }

    yobject* items = self->items;
    items = (nil == items) ? y_new_array(yobject, new_capacity) : y_resize_array(yobject, items, new_capacity);
    if(nil == items) {
        return false;
    } else {
        self->items = items;
        self->capacity = new_capacity;
        return true;
    }
}

inline void _ensure_capacity_(yarray self, ysize capacity)
{
    if(nil == self->items) {
        capacity = YARY_INIT_CAPACITY > capacity ? YARY_INIT_CAPACITY : capacity;
    }
    if(capacity > self->capacity) {
        _grow_items_(self, capacity);
    }
}

#undef _CHECK_RANGE_
#undef _CHECK_ITEM_
