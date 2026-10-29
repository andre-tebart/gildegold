/* 0x576680, the same as 0x576640 for the table bytes 0xf / 2 / 0x12 */

int ai_Unk576680(int *p)
{
    int kind;
    unsigned char c;

    if (p[0] != 0x19 || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    c = *(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d);
    if (c == 0xf || c == 2 || c == 0x12)
        return 1;
    return 0;
}
