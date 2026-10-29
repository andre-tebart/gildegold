/* 0x40f0c0, deletes the emitter *e (par_Delete); an invalid handle is a script error; 0 */

int ocmd_KillEmitter(D3EMITTER **e)
{
    if (mem_IsValid(*e)) {
        par_Delete((D3PARTSYS *)*e);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "KillEmitter: invalid handle...");
    return 0;
}
