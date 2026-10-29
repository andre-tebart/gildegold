/* 0x40f110, sets the emitter's start amplitudes (*a..*c / 100), the end amplitude amp3 (*d / 100) and amp4
   (*f / 100); 0, an invalid handle is a script error */

int ocmd_SetEmitterAmplitude(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterAmplitude: invalid handle...");
        return 0;
    }
    {
        float fa, fb, fc, fd, ff;

        fa = *a * 0.01f;
        fb = *b * 0.01f;
        fc = *c * 0.01f;
        ff = *f * 0.01f;
        fd = *d * 0.01f;
        em->amp[0] = fa;
        em->amp[1] = fb;
        em->amp[2] = fc;
        em->amp3 = fd;
        em->amp4 = ff;
    }
    return 0;
}
