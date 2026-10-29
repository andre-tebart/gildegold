/* 0x5729c0, the exam cancelled: the building 0x501190 (1, 1, +0xac) of the kind 0x1d and the character +0xc: 0x4f0900 (0, building, +0xb0, +0xb4) and 0x5a9880 (the owner record, character, building, +0xb0) */

void he_Unk5729c0(int ev)
{
    char *bld = (char *)gm_BuildQuery(1, 1, HM_I(ev, 0xac));
    char *per;

    if (bld == 0 || *(char *)((unsigned)&ai_BTypes + *(unsigned char *)bld * 0x24d) != 0x1d)
        return;
    unk_Unk4f0900(0, bld, HM_I(ev, 0xb0), HM_B(ev, 0xb4));
    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per != 0)
        ai_Unk5a9880((unsigned)&ai_Chars + *(unsigned short *)(bld + 0x27) * 0x21c, per, bld, HM_I(ev, 0xb0));
}
