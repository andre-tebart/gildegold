/* 0x4e3830, runs the scene party m (+0 / +0xc the owner, +8 the kind of the scene, +0x2c the window, +0x30 count, +0x34 the character ids) for
   the scene context ctx: the table 0x92c500 (5 ints per kind: +4 the callback of the kind 6 members, +8 the callback of the others, +0xc 1 when
   the members answer one after the other, 0 when all are asked at once) is walked over the member slots of 0x92cae0 (0x35c bytes each: +0
   the id, +8 the state, +0x14 the text). With 0x6f23f8 & 8 (the local run) every member's callback runs and its text is shown; with & 4 (the
   net run) the owner's machine asks the members (0x4c4480 / 0x4c4f50 send the answers, 0x4c3ae0 waits for them with the frame wait 0x52fe00,
   0x4e37a0 shows the texts, 0x4e3750 tells when nobody is left). The counter 0x6f0d7c grows by one per run. ROUGH: the net run is only sketched */

void ai_Unk4e3830(char *m, int ctx)
{
    char msg[0x7c];
    int saved, i, j, n, k, who, h;
    char *c, *slot;
    int *tab;
    int (*cb)();

    saved = ai_Unk4e52c0();
    ai_Unk4e36a0(m);
    if (ai_GameFlags & 4) {
        if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0] && ai_Unk6f0d8c == 0) {
            memset(msg, 0, sizeof(msg));
            *(int *)msg = *(int *)m;
            sprintf(msg + 4, "cut_type:%i", (unsigned char)m[8]);
            cm_SendExSysMessage(0x11, msg);
        }
        if (ai_Unk6f0d8c == 0)
            sv_Unk52fd80(ai_Unk1441488);
    }
    sv_SetStatusText(&ch_EmptyName);
    if (ai_GameFlags & 8) {
        n = *(unsigned char *)(m + 0x30);
        slot = ai_Unk92cae0[0];
        for (i = 0; i < n; i++, slot += 0x35c) {
            ai_Unk6f0d78 = i;
            who = *(int *)(m + 0x34 + i * 4);
            if (who == -1)
                continue;
            c = (char *)gm_FindChar(who);
            ai_Unk6f0d74 = c;
            if (c == 0) {
                *(int *)(m + 0x34 + i * 4) = -1;
                continue;
            }
            if (c[2] == 6) {
                ai_Unk6f0d84 = ai_Unk60d364 + ai_Unk6f0d80;
                ai_Unk6f0d88 = 0;
                ((int (*)())ai_Unk92c504[*(unsigned char *)(m + 8) * 5])(slot, ctx);
            } else {
                cb = (int (*)())ai_Unk92c508[*(unsigned char *)(m + 8) * 5];
                if (cb != 0)
                    cb(slot, ctx);
            }
            d2_SetForm(*(int *)(m + 0x2c), 0);
            io_PrintText(slot + 0x14);
            io_PrintText(&ai_Unk60c6c0);
        }
    } else if (ai_GameFlags & 4) {
        ai_Unk6f0d74 = (char *)ai_CharInts[ai_PlayerId] - 4;
        k = ai_Unk92c50c[*(unsigned char *)(m + 8) * 20];
        if (k == 1) {
            if (*(int *)(m + 0xc) == ai_CharInts[ai_PlayerId][0]) {
                /* the owner asks every member in turn (the slots of the humans are sent, the computer ones answered at once) */
                n = *(unsigned char *)(m + 0x30);
                slot = ai_Unk92cae0[0];
                for (i = 0; i < n; i++, slot += 0x35c) {
                    c = (char *)gm_FindChar(*(int *)slot);
                    if (c == 0 || c[2] == 6 || c[2] == 7 || *(int *)(slot + 8) != 0)
                        continue;
                    ai_Unk6f0d74 = c;
                    cb = (int (*)())ai_Unk92c508[*(unsigned char *)(m + 8) * 5];
                    if (cb != 0)
                        cb(slot, ctx);
                    ai_Unk4c4480(0x35c, slot);
                    ai_Unk4c4f50(*(int *)slot, 0);
                    h = cm_GetSlotState(*(int *)slot);
                    while (h == 0)
                        h = cm_GetSlotState(*(int *)slot) , sv_PumpNet();
                    ai_Unk4e37a0(m);
                    sv_RunFrame(ai_Unk60d360, 0);
                }
            } else {
                /* the others wait for the owner's slots and answer for their own character */
                n = *(unsigned char *)(m + 0x30);
                for (j = 0; j < n; j++)
                    if (*(int *)(ai_Unk92cae0[j]) == ai_CharInts[ai_PlayerId][0])
                        break;
                if (j < n) {
                    ai_Unk6f0d84 = ai_Unk60d364 + ai_Unk6f0d80;
                    ai_Unk6f0d88 = 0;
                    ai_Unk4c4480(0x35c, ai_Unk92cae0[j]);
                    ai_Unk4c4f50(ai_Unk6f0d74 != 0, 0);
                    while (cm_GetSlotState(0) == 0)
                        sv_PumpNet();
                }
                while (ai_Unk4e3750(m) == 0) {
                    ai_Unk4e37a0(m);
                    sv_SetStatusText(&ch_EmptyName);
                    sv_RunFrame(ai_Unk60d360, 0);
                }
            }
        } else if (k == 0) {
            /* all members answer at once: the owner waits for the slots (state +8 == 1), the others for their own */
            n = *(unsigned char *)(m + 0x30);
            ai_Unk4e37a0(m);
            sv_RunFrame(ai_Unk60d360, 0);
            while (ai_Unk4e3750(m) == 0) {
                ai_Unk4e37a0(m);
                sv_RunFrame(ai_Unk60d360, 0);
            }
        }
    }
    ai_Unk6f0d7c = ai_Unk6f0d7c + 1;
    ai_Unk4e52a0(saved);
}
