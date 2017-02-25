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

#ifndef _YMT_CLASS_H_
#define _YMT_CLASS_H_

#include "ymtcore.h"

#ifdef __cplusplus
extern "C" {
#endif

#define Y_MAX_CLASS_NAME 256

Y_SHARED yclass ycls_for_yclass(void);

#define ycls_new(type, name, length, super) (type*) ycls_alloc(sizeof(type), name, length, super)

Y_SHARED yclass ycls_alloc(ysize size, const char* name, yint length, yclass super);

Y_SHARED const char* ycls_name(yclass self);

Y_SHARED yclass ycls_super(yclass self);

Y_SHARED ybool ycls_is_instance(yclass self, yobject object);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif
