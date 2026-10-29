/* 0x57c690, the tutorial selection handler (1871 B, ROUGH SKELETON): arg 1 = the record; the lookup 0x4eaa50
   (record), the dword record +0x65, 0x4eb180 (record, &local, &local2, 0), the clock 0x6333cc into a local,
   0xf8a668 = -1, the window 0x4741f0 (0, 0, 0x62a284), 0x476f80 (0, 0) and 0x475600 (the window). The rest of
   the body (the selection entries, the pump and the tail) is not written yet, see build/dis57c690.txt */

int ahm_ResearchState();

int ai_Unk57c690(char *rec)
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
    r = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a284);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(r);
    /* ROUGH: the rest is not written yet */
    return 0;
}
