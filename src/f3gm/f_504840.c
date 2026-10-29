/* 0x504840, drops the character c one title rank (+0x165 - 1; 0xc for a master of kind 0x20, 0xb for kind 0x1f) and clears its int +0x174, both sent through 0x4c3d90. Returns -1 */

int gm_DropRank(char *c)
{
    char *m;
    int v;

    m = *(char **)(c + 0x16c);
    if (m != 0) {
        if (*m == ' ') {
            v = 0xc;
            goto send;
        }
        if (m != 0 && *m == 0x1f) {
            v = 0xb;
            goto send;
        }
    }
    v = (unsigned char)c[0x165] - 1;
send:
    cm_BeginFieldBlock(c, *(int *)(c + 4));
    cm_AddFieldValue(1, 1, 0x165, &v);
    cm_SendFieldValues();
    v = 0;
    cm_BeginFieldBlock(c, *(int *)(c + 4));
    cm_AddFieldValue(4, 1, 0x174, &v);
    cm_SendFieldValues();
    return -1;
}
