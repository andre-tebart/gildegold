/* 0x4fa160, income kind 3 (the record 0xd, the int at +0x1a8, which is cleared when flag 2 is set; the amount goes to the player only when it does not exceed 0x509e60 of the character; sum 0x91b648). ROUGH */

int unk_Unk4fa160(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;
    char *c;

    gv_GetTableRecord(0xd, rec);
    gm_Query(*(int *)((char *)gm_FirstBuilding(1) + 0x5d), 2, 6, 0, 0x115);
    for (k = 0; k < 6; k++) {
        if (out[k] == 0) {
            if (k >= 6)
                return 0;
            id &= 0xffff;
            c = CH(id);
            cnt = *(int *)(c + 0x1a8);
            x = (float)(rec[6] * 0.01f);
            x = (float)(x * cnt);
            if (!(x > 0))
                x = 0;
            n = (int)x;
            if (cnt > 0) {
                out[k] = 3;
                *(int *)(out + 8 + k * 4) = n;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                if (flags & 1) {
                    char *o, *b = (char *)gm_FirstBuilding(1);
                    o = (char *)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
                    if (n <= gm_GetGoodsValue(c))
                        cm_SendMoveGoodsUnchecked(*(int *)(o + 2), *(int *)(c + 4), ai_OwnPlayer, n);
                }
                if (flags & 2) {
                    *(int *)(c + 0x1a8) = 0;
                    *(int *)ai_Unk91b648 += n;
                }
            }
            return 1;
        }
    }
    return 0;
}
