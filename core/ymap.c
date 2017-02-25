#include "ymap.h"
#include "yclass.h"

static yclass _ymap_meta_ = nil;

yclass ycls_for_ymap(void)
{
    if(nil == _ymap_meta_) {
        struct ymap_meta* meta = ycls_new(struct ymap_meta, "ymap", -1, ycls_for_yobject());
        if(nil == meta) {
            y_core_oom();
        }
        _ymap_meta_ = (yclass)meta;
    }
    return _ymap_meta_;
}

ymap ymap_new(void)
{
    return ymap_for_capacity(YMAP_INIT_CAPACITY, YMAP_INIT_FACTOR);
}

ymap ymap_for_capacity(ysize capacity, ydouble factor)
{
    if(factor <= 0) {
        y_die("illegal load factor: %f", factor);
    }
    ymap self = YD_ALLOC_DATA(ymap, ycls_for_ymap());
    if(self != nil) {
        self->capacity = capacity;
        self->factor = factor;
    }
    return self;
}

ymap ymap_for_entries(yiterator entries)
{
    ymap self = ymap_new();
    if(self != nil) {
        ymap_update(self, entries);
    }
    return self;
}

ysize ymap_size(ymap self)
{
}

yobject ymap_get(ymap self, yobject key)
{
}

yobject ymap_get_or(ymap self, yobject key, yobject fallback)
{
}

yobject ymap_get_for(ymap self, yuint hash)
{
}

yobject ymap_get_or_for(ymap self, yuint hash, yobject fallback)
{
}

ybool ymap_has(ymap self, yobject key)
{
}

ybool ymap_has_hash(ymap self, yuint hash)
{
}

void ymap_put(ymap self, yobject key, yobject value)
{
}

void ymap_update(ymap self, yiterator entries)
{
}

void ymap_remove(ymap self, yobject key)
{
}

void ymap_remove_for(ymap self, yuint hash)
{
}

void ymap_clear(ymap self)
{
}

ybool ymap_has_value(ymap self, yobject value)
{
}

yiterator ymap_keys(ymap self)
{
}

yiterator ymap_values(ymap self)
{
}

yiterator ymap_entries(ymap self)
{
}
