/* 0x472a60, ends every live fade: frees the ones flagged remove-when-done (8), marks the others done (4) */

void d2_FinishFades(void)
{
    int i;
    D2FADE *p;

    for (i = 0; i < 32; i++) {
        p = d2_Fades[i];
        if (p) {
            if (p->flags & 8) {
                d2_ReleaseFade(p);
                d2_Fades[i] = 0;
            } else
                p->flags |= 4;
        }
    }
}
