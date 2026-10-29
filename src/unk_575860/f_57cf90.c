/* 0x57cf90, the tutorial window handler (3264 B, ROUGH SKELETON - the 0x57c690 shape): arg 1 = the record; the
   lookup 0x4eaa50 (record), the dword record +0x65, 0x4eb180 (record, &local, &local2, 0), the clock 0x6333cc,
   the nine dwords at esp+0x2c set to -1, 0xf8a668 = -1, the window 0x4741f0 (0, 0, 0x62a2a4), 0x476f80 (0, 0),
   0x475600 (window), 0x475f70 (window, 0). The rest of the body (the entries, the pump and the tail) is not
   written yet, see the disassembly */

int ai_Unk57cf90(char *rec)
{
    char *r;
    int a;
    int b;
    int l1;
    int l2;

    r = (char *)ahm_OwnerChar(rec);
    a = U_I(rec + 0x65);
    b = ahm_ResearchState(rec, &l1, &l2, 0);
    l1 = U_I((unsigned)&ai_Unk6333cc);
    U_I((unsigned)&d2_ClickedId) = -1;
    r = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2a4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(r);
    d2_SetForm(r, 0);
    /* ROUGH: the rest is not written yet */
    return 0;
}
