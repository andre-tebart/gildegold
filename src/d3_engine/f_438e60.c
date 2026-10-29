/* 0x438e60, the same without a context and without stopping. */

char d3_ForEachObject2(D3SCENE *scene, D3OBJECT *obj, unsigned int mask, void (*cb)(D3OBJECT *obj))
{
    D3SCENELISTS *s = (D3SCENELISTS *)scene;
    D3OBJECT *p;

    if (!mask)
        return 0;
    if (obj) {
        do {
            if (d3_TypeInMask(obj->type, mask))
                cb(obj);
            if (obj->child)
                d3_ForEachObject2(scene, obj->child, mask & ~0x200, cb);
            if (mask & 0x200)
                break;
            obj = obj->next;
            if (!obj)
                break;
        } while (!(obj->state & 1));
    } else {
        if (scene == d3_DefaultScene) {
            s->first = d3_TopFirst;
            s->last = d3_TopLast;
            s->parHead = d3_ParHead;
            s->parTail = d3_ParTail;
        }
        for (p = s->first; p && p != &d3_ObjEnd; p = p->next)
            d3_ForEachObject2(scene, p, mask, cb);
    }
    return 1;
}
