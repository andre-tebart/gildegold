/* 0x43cb20, appends obj after the last sibling of first. @deep B: 48 code hunks, no meaning difference after reading the original disassembly 0x43cb20..0x43cb7f: both sides set obj->state bit 0 = !first->parent (sete/xor/and/xor), store obj->parent = first->parent and walk first->next to the last sibling, then last->next = obj, obj->prev = last. The original keeps first in ecx (the loop cursor is the parameter itself) and obj in esi with ONE shared tail store (43cb69); ours loads obj into ecx and first into esi and VC6 duplicates the two tail stores for the empty-list path (43cb77..43cb7d). The same statements in another allocation / spelling; a local `last` cursor was tried and compiles to the same 48 hunks. */

void d3_AppendSibling(D3OBJECT *first, D3OBJECT *obj)
{
    D3_OBJBITS(obj).top = !first->parent;
    obj->parent = first->parent;
    while (first->next)
        first = first->next;
    first->next = obj;
    obj->prev = first;
}
