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

#include "yoptional.h"
#include "yclass.h"

static yuint _yopt_hash_(yobject self)
{
    yoptional my = (yoptional)self;
    return yobj_hash(my->ref != nil ? my->ref : self);
}

static ystring _yopt_string_(yobject self)
{
    yoptional my = (yoptional)self;
    return yobj_string(my->ref != nil ? my->ref : self);
}

static ybool _yopt_equal_(yobject self, yobject other)
{
    yoptional my = (yoptional)self;
    return my->ref != nil ? yobj_equals(my->ref, other) : false;
}

static void _yopt_destroy_(yobject self)
{
    yobj_release(((yoptional)self)->ref);
}

static yoptional _make_optional_(yobject ref);

static yclass _yopt_meta_ = nil;

yclass ycls_for_yoptional(void)
{
    if(nil == _yopt_meta_) {
        struct yoptional_meta* meat = ycls_new(struct yoptional_meta, "yoptional", -1, ycls_for_yobject());
        if(nil == meat) {
            y_core_oom();
        }
        meat->_hash_ = _yopt_hash_;
        meat->_equal_ = _yopt_equal_;
        meat->_string_ = _yopt_string_;
        meat->_destroy_ = _yopt_destroy_;
        _yopt_meta_ = (yclass)meat;
    }
    return _yopt_meta_;
}

yoptional yopt_of(yobject obj)
{
    if(yobj_is_nil(obj)) {
        y_die("%s cannot be null", y_name_of(obj));
    }
    return _make_optional_(yobj_retain(obj));
}

yoptional yopt_of_nullable(yobject obj)
{
    return _make_optional_(!yobj_is_nil(obj) ? yobj_retain(obj) : nil);
}

ybool yopt_is_present(yoptional self)
{
    yobj_check((yobject)self, ycls_for_yoptional());
    return !yobj_is_nil(self->ref);
}

yobject yopt_get(yoptional self)
{
    yobj_check((yobject)self, ycls_for_yoptional());
    if(yobj_is_nil(self->ref)) {
        y_die("optional value is null");
    }
    return self->ref;
}

yoptional _make_optional_(yobject ref)
{
    yoptional self = Y_ALLOC_OBJECT(yoptional, ycls_for_yoptional());
    if(self != nil) {
        self->ref = ref;
    }
    return self;
}
