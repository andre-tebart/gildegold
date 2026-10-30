/* 0x575030, the income of the meeting (the event 0xa1, ROUGH): the figure +0xc (0x505df0) and the building +0x10 (0x4fea50); the
   amount 0x5092e0 (building, 0x1d, 1) * 0x5ee01c + 0x5eb780, times 0x504ec0 (per, 2) and a random 0x42ec20 curve (0x5eec58 /
   0x5ebbe8), raises per +0xac for each of the count; then, with the random check 0x5eb780 / 0x5eb9f0, the first free word
   +0xc8 .. of the event gets the id 0x5a9e90 (index, ev +0xc8) */

unsigned short ai_Unk5a9e90(int param_1, unsigned short *param_2);                 /* 0x5a9e90 */

void ai_Unk575030(int ev)
{
    char *per, *bld;
    int v, n, i;
    float f;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per == 0)
        return;
    bld = 0;
    gm_FindById(&bld, 0, 0, HM_I(ev, 0x10));
    if (bld == 0)
        return;
    v = gm_BuildingSkill(bld, 0x1d, 1);
    f = (float)v * *(float *)(unsigned)&full_rdata_5ee01c + *(float *)(unsigned)&ai_Unk5eb780;
    n = gm_GetSkill(per, 2);
    for (i = n; i > 0; i--)
        HM_I(ev, 0xac) = HM_I(ev, 0xac) + (int)((util_RandomF() + f) * *(double *)((unsigned)&ai_Unk5eeab8 + 0x1a0) + *(double *)(unsigned)&ai_Unk5ebbe8) * f;
    if (util_RandomF() + *(float *)(unsigned)&ai_Unk5eb780 >= (-1.58818684e-23f))
        return;
    for (i = 0; i < 4; i++) {
        if (*(unsigned short *)(ev + 0xc8 + i * 2) == 0) {
            *(unsigned short *)(ev + 0xc8 + i * 2) = ai_Unk5a9e90(i, ev + 0xc8);
            break;
        }
    }
}
