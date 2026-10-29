/* 0x4f01d0, the message 0x4c60b0 type 0 (graveyard id +4 of g, object id +1 of o, value v, byte b); -1 when g or o is 0 or b is -1 */

int unk_Unk4f01d0(int *g, char *o, int v, int b)
{
    char m[0x1d];

    if (g != 0 && o != 0 && b != -1) {
        *(int *)(m + 4) = g[1];
        *(int *)(m + 8) = *(int *)(o + 1);
        *(int *)m = 0;
        m[0x10] = (char)b;
        *(int *)(m + 0xc) = v;
        *(short *)(m + 0x11) = 0;
        *(int *)(m + 0x13) = 0;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
