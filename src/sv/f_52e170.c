/* 0x52e170, clears the name plate records sv_NamePlates */

void sv_ClearNamePlates(void)
{
    SVPLATE *p;

    p = sv_NamePlates;
    do {
        p->obj = 0;
        p++;
    } while ((int)p < (int)(sv_NamePlates + 0x20));
}
