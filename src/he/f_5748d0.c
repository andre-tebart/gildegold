/* 0x5748d0, the run of the event 0xa1 (the meeting, ROUGH SKELETON): the four figures +0x8c .. (0x505df0) are collected; -2 / -1
   free; without a figure also; state 0 looks for the meeting room (+0xb0 > 0: 0x574fe0 else the building +0x10 (0x4fea50,
   0x501ce0)), state 1 groups them (0x524260 / 0x4c5230 / 0x4c52e0), later states the scene 0x5746b0 (0x524770), the payments
   (0x4fe140, 0x42ec00, 0x4c5410, 0x575030) and the free (0x53e1f0) */

void gm_GetGebaeudeHauptraum(unsigned char *b); /* 0x501ce0 */
void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g); /* 0x524260, the group register */
void ai_Unk4c5230(int a); /* 0x4c5230 */
int ai_Unk4c52e0();                 /* 0x4c52e0 */
void ai_Unk4c5410(int a, const char *s, unsigned char c); /* 0x4c5410 */
int sv_Unk524770();                 /* 0x524770 */
int ai_Unk574fe0();                 /* 0x574fe0 */
void ai_Unk575030();                /* 0x575030 */

void he_Unk5748d0(int ev)
{
    char *pers[4];
    char *bld;
    int n, i, state, x;

    n = 0;
    for (i = 0; i < 4; i++) {
        int id = HM_I(ev, 0x8c + i * 4);
        if (id != -1) {
            char *p = (char *)gm_FindChar(id);
            if (p != 0)
                pers[n++] = p;
        }
    }
    state = HM_I(ev, 0x70);
    if (state == -1 || state == -2 || n == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (state == 0) {
        x = -1;
        if (HM_I(ev, 0xb0) > 0) {
            x = ai_Unk574fe0(ev);
            HM_I(ev, 0xbc) = HM_I(ev, 0xbc) + 1;
        } else {
            bld = 0;
            gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
            if (bld != 0 && ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(bld) != 0) {
                sv_MoveToGroup(pers[0], bld, 0x1f, ev);
                ((void (*)(int, int))ai_Unk4c5230)(HM_I(pers[0], 4), HM_I(ev, 0xb4));
                ai_Unk4c52e0(HM_I(pers[0], 4), pers[1], 0, 0, 0, (char *)(unsigned)&full_data_629070);
            }
        }
        gm_AddTime(ev + 0x52, 0, 2, 0);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    }
    /* the later states: 1 the scene, 2 the payments, 3 the close (all ROUGH) */
    if (state == 1) {
        sv_Unk524770(ev);
        sv_MoveToGroup(0, 0, 0x1f, ev);
        he_Unk5746b0(ev);
        return;
    }
    if (state == 2) {
        gm_AddTime(ev + 0x52, 0, 2, 0);
        return;
    }
    ((void (*)(int, int))ai_Unk4c5230)(HM_I(pers[0], 4), HM_I(ev, 0xb4));
    ai_Unk575030(ev);
    he_Unk53e1f0((int *)ev);
}
