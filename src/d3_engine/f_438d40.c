/* 0x438d40, calls cb for every object of the masked types below obj (all top-level objects of the
   scene if obj is 0). 0x200 in mask: obj only, not its siblings. cb returns 0 to stop, < 0 to skip
   the children. */

char d3_ForEachObject(D3SCENE *scene, D3OBJECT *obj, unsigned int mask, D3OBJECTCB cb, void *ctx)
{
    D3SCENELISTS *s = (D3SCENELISTS *)scene;
    D3OBJECT *p;
    char r;

    if (!mask)
        return 0;
    if (obj) {
        do {
            r = 1;
            if (d3_TypeInMask(obj->type, mask) && !(r = cb(obj, ctx)))
                return 0;
            if (obj->child && !(r & 0x80))
                r = d3_ForEachObject(scene, obj->child, mask & ~0x200, cb, ctx);
            if (!r)
                return 0;
            if (mask & 0x200)
                return 1;
            obj = obj->next;
            if (!obj)
                return 1;
        } while (!(obj->state & 1));
        return 1;
    }
    if (scene == d3_DefaultScene) {
        s->first = d3_TopFirst;
        s->last = d3_TopLast;
        s->parHead = d3_ParHead;
        s->parTail = d3_ParTail;
    }
    for (p = s->first; p && p != &d3_ObjEnd; p = p->next)
        if (!d3_ForEachObject(scene, p, mask, cb, ctx))
            return 0;
    return 1;
}
