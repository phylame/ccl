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

#ifndef _YMT_OBJECT_H_
#define _YMT_OBJECT_H_

#include "ymtcore.h"
#include "yglobal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn yclass ycls_for_yobject(void)
 * @brief gets the class of yobject
 */
Y_SHARED yclass ycls_for_yobject(void);

/**
 * @fn yobject yobj_alloc(ysize size, yclass clazz)
 * @brief creates a new object with specified size and class
 * @param size size of the object data
 * @param clazz class of the object
 */
Y_SHARED yobject yobj_alloc(ysize size, yclass clazz);

Y_SHARED yclass yobj_class(yobject self);

Y_SHARED yuint yobj_hash(yobject self);

Y_SHARED ybool yobj_equals(yobject self, yobject other);

Y_SHARED ystring yobj_string(yobject self);

Y_SHARED yobject yobj_retain(yobject self);

Y_SHARED yobject yobj_release(yobject self);

Y_SHARED yuint yobj_count(yobject self);

#define yobj_is_nil(obj) (nil == obj || obj->_count_ == 0)

#define yobj_not_nil(obj)                       \
    do {                                        \
        if(yobj_is_nil(obj)) {                  \
            y_die("%s is nil", y_name_of(obj)); \
        }                                       \
    } while(false);

Y_SHARED void yobj_check(yobject self, yclass clazz);

Y_SHARED void yobj_print(yobject self);

Y_SHARED void yobj_println(yobject self);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
