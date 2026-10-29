/* 0x5ce1a0, the spy target handler with the flag set (775 B, ROUGH SKELETON: no Ghidra export; from the
   disassembly; the twin of 0x5cde90): param_1 the message record (the AIMSG: +0 the id, +2 the type
   6 / 7, +0x166 the kind 0xd refused), param_2 the request (+0x214 the target id). Types other than
   6 / 7: the target 0x505df0 (missing or kind 0xd -> 0x60); the list 0x4f3fe0 (the target id, the ids,
   the message id) must be nonempty (else 0x60) and goes to 0x5cd8c0 (the message, the target, the
   count, 1). Types 6 / 7 open the window at 0x5ce258. ROUGH SKELETON: the window path is a comment. */

int ai_SpyTargetFlagHandler(int param_1, int *param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int ids[0x100];
    int target;
    int n;

    if (*(char *)(param_1 + 2) != 6 && *(char *)(param_1 + 2) != 7) {
        if (*(unsigned char *)(param_1 + 0x166) == 0xd)
            return 0x60;
        target = gm_FindChar(param_2[0x214]);
        if (target == 0)
            return 0x60;
        if (*(unsigned char *)(target + 0x166) == 0xd)
            return 0x60;
        n = unk_Unk4f3fe0(param_2[0x214], ids, *(unsigned short *)param_1);
        if (n == 0)
            return 0x60;
        return ai_Unk5cd8c0(param_1, (int *)target, n, 1);
    }
    /* ROUGH SKELETON: the window at 0x5ce258 (the rows and the pump 0x5309a0) is not written yet */
    return 0x20;
}
