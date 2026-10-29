/* 0x576ba0, a transition callback on the kinds 0x2b / 0x10 for a building (table 0x7e3f30, byte 0x12) whose word
   +0x27 is the player's 0x6f25b8: the kind 0x2b with the step word p[3] 0xc / 0x1df and the flag +0x38 clear sets
   the flag and the state 0xa, and answers 3; the kind 0x10 sets the pending state 9 (p[2] 0), 0xb / 4 (p[2] 1 by
   the flag +0x38), applies it and answers 1 */

int ai_Unk576ba0(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
            return 3;
        if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
            return 3;
        if (U_I(S + 0x38))
            return 3;
        if (p[3] != 0xc && p[3] != 0x1df)
            return 3;
        U_I(S + 0x38) = 1;
        U_I(S + 4) = 0xa;
        U_I(S) = U_I(S + 4);
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 3;
    }
    if (p[0] != 0x10 || !p[1])
        return 3;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
        return 3;
    if (*(unsigned short *)(p[1] + 0x27) != *(unsigned short *)(unsigned)&ai_PlayerId)
        return 3;
    if (p[2] == 0 && !U_I(S + 0x38))
        U_I(S + 4) = 9;
    if (p[2] == 1) {
        if (U_I(S + 0x38) == 1)
            U_I(S + 4) = 0xb;
        else
            U_I(S + 4) = 4;
    }
    U_I(S) = U_I(S + 4);
    U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
    return 1;
}
