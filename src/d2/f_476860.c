/* 0x476860, frees a fade */

void d2_ReleaseFade(D2FADE *f)
{
    int i;

    for (i = 0; i < 0x20; i++) {
        if (d2_Fades[i] == f) {
            d2_Fades[i] = 0;
            d3if_FreeSprite((int)f->sprite);
            mem_Free(f);
            return;
        }
    }
}
