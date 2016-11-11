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

#ifndef _YMT_LIST_H_
#define _YMT_LIST_H_

#include "yiterator.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ylist_meta {
    Y_OBJECT_META
};

typedef struct _ynode {
    yobject item;
    struct _ynode* prev;
    struct _ynode* next;
} * ynode;

Y_BEGIN_OBJECT(ylist)
ynode first;
ynode last;
ysize size;
Y_END_OBJECT(ylist)

Y_SHARED yclass ycls_for_ylist(void);

#define ylst_check(self) yobj_check((yobject)self, ycls_for_ylist())

Y_SHARED ylist ylst_new(void);

Y_SHARED ylist ylst_for_iterator(yiterator);

Y_SHARED ylist ylst_for_objects(const yobject*, ysize);

Y_SHARED ysize ylst_size(ylist);

#define ylst_is_empty(self) (yobj_is_nil(self) || ylst_size(self) == 0)

#define ylst_has(self, obj) (ylst_find(self, obj) != Y_NOT_FOUND)

#define ylst_iterator(self) ylst_is_empty(self) ? yitr_empty() : ylst_iterator_from(self, 0)

Y_SHARED yiterator ylst_iterator_from(ylist, yuint);

Y_SHARED yint ylst_find(ylist, yobject);

Y_SHARED yint ylst_rfind(ylist, yobject);

Y_SHARED ysize ylst_dump(ylist, yobject*, yint);

Y_SHARED yobject ylst_first(ylist);

Y_SHARED yobject ylst_last(ylist);

Y_SHARED yobject ylst_get(ylist, yuint);

Y_SHARED ysize ylst_count(ylist, yobject);

Y_SHARED yobject ylst_set(ylist, yuint, yobject);

Y_SHARED void ylst_append(ylist, yobject);

Y_SHARED void ylst_prepend(ylist, yobject);

Y_SHARED void ylst_insert(ylist, yuint, yobject);

Y_SHARED void ylst_extend(ylist, yiterator);

Y_SHARED yobject ylst_remove_at(ylist, yuint);

Y_SHARED ybool ylst_remove(ylist, yobject);

Y_SHARED void ylst_clear(ylist);

Y_SHARED void ylst_remove_in(ylist, yiterator);

Y_SHARED void ylst_retain_in(ylist, yiterator);

Y_SHARED yobject ylst_pop(ylist);

#define ylst_push(self, obj) ylst_append(self, obj)

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
