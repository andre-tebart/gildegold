/* 0x576640, 1 for the kind 0x19 with a table byte 0xf or 2 (a missing record / kind answers 1) */

int ai_Unk576640(int *p)
{
    int kind;
    unsigned char c;

    if (p[0] != 0x19 || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    c = *(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d);
    if (c == 0xf || c == 2)
        return 1;
    return 0;
}
