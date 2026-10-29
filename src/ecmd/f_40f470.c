/* 0x40f470, sets the emitter's plane normal from *x / *y / *z (in hundredths) and the plane distance *d; 0, an
   invalid handle is a script error */

int ocmd_SetEmitterPlane(D3EMITTER **e, int *x, int *y, int *z, int *d)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterPlane: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.01f;
        fy = *y * 0.01f;
        fz = *z * 0.01f;
        em->plane[0] = fx;
        em->plane[1] = fy;
        em->plane[2] = fz;
    }
    em->plane3 = (float)*d;
    return 0;
}
