#include "yclass.h"
#include "yobject.h"
#include "ystring.h"
#include <stdlib.h>
#include <string.h>

static yclass _yobj_meta_ = nil;
static yclass _ycls_meta_ = nil;

#define _y_retain_ycls_meta_() (yclass) yobj_retain((yobject)(_ycls_meta_ != nil ? _ycls_meta_ : ycls_for_yclass()))

#define _y_retain_yobj_meta_() (yclass) yobj_retain((yobject)(_yobj_meta_ != nil ? _yobj_meta_ : ycls_for_yobject()))

static yclass _y_alloc_class_(ysize size, const char* name, yint length, yclass super, yclass);

yclass ycls_for_yobject(void)
{
    if(nil == _yobj_meta_) {
        _yobj_meta_ = _y_alloc_class_(sizeof(struct ymt_class), "yobject", -1, nil, nil);
        if(nil == _yobj_meta_) {
            y_core_oom();
        }
        _yobj_meta_->_class_ = _y_retain_ycls_meta_();
        _yobj_meta_->_super_ = nil;
    }
    return _yobj_meta_;
}

yclass ycls_for_yclass(void)
{
    if(nil == _ycls_meta_) {
        _ycls_meta_ = _y_alloc_class_(sizeof(struct ymt_class), "yclass", -1, nil, nil);
        if(nil == _ycls_meta_) {
            y_core_oom();
        }
        _ycls_meta_->_class_ = _ycls_meta_;
        _ycls_meta_->_super_ = _y_retain_yobj_meta_();
    }
    return _ycls_meta_;
}

static yobject _y_alloc_object_(ysize size, yclass clazz);

yobject yobj_alloc(ysize size, yclass clazz)
{
    if(yobj_is_nil(clazz)) {
        y_die("%s cannot be nil", y_name_of(clazz));
    }
    yobject self = _y_alloc_object_(size, clazz);
    if(self != nil) {
        // init
    }
    return self;
}

// *********************** //
// *** yobject methods *** //
// *********************** //

static yuint _yobj_hash_(yobject self)
{
    return (uintptr_t)self;
}

static ybool _yobj_equal_(yobject self, yobject other)
{
    return self == other;
}

static ystring _yobj_string_(yobject self)
{
    char buf[Y_MAX_CLASS_NAME + 1 + sizeof(yuint)];
    sprintf(buf, "%s@%u", ycls_name(self->_class_), yobj_hash(self));
    return ystr_for_bytes(buf, -1);
}

yclass yobj_class(yobject self)
{
    yobj_not_nil(self);
    return self->_class_;
}

#define _invoke_hash_(self, fun, ...) return fun(self)

yuint yobj_hash(yobject self)
{
    yobj_not_nil(self);
    Y_FORWARD_METHOD(self, _hash_, _invoke_hash_);
    return _yobj_hash_(self);
}

#undef _invoke_hash_

#define _invoke_equal_(self, fun, ...) return fun(self, y_first_arg(__VA_ARGS__))

ybool yobj_equals(yobject self, yobject other)
{
    yobj_not_nil(self);
    if(yobj_is_nil(other)) {
        return false;
    }
    Y_FORWARD_METHOD(self, _equal_, _invoke_equal_, other);
    return _yobj_equal_(self, other);
}

#undef _invoke_hash_

#define _invoke_string_(self, fun, ...) return fun(self)

ystring yobj_string(yobject self)
{
    yobj_not_nil(self);
    Y_FORWARD_METHOD(self, _string_, _invoke_string_);
    return _yobj_string_(self);
}

#undef _invoke_string_

yobject yobj_retain(yobject self)
{
    yobj_not_nil(self);
    ++self->_count_;
    return self;
}

#define _invoke_destroy_(self, fun, ...) \
    fun(self);                           \
    break

yobject yobj_release(yobject self)
{
    if(yobj_is_nil(self)) {
        return nil;
    } else if(1 == self->_count_) {
        Y_FORWARD_METHOD(self, _destroy_, _invoke_destroy_);
        if(1 == self->_count_) { // may be retained by sub destroy
            free(self);
            return nil;
        } else {
            return self;
        }
    } else {
        --self->_count_;
        return self;
    }
}

#undef invoke_destroy

yuint yobj_count(yobject self)
{
    yobj_not_nil(self);
    return self->_count_;
}

static ybool _y_is_instance_of_(yclass clazz, yobject object);

void yobj_check(yobject self, yclass clazz)
{
    yobj_not_nil(self);
    yobj_not_nil((yclass)clazz);
    if(!_y_is_instance_of_(ycls_for_yclass(), (yobject)clazz)) {
        y_die("%s is not yclass", y_name_of(clazz));
    }

    if(!_y_is_instance_of_(clazz, self)) {
        y_die("object is not insance of %s", ycls_name(clazz));
    }
}

void yobj_print(yobject self)
{
    yobj_not_nil(self);
    wprintf(L"%s", ystr_chars(yobj_string(self)));
}

void yobj_println(yobject self)
{
    yobj_not_nil(self);
    wprintf(L"%s\n", ystr_chars(yobj_string(self)));
}

// ********************** //
// *** yclass methods *** //
// ********************** //

yclass ycls_alloc(ysize size, const char* name, yint length, yclass super)
{
    yclass self = _y_alloc_class_(size, name, length, super, ycls_for_yclass());
    if(self != nil) {
        self->_class_ = _y_retain_ycls_meta_();
    }
    return self;
}

const char* ycls_name(yclass self)
{
    yobj_check((yobject)self, ycls_for_yclass());
    return (const char*)self->_name_;
}

yclass ycls_super(yclass self)
{
    yobj_check((yobject)self, ycls_for_yclass());
    return self->_super_;
}

ybool ycls_is_instance(yclass self, yobject object)
{
    yobj_check((yobject)self, ycls_for_yclass());
    if(yobj_is_nil(object)) {
        return false;
    }
    return _y_is_instance_of_(self, object);
}

// ************************ //
// *** static functions *** //
// ************************ //

yobject _y_alloc_object_(ysize size, yclass clazz)
{
    if(0 == size) {
        y_die("size of object cannot be 0");
    }
    yobject self = calloc(1, size);
    if(nil == self) {
        return nil;
    }
    self->_class_ = clazz != nil ? (yclass)yobj_retain((yobject)clazz) : nil;
    self->_count_ = 1;
    self->_size_ = size;
    return self;
}

void _ycls_destroy_(yobject self)
{
    free(((yclass)self)->_name_);
}

yclass _y_alloc_class_(ysize size, const char* name, yint length, yclass super, yclass clazz)
{
    if(0 == size) {
        y_die("size of class cannot be 0");
    }
    if(nil == name || 0 == length) {
        y_die("name of class cannot be null or empty");
    } else if(length < 0) {
        length = strlen(name);
        if(0 == length) {
            y_die("name of class cannot be null or empty");
        }
    }
    if(length >= Y_MAX_CLASS_NAME) {
        y_die("length(%d) of class name more than %d", length, Y_MAX_CLASS_NAME);
    }

    yclass self = (yclass)_y_alloc_object_(size, clazz);
    if(nil == self) {
        return nil;
    }
    self->_name_ = y_array_dump(name, 0, sizeof(char), length, length + 1);
    if(nil == self->_name_) {
        free(self);
        return nil;
    }
    self->_length_ = length;
    self->_super_ = super != nil ? (yclass)yobj_retain((yobject)super) : nil;
    self->_destroy_ = _ycls_destroy_;
    return self;
}

ybool _y_is_instance_of_(yclass self, yobject object)
{
    yclass clazz = object->_class_;
    do {
        if(self == clazz) {
            return true;
        }
    } while(clazz != nil);
    return false;
}
