/* 0x4f0430, the message 0x4c60b0 type 1 (graveyard id +4 of g, object id +1 of o, value v, short s, int x, byte 0xff); -1 when g or o is 0 */

int unk_Unk4f0430(int *g, char *o, int v, short s, int x)
{
    char m[0x1d];

    if (g != 0 && o != 0) {
        *(int *)(m + 4) = g[1];
        *(int *)(m + 0xc) = v;
        *(int *)(m + 8) = *(int *)(o + 1);
        *(int *)m = 1;
        m[0x10] = (char)0xff;
        *(short *)(m + 0x11) = s;
        *(int *)(m + 0x13) = x;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
