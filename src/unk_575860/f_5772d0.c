/* 0x5772d0, the fight kind of the record 0x6f2310: -1 without a record, for the kinds outside 0x92 .. 0x97 /
   0x2db, and while the flag S+0x38 is set; else 0 for the kind 0x92, 1 for the other kinds */

int ai_Unk5772d0(void)
{
    unsigned short w;
    int v;

    if (!U_I((unsigned)&sv_PanelShown))
        return -1;
    w = *(unsigned short *)U_I((unsigned)&sv_PanelShown);
    v = w;
    if (v < 0x92)
        return -1;
    if (v > 0x97 && v != 0x2db)
        return -1;
    if (U_I(S + 0x38))
        return -1;
    return w != 0x92;
}
