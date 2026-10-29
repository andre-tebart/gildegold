/* 0x40f8a0, raises the emitter's trigger bit; 0, an invalid handle is a script error */

int ocmd_TriggerEmitter(D3EMITTER **e)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "TriggerEmitter: invalid handle...");
        return 0;
    }
    em->trigger = 1;
    return 0;
}
