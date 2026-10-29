/* 0x4f0900, the message 0x4c60b0 type 3 (graveyard id +4 of g or -1, object id +1 of o, value v, byte b); -1 when o is 0 or b is -1 */

int unk_Unk4f0900(int *g, char *o, int v, int b)
{
    char m[0x1d];

    if (o != 0 && b != -1) {
        if (g == 0)
            *(int *)(m + 4) = -1;
        else
            *(int *)(m + 4) = g[1];
        *(int *)(m + 8) = *(int *)(o + 1);
        *(int *)m = 3;
        *(int *)(m + 0xc) = v;
        m[0x10] = (char)b;
        *(short *)(m + 0x11) = 0;
        *(int *)(m + 0x13) = 0;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
