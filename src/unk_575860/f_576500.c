/* 0x576500, a callback predicate: the kinds 0x19 / 0x1a with a building whose table byte is 0xf; the kind 0x19
   matches when the step word p[3] is 0x10e, the kind 0x1a when the building word 0x6f2310 is 0x10e; a missing
   pointer or another kind answers p[0] != 0x1b. 0x576583 nop, the next start at 0x576590 */

int ai_Unk576500(int *p)
{
    int kind;

    if (p[0] == 0x19 && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
            return 0;
        return p[3] == 0x10e;
    }
    if (p[0] == 0x1a && p[1]) {
        kind = *(unsigned char *)p[1];
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
            return 1;
        return *(unsigned short *)(unsigned)&sv_PanelShown != 0x10e;
    }
    return p[0] != 0x1b;
}
