/* 0x5780e0, predicate (49 B): -4 without the gate 0x6f265c; 0 while the input is idle (0xf8a668 == -1) or the
   current window 0x5fe710 is not the control S+0x4c; 1 when it is (the 0x577a00 shape on S+0x4c) */

int ai_Unk5780e0(void)
{
    if (!U_I((unsigned)&full_data_6f265c))
        return -4;
    if (U_I((unsigned)&d2_ClickedId) != -1 && U_I((unsigned)&ai_Unk5fe710) == U_I(S + 0x4c))
        return 1;
    return 0;
}
