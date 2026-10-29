/* 0x577330, a transition callback on the flag S+0x38: it is raised to 1 by the kind 0x28 (step word 0x155), to 2
   by the kind 0x27 (0x1d0) and to 3 by the kind 0x24 on the player's building (table byte 0x12, word +0x27 =
   0x6f25b8), each when the flag already reaches the previous level (1) and the step fits; the kinds 0x1a / 0x1c
   answer 1, everything else 0 */

int ai_Unk577330(int *p)
{
    int kind;

    if (U_I(S + 0x38) >= 0 && p[0] == 0x28 && p[3] == 0x155) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    if (U_I(S + 0x38) >= 1 && p[0] == 0x27 && p[3] == 0x1d0) {
        U_I(S + 0x38) = 2;
        return 1;
    }
    if (U_I(S + 0x38) >= 2 && p[1] && p[0] == 0x24) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId) {
            U_I(S + 0x38) = 3;
            return 1;
        }
    }
    if (p[0] == 0x1a || p[0] == 0x1c)
        return 1;
    return 0;
}
