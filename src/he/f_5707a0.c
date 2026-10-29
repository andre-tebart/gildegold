/* 0x5707a0, the run of the event 0x8a (the plunder of the buildings +0xb4 .. by the character +0xac): -1 / -2 or a character without the byte +8 -> 0x524260 and free; states 0 .. 4: 0 registers the character (0x524260) and state -1; 1 starts the work 0x4c52e0 (text 0x628eb8) on the building +0xb4 (+0xb0 index); 2 looks the character up, waits 0xa hours while its job +0x128 runs, else finds the nearest target 0x4a5d30 (100.0f), compares 0x4f24a0 / 0x50a640 / 0x5085b0, sends the message 0x8b (0x4c4b70) or calls 0x570bf0 / 0x570c70 by 0x42ec00 (2), stores the word into +0xc8 [+0xb0], steps +0xb0 and state; 3 the boss of the character -> 0x4c52e0; 4 waits 4 hours while its job runs; ROUGH */

int ai_Unk4a5d30(struct AIPER *p, unsigned int fl, int n, unsigned short *ids, float lo, float hi, unsigned short *id); /* 0x4a5d30 */
int unk_Unk4f24a0();                 /* 0x4f24a0 */
int gm_GetWorth(unsigned short id);                 /* 0x50a640 */
int gm_ConvertMoney(int a, unsigned char k); /* 0x5085b0 */
void cm_SendMoveGoodsUnchecked(int a, int b, unsigned char c, int d); /* 0x4c4830 */
void cm_SendSetField(int a, int b, int c, int d, int e); /* 0x4c4a60 */
void he_Unk570bf0(int *a, int b);   /* 0x570bf0 */
void he_Unk570c70(int *a, int b);   /* 0x570c70 */

void he_Unk5707a0(int ev)
{
    char msg[0x70];
    char *per = (char *)gm_FindChar(HM_I(ev, 0xac));
    int state = HM_I(ev, 0x70);
    int r, id, n, best;
    unsigned short idx, v;
    int rec;

    if (state == -1 || state == -2) {
        if (per != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)per, 0);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (per == 0 || per[8] == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (state) {
    case 0:
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        HM_I(ev, 0x84) = -1;
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 1:
        r = gm_BuildQuery(1, 1, HM_I(ev, 0xb4 + HM_I(ev, 0xb0) * 4));
        if (r == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        ai_Unk4c52e0(HM_I(ev, 0xac), *(int *)(r + 1), -1, 0, 1, (char *)(unsigned)&full_data_628eb8);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 2:
        r = gm_FindChar(HM_I(ev, 0xac));
        if (r == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        if (HM_I(HM_I(r, 0x184), 0x128) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 0xa, 0);
            return;
        }
        if (ai_Unk4a5d30((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, ((unsigned)&he_Events + 0x58c8), 4, ev + 0xc8, 0, 100.0f, (int *)&idx) == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        id = idx;
        best = unk_Unk4f24a0((unsigned)&ai_Chars + id * 0x21c);
        n = ((int (*)(int))gm_ConvertMoney)(gm_GetWorth(id));
        *(unsigned short *)(ev + 0xc8 + HM_I(ev, 0xb0) * 2) = idx;
        if (n > best) {
            if (HM_B((unsigned)&ai_Chars + id * 0x21c, 2) == 6 || HM_B((unsigned)&ai_Chars + id * 0x21c, 2) == 7) {
                HM_B(msg, 4) = 0x8b;
                HM_I(msg, 8) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
                HM_I(msg, 0xc) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
                *(HETIME *)(msg + 0x28) = ai_Time;
                HM_B(msg, 0x3a) = 9;
                HM_I(msg, 0x5c) = HM_I(ev, 0xc);
                HM_I(msg, 0x60) = HM_I(ev, 0xac);
                HM_I(msg, 0x68) = best;
                cm_SendLongMessage(msg);
            } else {
                rec = HM_I(ev, 0xc);
                msg[0] = HM_I(ev, 0xc);
                msg[4] = HM_I(ev, 0xac);
                msg[8] = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
                msg[0xc] = best;
                if (util_Random(2) != 0)
                    he_Unk570bf0((int *)msg, (unsigned)&ai_Chars + id * 0x21c);
                else
                    he_Unk570c70((int *)msg, (unsigned)&ai_Chars + id * 0x21c);
            }
        }
        HM_I(ev, 0xb0) = HM_I(ev, 0xb0) + 1;
        if (HM_I(ev, 0xb0) < 5 && HM_I(ev, 0xb4 + HM_I(ev, 0xb0) * 4) != 0) {
            HM_I(ev, 0x70) = HM_I(ev, 0x70) - 1;
            return;
        }
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 3:
        r = gm_FindChar(HM_I(ev, 0xac));
        if (r == 0 || HM_I(r, 0x16c) == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        ai_Unk4c52e0(HM_I(ev, 0xac), *(int *)(HM_I(r, 0x16c) + 1), -1, 0, 0, (char *)(unsigned)&full_data_628eb8);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 4:
        r = gm_FindChar(HM_I(ev, 0xac));
        if (r == 0 || HM_I(r, 0x16c) == 0 || HM_I(HM_I(r, 0x184), 0x128) == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 4, 0);
        return;
    }
}
