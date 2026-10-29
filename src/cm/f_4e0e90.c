/* 0x4e0e90, a divorce / separation message m (+0x34 / +0x38 the two): when the partner (+0x5c of the first) is not of the kind 0xf the
   first, if a person of the kinds 6 / 7, gets the text 0x11c1 + 0x143e8c8 (0x4e48b0) and 1 is returned; else the second gets the state
   (0x4c4e00, 1). Returns 0 otherwise */

int ai_Unk4e0e90(char *m)
{
    char buf[0x200];
    char *a, *b, *p;

    a = (char *)gm_FindChar(*(int *)(m + 0x34));
    b = (char *)gm_FindChar(*(int *)(m + 0x38));
    if (a != 0 && b != 0) {
        p = (char *)gm_FindChar(*(int *)(a + 0x5c));
        if (p != 0 && p[2] != 0xf) {
            if (a[2] == 6 || a[2] == 7) {
                txt_Format(buf, 0x11c1);
                strcat(buf + 8, ai_Unk143e8c8);
                ai_Unk4e48b0(a, m, buf + 8, &ch_EmptyName);
            }
            return 1;
        }
        cm_SendKillCharacter(*(int *)(b + 4), 1);
    }
    return 0;
}
