/* 0x576600, 1 unless the kind 0x19 has a building whose table byte is not 0xf */

int ai_Unk576600(int *p)
{
    int kind;

    if (p[0] != 0x19 || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0xf)
        return 0;
    return 1;
}
