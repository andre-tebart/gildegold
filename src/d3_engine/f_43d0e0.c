/* 0x43d0e0, unlinks obj from its siblings and parent. */

void d3_Unlink(D3OBJECT *obj)
{
    if (obj->next) {
        obj->next->prev = obj->prev;
        if (obj->prev)
            obj->prev->next = obj->next;
        else if (obj->parent->child == obj)
            obj->parent->child = obj->next;
    } else if (obj->prev)
        obj->prev->next = 0;
    else if (obj->parent->child == obj)
        obj->parent->child = 0;
    obj->parent = 0;
    obj->prev = 0;
    obj->next = 0;
    obj->state &= 0xfd;
}
