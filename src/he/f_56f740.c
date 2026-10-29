/* 0x56f740, the domicile action of the construction run (a = the character, b = the event, c = the event 0x18 found for b + 0xc4): only the type 0x18 and fewer than 0x14 tries; b + 0xc4 + 1; when the workplace of 0x505df0 (c + 0xd8) is at least 1850 from the one of a, b + 0xac = c + 0xb4 (0x4c5230 / 0x4c52e0) and 0; else the sums 0x504ec0 (the record of the player c + 8, 2) + (c + 0xe8 + 1) * 0.5f against 0x504ec0 (0x505df0 (c + 0xc4), 2) + (b + 0xc0 + 1) * 0.5f: the first smaller -> the texts 0x4028 / 0x4029, the state -1 (0x4c4c60) and 1; -1 when nothing */

int he_Unk56f740(int a, int b, char *c)
{
    char text[0x400];
    int per, bld1, bld2, other;
    unsigned short pl;
    float dx, dy, dz;

    if (c == 0 || *c != 0x18)
        return -1;
    if (*(unsigned int *)(b + 0xc4) >= 0x14)
        return -1;
    (*(unsigned int *)(b + 0xc4))++;
    per = gm_FindChar(HM_I(c, 0xd8));
    if (per == 0 || HM_I(per, 0x184) == 0)
        return -1;
    bld1 = HM_I(HM_I(a, 0x184), 0x34);
    bld2 = HM_I(HM_I(per, 0x184), 0x34);
    dx = *(float *)(bld1 + 0x4c) - *(float *)(bld2 + 0x4c);
    dy = *(float *)(bld1 + 0x50) - *(float *)(bld2 + 0x50);
    dz = *(float *)(bld1 + 0x54) - *(float *)(bld2 + 0x54);
    if (sqrt(dx * dx + dy * dy + dz * dz) >= 1850.0f) {
        if (HM_I(c, 0xb4) != HM_I(b, 0xac)) {
            HM_I(b, 0xac) = HM_I(c, 0xb4);
            ai_Unk4c5230(HM_I(a, 4));
            ai_Unk4c52e0(HM_I(a, 4), HM_I(c, 0xb4), -1, 0, 1, (char *)(unsigned)&full_data_628dec);
        }
        return 0;
    }
    pl = *(unsigned short *)(c + 8);
    other = gm_FindChar(HM_I(c, 0xc4));
    if (gm_GetSkill((unsigned)&ai_Chars + pl * 0x21c, 2) + (HM_I(c, 0xe8) + 1) * 0.5f <
        gm_GetSkill(other, 2) + (HM_I(b, 0xc0) + 1) * 0.5f) {
        HM_I(c, 0x84) = cm_SendExSetHE(-1, c);
        txt_Format(text, 0x4028, *(unsigned short *)other);
        he_ShowNews(*(int *)((unsigned)&ai_CharInts + pl * 0x21c), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e58);
        txt_Format(text, 0x4029, *(unsigned short *)((unsigned)&ai_Chars + pl * 0x21c));
        he_ShowNews(HM_I(other, 4), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e58);
        return 1;
    }
    return -1;
}
