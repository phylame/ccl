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

#ifndef _YMT_ARRAY_H_
#define _YMT_ARRAY_H_

#include "yiterator.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef YARY_INIT_CAPACITY
#define YARY_INIT_CAPACITY 16
#endif

struct yarray_meta {
    YOBJ_META
};

YD_DEC_DATA(yarray)
ysize size;
ysize capacity;
yobject* items;
YD_END_DATA(yarray)

Y_SHARED yclass ycls_for_yarray(void);

#define yary_check(self) yobj_check((yobject)self, ycls_for_yarray())

#define yary_new() yary_for_capacity(YARY_INIT_CAPACITY)

Y_SHARED yarray yary_for_capacity(ysize);

Y_SHARED yarray yary_for_iterator(yiterator);

Y_SHARED yarray yary_for_objects(const yobject*, ysize);

Y_SHARED void yary_resize(yarray, ysize);

Y_SHARED ysize yary_size(yarray);

#define yary_is_empty(self) (yobj_is_nil(self) || yary_size(self) == 0)

#define yary_has(self, obj) (yary_find(self, obj) != Y_NOT_FOUND)

#define yary_iterator(self) yary_is_empty(self) ? yitr_empty() : yary_iterator_from(self, 0)

Y_SHARED yiterator yary_iterator_from(yarray, yuint);

Y_SHARED yint yary_find(yarray, yobject);

Y_SHARED yint yary_rfind(yarray, yobject);

Y_SHARED ysize yary_dump(yarray, yobject*, yint);

Y_SHARED yobject yary_first(yarray);

Y_SHARED yobject yary_last(yarray);

Y_SHARED yobject yary_get(yarray, yuint);

Y_SHARED ysize yary_count(yarray, yobject);

Y_SHARED void yary_set(yarray, yuint, yobject);

Y_SHARED void yary_append(yarray, yobject);

#define yary_prepend(self, obj) yary_insert(self, 0, obj)

Y_SHARED void yary_insert(yarray, yuint, yobject);

Y_SHARED void yary_extend(yarray, yiterator);

Y_SHARED void yary_remove_at(yarray, yuint);

Y_SHARED ybool yary_remove(yarray, yobject);

Y_SHARED void yary_clear(yarray);

Y_SHARED void yary_remove_in(yarray, yiterator);

Y_SHARED void yary_retain_in(yarray, yiterator);

Y_SHARED void yary_sort(yarray);

Y_SHARED void yary_reverse(yarray);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
