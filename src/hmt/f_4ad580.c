/* 0x4ad580, decides election hmt_PendingElection: the best scored (0x4ad3e0, relation counted twice) of up to four
   candidates gets the office object, the human players get message 0xa5; without a holder the candidates lose
   the office bit
   @deep B: 50 code hunks left after the one meaning fix (the original compares hmt_PendingElection unsigned: (unsigned int) < 3, jae; now equal there). The rest are the same operation spelled differently: the candidate limit n > 3 here against cmp edi,0x4/jge in the original, the best test fld best/fcomp cand here against fld cand/fcomp best there (with test ah,0x5/jp against and eax,0x4100/jne), and the AIMSG field stores in another order (same offsets once the pushed esp is accounted for: type base+4, unk08 base+8, time base+0x28, arg[5] base+0x6c). No missing statement, constant or callee left. Q1/Q2 also open (placeholder name, cm_SendLongMessage and the 0x5176d0/0x53dda0 rename block) */

void hmt_Unk4ad580(void)
{
    HMTCAND cand[4];
    HMTOBJ o;
    AIMSG msg;
    float best;
    int type;
    unsigned int bit;
    int n, i;
    void *obj;
    AICHAR *win;

    if ((unsigned int)hmt_PendingElection < 3) {
        type = hmt_ElectionObjTypes[hmt_PendingElection];
        bit = hmt_ElectionOfficeBits[hmt_PendingElection];
        if (amt_GetFirstOffice(type, &o)) {
            memset(cand, 0, sizeof(cand));
            n = 0;
            for (i = 0; i < 768; i++) {
                if (n > 3)
                    break;
                if (ai_Chars[i].flags1cc & bit) {
                    cand[n].c = &ai_Chars[i];
                    n++;
                }
            }
            obj = amt_GetSuperiorOfficeHolder(type);
            i = 0;
            if (obj == 0) {
                for (i = 0; i < n; i++) {
                    if (cand[i].c == 0)
                        break;
                    cm_SendSetField(cand[i].c->netId, 0x1cc, 4, 0, bit);
                }
            } else {
                win = 0;
                best = -1.0f;
                for (i = 0; i < n; i++)
                    cand[i].score = hmt_Score(cand[i].c, obj, type, 1);
                for (i = 0; i < n; i++) {
                    if (best < cand[i].score) {
                        best = cand[i].score;
                        win = cand[i].c;
                    }
                }
                if (win != 0) {
                    amt_SendOfficeMessage(o.obj, (int)win, 0, 1, 0xff);
                    if (win->kind == 6 || win->kind == 7 || win->kind == 5) {
                        msg.to = win->netId;
                        msg.time = ai_Time;
                        msg.type = 0xa5;
                        msg.fromId = -1;
                        msg.flags = 1;
                        msg.arg[4] = msg.to;
                        msg.arg[5] = type;
                        cm_SendLongMessage(&msg);
                        for (i = 0; i < 768; i++) {
                            if (ai_Chars[i].id != 0xffff && &ai_Chars[i] != win
                                && (ai_Chars[i].kind == 6 || ai_Chars[i].kind == 7)) {
                                msg.to = ai_Chars[i].netId;
                                cm_SendLongMessage(&msg);
                            }
                        }
                    }
                }
            }
            hmt_SendElectionMessage(-1);
        }
    }
}
