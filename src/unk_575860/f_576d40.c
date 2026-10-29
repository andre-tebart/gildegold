/* 0x576d40, the twin of 0x576ba0 for the kinds 0x2b (step word 0x78 / 0x79 / 0x7a) and 0x11 */

int ai_Unk576d40(int *p)
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
        if (p[3] != 0x78 && p[3] != 0x79 && p[3] != 0x7a)
            return 3;
        U_I(S + 0x38) = 1;
        U_I(S + 4) = 0xa;
        U_I(S) = U_I(S + 4);
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        return 3;
    }
    if (p[0] != 0x11 || !p[1])
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
