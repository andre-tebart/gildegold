/* 0x40f6f0, sets the emitter's rebirth-fill flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterRebirthFill(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterRebirthFill: invalid handle...");
        return 0;
    }
    em->rebirthfill = *on;
    return 0;
}
