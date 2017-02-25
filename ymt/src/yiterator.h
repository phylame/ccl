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

#ifndef _YMT_ITERATOR_H_
#define _YMT_ITERATOR_H_

#include "yobject.h"

#ifdef __cplusplus
extern "C" {
#endif

struct yiterator_meta {
    Y_OBJECT_META
};

#define _YITR_DATA_        \
    yany data;             \
    ybool (*test)(yany);   \
    yobject (*next)(yany); \
    void (*remove)(yany);  \
    void (*cleanup)(yany);

typedef struct {
    _YITR_DATA_
} yitr_data;

Y_BEGIN_OBJECT(yiterator)
_YITR_DATA_
Y_END_OBJECT(yiterator)

#undef _YITR_DATA_

typedef void (*yf_consumer)(yobject, yany);
typedef ybool (*yf_filter)(yobject, yany);

Y_SHARED yclass ycls_for_yiterator(void);

Y_SHARED yiterator yitr_alloc(yitr_data);

Y_SHARED yiterator yitr_empty(void);

#define yitr_check(self)                                           \
    do {                                                           \
        if(!ycls_is_instance(ycls_for_yiterator(), (yobject)self)) \
            y_die("not a yiterator");                              \
    } while(0)

#define yitr_error_stopped() y_die("no more item in iterator")

#define yitr_error_unsupported(what) y_die("%s is unsupported in iterator", what)

#define yitr_error_nostart() y_die("iterator is not started")

Y_SHARED ybool yitr_has(yiterator);

Y_SHARED yobject yitr_next(yiterator);

Y_SHARED void yitr_remove(yiterator);

Y_SHARED void yitr_foreach(yiterator, yf_consumer, yany);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
