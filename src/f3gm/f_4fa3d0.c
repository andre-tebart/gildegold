/* 0x4fa3d0, income kind 5 (the record 0xb, the int at +0x19c; sum 0x91b6d0); the message goes to all players (-1). ROUGH */

int unk_Unk4fa3d0(unsigned id, char *out, unsigned char flags)
{
    int rec[9];
    int k, n, cnt;
    float x;

    gv_GetTableRecord(0xb, rec);
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
                out[k] = 5;
                *(int *)(out + 8 + k * 4) = n;
                *(int *)(out + 0x20 + k * 4) = rec[6];
                if (flags & 1) {
                    cm_SendMoveGoodsUnchecked(-1, *(int *)(CH(id) + 4), 0, n);
                    ai_Unk6f1b98++;
                }
                if (flags & 2)
                    *(int *)ai_Unk91b6d0 += n;
            }
            return 1;
        }
    }
    return 0;
}
