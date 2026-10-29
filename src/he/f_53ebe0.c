/* 0x53ebe0, removes the gfx infos of the object `obj` */

void he_RemoveGfxInfos(int obj)
{
    int *p = (int *)((unsigned)&he_RunHandlers + 0x2a0);

    do {
        if (p[2] == obj)
            he_Unk53e840(p);
        p += 4;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x6a0));
}
