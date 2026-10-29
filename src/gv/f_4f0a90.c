/* 0x4f0a90, the message 0x4c60b0 type 4 (graveyard id +4 of g or -1, object id +1 of o, value v); -1 when o is 0 */

int unk_Unk4f0a90(int *g, char *o, int v)
{
    char m[0x1d];

    if (o == 0)
        return -1;
    if (g == 0)
        *(int *)(m + 4) = -1;
    else
        *(int *)(m + 4) = g[1];
    *(int *)(m + 8) = *(int *)(o + 1);
    *(int *)m = 4;
    *(int *)(m + 0xc) = v;
    m[0x10] = 0;
    *(short *)(m + 0x11) = 0;
    *(int *)(m + 0x13) = 0;
    return ((int (*)(void *))ai_Unk4c60b0)(m);
}
