/* 0x4f9220, advances the message event record 0x91b480 (ORDMSGREC): disabled it does nothing; idle (phase 0) it
   adds random * scale + base to the accumulator and, above the limit, counts the message, timestamps the record,
   picks the next phase from a 16-entry table chosen by the stage (stage 0 goes straight to phase 3, stage 4 resets
   the stage), sends the long message 0x59 / 0x4e / 0x50 to the player (character ai_PlayerChar, gm_AddTime 0x4fe140
   later) and finally the whole record (message 0x4c5e80, tag 'set '); running, it returns while he_FindEvent (1, 0,
   the phase's object) exists, else clears the phase and the accumulator.
   @deep meaning read against disasm 0x4f9220 and fixed: gm_AddTime is the 4-argument call (stamp, Random(4) hours +7 outside 7..0x16, Random(2)*0x1e minutes, 0; the original pushes secs/mins/hours and cleans 0x10), the stage 2/3 table loops are constant loops (the ?: folded to sete arithmetic), r[8]=r[7] after a phase message, and the tail is the generic block copy ORD_COPY3 (forces the ebp frame). Also named ord_UpdateMessageEvent, util_RandomF, ai_TimeHour. Remaining hunks 4f923a/4f941d/4f9457/4f9619/4f964b are the phase-3 block order, the merged branch tails and the register/slot allocation (constant 0 in edi vs esi, r at ebp-0x24 vs ebp-0x64); no statement left. Q4 still needs the 0xf8 long-message view, the ORDMSGREC r (partly done) and the ai_PlayerChar+4 view. Note: best.csv held the earlier 2-argument gm_AddTime spelling (364 vs 433), which is semantically wrong; `uv run ledger grade accept` reset the reference to this correct source.
   outside 7 .. 0x16, Random(2) * 0x1e minutes, 0), not the draft's 2-argument t (the original pushes secs 0, mins,
   hours and cleans 0x10); the stage 2 / 3 table loops are two constant loops (the draft's ?: folded to sete
   arithmetic); the tail is the generic block copy ORD_COPY3 (ebp frame). Left (diffclass): code hunks 4f923a /
   4f941d / 4f9457 / 4f9619 / 4f964b - the phase 3 block order and the merged branch tails, register allocation (the
   constant 0 in edi vs the original's esi) and the record / message views for Q4 (r / out / msg are still raw). */

int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */

/* unlinked */
static __inline void ord_Stamp(char *d)
{
    *(int *)d = *(int *)ai_Time;
    *(int *)(d + 4) = *(int *)(ai_Time + 4);
    *(int *)(d + 8) = *(int *)(ai_Time + 8);
    *(short *)(d + 0xc) = *(short *)(ai_Time + 0xc);
}

void ord_UpdateMessageEvent(void)
{
    int r[9];
    int out[10];
    char msg[0xf8];
    int tbl[16];
    float f;
    int i, ph;

    if (*(int *)ord_MessageRecord != 1)
        return;
    memcpy(r, ord_MessageRecord, 0x24);
    if (r[7] < 1) {
        f = util_RandomF() * *(float *)&r[4] + *(float *)&r[3] + *(float *)&r[5];
        *(float *)&r[5] = f;
        if (f > *(float *)&r[6]) {
            memset(msg, 0, sizeof(msg));
            r[1]++;
            r[2] = *(int *)ai_Time;
            if (r[8] < 4) {
                if (r[8] == 0) {
                    r[7] = 3;
                    goto phase3;
                }
                tbl[15] = 2;
                tbl[14] = 2;
                tbl[8] = 1;
                tbl[7] = 1;
                tbl[1] = 0;
                tbl[0] = 0;
                if (r[8] == 1) {
                    for (i = 2; i < 7; i++)
                        tbl[i] = 1;
                    for (i = 9; i < 14; i++)
                        tbl[i] = 2;
                } else {
                    for (i = 2; i < 7; i++)
                        tbl[i] = 0;
                    if (r[8] == 2) {
                        for (i = 9; i < 14; i++)
                            tbl[i] = 2;
                    } else {
                        for (i = 9; i < 14; i++)
                            tbl[i] = 1;
                    }
                }
                r[7] = tbl[util_Random(0x10) & 0xffff];
                r[7]++;
                if (r[7] == 1) {
                    msg[4] = 0x59;
                    *(int *)(msg + 8) = *(int *)(ai_PlayerCharC + 4);
                    *(int *)(msg + 0xc) = -1;
                    ord_Stamp(msg + 0x28);
                    msg[0x36] = 2;
                    *(int *)(msg + 0x58) = (util_Random(3) & 0xffff) * 3;
                    ord_Stamp(msg + 0x28);
                    *(int *)(msg + 0x2e) = 0;
                    if (ai_TimeHour < 0x17 && ai_TimeHour >= 7)
                        gm_AddTime(msg + 0x28, util_Random(4), (util_Random(2) & 0xffff) * 0x1e, 0);
                    else
                        gm_AddTime(msg + 0x28, util_Random(4) + 7, (util_Random(2) & 0xffff) * 0x1e, 0);
                    cm_SendLongMessage(msg);
                } else if (r[7] == 2) {
                    msg[4] = 0x4e;
                    *(int *)(msg + 8) = *(int *)(ai_PlayerCharC + 4);
                    *(int *)(msg + 0xc) = -1;
                    *(int *)(msg + 0x58) = -1;
                    ord_Stamp(msg + 0x28);
                    msg[0x36] = 1;
                    *(int *)(msg + 0x9c) = 1000;
                    *(int *)(msg + 0x2e) = 0;
                    if (ai_TimeHour < 0x17)
                        gm_AddTime(msg + 0x28, util_Random(4), (util_Random(2) & 0xffff) * 0x1e, 0);
                    else
                        gm_AddTime(msg + 0x28, util_Random(4) + 7, (util_Random(2) & 0xffff) * 0x1e, 0);
                    cm_SendLongMessage(msg);
                } else if (r[7] == 3)
                    goto phase3;
            } else {
                r[8] = 0;
                r[7] = 3;
              phase3:
                msg[4] = 0x50;
                *(int *)(msg + 8) = *(int *)(ai_PlayerCharC + 4);
                ord_Stamp(msg + 0x28);
                *(int *)(msg + 0xc) = -1;
                msg[0x36] = 2;
                *(int *)(msg + 0x58) = -1;
                *(int *)(msg + 0x9c) = 1000;
                if (ai_TimeHour < 0x17)
                    gm_AddTime(msg + 0x28, util_Random(4), (util_Random(2) & 0xffff) * 0x1e, 0);
                else
                    gm_AddTime(msg + 0x28, util_Random(4) + 7, (util_Random(2) & 0xffff) * 0x1e, 0);
                cm_SendLongMessage(msg);
            }
            r[8] = r[7];
        }
    } else {
        switch (r[7]) {
        case 1:
            ph = 0x59;
            break;
        case 2:
            ph = 0x4e;
            break;
        case 3:
            ph = 0x50;
            break;
        default:
            goto clear;
        }
        if (he_FindEvent(1, 0, ph) != 0)
            return;
      clear:
        r[7] = 0;
        *(float *)&r[5] = 0.0f;
    }
    ORD_COPY3(out, r, 0x24);
    out[9] = 0x73657420;
    cm_SendMessage56(out);
}
