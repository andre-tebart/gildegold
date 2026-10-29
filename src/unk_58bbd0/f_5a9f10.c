/* 0x5a9f10, a Domizil entry in a range (191 B): 0x42ec00 (the count) picks an index into the table 0x62c420; each
   try returns the entry's +0 id when its +0x18 word lies in param_1 .. param_2, else the index advances modulo
   the count; 0 when no entry does */

unsigned short ai_Unk5a9f10(int param_1, int param_2)
{
    int n;
    int i;

    i = util_Random(U_I((unsigned)&full_data_62cb58)) & 0xffff;
    n = U_I((unsigned)&full_data_62cb58);
    if (n == 0)
        return 0;
    while (n != 0) {
        if ((int)U_W((unsigned)&full_data_62c420 + i * 0x1c + 0x18) >= param_1 &&
            (int)U_W((unsigned)&full_data_62c420 + i * 0x1c + 0x18) <= param_2)
            return U_W((unsigned)&full_data_62c420 + i * 0x1c);
        i = (i + 1) % U_I((unsigned)&full_data_62cb58);
        n--;
    }
    return 0;
}
