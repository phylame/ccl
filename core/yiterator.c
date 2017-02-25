#include "yiterator.h"
#include "yobject.h"
#include "yclass.h"

static void _yitr_destroy_(yobject self)
{
    yiterator my = (yiterator)self;
    if(my->cleanup != nil) {
        my->cleanup(my->data);
    }
}

static yclass _yitr_meta_ = nil;

yclass ycls_for_yiterator(void)
{
    if(nil == _yitr_meta_) {
        struct yiterator_meta* meta = ycls_new(struct yiterator_meta, "yiterator", -1, ycls_for_yobject());
        if(nil == meta) {
            y_core_oom();
        }
        meta->_destroy_ = _yitr_destroy_;
        _yitr_meta_ = (yclass)meta;
    }
    return _yitr_meta_;
}

yiterator yitr_alloc(yitr_data data)
{
    if(nil == data.data) {
        y_die("%s cannot be nil", y_name_of(data.data));
    }
    if(nil == data.test) {
        y_die("%s cannot be nil", y_name_of(data.test));
    }
    if(nil == data.next) {
        y_die("%s cannot be nil", y_name_of(data.next));
    }
    yiterator self = YD_ALLOC_DATA(yiterator, ycls_for_yiterator());
    if(self != nil) {
        self->data = data.data;
        self->test = data.test;
        self->next = data.next;
        self->remove = data.remove;
        self->cleanup = data.cleanup;
    }
    return self;
}

ybool yitr_has(yiterator self)
{
    yobj_check((yobject)self, ycls_for_yiterator());
    return self->test(self->data);
}

yobject yitr_next(yiterator self)
{
    if(!yitr_has(self)) {
        yitr_error_stopped();
    }
    return self->next(self->data);
}

void yitr_remove(yiterator self)
{
    yobj_check((yobject)self, ycls_for_yiterator());
    if(nil == self->remove) {
        yitr_error_unsupported("remove");
    }
    self->remove(self->data);
}

void yitr_foreach(yiterator self, yf_consumer consumer, yany arg)
{
    if(nil == consumer) {
        return;
    }
    while(yitr_has(self)) {
        consumer(yitr_next(self), arg);
    }
}

static ybool _ie_test_(yany data)
{
    return false;
}

static yobject _ie_next_(yany data)
{
    yitr_error_nostart();
}

yiterator yitr_empty(void)
{
    yiterator self = YD_ALLOC_DATA(yiterator, ycls_for_yiterator());
    if(self != nil) {
        self->data = nil;
        self->test = _ie_test_;
        self->next = _ie_next_;
    }
    return self;
}
