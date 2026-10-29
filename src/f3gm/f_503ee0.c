/* 0x503ee0, copies the 6-byte record n of the table 0x61a688 (0x5f records; larger n: the first one) to out */

void gm_Unk503ee0(unsigned *out, int n)
{
    if (n > 0x5e) {
        *out = *(unsigned *)ai_Unk61a688;
        *(short *)(out + 1) = *(short *)(ai_Unk61a688 + 4);
        return;
    }
    *out = *(unsigned *)(ai_Unk61a688 + n * 6);
    *(short *)(out + 1) = *(short *)(ai_Unk61a688 + 4 + n * 6);
}
