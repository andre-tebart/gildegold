/* 0x5ba620, the Domizil table lookup (64 B): walks the 0x1c-byte entries of 0x62c420 while the id at +0 is not
   0; returns the entry of the id, 0 when absent */

int ai_Unk5ba620(int param_1)
{
    int i;

    for (i = 0; U_I((unsigned)&full_data_62c420 + i * 0x1c) != 0; i++)
        if (U_I((unsigned)&full_data_62c420 + i * 0x1c) == param_1)
            return (unsigned)&full_data_62c420 + i * 0x1c;
    return 0;
}
