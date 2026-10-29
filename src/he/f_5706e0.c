/* 0x5706e0, the alloc of the event 0x8a: paused -> nothing; 0x4a6570 (the record of the player +8, 5, 4, 0, 0, 0, 5, ev + 0xb4) -> 0 -> free; the slot +0xb4 of the found index (when < 5) cleared, +0xb0 = 0, due clock now + 1 hour, state 0 */

int ai_Unk4a6570(struct AIPER *p, int a, __int64 mask, float lo, float hi, unsigned char n, int *out); /* 0x4a6570 */

void he_Unk5706e0(int ev)
{
    int r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    r = ai_Unk4a6570((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, 5, 4, 0, 0, 5, ev + 0xb4);
    if (r == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (r < 5)
        HM_I(ev, 0xb4 + r * 4) = 0;
    HM_I(ev, 0xb0) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    HM_I(ev, 0x84) = cm_SendExSetHE(0, ev);
}
