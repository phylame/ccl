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

#ifndef _YMT_OPTIONAL_H_
#define _YMT_OPTIONAL_H_

#include "yobject.h"

#ifdef __cplusplus
extern "C" {
#endif

struct yoptional_meta {
    Y_OBJECT_META
};

Y_BEGIN_OBJECT(yoptional)
yobject ref;
Y_END_OBJECT(yoptional)

Y_SHARED yclass ycls_for_yoptional(void);

Y_SHARED yoptional yopt_of(yobject);

Y_SHARED yoptional yopt_of_nullable(yobject);

Y_SHARED ybool yopt_is_present(yoptional);

Y_SHARED yobject yopt_get(yoptional);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
