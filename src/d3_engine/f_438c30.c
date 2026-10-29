/* 0x438c30, the next sibling that starts a new group (state bit 0), for top-level objects. */
D3OBJECT *d3_NextGroup(D3OBJECT *obj)
{
    D3OBJECT *parent = obj->parent;

    obj = obj->next;
    if (!parent)
        while (obj && !(obj->state & 1))
            obj = obj->next;
    return obj;
}
