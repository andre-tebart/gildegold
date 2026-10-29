/* 0x53ec10, calls 0x4471b0 for the gfx infos of the object `obj` */

void he_Unk53ec10(int obj, int a)
{
    int *p = (int *)((unsigned)&he_RunHandlers + 0x2ac);

    do {
        if (p[-1] == obj)
            d3if_Show(*p, a);
        p += 4;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x6ac));
}
