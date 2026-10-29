/* 0x4e19a0, tells the party m (+0x30 count, +0x34 ids) about the thing +0x7c / +0x80 (text 0x3fd9, then 0x143e8c0 / 0x143e8bc by the flag
   0x6f23f8 & 4): every one of the kinds 6 / 7 gets it (0x4e48b0). Returns 1 */

int ai_Unk4e19a0(char *m)
{
    char buf[0x200];
    char *p;
    int i, o;

    o = gm_FindBuildingById(*(int *)(m + 0x7c));
    if (o == 0)
        return 0;
    txt_Format(buf, 0x3fd9, o, *(int *)(m + 0x80));
    strcat(buf, (ai_GameFlags & 4) != 0 ? ai_Unk143e8c0 : ai_Unk143e8bc);
    for (i = 0; i < (unsigned char)m[0x30]; i++) {
        p = (char *)gm_FindChar(*(int *)(m + 0x34 + i * 4));
        if (p != 0 && (p[2] == 6 || p[2] == 7))
            ai_Unk4e48b0(p, m, buf, &ch_EmptyName);
    }
    return 1;
}
