/* 0x40f750, sets the emitter's texture mode from *mode; 0, an invalid handle is a script error */

int ocmd_SetEmitterTextureMode(D3EMITTER **e, int *mode)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterTextureMode: invalid handle...");
        return 0;
    }
    em->texmode = *mode;
    return 0;
}
