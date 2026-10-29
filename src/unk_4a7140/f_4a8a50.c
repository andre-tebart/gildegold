/* 0x4a8a50, decide callback: the decision record ctx[1] proposes a value (its fn) within min..max, into m
   (kind 0x11) */

int ai_Unk4a8a50(int a, AIPER *p, int *ctx, int d, unsigned char *m)
{
    AIDEC dec;
    int v;

    if (gv_GetTableRecord(ctx[1], &dec) != 0) {
        if (((int (*)(AIPER *, int, int *))dec.fn)(p, (unsigned char)dec.unk00, &v) != 0 && v != dec.cur
            && v >= dec.min && v <= dec.max) {
            m[0] = 0x11;
            *(unsigned int *)(m + 4) = (unsigned char)dec.unk00;
            *(int *)(m + 8) = v;
            return 1;
        }
    }
    return 0;
}
