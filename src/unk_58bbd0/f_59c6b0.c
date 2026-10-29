/* 0x59c6b0, the clicked record of the exchange list (48 B): with 0x1467398 set, the index of the record whose
   control (record + 0) is the clicked control 0x5fe710, else -1 */

int ai_Unk59c6b0(int *param_1)
{
    int i;

    if (U_I(((unsigned)&dm_L + 0x18)) != 0) {
        for (i = 0; i < 16; i++) {
            if (param_1[i * 7] != -1 && U_I((unsigned)&ai_Unk5fe710) == param_1[i * 7])
                return i;
        }
    }
    return -1;
}
