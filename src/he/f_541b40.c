/* 0x541b40, the script message handler of the inn meeting ("stammtisch", see 0x53f430): `cmd` is a four letter code ("join" 0x6a6f696e, "cler" 0x636c6572 = clear, "exec" 0x65786563, "leav" 0x6c656176, "new " 0x6e657720), a2 the inn record, a3 its staff record (4 members at +0x1c), a4 the figure id. Collects the member players (0x505df0), tells them (0x53dee0, the news 0x1893 .. 0x189d) and settles the money / relations of an executed meeting (ROUGH) */

float gm_GoodsPrice(unsigned id, unsigned char p);

void he_Unk541b40(unsigned cmd, int a2, int a3, int a4)
{
    int n = 0;
    int i, j, k, pm;
    int mem[4];
    unsigned short *fig;
    int *pp;
    int idx;
    int amt, total, v;
    char big[1024];
    char s1[256];
    char s2[256];
    char c;

    idx = *(unsigned short *)(a2 + 0x27) * 0x21c;
    pp = (int *)(a3 + 0x1c);
    for (i = 4; i != 0; i--, pp++) {
        pm = gm_FindChar(*pp);
        if (pm != 0) {
            mem[n] = pm;
            n++;
        }
    }
    if (cmd < 0x6a6f696f) {
        if (cmd == 0x6a6f696e) {
            fig = (unsigned short *)gm_FindChar(a4);
            if (fig != 0) {
                txt_Format(big, 0x189b, a2, *fig);
                for (j = 0; j < n; j++) {
                    c = *(char *)(mem[j] + 2);
                    if (c == 6 || c == 7)
                        he_MessageBoxQuickjump(*(int *)(mem[j] + 4), -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
                }
                if (*(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 6 || *(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 7) {
                    if (n == 3) {
                        txt_Format(s1, 0x1894, a2, *fig);
                        txt_Format(s2, 0x1896);
                        txt_Format(big, "%s%s", s1, s2);
                    } else {
                        txt_Format(big, 0x1894, a2, *fig);
                    }
                    he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + idx), -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
                    return;
                }
            }
        } else if (cmd == 0x636c6572) {
            txt_Format(big, 0x189d, a2);
            if (n > 0) {
                for (j = 0; j < n; j++) {
                    c = *(char *)(mem[j] + 2);
                    if (c == 6 || c == 7)
                        he_MessageBoxQuickjump(*(int *)(mem[j] + 4), -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
                }
                return;
            }
        } else if (cmd == 0x65786563) {
            amt = (int)(gm_GoodsPrice(0x17a, U_I((unsigned)&ai_OwnPlayer)) + gm_GoodsPrice(0x179, U_I((unsigned)&ai_OwnPlayer)));
            total = 0;
            txt_Format(big, 0x189a, a2, amt);
            if (n > 0) {
                total = amt * n;
                for (j = 0; j < n; j++) {
                    pm = mem[j];
                    cm_SendMoveGoodsUnchecked(U_I((unsigned)&ai_CharInts + idx), *(int *)(pm + 4), U_I((unsigned)&ai_OwnPlayer), amt);
                    if (*(char *)(pm + 2) == 6 || *(char *)(pm + 2) == 7)
                        he_MessageBoxQuickjump(*(int *)(pm + 4), -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
                    cm_SendRelations(U_I((unsigned)&ai_CharInts + idx), *(int *)(pm + 4), 3, 1);
                    cm_SendRelations(*(int *)(pm + 4), U_I((unsigned)&ai_CharInts + idx), 3, 1);
                }
            }
            if (*(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 6 || *(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 7) {
                txt_Format(big, 0x1893, a2, total);
                he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + idx), -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
            }
            if (n > 0) {
                for (i = 0; i < n; i++) {
                    for (j = 0; j < n; j++) {
                        if (i != j) {
                            cm_SendRelations(*(int *)(mem[i] + 4), *(int *)(mem[j] + 4), 3, 1);
                            cm_SendRelations(*(int *)(mem[j] + 4), *(int *)(mem[i] + 4), 3, 1);
                        }
                    }
                }
                return;
            }
        }
    } else if (cmd == 0x6c656176) {
        fig = (unsigned short *)gm_FindChar(a4);
        if (fig != 0) {
            txt_Format(big, 0x189c, a2, *fig);
            for (j = 0; j < n; j++) {
                c = *(char *)(mem[j] + 2);
                if ((c == 6 || c == 7) && (v = *(int *)(mem[j] + 4)) != *(int *)(fig + 2))
                    he_MessageBoxQuickjump(v, -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
            }
            if (*(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 6 || *(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 7) {
                if (n == 1) {
                    txt_Format(s1, 0x1895, a2, *fig);
                    txt_Format(s2, 0x1897);
                    txt_Format(big, "%s%s", s1, s2);
                } else {
                    txt_Format(big, 0x1895, a2, *fig);
                }
                he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + idx), -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
            }
            if (n == 4) {
                txt_Format(big, 0x1899, a2);
                pp = (int *)(unsigned)&ai_CharInts;
                do {
                    if ((*((char *)pp - 2) == 6 || *((char *)pp - 2) == 7) && (v = *pp) != U_I((unsigned)&ai_CharInts + idx)) {
                        for (k = 0; k < 4; k++) {
                            if (v == *(int *)(mem[k] + 4)) {
                                if (k < 4)
                                    he_MessageBoxQuickjump(v, -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
                                break;
                            }
                        }
                    }
                    pp += 0x87;
                } while ((int)pp < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
            }
        }
    } else if (cmd == 0x6e657720) {
        txt_Format(big, 0x1898, a2);
        pp = (int *)(unsigned)&ai_CharInts;
        do {
            if ((*((char *)pp - 2) == 6 || *((char *)pp - 2) == 7) && *pp != U_I((unsigned)&ai_CharInts + idx))
                he_MessageBoxQuickjump(*pp, -1, big, 0, 0x5ec, *(int *)(a2 + 1), -1, 0, (char *)(unsigned)&ch_EmptyName);
            pp += 0x87;
        } while ((int)pp < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2));
        return;
    }
}
