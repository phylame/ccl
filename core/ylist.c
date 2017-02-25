#include "ylist.h"
#include "yclass.h"
#include "yobject.h"

#define _CHECK_RANGE_(self, index)                                  \
    if(index >= self->size) {                                       \
        y_die("index(%u) out of range[0, %Iu)", index, self->size); \
    }

#define _CHECK_ITEM_(obj)                        \
    if(yobj_is_nil(obj)) {                       \
        y_die("ylist does not accept nil item"); \
    }

static yclass _ylst_meta_ = nil;

static void _ylst_destroy_(yobject);

yclass ycls_for_ylist(void)
{
    if(nil == _ylst_meta_) {
        struct ylist_meta* meta = ycls_new(struct ylist_meta, "ylist", -1, ycls_for_yobject());
        if(nil == meta) {
            y_core_oom();
        }
        meta->_destroy_ = _ylst_destroy_;
        _ylst_meta_ = (yclass)meta;
    }
    return _ylst_meta_;
}

ylist ylst_new(void)
{
    ylist self = YD_ALLOC_DATA(ylist, ycls_for_ylist());
    if(self != nil) {
        self->first = nil;
        self->last = nil;
        self->size = 0;
    }
    return self;
}

ylist ylst_for_iterator(yiterator iterator)
{
    ylist self = ylst_new();
    if(self != nil) {
        ylst_extend(self, iterator);
    }
    return self;
}

ylist ylst_for_objects(const yobject* objects, ysize size)
{
    ylist self = ylst_new();
    if(nil == self) {
        return nil;
    }
    if(nil == objects || 0 == size) {
        return self;
    }
    for(yuint i = 0; i < size; ++i) {
        ylst_append(self, objects[i]);
    }
    return self;
}

ysize ylst_size(ylist self)
{
    ylst_check(self);
    return self->size;
}

static yiterator _make_iterator_(ylist, yuint);

yiterator ylst_iterator_from(ylist self, yuint from)
{
    ylst_check(self);
    _CHECK_RANGE_(self, from)
    return _make_iterator_(self, from);
}

yint ylst_find(ylist self, yobject obj)
{
    ylst_check(self);
    if(yobj_is_nil(obj)) {
        return Y_NOT_FOUND;
    }
    yuint index = 0;
    for(ynode node = self->first; node != nil; node = node->next) {
        if(yobj_equals(obj, node->item)) {
            return index;
        }
        ++index;
    }
    return Y_NOT_FOUND;
}

yint ylst_rfind(ylist self, yobject obj)
{
    ylst_check(self);
    if(yobj_is_nil(obj)) {
        return Y_NOT_FOUND;
    }
    yuint index = self->size;
    for(ynode node = self->last; node != nil; node = node->prev) {
        --index;
        if(yobj_equals(obj, node->item)) {
            return index;
        }
    }
    return Y_NOT_FOUND;
}

ysize ylst_dump(ylist self, yobject* objects, yint count)
{
    ylst_check(self);
    if(nil == objects || 0 == count) {
        return 0;
    }
    if(count < 0) {
        count = self->size;
    } else {
        count = count < self->size ? count : self->size;
    }
    yuint i = 0;
    for(ynode node = self->first; node != nil && i != count; node = node->next, ++i) {
        *(objects + i) = yobj_retain(node->item);
    }
    return count;
}

yobject ylst_first(ylist self)
{
    ylst_check(self);
    return self->first != nil ? self->first->item : nil;
}

yobject ylst_last(ylist self)
{
    ylst_check(self);
    return self->last != nil ? self->last->item : nil;
}

static ynode _node_at_(ylist, yuint);

yobject ylst_get(ylist self, yuint index)
{
    ylst_check(self);
    _CHECK_RANGE_(self, index)
    return _node_at_(self, index)->item;
}

ysize ylst_count(ylist self, yobject obj)
{
    ylst_check(self);
    if(yobj_is_nil(obj)) {
        return 0;
    }
    ysize count = 0;
    for(ynode node = self->first; node != nil; node = node->next) {
        if(yobj_equals(obj, node->item)) {
            return ++count;
        }
    }
    return count;
}

yobject ylst_set(ylist self, yuint index, yobject object)
{
    ylst_check(self);
    _CHECK_RANGE_(self, index)
    _CHECK_ITEM_(object)
    ynode node = _node_at_(self, index);
    yobject old = node->item;
    node->item = yobj_retain(object);
    return yobj_release(old);
}

static ynode _make_node_(ynode, yobject, ynode);

void ylst_append(ylist self, yobject object)
{
    ylst_check(self);
    _CHECK_ITEM_(object)
    ynode node = _make_node_(self->last, object, nil);
    if(nil == node) {
        y_log(L"cannot create node of ylist");
        return;
    }
    if(nil == self->last) {
        self->first = node;
    } else {
        self->last->next = node;
    }
    self->last = node;
    ++self->size;
}

void ylst_prepend(ylist self, yobject object)
{
    ylst_check(self);
    _CHECK_ITEM_(object)
    ynode node = _make_node_(nil, object, self->first);
    if(nil == node) {
        y_log(L"cannot create node of ylist");
        return;
    }
    if(nil == self->first) {
        self->last = node;
    } else {
        self->first->prev = node;
    }
    self->first = node;
    ++self->size;
}

static ybool _link_before_(ylist, yobject, ynode);

void ylst_insert(ylist self, yuint index, yobject obj)
{
    ylst_check(self);
    if(index > self->size) {
        y_die("index(%u) out of range[0, %Iu]", index, self->size);
    }
    _CHECK_ITEM_(obj)
    if(index == self->size) {
        ylst_append(self, obj);
    } else {
        _link_before_(self, obj, _node_at_(self, index));
    }
}

void ylst_extend(ylist self, yiterator iterator)
{
    ylst_check(self);
    yitr_check(iterator);
    while(yitr_has(iterator)) {
        ylst_append(self, yobj_retain(yitr_next(iterator)));
    }
}

static yobject _unlink_node_(ylist, ynode);

yobject ylst_remove_at(ylist self, yuint index)
{
    ylst_check(self);
    _CHECK_RANGE_(self, index)
    return _unlink_node_(self, _node_at_(self, index));
}

ybool ylst_remove(ylist self, yobject obj)
{
    ylst_check(self);
    if(yobj_is_nil(obj)) {
        return false;
    }
    for(ynode node = self->first; node != nil; node = node->next) {
        if(yobj_equals(obj, node->item)) {
            _unlink_node_(self, node);
            return true;
        }
    }
    return false;
}

static void _remove_all_(ylist);

void ylst_clear(ylist self)
{
    ylst_check(self);
    _remove_all_(self);
}

void ylst_remove_in(ylist self, yiterator i)
{
    ylist items = ylst_for_iterator(i);
    if(nil == items) {
        return;
    }
    for(ynode node = self->first; node != nil; node = node->next) {
        if(ylst_has(items, node->item)) {
            _unlink_node_(self, node);
        }
    }
}

void ylst_retain_in(ylist self, yiterator i)
{
    ylist items = ylst_for_iterator(i);
    if(nil == items) {
        return;
    }
    for(ynode node = self->first; node != nil; node = node->next) {
        if(!ylst_has(items, node->item)) {
            _unlink_node_(self, node);
        }
    }
}

yobject ylst_pop(ylist self)
{
    ylst_check(self);
    if(0 == self->size) {
        return nil;
    }
    return _unlink_node_(self, self->last);
}

ynode _make_node_(ynode prev, yobject item, ynode next)
{
    ynode node = y_new_item(struct _ynode);
    if(nil == node) {
        return nil;
    }
    node->prev = prev;
    node->item = yobj_retain(item);
    node->next = next;
    return node;
}

void _ylst_destroy_(yobject self)
{
    _remove_all_((ylist)self);
}

void _remove_all_(ylist self)
{
    if(0 == self->size) {
        return;
    }
    for(ynode node = self->first, next = nil; node != nil;) {
        next = node->next;
        yobj_release(node->item);
        free(node);
        node = next;
    }
    self->first = self->last = nil;
    self->size = 0;
}

ynode _node_at_(ylist self, yuint index)
{
    if(index < (self->size >> 1)) {
        ynode node = self->first;
        for(yuint i = 0; i < index; ++i) {
            node = node->next;
        }
        return node;
    } else {
        ynode node = self->last;
        for(yuint i = self->size - 1; i > index; --i) {
            node = node->prev;
        }
        return node;
    }
}

ybool _link_before_(ylist self, yobject item, ynode node)
{
    ynode _new = _make_node_(node->prev, item, node);
    if(nil == _new) {
        return false;
    }
    if(nil == node->prev) {
        self->first = _new;
    } else {
        node->prev->next = _new;
    }
    node->prev = _new;
    ++self->size;
    return true;
}

yobject _unlink_node_(ylist self, ynode node)
{
    yobject item = node->item;
    ynode prev = node->prev, next = node->next;
    if(nil == prev) {
        self->first = next;
    } else {
        prev->next = next;
        node->prev = nil;
    }

    if(nil == next) {
        self->last = prev;
    } else {
        next->prev = prev;
        node->next = nil;
    }
    free(node);
    --self->size;
    return yobj_release(item);
}

struct _ylst_iterator_ {
    ylist list;
    ynode first;
    ynode last;
};

static ybool _li_test_(yany data)
{
    struct _ylst_iterator_* ip = data;
    return ip->first != nil;
}

static yobject _li_next_(yany data)
{
    struct _ylst_iterator_* ip = data;
    if(nil == ip->first) {
        yitr_error_stopped();
    }
    ip->last = ip->first;
    ip->first = ip->first->next;
    return ip->last->item;
}

static void _li_remove_(yany data)
{
    struct _ylst_iterator_* ip = data;
    if(nil == ip->last) {
        yitr_error_nostart();
    }
    _unlink_node_(ip->list, ip->last);
    ip->first = ip->first->next;
    ip->last = nil;
}

static void _li_cleanup_(yany data)
{
    struct _ylst_iterator_* ip = data;
    for(; ip->first != nil; ip->first = ip->first->next) {
        yobj_release((yobject)ip->first->item);
    }
    free(ip);
}

yiterator _make_iterator_(ylist self, yuint from)
{
    struct _ylst_iterator_* li = y_new_item(struct _ylst_iterator_);
    if(nil == li) {
        return nil;
    }
    li->list = (ylist)yobj_retain((yobject)self);
    li->first = _node_at_(self, from);
    li->last = nil;
    yitr_data data = {
        .data = li, .cleanup = _li_cleanup_, .test = _li_test_, .next = _li_next_, .remove = _li_remove_
    };
    yiterator i = yitr_alloc(data);
    if(nil == i) {
        _li_cleanup_(li);
    }
    return i;
}
