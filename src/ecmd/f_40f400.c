/* 0x40f400, sets the emitter's random velocity range from *x / *y / *z (in hundredths); 0, an invalid handle is a
   script error */

int ocmd_SetEmitterRndVelocity(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterRndVelocity: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.01f;
        fy = *y * 0.01f;
        fz = *z * 0.01f;
        em->rnd[0] = fx;
        em->rnd[1] = fy;
        em->rnd[2] = fz;
    }
    return 0;
}
