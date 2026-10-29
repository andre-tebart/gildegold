/* 0x4fa2a0, income kind 4 (the record 0xc, the sum of the ints at +0x1a0 and +0x1a4; sum 0x91b68c). ROUGH */

int unk_Unk4fa2a0(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;
    char *c;

    gv_GetTableRecord(0xc, rec);
    gm_Query(*(int *)((char *)gm_FirstBuilding(1) + 0x5d), 2, 6, 0, 0x115);
    c = CH(id);
    cnt = *(int *)(c + 0x1a4) + *(int *)(c + 0x1a0);
    x = (float)(rec[6] * 0.01f * *(int *)(c + 0x1a4));
    if (!(x > 0))
        x = 0;
    n = (int)x;
    for (k = 0; k < 6; k++) {
        if (out[k] == 0) {
            if (k >= 6)
                return 0;
            if (cnt > 0) {
                out[k] = 4;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                *(int *)(out + 8 + k * 4) = n;
                if (flags & 1) {
                    char *o, *b = (char *)gm_FirstBuilding(1);
                    o = (char *)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
                    cm_SendMoveGoodsUnchecked(*(int *)(o + 2), *(int *)(CH(id) + 4), 0, n);
                    ai_Unk6f1b98++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b68c += n;
            }
            return 1;
        }
    }
    return 0;
}
