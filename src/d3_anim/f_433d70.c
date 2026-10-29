/* 0x433d70, gives the object a path animation through nkeys keys (copied; times in frames). @deep B draft: 20 code hunks (16 bytes off; position/angle copies per component, the inline-asm key copy D3PATH_COPY) - deep matching. Readability is blocked on shared fields: Q2 obj->rotation (118 refs; the d3.h union view made d3_MirDrawObject stop matching), Q4 D3_OBJPATH (its typed-field fix also regressed d3_MirDrawObject) and one raw cast. */

D3ANIM *d3anim_CreatePath(D3OBJECT *obj, D3ANIMFLAGS f, int nkeys, D3ANIMKEY *keys)
{
    D3PATH *p = 0;
    int i;

    if (D3_OBJPATH(obj))
        d3anim_FreePath(obj);
    if (obj->type == 4 || obj->type == 3 || obj->type0 == 4) {
        p = mem_Alloc(sizeof(D3PATH), "d3_int:CreateObjAnim");
        p->nkeys = nkeys;
        p->flags = f;
        p->unk10 = 100;
        p->fadeEnd = 1.0f;
        p->timeout = 0;
        p->flags.b[1] &= ~0x20;
        p->flags.b[8] = p->flags.b[8] & ~0x20 | 0x10;
        p->keys = mem_Alloc(nkeys * sizeof(D3PATHKEY), "d3_int:CrObjAnimFrameData");
        D3PATH_COPY(p->keys, keys, p->nkeys * sizeof(D3PATHKEY));
        p->keys[p->nkeys - 1].time = p->keys[p->nkeys - 2].time;
        for (i = 0; i < p->nkeys; i++)
            p->keys[i].time *= 3;
        p->pos.x = obj->pos.x;
        p->pos.y = obj->pos.y;
        p->pos.z = obj->pos.z;
        p->rot.x = obj->rotation.x;
        p->rot.y = obj->rotation.y;
        p->rot.z = obj->rotation.z;
        if (!(p->flags.b[1] & 2)) {
            p->key = 0;
            p->time = 0;
            p->next = 1;
        } else {
            p->key = p->nkeys - 1;
            p->time = p->keys[p->key].time - 1;
            p->next = d3anim_NextFrame(p->flags, p->key, 0, p->key, p->key);
        }
        d3path_Init(p);
    }
    D3_OBJPATH(obj) = p;
    ((D3ANIMOBJ *)obj)->time = d3_Time;
    return (D3ANIM *)p;
}
