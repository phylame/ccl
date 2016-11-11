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

#ifndef _YMT_MAP_H_
#define _YMT_MAP_H_

#include "yiterator.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef YMAP_INIT_CAPACITY
#define YMAP_INIT_CAPACITY 16
#endif

#ifndef YMAP_INIT_FACTOR
#define YMAP_INIT_FACTOR 0.75F
#endif

struct ymap_meta {
    Y_OBJECT_META
};

typedef struct _ymap_entry_ {
    yuint hash;
    yobject key;
    yobject value;
    struct _ymap_entry_* next;
} yentry;

Y_BEGIN_OBJECT(ymap)
ysize size;
ydouble factor;
ysize capacity;
yentry** table;
Y_END_OBJECT(ymap)

Y_SHARED yclass ycls_for_ymap(void);

Y_SHARED ymap ymap_new(void);

Y_SHARED ymap ymap_for_capacity(ysize capacity, ydouble factor);

Y_SHARED ymap ymap_for_entries(yiterator entries);

#define ymap_check(self) yobj_check((yobject)self, ycls_for_ymap())

Y_SHARED ysize ymap_size(ymap self);

#define ymap_is_empty(self) (yobj_is_nil(self) || ymap_size(self) == 0)

Y_SHARED yobject ymap_get(ymap self, yobject key);

Y_SHARED yobject ymap_get_or(ymap self, yobject key, yobject fallback);

Y_SHARED yobject ymap_get_for(ymap self, yuint hash);

Y_SHARED yobject ymap_get_or_for(ymap self, yuint hash, yobject fallback);

Y_SHARED ybool ymap_has(ymap self, yobject key);

Y_SHARED ybool ymap_has_hash(ymap self, yuint hash);

Y_SHARED void ymap_put(ymap self, yobject key, yobject value);

Y_SHARED void ymap_update(ymap self, yiterator entries);

Y_SHARED void ymap_remove(ymap self, yobject key);

Y_SHARED void ymap_remove_for(ymap self, yuint hash);

Y_SHARED void ymap_clear(ymap self);

Y_SHARED ybool ymap_has_value(ymap self, yobject value);

Y_SHARED yiterator ymap_keys(ymap self);

Y_SHARED yiterator ymap_values(ymap self);

Y_SHARED yiterator ymap_entries(ymap self);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
