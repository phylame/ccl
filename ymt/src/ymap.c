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
    ymap self = Y_ALLOC_OBJECT(ymap, ycls_for_ymap());
    if(self != nil) {
        self->capacity = capacity;
        self->factor = factor;
        self->table = y_new_array(yentry*, capacity);
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
    ymap_check(self);
    return self->size;
}

static yentry* get_entry_by_key(ymap self, yobject key);

yobject ymap_get(ymap self, yobject key)
{
    ymap_check(self);
    if(yobj_is_nil(key)) {
        return nil;
    }
    yentry* e = get_entry_by_key(self, key);
    return nil == e ? nil : e->value;
}

yobject ymap_get_or(ymap self, yobject key, yobject fallback)
{
    ymap_check(self);
    if(yobj_is_nil(key)) {
        return nil;
    }
    yentry* e = get_entry_by_key(self, key);
    return nil == e ? fallback : e->value;
}

static yentry* get_entry_by_hash(ymap self, int hash);

yobject ymap_get_for(ymap self, yuint hash)
{
    ymap_check(self);
    yentry* e = get_entry_by_hash(self, hash);
    return nil == e ? nil : e->value;
}

yobject ymap_get_or_for(ymap self, yuint hash, yobject fallback)
{
    ymap_check(self);
    yentry* e = get_entry_by_hash(self, hash);
    return nil == e ? fallback : e->value;
}

ybool ymap_has(ymap self, yobject key)
{
    if(yobj_is_nil(key)) {
        return false;
    }
    return ymap_get(self, key) != nil;
}

ybool ymap_has_hash(ymap self, yuint hash)
{
    return ymap_get_for(self, hash) != nil;
}

void ymap_put(ymap self, yobject key, yobject value)
{
    ymap_check(self);
    yobj_not_nil(key);
    yobj_not_nil(value);
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

static inline int index_for(int h, int length)
{
    return h & (length - 1);
}

yentry* get_entry_by_key(ymap self, yobject key)
{
    if(0 == self->size) {
        return nil;
    }
    const int hash = yobj_hash(key);
    yobject k;
    for(yentry* e = self->table[index_for(hash, self->capacity)]; e != nil; e = e->next) {
        k = e->key;
        if(hash == e->hash && (k == key || yobj_equals(key, k))) {
            return e;
        }
    }
    return nil;
}

yentry* get_entry_by_hash(ymap self, int hash)
{
    if(0 == self->size) {
        return nil;
    }

    for(yentry* e = self->table[index_for(hash, self->capacity)]; e != nil; e = e->next) {
        if(hash == e->hash) {
            return e;
        }
    }
    return nil;
}
