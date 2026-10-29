/* 0x5769c0, the kinds 0x1b / 0x1c with a building whose table byte is 0xd: the kind 0x1b only while the pending
   state is 4 (state 0xa), the kind 0x1c in any state (state 0xb); else 3. Ends 0x576a7f; 0x576a80 is a new start
   without nop padding */

int ai_Unk5769c0(int *p)
{
    int kind;

    if (p[0] == 0x1b) {
        if (!p[1])
            goto next;
        if (U_I(S + 4) != 4)
            goto next;
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xd)
            goto fail;
        U_I(S) = 0xa;
        U_I(S + 4) = 0xa;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
next:
    if (p[0] == 0x1c) {
        if (!p[1])
            goto fail;
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xd)
            goto fail;
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 1;
    }
fail:
    return 3;
}
