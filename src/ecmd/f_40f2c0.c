/* 0x40f2c0, sets the emitter's start sizes from *x / *y / *z; 0, an invalid handle is a script error */

int ocmd_SetEmitterSize(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterSize: invalid handle...");
        return 0;
    }
    em->size[0] = (float)*x;
    em->size[1] = (float)*y;
    em->size[2] = (float)*z;
    return 0;
}
