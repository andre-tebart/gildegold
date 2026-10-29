/* 0x4fa060, income kind 2 (the record 10, the int at +0x19c; sum 0x91b604): a copy of 0x4f9f60 */

int unk_Unk4fa060(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;

    gv_GetTableRecord(10, rec);
    for (k = 0; k < 6; k++) {
        if (out[k] == 0) {
            if (k >= 6)
                return 0;
            cnt = *(int *)(CH(id) + 0x19c);
            x = (float)(rec[6] * 0.01f);
            x = (float)(x * cnt);
            if (!(x > 0))
                x = 0;
            n = (int)x;
            if (cnt > 0) {
                out[k] = 2;
                *(int *)(out + 8 + k * 4) = n;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                if (flags & 1) {
                    char *o, *b = (char *)gm_FirstBuilding(1);
                    o = (char *)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
                    cm_SendMoveGoodsUnchecked(*(int *)(o + 2), *(int *)(CH(id) + 4), 0, n);
                    ai_Unk6f1b98++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b604 += n;
            }
            return 1;
        }
    }
    return 0;
}
