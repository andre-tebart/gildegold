/* 0x4e3230, 0x403390 (arguments, if any, are passed on untouched) unless the replay mode is on */

int ai_Unk4e3230(void)
{
    if (ai_Unk6f0d8c != 0)
        return 0;
    return ch_Create();
}
