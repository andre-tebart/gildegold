/* 0x4fccc0, the yearly love / marriage step of the character c (kind < 10, active, byte +9 = 1, +0x1b1 = 0, +0x1c8 bit 0x40000 clear; its partner o = 0x505df0 (+0x5c)): a float +0x7c above 1.0 is pulled down by 0x4c4ac0 (-0.5 * random - the value) and the pair gets the event 0x4c4650 (8, ..) with a message 7 (0x4c4fa0, text number 0x12 ..); otherwise (the partner active, the age +10 below 0x27) the number of the relatives at +0x68 .. (4 .. 8; a player (6 / 7) or a priest (5) among them ends it) sets the float +0x7c to (random + 1) * the table value 0x617734 [count]. ROUGH */

float ai_Unk617734[6] = { 0.240015402f, 0.222200006f, 0.125f, 0.0555500016f, 0.0138499998f, 0.0f };  /* 0x617734, a float per number of children / relatives */
void gm_AddTime(struct GCLOCK *c, int hours, int mins, int secs);                     /* 0x4fe140 */
void cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);                     /* 0x4c4650 */
int cm_GetSlotState();                     /* 0x4c3ae0 */
int cm_GetSlotValue(int h);                     /* 0x4c3b10 */
void ai_Unk4c4fa0(void *p);                     /* 0x4c4fa0 */
int gm_TitleNumber();                     /* 0x5043d0 */
int ai_WorksForMaster(struct AICHAR *c);                     /* 0x47dd70 */

void unk_Unk4fccc0(char *c)
{
    int clk[4];
    int msg[0x45];
    char *o, *q2, *mate, *p;
    int *q;
    int arg, t, cnt, cnt2, k, h, n, bld;
    float fv;
    char kind;

    if ((unsigned char)c[2] > 9)
        return;
    if (c[8] == 0)
        return;
    if (c[9] != 1)
        return;
    if (c[0x1b1] != 0)
        return;
    if ((*(unsigned *)(c + 0x1c8) & 0x40000) != 0)
        return;
    o = gm_FindChar(*(int *)(c + 0x5c));
    if (1.0 < *(float *)(c + 0x7c)) {
        ai_Unk4c4ac0(*(int *)(c + 4), 0x7c, (float)(util_RandomF2() * -0.5 - *(float *)(c + 0x7c)));
        ai_Unk6f1b98++;
        if (o == 0)
            return;
        memcpy(clk, ai_Time, 14);
        arg = 2;
        gm_AddTime(clk, 8, 0, 0);
        *(short *)(clk + 1) = util_Random(4) + 8;
        bld = *(int *)(c + 0x170);
        *(short *)((char *)clk + 6) = 0;
        *(short *)(clk + 2) = 0;
        t = bld == 0 ? -1 : *(int *)(bld + 1);
        mate = 0;
        if (o[2] == 6 || o[2] == 7) {
            cnt = 0;
            q = (int *)(o + 0x68);
            for (k = 4; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            if (cnt == 0)
                arg = (unsigned char)o[9];
            mate = o;
            if (*(int *)(o + 0x170) == 0)
                t = bld == 0 ? -1 : *(int *)(bld + 1);
            else
                t = *(int *)(*(int *)(o + 0x170) + 1);
        } else if (c[2] == 6 || c[2] == 7) {
            cnt = 0;
            q = (int *)(c + 0x68);
            for (k = 4; k != 0; k--, q++)
                if (*q != -1)
                    cnt++;
            if (cnt == 0)
                arg = (unsigned char)c[9];
            mate = c;
            if (bld == 0) {
                if (*(int *)(o + 0x170) == 0)
                    t = -1;
                else
                    t = *(int *)(*(int *)(o + 0x170) + 1);
            } else
                t = *(int *)(bld + 1);
        }
        h = ((int (*)(int, void *, void *, int, int, int, int, int))cm_SendCreateCharacter)(8, *(int *)(o + 4), *(int *)(c + 4), 0, t, 0, 0, arg);
        n = cm_GetSlotState(h);
        while (n == 0) {
            sv_PumpNet();
            n = cm_GetSlotState(h);
        }
        if (cm_GetSlotState(h) == 2)
            return;
        p = (char *)cm_GetSlotValue(h);
        if (mate != 0) {
            arg = 0x12;
            memset(msg, 0, sizeof(msg));
            ((char *)msg)[8] = 7;
            msg[3] = *(int *)(mate + 4);
            msg[5] = -1;
            msg[6] = clk[0];
            msg[7] = *(int *)((char *)clk + 4);
            msg[8] = *(int *)((char *)clk + 8);
            *(short *)(msg + 9) = *(short *)((char *)clk + 12);
            ((char *)msg)[0x26] = (char)0xa0;
            ((char *)msg)[0x30] = 2;
            msg[0xd] = msg[3];
            msg[0xe] = *(int *)(p + 4);
            cm_BeginFieldBlock(p, msg[0xe]);
            cm_AddFieldDelta(1, 1, 2, &arg);
            cm_SendFieldDeltas();
            ai_Unk4c4fa0(msg);
            ai_Unk6f1b98++;
            return;
        }
    }
    if (o != 0 && o[8] != 0 && *(unsigned short *)(c + 10) < 0x27) {
        cnt = 0;
        q = (int *)(c + 0x68);
        for (k = 3; k < 8; k++, q++) {
            if (*q != -1) {
                q2 = gm_FindChar(*q);
                kind = q2[2];
                if (kind == 6 || kind == 7 || kind == 5)
                    return;
                if (kind != 0x0f)
                    cnt++;
            }
        }
        if (cnt != 4) {
            if (o[2] == 6 || o[2] == 7) {
                cnt2 = 0;
                q = (int *)(o + 0x68);
                for (k = 3; k < 8; k++, q++) {
                    if (*q != -1) {
                        q2 = gm_FindChar(*q);
                        kind = q2[2];
                        if (kind == 6 || kind == 7 || kind == 5)
                            return;
                        if (kind != 0x0f)
                            cnt2++;
                    }
                }
                if (cnt2 == 5)
                    return;
            }
            fv = (float)((util_RandomF2() + 1.0) * ai_Unk617734[cnt]);
            ai_Unk4c4ac0(*(int *)(c + 4), 0x7c, fv);
            ai_Unk6f1b98++;
        }
    }
}
