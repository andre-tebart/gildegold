/* 0x52e190, releases the name plates: the objects lose their plate hook (+0x224), the records are cleared */

void sv_ReleaseNamePlates(void)
{
    SVPLATE *p;

    p = sv_NamePlates;
    do {
        if (p->obj != 0)
            ((SVPLATEOBJ *)p->obj)->plate = 0;
        p->obj = 0;
        p++;
    } while ((int)p < (int)(sv_NamePlates + 0x20));
}
