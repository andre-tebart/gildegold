/* 0x4e0040, tells the others of the party m (+0x30 count, +0x34 ids; +0x7d: the challenger is not the first) about the duel: the text
   0x11c8 (the name of the first) to every one of the kinds 6 / 7 after the first (0x4e48b0), with 0x11c5 + the text 0x143e8e4 for the first
   when m + 0x7d is 0. Returns 1 */

int ai_Unk4e0040(char *m)
{
    char buf[0x200];
    char *p, *q;
    int i;

    if (m[0x7d] == 0) {
        p = (char *)gm_FindChar(*(int *)(m + 0x34));
        if (p == 0)
            return 0;
        if (p[2] == 6 || p[2] == 7) {
            txt_Format(buf, 0x11c5);
            strcat(buf + 8, ai_Unk143e8e4);
            ai_Unk4e48b0(p, m, buf + 8, &ch_EmptyName);
        }
        txt_Format(buf, 0x11c8, *(unsigned short *)p);
    } else {
        p = (char *)gm_FindChar(*(int *)(m + 0x34));
        if (p == 0)
            return 0;
        txt_Format(buf, 0x11c8, *(unsigned short *)p);
    }
    for (i = 1; i < (unsigned char)m[0x30]; i++) {
        q = (char *)gm_FindChar(*(int *)(m + 0x34 + i * 4));
        if (q != 0 && (q[2] == 6 || q[2] == 7))
            ai_Unk4e48b0(q, m, buf, &ch_EmptyName);
    }
    return 1;
}
