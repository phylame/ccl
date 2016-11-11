#include "yopool.h"
#include "yclass.h"

static yclass _yopo_meta_ = nil;
static ylist _pool_stack_ = nil;

static void _yopo_destroy_(yobject);

yclass ycls_for_yopool(void)
{
    if(nil == _yopo_meta_) {
        struct yopool_meta* meta = ycls_new(struct yopool_meta, "yopool", -1, ycls_for_yobject());
        if(nil == meta) {
            y_core_oom();
        }
        meta->_destroy_ = _yopo_destroy_;
        _yopo_meta_ = (yclass)meta;
    }
    return _yopo_meta_;
}

yopool yopo_new(void)
{
    yopool self = Y_ALLOC_OBJECT(yopool, ycls_for_yopool());
    if(self != nil) {
        self->objects = ylst_new();
        if(nil == _pool_stack_) {
            _pool_stack_ = ylst_new();
        }
        ylst_push(_pool_stack_, (yobject)self);
        yobj_release((yobject)self);
    }
    return self;
}

yopool yopo_current(void)
{
    return _pool_stack_ != nil ? (yopool)ylst_last(_pool_stack_) : nil;
}

void yopo_drain(yopool self)
{
    yopo_check(self);
    yiterator i = ylst_iterator(self->objects);
    while(yitr_has(i)) {
        yobj_release(yitr_next(i));
    }
}

void _yopo_destroy_(yobject self)
{
    yopo_drain((yopool)self);
    ylst_remove(_pool_stack_, self);
}
