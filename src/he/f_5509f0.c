/* 0x5509f0, a Ghidra miss (disassembled): the allocation handler of the "Sensenmann" event: due now, walks the 0x100 buildings (+0xb8 the start, +0xbc the step, the mask 0x807c4110 as in the plague event 0x54a020) for the next one that is in use, notes it in +0xb4 (its first id word), creates the object (0x550070) and posts the event 0xf later; frees the record when nothing is found / the object fails (ROUGH) */

int ai_Unk4e3620();
int ai_Unk4e3640();
void ai_Unk5a92b0(void);

void he_Unk5509f0(int ev)
{
    char text[0x200];
    int n = 0x100;
    int idx = *(int *)(ev + 0xb8);
    int esi;

    *(HETIME *)(ev + 0x52) = ai_Time;
    do {
        unsigned lo = 0, hi = 0;
        int e, k;

        esi = U_I((unsigned)&ai_Blds) + idx * 0xa9;
        e = U_I((unsigned)&ai_BTypes) + *(unsigned char *)esi * 0x24d;
        k = *(unsigned char *)e;
        if (k > 0x1f)
            hi = 1u << (k - 0x20);
        else
            lo = 1u << k;
        if (*(unsigned short *)(esi + 0x27) != 0xffff && ((lo & 0x807c4110) != 0 || (hi & 1) != 0))
            break;
        idx = (idx + *(int *)(ev + 0xbc)) & 0xff;
        esi = 0;
        n--;
    } while (n != 0);
    *(int *)(ev + 0xb8) = (*(int *)(ev + 0xbc) + idx) & 0xff;
    if (esi == 0)
        he_Unk53e1f0((int *)ev);
    else
        txt_Format(text, (char *)((unsigned)&ai_Unk625820 + 0x1760), esi);
    *(int *)(ev + 0xb4) = *(int *)(esi + 1);
    if (he_Unk550070(ev) == 0)
        he_Unk53e1f0((int *)ev);
    gm_AddTime(ev + 0x52, 0, 0xf, 0);
}
