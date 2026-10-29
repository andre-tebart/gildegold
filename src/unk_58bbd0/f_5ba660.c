/* 0x5ba660, the Domizil table lookup with the index out (80 B): like 0x5ba620, and the index of the entry is
   stored at *param_2 when that is not 0 */

int ai_Unk5ba660(int param_1, int *param_2)
{
    int i;

    for (i = 0; U_I((unsigned)&full_data_62c420 + i * 0x1c) != 0; i++)
        if (U_I((unsigned)&full_data_62c420 + i * 0x1c) == param_1) {
            if (param_2 != 0)
                *param_2 = i;
            return (unsigned)&full_data_62c420 + i * 0x1c;
        }
    return 0;
}
