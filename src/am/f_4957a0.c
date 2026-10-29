/* 0x4957a0, whether an action is on the evaluation stack */

int ai_Unk4957a0(int k)
{
    int i;

    for (i = 0; i < ai_Unkf83ea4; i++) {
        if (ai_Unkf86900[i] == k)
            return 1;
    }
    return 0;
}
