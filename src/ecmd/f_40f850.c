/* 0x40f850, sets the emitter's max trigger from *n; 0, an invalid handle is a script error */

int ocmd_SetEmitterMaxTrigger(D3EMITTER **e, int *n)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterMaxTrigger: invalid handle...");
        return 0;
    }
    em->maxtrigger = (float)*n;
    return 0;
}
