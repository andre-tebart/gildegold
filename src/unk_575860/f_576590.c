/* 0x576590, the kind 0x19 with a building whose table byte is 0xf and the step word p[3] 0x110 */

int ai_Unk576590(int *p)
{
    int kind;

    if (p[0] != 0x19 || !p[1])
        return 0;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
        return 0;
    if (p[3] != 0x110)
        return 0;
    return 1;
}
