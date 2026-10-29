/* 0x40f1b0, sets the emitter's start phase speeds (*a..*c / 100), phase3 (*d / 100) and phase4 (*f / 100); 0,
   an invalid handle is a script error */

int ocmd_SetEmitterPhasespeed(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterPhasespeed: invalid handle...");
        return 0;
    }
    {
        float fa, fb, fc, fd, ff;

        fa = *a * 0.01f;
        fb = *b * 0.01f;
        fc = *c * 0.01f;
        ff = *f * 0.01f;
        fd = *d * 0.01f;
        em->phase[0] = fa;
        em->phase[1] = fb;
        em->phase[2] = fc;
        em->phase3 = fd;
        em->phase4 = ff;
    }
    return 0;
}
