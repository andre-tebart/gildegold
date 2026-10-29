/* 0x5a8850, the weights of a setting refreshed (31 B): 0x5a84b0 then 0x5a83b0 (param_1, the record) */

void ai_Unk5a8850(int param_1)
{
    int rec;

    rec = ai_Unk5a84b0(*(int *)(param_1 + 1));
    if (rec != 0)
        ai_Unk5a83b0(param_1, (char *)rec);
}
