/* 0x4f0e80, the message 0x4c60f0 (g +4, o +1, v, byte b, int x at +0xd, int y at +0x11) */

void unk_Unk4f0e80(int *g, int x, char *o, int v, char b, int y)
{
    char m[0x19];

    *(int *)m = g[1];
    *(int *)(m + 4) = *(int *)(o + 1);
    m[0xc] = b;
    *(int *)(m + 8) = v;
    *(int *)(m + 0xd) = x;
    *(int *)(m + 0x11) = y;
    ai_Unk4c60f0(m);
}
