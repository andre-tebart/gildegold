/* 0x40f5d0, sets the emitter's max trigger (*a / 100), texture mode *b, the init / rebirth fill flags *c / *d and
   the trigger flags *f / *g; 0, an invalid handle is a script error */

int ocmd_SetEmitterFlags(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f, int *g)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterFlags: invalid handle...");
        return 0;
    }
    em->maxtrigger = *a * 0.01f;
    em->texmode = *b;
    em->initfill = *c;
    em->rebirthfill = *d;
    em->istrigger = *f;
    em->triggeronce = *g;
    return 0;
}
