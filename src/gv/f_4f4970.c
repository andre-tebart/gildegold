/* 0x4f4970, the level-up check of the character slot i (a living one of kind < 10 with +8 set): when its experience (0x50a640) exceeds the threshold table 0x5ee0f8 [level + 1] the level (+0xd) rises: at level 1 it needs the
   building kind 5 object 0xf with item 0x110 (the rank changes by 0x4000 via 0x4c4a60, the player gets the message 0x161d), otherwise the new level goes to the player's message 0x69 (type 9, level at +0x58) or the character's byte
   (0x4c3ba0 / 0x4c3c00 (1, 1, 0xd)); the other players are told too (the first 0x62489c of them; a kind 5 character moves the date by rand(8) * 7 + rand(4)). ROUGH */

void unk_Unk4f4970(unsigned i)
{
    char *c;
    int lv, exp, k, n;
    unsigned nl;
    char *o;
    char *it;
    char *q;
    char m[0xf8];
    char nb;
    int slot;

    slot = i & 0xffff;
    c = ai_Chars[slot];
    if (*(short *)c != -1 && (unsigned char)c[2] < 10 && c[8] != 0 && c[2] != 0) {
        exp = (int)gm_GetWorth(i);
        lv = (unsigned char)c[0xd];
        nl = lv + 1;
        if (nl > 10)
            nl = 10;
        if (unk_Tab5ee0f8[nl] < exp) {
            if (lv == 1) {
                o = (char *)gm_BuildQuery(1, 5, 0xf);
                if (o != 0 && (it = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x110)) != 0) {
                    cm_SendSetField(*(int *)(c + 4), 0x1c8, 4, 0x4000, 0);
                    if (c[2] == 6 || c[2] == 7) {
                        txt_Format(m, 0x161d, *(unsigned short *)c);
                        he_MessageBoxQuickjump(*(int *)(c + 4), -1, m, 0, 0x5fd, *(int *)(o + 1), *(int *)(it + 2), 0, "_NACHRICHTEN_HS_25");
                        return;
                    }
                }
            } else if (unk_Tab5ee0f8[nl] < exp && lv < 9) {
                nb = (char)(lv + 1);
                if (c[2] == 6 || c[2] == 7) {
                    memset(m, 0, 0xf8);
                    *(int *)(m + 0x4) = *(int *)(c + 4);
                    *(int *)(m + 0x28) = *(int *)ai_Time;
                    *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                    m[0] = 0x69;
                    *(int *)(m + 0x8) = -1;
                    m[0x36] = 9;
                    *(short *)(m + 0x30) = 7;
                    m[0x58] = nb;
                    *(int *)(m + 0x5c) = *(int *)(c + 4);
                    cm_SendLongMessage(m);
                } else {
                    cm_BeginFieldBlock(c, *(int *)(c + 4));
                    cm_AddFieldDelta(1, 1, 0xd, &nb);
                    cm_SendFieldDeltas();
                }
                if (c[2] == 5 || c[2] == 6 || c[2] == 7) {
                    n = 0;
                    q = &ai_Chars[0][2];
                    do {
                        if (n >= (int)ai_NumPlayers)
                            return;
                        if ((*q == 6 || *q == 7) && q - 2 != c) {
                            memset(m, 0, 0xf8);
                            *(int *)(m + 0x4) = *(int *)(q + 2);
                            *(int *)(m + 0x28) = *(int *)ai_Time;
                            *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                            m[0] = 0x69;
                            *(int *)(m + 0x8) = -1;
                            m[0x36] = 9;
                            *(short *)(m + 0x30) = 7;
                            if (c[2] == 5) {
                                k = util_Random(8);
                                k = util_Random(4);
                                ((void (*)(void *, int))gm_AddTime)(m + 0x28, k);
                            }
                            *(int *)(m + 0x5c) = *(int *)(c + 4);
                            m[0x58] = nb;
                            cm_SendLongMessage(m);
                            n++;
                        }
                        q += 0x21c;
                    } while ((int)q < (unsigned)((unsigned)ai_Chars + 0x65402));
                }
            }
        }
    }
}
