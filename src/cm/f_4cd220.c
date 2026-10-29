/* 0x4cd220, handler of type 0x68: the data[4] records (0x40 bytes each) of the joined message become the list of the
   object data[0] (+0x29, rt_CopyPoints; the old list is freed) */

int ai_Unk4cd220(G17MSG *m, G17STAT *s)
{
    char buf[0x240];
    char *q;
    int n, i, prev;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 0;
        s->value = 0;
    }
    q = gm_FindObjectInst(G17_I(*m, 0));
    if (q == 0)
        return -1;
    if (*(int *)(q + 0x29) != 0) {
        rt_FreePoints(*(int *)(q + 0x29));
        *(int *)(q + 0x29) = 0;
    }
    n = G17_I(*m, 4);
    memcpy(buf, ai_Unka68d40, n << 6);
    prev = 0;
    for (i = 0; i < n; i++) {
        *(int *)(buf + i * 0x40 + 0x34) = prev;
        *(int *)(buf + i * 0x40 + 0x38) = (int)(buf + i * 0x40 + 0x3c);
        prev = (int)(buf + i * 0x40 + 0x3c) - 0x40;
    }
    *(int *)(buf + n * 0x40 - 4) = 0;
    *(int *)(q + 0x29) = rt_CopyPoints(buf);
    if (s != 0)
        s->state = 1;
    return 0;
}
