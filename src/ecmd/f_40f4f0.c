/* 0x40f4f0, sets the emitter's particle time (*t) and the three alpha values *a / *b / *c; 0, an invalid handle is
   a script error */

int ocmd_SetEmitterTimeAndAlpha(D3EMITTER **e, int *a, int *b, int *c, int *t)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterTimeAndAlpha: invalid handle...");
        return 0;
    }
    em->alpha[0] = *a;
    em->alpha[1] = *b;
    em->alpha[2] = *c;
    em->time = (float)*t;
    return 0;
}
