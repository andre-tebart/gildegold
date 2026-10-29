/* 0x54ff20, a Ghidra miss (disassembled): the run handler of the "Taschendieb" cleanup (+0xac a building id of 0x502020): in the state 0 every figure of the tavern (job record +0x180 with +0x2c == +0xac, boss +0x16c, 0.5 chance) gets the 0x4fdc10 object, is stopped (0x4c5970 / 0x4c5b90) and sent (0x4c52e0 "0x626f58") to its object; posted 0x1e units ahead and ends when the due time +0xb0 is over (ROUGH) */

void he_Unk54ff20(int ev)
{
    int st, ebp, esi, o;

    st = *(int *)(ev + 0x70);
    if (st < -2)
        return;
    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st != 0)
        return;
    ebp = gm_FindBuildingById(*(int *)(ev + 0xac));
    if (ebp == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    for (esi = (unsigned)&ai_CharInts; esi < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); esi += 0x21c) {
        if (*(unsigned char *)(esi + 4) > 0 && *(int *)(esi + 0x180) != 0 && *(int *)(esi + 0x168) == ebp && *(int *)(esi + 0x16c) != 0
            && !(util_RandomF() >= 0.5) && *(int *)(*(int *)(esi + 0x180) + 0x2c) == *(int *)(ev + 0xac)) {
            o = unk_Unk4fdc10(*(int *)(esi + 0x16c));
            ai_Unk4c5970(*(int *)esi, 0, 0);
            ai_Unk4c5b90(*(int *)esi);
            if (o != 0)
                ai_Unk4c52e0(*(int *)esi, *(int *)(*(int *)(esi + 0x16c) + 1), *(int *)(o + 2), 0, 0, "Unkendunk");
        }
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    if (gm_CompareTime((unsigned)&ai_Time, ev + 0xb0) == 1)
        he_Unk53e1f0((int *)ev);
}
