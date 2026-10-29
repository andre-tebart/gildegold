/* 0x5773f0, the fight kind of the record 0x6f2310 (0x92 .. 0x97) merged with the flag S+0x38: -1 without a
   record / outside the kinds; flag 0: 0 for 0x92, 3 else; flag 1: 1 for 0x95, 4 else; flag 2: 2 */

int ai_Unk5773f0(void)
{
    unsigned short w;
    int v;

    if (!U_I((unsigned)&sv_PanelShown))
        return -1;
    w = *(unsigned short *)U_I((unsigned)&sv_PanelShown);
    v = w;
    if (v < 0x92 || v > 0x97)
        return -1;
    if (U_I(S + 0x38) == 0)
        return w == 0x92 ? 0 : 3;
    if (U_I(S + 0x38) == 1)
        return w == 0x95 ? 1 : 4;
    if (U_I(S + 0x38) == 2)
        return 2;
    return -1;
}
