/* 0x40f320, sets the emitter's start velocities from *x / *y / *z (in hundredths); 0, an invalid handle is a script
   error */

int ocmd_SetEmitterVelocity(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterVelocity: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.01f;
        fy = *y * 0.01f;
        fz = *z * 0.01f;
        em->vel[0] = fx;
        em->vel[1] = fy;
        em->vel[2] = fz;
    }
    return 0;
}
