/* 0x5a88e0, the factor of a setting (31 B): the +0x38 of the 0x5a8540 record (= +0x44 of the record), else 1.0f */

float ai_SettingFactor(int param_1)
{
    int p;

    p = ai_Unk5a8540(*(int *)(param_1 + 1));
    if (p != 0)
        return *(float *)(p + 0x38);
    return 1.0f;
}
