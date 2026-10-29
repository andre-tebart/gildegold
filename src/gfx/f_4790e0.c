/* 0x4790e0, unlocks a surface */

int d2_UnlockSurface(D2SURF *s)
{
    if (s->locked == 0)
        return 0;
    if (s->DDSurface != 0) {
        dd_UnlockSurface(s->DDSurface);
        s->bits = 0;
    }
    s->locked = 0;
    return 1;
}
