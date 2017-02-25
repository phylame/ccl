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

#ifndef _YMT_OPOOL_H_
#define _YMT_OPOOL_H_

#include "yobject.h"
#include "ylist.h"

#ifdef __cplusplus
extern "C" {
#endif

struct yopool_meta {
    YOBJ_META
};

YD_DEC_DATA(yopool)
ylist objects;
YD_END_DATA(yopool)

Y_SHARED yclass ycls_for_yopool(void);

Y_SHARED yopool yopo_new(void);

Y_SHARED yopool yopo_current(void);

#define yopo_check(self) yobj_check((yobject)self, ycls_for_yopool())

Y_SHARED void yopo_drain(yopool);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
