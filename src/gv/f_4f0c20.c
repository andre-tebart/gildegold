/* 0x4f0c20, the message 0x4c60b0 type 5 (graveyard id -1, object id +1 of o, value v, byte b, int x); -1 when o is 0 or b is -1 */

int unk_Unk4f0c20(char *o, int v, int b, int x)
{
    char m[0x1d];

    if (o != 0 && b != -1) {
        *(int *)(m + 8) = *(int *)(o + 1);
        m[0x10] = (char)b;
        *(int *)m = 5;
        *(int *)(m + 4) = -1;
        *(int *)(m + 0xc) = v;
        *(short *)(m + 0x11) = 0;
        *(int *)(m + 0x13) = x;
        return ((int (*)(void *))ai_Unk4c60b0)(m);
    }
    return -1;
}
