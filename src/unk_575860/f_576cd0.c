/* 0x576cd0, a predicate: 1 for the kinds 0x1a / 0x19 / 0x1c, and for the kind 0x2b on the player's building (byte
   0x12, word +0x27 = 0x6f25b8) with the step word 0xc / 0x1df; else 0 */

int ai_Unk576cd0(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId
                && (p[3] == 0xc || p[3] == 0x1df))
            return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x19 || p[0] == 0x1c)
        return 1;
    return 0;
}
