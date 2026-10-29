/* 0x40f690, sets the emitter's init-fill flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterInitFill(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterInitFill: invalid handle...");
        return 0;
    }
    em->initfill = *on;
    return 0;
}
