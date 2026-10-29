/* 0x4e3250, the scene helpers 0x4e3250 .. 0x4e3640 pass their arguments on to the sound / object calls unless the replay mode (0x6f0d8c) is on */

int ai_Unk4e3250(int a)
{
    if (ai_Unk6f0d8c != 0)
        return 1;
    return ch_Delete(a);
}
