/* 0x54a7b0, a Ghidra miss (disassembled; 1648 B): the run handler of the raid of a gang ("Taschendieb": the six figures +0x8c .. are the gang, +0xd2 the one handled next, +0xd4 its leader, +0xac .. the buildings they are sent to, +0x10 the home building, +0xc4 the start clock, +0xd8 the number of steals left): state 0 sends the figure that is idle (+0x128 of its job record 0) to a random building of the kinds 5 / 7 / 0x16 / 0xf / 0xa (0x501190, 0x4c52e0 with 0x626b6c) after a theft from a random figure of 0x300 (a roll of 0x7e against the strength difference: the news 0x19f3, else the loot 0x4c4830 and the news 0x19f2); state 1 (also entered when the leader is gone or after 0x1e0 time units) collects the gang (0x4c52e0), pays the sum rolled from their abilities (0x5085f0 * (1 + 0.xx * 0x5092e0)) to the leader, adds it to the building (+0x4d), and shows the news 0x19f4  (ROUGH) */

int gm_GetGoodsValue();
void gm_GetGebaeudeHauptraum(unsigned char *b);

void he_Unk54a7b0(int ev)
{
    char text[0x100];
    int goals[4];
    int r_, dt, ebp, esi = 0, edi, s, idx = 0, rec, st, i, p, sum = 0, o, amt, found = 0, v, k;

    dt = gm_MinutesBetween(ev + 0xc4, (unsigned)&ai_Time);
    ebp = gm_FindChar(*(int *)(ev + 0xd4));
    s = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (s != 0 && *(unsigned short *)(s + 0x27) != 0xffff)
        ebp = (unsigned)&ai_Chars + *(unsigned short *)(s + 0x27) * 0x21c;
    if (*(int *)(ev + 0x70) == -2 || ebp == 0) {
        *(int *)(ev + 0x70) = 1;
    } else {
        esi = 0;
        for (;;) {
            if (*(unsigned char *)(ev + 0xd2) < 6) {
                idx = *(unsigned char *)(ev + 0xd2);
                esi = gm_FindChar(*(int *)(ev + 0x8c + idx * 4));
                *(unsigned char *)(ev + 0xd2) = *(unsigned char *)(ev + 0xd2) + 1;
                if (esi == 0)
                    continue;
            } else if (esi == 0) {
                *(unsigned char *)(ev + 0xd2) = 0;
                return;
            }
            break;
        }
        *(int *)(ev + 0x70) = dt > 0x1e0;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 6, 0);
        *(unsigned char *)(ev + 0xd2) = *(unsigned char *)(ev + 0xd2) % 6;
    }
    st = *(int *)(ev + 0x70);
    if (st == 1) {
        s = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (s != 0 && ebp != 0) {
            o = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(s);
            for (i = 0; i < 6; i++) {
                p = gm_FindChar(*(int *)(ev + 0x8c + i * 4));
                if (p != 0) {
                    int val = (int)((float)*(unsigned char *)(p + 0x82) * 0.023809524f + 1.0f);

                    k = (unsigned short)util_Random(4) + 3;
                    for (; k > 0; k--)
                        sum += (unsigned short)util_Random((val + 1) * val) + val * 2;
                    ai_Unk4c5230(*(int *)(p + 4));
                    ai_Unk4c52e0(*(int *)(p + 4), *(int *)(s + 1), *(int *)(o + 2), 0, 0, "Taschendieb");
                    sv_MoveToGroup(0, *(unsigned short *)p, 0, 0);
                }
            }
            v = gm_PlayerFactor(sum, *(unsigned char *)(unsigned)&ai_OwnPlayer);
            amt = (int)(((float)gm_BuildingSkill(s, 1, 1) * 0.5f + 1.0f) * (float)v);
            cm_SendMoveGoodsUnchecked(*(int *)(ebp + 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, amt);
            *(int *)(s + 0x4d) = *(int *)(s + 0x4d) + amt;
            if (*(char *)(ebp + 2) == 6) {
                r_ = gm_PlayerRecord(ebp);
                if (r_ != 0)
                    *(int *)(r_ + 0x18) = *(int *)(r_ + 0x18) + amt;
                r_ = gm_FindCharacterByMaster(s);
                if (r_ == 0 || (*(unsigned char *)(r_ + 0x1b4) & 1) == 0) {
                    txt_Format(text, 0x19f4, amt);
                    he_ShowNews(*(int *)(ebp + 4), *(int *)(ebp + 4), text, 0, 0x5f0, "");
                }
            }
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st != 0)
        return;
    if (esi != 0 && *(int *)(*(int *)(esi + 0x184) + 0x128) == 0) {
        int goalkind[5];
        int tries, victim = 0, n = 0;

        goalkind[0] = 10;
        goalkind[1] = 5;
        goalkind[2] = 7;
        goalkind[3] = 0x16;
        goalkind[4] = 0xf;
        edi = gm_BuildQuery(1, 1, *(int *)(ev + 0xac + idx * 4));
        o = goalkind[(unsigned short)util_Random(5)];
        if (edi != 0) {
            int sv = (int)((float)*(unsigned char *)(esi + 0x82) * 0.023809524f + 1.0f);
            int cap;

            cap = ((unsigned short)util_Random((sv + 1) * sv) + sv * 2) * 2;
            v = gm_PlayerFactor(cap, *(unsigned char *)(unsigned)&ai_OwnPlayer);
            if (*(unsigned char *)(ev + 0xd8) != 0) {
                for (tries = 0x20; tries > 0 && victim == 0; tries--) {
                    rec = (unsigned)&ai_Chars + (unsigned short)util_Random(0x300) * 0x21c;
                    if (*(unsigned short *)rec != 0xffff && *(unsigned char *)(rec + 8) > 0 && *(unsigned char *)(rec + 2) < 10
                        && *(char *)(rec + 0x165) != 2 && *(char *)(rec + 0x165) != 1 && *(int *)(rec + 4) != *(int *)(s + 4)
                        && gm_GetGoodsValue(rec) >= v)
                        victim = rec;
                }
                *(unsigned char *)(ev + 0xd8) = *(unsigned char *)(ev + 0xd8) - 1;
                if (victim != 0) {
                    if (*(unsigned char *)(victim + 0x82) - *(unsigned char *)(s + 0x82) > (unsigned short)util_Random(0x7e)) {
                        if (*(char *)(victim + 2) == 6 || *(char *)(victim + 2) == 7) {
                            txt_Format(text, 0x19f3);
                            he_ShowNews(*(int *)(victim + 4), *(int *)(victim + 4), text, 0, 0x5f0, (char *)0);
                        }
                    } else {
                        cm_SendMoveGoodsUnchecked(-1, *(int *)(victim + 4), *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
                        if (*(char *)(victim + 2) == 6 || *(char *)(victim + 2) == 7) {
                            txt_Format(text, 0x19f2, v);
                            he_ShowNews(*(int *)(victim + 4), *(int *)(victim + 4), text, 0, 0x5f0, "_NACHRICHTEN_HS_19");
                        }
                    }
                }
            }
        }
        edi = gm_BuildQuery(1, 5, o);
        if (edi == 0)
            edi = gm_BuildQuery(1, 5, 10);
        else {
            while (n < 4) {
                goals[n++] = edi;
                if ((edi = gm_BuildNext()) == 0)
                    break;
            }
            edi = goals[(unsigned short)util_Random(n)];
        }
        sv_MoveToGroup(edi, 0, *(unsigned short *)esi, ev);
        ai_Unk4c5230(*(int *)(esi + 4));
        ai_Unk4c52e0(*(int *)(esi + 4), *(int *)(edi + 1), -1, 0, 1, "Taschendieb");
        *(int *)(ev + 0xac + idx * 4) = *(int *)(edi + 1);
    }
}
