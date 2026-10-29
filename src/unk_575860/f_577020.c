/* 0x577020, the predicate twin of 0x576ee0: 1 for a building of the table byte 0x2 with the step word
   0x5 / 0x2 / 0xd / 0x8 and for the kinds 0x1a / 0x19 / 0x1c; else 0 */

int ai_Unk577020(int *p)
{
    int kind;

    if (p[0] == 0x2b && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 2
                && *(unsigned short *)(p[1] + 0x27) == *(unsigned short *)(unsigned)&ai_PlayerId
                && (p[3] == 5 || p[3] == 2 || p[3] == 0xd || p[3] == 8))
            return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x19 || p[0] == 0x1c)
        return 1;
    return 0;
}
