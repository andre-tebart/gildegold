/* 0x40f800, sets the emitter's trigger-once flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterTriggerOnce(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterTriggerOnce: invalid handle...");
        return 0;
    }
    em->triggeronce = *on;
    return 0;
}
