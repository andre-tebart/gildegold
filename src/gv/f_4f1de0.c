/* 0x4f1de0, the per-turn work of the graveyard object o (owner +0x27): walks the entries of the list 0xff that are in use (0x4ef7c0 with 0x4ef770); an entry whose character (+4) is gone (0x505df0) is cleared or written off
   (0x4f0c20 / 0x4f0900 messages, at most 0x40 per round, then it waits for the answer 0x4c3ae0), a living one whose time (+10) is up is paid for / released (0x4f0700 / 0x4f0900 / message 0x92), its owner may lose
   liking (0x4c4b00); at the end up to 4 graves are topped up (0x4f0d70 with 1 .. 2 new dead) and a player owner is told (0x5a9780 / 0x5a9690). ROUGH: the __ftol arguments and several constants are not read. */

void unk_Unk4f1de0(char *o)
{
    unsigned short ow;
    int h, rounds;
    int *e;
    char *pl;
    int paid, nrel, sum;
    unsigned s;
    int t, k, n;
    char *c;
    char m[0xf8];
    int hv;

    ow = *(unsigned short *)(o + 0x27);
    h = -1;
    pl = 0;
    rounds = 0;
    nrel = 0;
    paid = 0;
    sum = 0;
    if (ow != 0xffff && ow != *(unsigned short *)ai_PlayerCharC)
        pl = ai_Chars[ow];
    e = (int *)gv_FindEntry(0, (unsigned char *)o, 0, gv_EntryIsUsed, 0xff);
    do {
        if (e == 0) {
            n = unk_Unk4ef440((unsigned char *)o);
            if (n < 4)
                unk_Unk4f0d70(o, (util_RandomU(2) & 0xffff) + 1);
            if (pl != 0 && (pl[2] == 6 || pl[2] == 7)) {
                if (paid > 0)
                    ai_Unk5a9780(pl, o, paid, sum);
                if (nrel != 0)
                    ai_Unk5a9690(pl, o);
            }
            return;
        }
        if (rounds > 0x3f && h != -1) {
            while (cm_GetSlotState(h) == 0) {
                cm_FlushSendQueue();
                cm_ReceiveMessages();
                cm_ExecCommands();
            }
            rounds = 0;
            h = -1;
        }
        c = gm_FindChar(e[1]);
        if (c == 0) {
            t = e[0];
            k = 0;
            if (gv_EntryCounts[t] > 0) {
                int *z = (int *)e[4];
                do {
                    if (*z != -1) {
                        t = (int)util_RandomF();
                        if (t < 1 || ((gv_EntryIndex(*(char **)(o + 0x65), e) ^ *(int *)ai_Time) & 1) != 0)
                            goto next;
                        h = unk_Unk4f0c20(o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), -t);
                        goto counted;
                    }
                    k++;
                    z += 0x11;
                } while (k < gv_EntryCounts[t]);
            }
            h = unk_Unk4f0900(0, o, t, gv_EntryIndex(*(char **)(o + 0x65), e));
        counted:
            rounds++;
        } else {
            t = (int)util_RandomF();
            if (t > 0) {
                if (((gv_EntryIndex(*(char **)(o + 0x65), e) ^ *(int *)ai_Time) & 1) == 0) {
                    h = unk_Unk4f0c20(o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), -t);
                    rounds++;
                }
            }
            if (pl != 0) {
                if (c != pl) {
                    s = *(int *)ai_Time & 3;
                    if (s == (*(unsigned *)(c + 4) & 3) && (float)(unsigned char)e[3] * 0.003921569f < 0.66f) {
                        h = ((int (*)(void *, void *, int))cm_SendRelations)(*(int *)(pl + 4), *(int *)(c + 4), -5 - (util_RandomU(0x10) & 0xffff));
                        rounds++;
                        nrel++;
                    }
                }
                if (*(unsigned short *)((char *)e + 10) == *(unsigned short *)ai_Time) {
                    h = unk_Unk4f2330((unsigned short *)c, *((unsigned char *)e + 0xc), e[0], 0, &hv);
                    if (c == pl) {
                        h = unk_Unk4f0700((int *)c, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), 10, 0);
                    } else if (c[2] == 6 || c[2] == 7) {
                        memset(m, 0, 0xf8);
                        *(int *)(m + 4) = *(int *)(c + 4);
                        *(int *)(m + 0x28) = *(int *)ai_Time;
                        *(int *)(m + 0x2c) = *(int *)(ai_Time + 4);
                        *(int *)(m + 0x30) = *(int *)(ai_Time + 8);
                        *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
                        m[0x36] = 9;
                        m[0] = (char)0x92;
                        *(int *)(m + 0x58) = *(int *)(o + 1);
                        *(int *)(m + 0x5c) = e[0];
                        m[0xb0] = (char)gv_EntryIndex(*(char **)(o + 0x65), e);
                        *(int *)(m + 0xb8) = 10;
                        *(int *)(m + 0xb4) = h;
                        h = cm_SendLongMessage(m);
                    } else {
                        k = (int)gm_Unk509ee0(c, ai_OwnPlayer);
                        if (h < k) {
                            h = unk_Unk4f0700((int *)c, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e), 10, h);
                            rounds++;
                            paid++;
                            goto next;
                        }
                        h = unk_Unk4f0900(0, o, e[0], gv_EntryIndex(*(char **)(o + 0x65), e));
                    }
                    rounds++;
                }
            }
        }
    next:
        e = (int *)gv_FindEntry(0, (unsigned char *)o, (unsigned)e, gv_EntryIsUsed, 0xff);
    } while (1);
}
