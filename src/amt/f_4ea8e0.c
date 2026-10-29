/* 0x4ea8e0, tells the character c (+0xd > 1, +4 the id) the offices it could apply for (0x4e7150, 6 of them): the text 0x13ac and the names 0x253 + kind (0x5ec channel) */

void amt_Unk4ea8e0(char *c)
{
    AMTCAND cand[6];
    char buf[2048];
    int n, i;

    if ((unsigned char)c[0xd] >= 2 && (n = amt_GetOfficeCandidates(c, 6, cand)) != 0) {
        memset(buf, 0, sizeof(buf));
        txt_Format(buf, 0x13ac);
        for (i = 0; i < n; i++)
            txt_Format(buf + strlen(buf), "%s$A", cand[i].kind + 0x253);
        he_ShowNews(*(int *)(c + 4), -1, buf, 1, 0x5ec, 0);
    }
}
