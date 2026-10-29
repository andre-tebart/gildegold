/* 0x40f560, sets the emitter's colour bytes r / g / b / a from *r / *g / *b / *a; 0, an invalid handle is a script
   error */

int ocmd_SetEmitterColor(D3EMITTER **e, int *r, int *g, int *b, int *a)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterColor: invalid handle...");
        return 0;
    }
    em->r = *r;
    em->g = *g;
    em->b = *b;
    em->a = *a;
    return 0;
}
