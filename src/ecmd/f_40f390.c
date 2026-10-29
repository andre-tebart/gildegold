/* 0x40f390, sets the emitter's accelerations from *x / *y / *z (in thousandths); 0, an invalid handle is a script
   error */

int ocmd_SetEmitterAcceleration(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterAcceleration: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.001f;
        fy = *y * 0.001f;
        fz = *z * 0.001f;
        em->acc[0] = fx;
        em->acc[1] = fy;
        em->acc[2] = fz;
    }
    return 0;
}
