/* 0x40f7a0, sets the emitter's is-trigger flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterIsTrigger(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterIsTrigger: invalid handle...");
        return 0;
    }
    em->istrigger = *on;
    return 0;
}
