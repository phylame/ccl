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

#ifndef _YMT_CORE_H_
#define _YMT_CORE_H_

#include "ymtdef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* macro for creating object data, must be the head of structure. */
#define Y_OBJECT                              \
    yclass _class_; /* class of the object */ \
    ysize _count_;  /* reference count */     \
    ysize _size_;   /* size of the object strucutre */

/* macro for creating class object, must be the head of structure. */
#define Y_OBJECT_META                                                                    \
    Y_OBJECT                                                                             \
    char* _name_;   /* name of this class */                                             \
    ysize _length_; /* length of the name */                                             \
    yclass _super_; /* super class of this class */                                      \
    /* list overridable functions of yobject */                                          \
    yuint (*_hash_)(yobject);          /* get hash code of the object */                 \
    bool (*_equal_)(yobject, yobject); /* equal operation of the object and the other */ \
    ystring (*_string_)(yobject);      /* make ystring represented the object */         \
    void (*_destroy_)(yobject);        /* destroy the object */

/* structure of yobject */
struct ymt_object {
    Y_OBJECT
};

/* structure of yclass */
struct ymt_class {
    Y_OBJECT_META
};

#define _Y_META_NAME_(name) _ymt_##name##_d_

#define _Y_META_POINTER_(name) name

#define Y_BEGIN_OBJECT(name)             \
    typedef struct _Y_META_NAME_(name) { \
    Y_OBJECT

#define Y_END_OBJECT(name) \
    }                      \
    *_Y_META_POINTER_(name);

#define Y_ALLOC_OBJECT(type, clazz) (_Y_META_POINTER_(type)) yobj_alloc(sizeof(struct _Y_META_NAME_(type)), clazz)

#define Y_FORWARD_METHOD(self, name, how, ...)         \
    do {                                               \
        yclass clazz = self->_class_;                  \
        while(clazz != nil) {                          \
            if(clazz->name != nil) {                   \
                how(self, clazz->name, ##__VA_ARGS__); \
            } else {                                   \
                clazz = clazz->_super_;                \
            }                                          \
        };                                             \
    } while(false)

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
