/* 0x40edc0, deletes the particle system *ps (par_Delete); an invalid handle is a script error; 0 */

int ocmd_KillParticle(D3PARTSYS **ps)
{
    if (mem_IsValid(*ps)) {
        par_Delete(*ps);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_KillParticle: invalid handle...");
    return 0;
}
