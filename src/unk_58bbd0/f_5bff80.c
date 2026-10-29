/* 0x5bff80, the release of a record list (39 B): 0x475e40 (the slider +8, 1) for each of the n records of
   0x38 bytes */

int ai_Unk5bff80(int param_1, int param_2)
{
    for (; param_1 > 0; param_1--) {
        d2_SetObjEnabled(U_I(param_2 + 8), 1);
        param_2 += 0x38;
    }
    return 0;
}
