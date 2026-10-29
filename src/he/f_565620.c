/* 0x565620 */

/* 0x565620, a Ghidra miss: the run handler of the healing event ("Heilen", type 0x6c): state 0 heals the player 0x505df0 (+0xac) step by step: the elapsed time since +0x60 (0x4fe280) * 0.1 * 5 is the step, the stat 0x4fc8e0 times 100.0
   plus the step must stay below 100.0 (0x4fca40 (id, ftol (step)), the rest +0xb0 -= step, the next run 10 units ahead), else the next state; +0x60 = now; state 1 releases the lock 0x524260 (obj +0x10, 0, id) and sends the
   news 0x1a18 ("_NACHRICHTEN_HS_80") then frees; -1 / -2 release the lock and free (ROUGH) */
float unk_Unk4fc8e0(short *c);

void he_Unk565620(int ev)
{
    int p, o, d;
    float step;
    double v;
    char text[0x200];

    switch (*(int *)(ev + 0x70)) {
    case 0:
        p = gm_FindChar(*(int *)(ev + 0xac));
        if (p == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        d = gm_MinutesBetween(ev + 0x60, (unsigned)&ai_Time);
        step = (float)d * 0.1f * 5.0f;
        v = unk_Unk4fc8e0(p) * 100.0;
        if (*(unsigned *)(ev + 0xb0) > 0 && step + v < 100.0) {
            gm_AddHealth(*(unsigned short *)p, (int)step);
            *(int *)(ev + 0xb0) = (int)((double)*(unsigned *)(ev + 0xb0) - step);
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 10, 0);
        } else {
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
        }
        *(HETIME *)(ev + 0x60) = ai_Time;
        return;
    case 1:
        p = gm_FindChar(*(int *)(ev + 0xac));
        o = gm_FindBuildingById(*(int *)(ev + 0x10));
        if (p != 0 && o != 0) {
            sv_MoveToGroup(o, *(unsigned short *)p, 0, 0);
            txt_Format(text, 0x1a18, o, *(unsigned short *)p);
            he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f2, *(int *)(ev + 0x10), -1, 0, (unsigned)&full_data_627b0c);
        }
        he_Unk53e1f0((int *)ev);
        return;
    case -2:
    case -1:
        p = gm_FindChar(*(int *)(ev + 0xac));
        if (p != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)p, 0);
        he_Unk53e1f0((int *)ev);
        return;
    default:
        return;
    }
}
