/* 0x5a8900, the factor of a setting set (28 B): the +0x44 of the 0x5a84b0 record */

void ai_Unk5a8900(int param_1, float param_2)
{
    int rec;

    rec = ai_Unk5a84b0(*(int *)(param_1 + 1));
    if (rec != 0)
        *(float *)(rec + 0x44) = param_2;
}
