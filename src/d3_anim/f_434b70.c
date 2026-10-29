/* 0x434b70, drops a reference; unreferenced animations stay cached while they fit. */

void d3anim_Release(D3ANIMDATA *a, char now)
{
    D3ANIMDATA *p;
    int i, used, size;

    if (--a->refs > 0)
        return;
    if (now) {
        a->next->prev = a->prev;
        a->prev->next = a->next;
        for (i = 0; i < a->nframes; i++) {
            if (a->frames[i].pos) {
                mem_Free(a->frames[i].pos);
                a->frames[i].pos = 0;
            }
            if (a->frames[i].packed) {
                mem_Free(a->frames[i].packed);
                a->frames[i].packed = 0;
            }
            if (a->frames[i].normals) {
                mem_Free(a->frames[i].normals);
                a->frames[i].normals = 0;
            }
        }
        mem_Free(a->frames);
        a->frames = 0;
        mem_Free(a);
        return;
    }
    a->refs = 0;
    used = 0;
    for (p = d3anim_Head.next; p != &d3anim_Tail; p = p->next) {
        if (p->refs == 0)
            used += d3anim_GetSize(p);
    }
    size = d3anim_GetSize(a);
    if (size + used <= d3anim_CacheMax)
        return;
    if (d3anim_Purge(d3anim_CacheMax - size, used))
        return;
    d3anim_Release(a, 1);
}
