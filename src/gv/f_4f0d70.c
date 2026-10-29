/* 0x4f0d70, the message 0x4c60b0 type 6 (graveyard id -1, object id +1 of o, value x at +0x13); -1 when o is 0 */

int unk_Unk4f0d70(char *o, int x)
{
    char m[0x1d];

    if (o == 0)
        return -1;
    *(int *)(m + 0xc) = 0;
    m[0x10] = 0;
    *(short *)(m + 0x11) = 0;
    *(int *)m = 6;
    *(int *)(m + 4) = -1;
    *(int *)(m + 8) = *(int *)(o + 1);
    *(int *)(m + 0x13) = x;
    return ((int (*)(void *))ai_Unk4c60b0)(m);
}
