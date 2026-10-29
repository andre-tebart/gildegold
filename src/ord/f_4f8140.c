/* 0x4f8140, an order handler (c, m; kinds 1 / 3) for the jobs 1 .. 0x1b (+0x166): kind 3 of a player runs 0x5ce7f0, kind 1 writes the text 0x1e23 (m + 0x14); else 2 (1 for other jobs and kinds) @deep meaning settled and readable; the remaining 85 code bytes are allocation only: the original keeps m[0] in ecx and the constant 2 in eax (hoisted to the prologue, re-materialised after the txt_Format call) and pushes ebx+esi (job in bl), ours uses eax/ecx+edx and pushes esi only - the same statements in the same order (tools/disasm.py 0x4f8140 vs asmdiff). Tried and reverted: r = 2 as a separate assignment and as an initialiser with if/else-if and one final return r, the return-2/return-1 form, single-return with r = 1 in both else branches (that one pushes edi and is worse). Needs match-deep's register-allocation toolbox */

int ord_DismissWorkerOrder(char *c, int *m)
{
    unsigned char j;

    if (m[0] == 3 || m[0] == 1) {
        j = ORD_CHAR(c)->job;
        if (j != 0 && j <= 0x1b) {
            if (m[0] == 3 && ORD_CHAR(c)->kind == 6)
                return ai_WorkerDismissalWindow(c, m);
            if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
                txt_Format(ORD_MSG(m)->text, "%s", 0x1e23);
            return 2;
        }
    }
    return 1;
}
