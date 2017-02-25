#include <stdio.h>
#include "ylist.h"

void foo(yobject o, yany arg)
{
    yobj_println(o);
    printf("%d\n", (int)arg);
}

int main(int argc, char** argv)
{
    ylist ls = ylst_new();
    yobj_println(ls);
    ylst_append(ls, ystr_for_chars(L"hello", -1));
    yitr_foreach(ylst_iterator(ls), foo, (yany)12);
    printf("%d, %d\n", ylst_size(ls), ylst_is_empty(ls));
    return 0;
}
