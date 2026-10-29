/* 0x434af0, frees the least recently used unreferenced animations until size <= limit. */

char d3anim_Purge(int limit, int size)
{
    D3ANIMDATA *a, *oldest;
    unsigned int t;

    if (limit < 0)
        return 0;
    while (size > limit) {
        oldest = 0;
        t = d3anim_Clock;
        for (a = d3anim_Head.next; a != &d3anim_Tail; a = a->next) {
            if (a->refs == 0 && a->lastuse <= t) {
                t = a->lastuse;
                oldest = a;
            }
        }
        if (!oldest)
            return 0;
        size -= d3anim_GetSize(oldest);
        d3anim_Release(oldest, 1);
    }
    return 1;
}
