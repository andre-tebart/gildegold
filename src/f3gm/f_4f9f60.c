/* 0x4f9f60, income kind 1 of the character id (rate of the record 9 * 0.01 * the character's int at +0x19c, not below 0): stores kind 1, the amount (+8 + 4k) and the record's rate (+0x20 + 4k) in the first free slot k of the 6 of out;
   flag 1 sends the amount to the player, flag 2 adds it to the sum 0x91b5c0. Returns 1 (slot found) or 0 */

int unk_Unk4f9f60(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;

    gv_GetTableRecord(9, rec);
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
                out[k] = 1;
                *(int *)(out + 8 + k * 4) = n;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                if (flags & 1) {
                    char *o, *b = (char *)gm_FirstBuilding(1);
                    o = (char *)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
                    cm_SendMoveGoodsUnchecked(*(int *)(o + 2), *(int *)(CH(id) + 4), 0, n);
                    ai_Unk6f1b98++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b5c0 += n;
            }
            return 1;
        }
    }
    return 0;
}
