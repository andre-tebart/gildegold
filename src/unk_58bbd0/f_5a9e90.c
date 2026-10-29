/* 0x5a9e90, a free entry of the Domizil table (215 B): 0x42ec00 (the count 0x62cb58) picks an index into the 0x42
   entries of 0x1c bytes at 0x62c420; each try returns the entry's +0 id when it is not in the param_1 words of
   param_2, else the index advances modulo the count; 0 when every id is taken */

unsigned short ai_Unk5a9e90(int param_1, unsigned short *param_2)
{
    int n;
    int i;
    int k;

    i = util_Random(U_I((unsigned)&full_data_62cb58)) & 0xffff;
    n = U_I((unsigned)&full_data_62cb58);
    if (n == 0)
        return 0;
    while (n != 0) {
        for (k = 0; k < param_1; k++) {
            if (param_2[k] == U_I((unsigned)&full_data_62c420 + i * 0x1c))
                break;
        }
        if (k >= param_1)
            return U_W((unsigned)&full_data_62c420 + i * 0x1c);
        i = (i + 1) % U_I((unsigned)&full_data_62cb58);
        n--;
    }
    return 0;
}
