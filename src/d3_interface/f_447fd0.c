/* 0x447fd0, moves the top-level object `obj` (with its children) from scene `from` to the end of scene `to` */

char d3if_MoveObject(D3OBJECT *obj, D3IFSCENE *from, D3IFSCENE *to)
{
    D3IFMOVE m;
    D3OBJECT *o;

    if (!obj || !from || !to || obj->parent)
        return 0;
    if (from == to)
        return 1;
    if (from == (D3IFSCENE *)d3_DefaultScene) {
        from->first = d3_TopFirst;
        from->last = d3_TopLast;
        from->par1 = d3_ParHead;
        from->par2 = d3_ParTail;
    }
    for (o = from->first; o != &d3_ObjEnd; o = o->next)
        if (o == obj)
            break;
    if (o != obj)
        return 0;
    if (obj->prev == &d3_TopHead)
        from->first = obj->next;
    else
        obj->prev->next = obj->next;
    if (obj->next == &d3_ObjEnd)
        from->last = obj->prev;
    else
        obj->next->prev = obj->prev;
    if (from == (D3IFSCENE *)d3_DefaultScene) {
        d3_TopFirst = from->first;
        d3_TopLast = from->last;
        d3_ParHead = from->par1;
        d3_ParTail = from->par2;
    }
    m.from = from;
    m.to = to;
    d3_ForEachObject((D3SCENE *)from, obj, 0x3ff, (D3OBJECTCB)d3if_MoveCB, &m);
    if (to == (D3IFSCENE *)d3_DefaultScene) {
        to->first = d3_TopFirst;
        to->last = d3_TopLast;
        to->par1 = d3_ParHead;
        to->par2 = d3_ParTail;
    } else if (!to->first)
        d3if_InitScene(to);
    obj->next = &d3_ObjEnd;
    o = to->last;
    o->next = obj;
    obj->prev = o;
    to->last = obj;
    d3_InflateObject(obj);
    if (to == (D3IFSCENE *)d3_DefaultScene) {
        d3_TopFirst = to->first;
        d3_TopLast = to->last;
        d3_ParHead = to->par1;
        d3_ParTail = to->par2;
    }
    return 1;
}
