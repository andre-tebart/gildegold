/* 0x478fc0, frees a surface */

int d2_FreeSurface(D2SURF *s)
{
    if (s == 0)
        return 0;
    if (s->locked != 0)
        d2_UnlockSurface(s);
    if (s->external == 0) {
        if (s->bits != 0) {
            mem_Free(s->bits);
            s->bits = 0;
        }
        if (s->DDSurface != 0)
            dd_ReleaseSurface(s->DDSurface);
    }
    mem_Free(s);
    return 1;
}
