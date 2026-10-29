/* 0x40f8f0, moves the particle system *ps to (*x, *y, *z) (par_SetPos); 0, an invalid handle is a script error */

int ocmd_SetParticlePos(D3PARTSYS **ps, int *x, int *y, int *z)
{
    D3PARTSYS *p = *ps;
    D3VECTOR4 pos;

    if (!mem_IsValid(p)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetParticlePos: invalid handle...");
        return 0;
    }
    pos.x = (float)*x;
    pos.y = (float)*y;
    pos.z = (float)*z;
    par_SetPos(p, (D3VECTOR *)&pos);
    return 0;
}
