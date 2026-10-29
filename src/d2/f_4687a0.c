/* 0x4687a0, drops the picture that was used longest ago (not locked, not in use); 0 if there is none */

int d2_FreeOldest(void)
{
    int i, best;
    unsigned int t;
    D2PIC *p;

    best = -1;
    t = d3_Time;
    for (i = 1; i < d2_NPics; i++) {
        p = &d2_Pics[i];
        if (p->lastuse < t && p->refs <= 0 && !(p->flags & 1) && p->data && p->size) {
            t = p->lastuse;
            best = i;
        }
    }
    if (best == -1)
        return 0;
    mem_Free(d2_Pics[best].data);
    d2_Pics[best].data = NULL;
    d2_Pics[best].data = NULL;     /* the original stores it twice (it reloads d2_Pics in between) */
    d2_PicBytes -= d2_Pics[best].size;
    return 1;
}
