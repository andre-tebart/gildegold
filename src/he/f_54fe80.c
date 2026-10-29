/* 0x54fe80, a Ghidra miss: the init of the effect event: the due clock now, +0xb0 = now + the duration of the effect record 0x178 (0x5ba620, +4); no record: freed */

void he_Unk54fe80(int ev)
{
    int r;

    *(HETIME *)(ev + 0x52) = ai_Time;
    r = ai_Unk5ba620(0x178);
    if (r != 0 && *(int *)(r + 4) != 0) {
        *(HETIME *)(ev + 0xb0) = ai_Time;
        gm_AddTime(ev + 0xb0, 0, *(int *)(r + 4), 0);
        return;
    }
    he_Unk53e1f0((int *)ev);
}
