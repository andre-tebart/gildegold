/* 0x5cc2b0, the espionage relation handler (1104 B, ROUGH SKELETON: no Ghidra export; from the
   disassembly): param_1 is the message record (the AIMSG: +2 the type 6, +4 the handle, +0x194 the
   amount), param_2 the request (dword +0: 3 / 4, +0x214 / +0x218 the two character ids). Types other
   than 6: the two rolls 0x42ec00 (0x28) + 0xf; the amount < 3 returns 0x20; the two characters
   0x505df0 (missing -> 0x60); the two lines 0x4c4b00 (each handle, the other handle, the roll, 1); the
   close 0x4c5f60 (-3); a target of the type 6 / 7 gets the message; the rest (the window at 0x5cc433)
   is left as a comment. ROUGH SKELETON. */

int ai_EspionageRelation(int param_1, int *param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int chr1;
    int chr2;
    int r1;
    int r2;

    if (*(char *)(param_1 + 2) != 6) {
        r1 = (util_Random(0x28) & 0xffff) + 0xf;
        r2 = (util_Random(0x28) & 0xffff) + 0xf;
        if (*(int *)(param_1 + 0x194) < 3)
            return 0x20;
        chr1 = gm_FindChar(param_2[0x214]);
        if (chr1 == 0)
            return 0x60;
        chr2 = gm_FindChar(param_2[0x218]);
        if (chr2 == 0)
            return 0x60;
        cm_SendRelations(*(int *)(chr1 + 4), *(int *)(chr2 + 4), r1, 1);
        cm_SendRelations(*(int *)(chr2 + 4), *(int *)(chr1 + 4), r2, 1);
        cm_SendMoneyDelta(*(int *)(chr1 + 4), 0xfffffffd);
        /* ROUGH SKELETON: the message chain and the return value are not written yet */
        return 0x10;
    }
    /* ROUGH SKELETON: type 6 opens the window at 0x5cc433 (the 0x5c1130 menus, the records and the pump
       0x5309a0 are not written yet) */
    return 0;
}
