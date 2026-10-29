/* 0x5764b0, a callback predicate: 0 when the step kind is 0x1a, the record p[1] names a building kind whose table
   byte (0x7e3f30, 0x24d bytes per kind) is 0x12 and the word 0x6f2310 is 0x74; else 1. 65 B, the next start
   0x576500 is not Ghidra's (its 1776 B record begins here) */

int ai_Unk5764b0(int *p)
{
    int kind;

    if (p[0] != 0x1a || !p[1])
        return 1;
    kind = *(unsigned char *)p[1];
    if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) != 0x12)
        return 1;
    if (*(unsigned short *)(unsigned)&sv_PanelShown != 0x74)
        return 1;
    return 0;
}
