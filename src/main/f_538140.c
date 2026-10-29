/* 0x538140, "main_RundenBeginn": the start of a game round (0x6f2598 = the round number): for every active character (0x770b80 stride 0x21c, kind 1 .. 8) the one-shot message texts (0x5176d0 formats 0x15c5 /
   0x2052 / 0x2054 / 0x2053 / 0x3fd7 written into the lists of 0x53eec0, which decrements the counter byte of an existing entry) are queued: the outstanding bills of the buildings of the kinds 6 / 5 (0x506430),
   the items 0x2c1 / 0x15b / 0x2c4 (0x5ba6b0), the flags 0x100000 / 0x200000, the guild house (+0x16e) and the building list (0x501190 (2, 4, c, 5, 5)); finally 0x4c5f60 (id, the list length 0x53ef80 + 4);
   ROUGH */

void main_Unk538140(void)
{
    unsigned char *c;
    char b0[0x100], b1[0x100], b2[0x100], b3[0x100], b4[0x200], b5[0x200], b6[0x200], hdr[0xd20], line[0x35];
    char *r;
    int o, i, k, m, t;

    hdr[0] = *(char *)(unsigned)&ch_EmptyName;
    memset(hdr + 1, 0, 0x3ff);
    net_PrintStats();
    net_ClearStats();
    sprintf(hdr, "main_RundenBeginn(): init round no %i", U_I((unsigned)&full_data_6f2598));
    c = (unsigned char *)((unsigned)&ai_Chars + 0x2);
    do {
        if (*(short *)(c - 2) != -1 && *c != 0 && *c < 9 && (U_I((unsigned)&ai_Unk1460ae0) == -1 || (ai_CanAct(c - 2) != 0 && (U_I((unsigned)&ai_Unk1460ae0) == -1 || *c != 7)))) {
            if (*c == 6 || *c == 5) {
                o = gm_PlayerRecord(c - 2);
                i = 0;
                do {
                    t = gm_Unk5044b0(*(unsigned short *)(c - 2), i);
                    if (gm_TitlesLeft(t) == 6) {
                        txt_Format(b4, 0x15c5, *(unsigned char *)(i + 0x74 + o) + 0x130);
                        r = (char *)he_Unk53eec0(*(unsigned short *)(c - 2), b4, 1, 0);
                        if (r != 0)
                            *r = *r - 1;
                    }
                    i++;
                } while (i < 0x11);
            }
            k = ai_InventoryHas(c - 2, 0x2c1);
            if (k != 0) {
                txt_Format(b1, 0x2052, 0xf2a);
                r = (char *)he_Unk53eec0(*(unsigned short *)(c - 2), b1, 6, 0);
                if (r != 0)
                    *r = *r - 1;
            }
            k = ai_InventoryHas(c - 2, 0x15b);
            if (k != 0) {
                txt_Format(b2, 0x2052, 0xc5e);
                r = (char *)he_Unk53eec0(*(unsigned short *)(c - 2), b2, 1, 0);
                if (r != 0)
                    *r = *r - 1;
            }
            k = ai_InventoryHas(c - 2, 0x2c4);
            if ((*(unsigned *)(c + 0x1ca) & 0x100000) != 0) {
                if (k == 0) {
                    txt_Format(b3, 0x2054, 0xf18);
                    r = (char *)he_Unk53eec0(*(unsigned short *)(c - 2), b3, -3, 0);
                    if (r != 0)
                        *r = *r - 1;
                }
                cm_SendSetField(*(int *)(c + 2), 0x1cc, 4, 0, 0x100000);
            }
            if ((*(unsigned *)(c + 0x1ca) & 0x200000) != 0) {
                if (k == 0) {
                    txt_Format(b0, 0x2054, 0xf2c);
                    r = (char *)he_Unk53eec0(*(unsigned short *)(c - 2), b0, -8, 0);
                    if (r != 0)
                        *r = *r - 1;
                }
                cm_SendSetField(*(int *)(c + 2), 0x1cc, 4, 0, 0x200000);
            }
            gm_Unk50af00(*(unsigned short *)(c - 2), line);
            if (*(int *)(c + 0x16e) != 0 && (m = gm_BuildingSkill(*(int *)(c + 0x16e), 3, 1)) != 0) {
                txt_Format(b5, 0x2053, *(int *)(c + 0x16e));
                r = (char *)he_Unk53eec0(*(unsigned short *)(c - 2), b5, m, 0);
                if (r != 0)
                    *r = *r - 1;
            }
            o = gm_BuildQuery(2, 4, *(unsigned short *)(c - 2), 5, 5);
            while (o != 0) {
                m = gm_BuildingSkill(o, 3, 1);
                if (m != 0) {
                    txt_Format(b6, 0x3fd7, o);
                    r = (char *)he_Unk53eec0(*(unsigned short *)(c - 2), b6, m, 0);
                    if (r != 0)
                        *r = *r - 1;
                }
                o = gm_BuildNext();
            }
            cm_SendMoneyDelta(*(int *)(c + 2), *(int *)(line + 0x31) + 4 + he_Unk53ef80(*(unsigned short *)(c - 2)));
        }
        c += 0x21c;
    } while ((int)c < (unsigned)((unsigned)ai_Chars + 0x65402));
}
