/* 0x52bb70, runs the key handler 0x5309a0 (0, keys & ~0xc0 | 0x100200) for as long as the object id (0x4284a0) is flagged +0x124 & 1 */

void sv_Unk52bb70(int id)
{
    unsigned fl;
    char *o;

    fl = (SV_U((unsigned)&sv_FrameMask) & 0xffffff3f) | 0x100200;
    o = (int)evt_FindById(id);
    while (o != 0 && (*(unsigned char *)(o + 0x124) & 1) != 0) {
        sv_RunFrame(0, fl);
        o = (int)evt_FindById(id);
    }
}
