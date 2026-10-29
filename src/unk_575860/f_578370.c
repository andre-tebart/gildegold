/* 0x578370, the tutorial stop (196 B): the six handles 0x6f2640 .. 0x6f2658 unloaded when set (0x426ba0), the
   record chain 0x6f2658 freed by 0x578440 and cleared; the state record 0x5eec60 reset (+0, +8, +0xc, +0x10,
   +0x14, +0x18, +0x1c = 0, +0x20 = -1, the forms +0x24 / +0x34 released (0x4758f0) and -1, +0x14 = 0 again),
   the gates 0x6f265c / 0x6f2660 = 0; no return value */

void ai_Unk578370(void)
{
    int i;

    for (i = (unsigned)&full_data_6f2640; i < (int)((unsigned)&full_data_6f2640 + 0x18); i += 4) {
        if (U_I(i))
            sb_UnloadSampleBank(U_I(i));
    }
    ai_Unk578440(U_I((unsigned)&full_data_6f2658));
    U_I((unsigned)&full_data_6f2658) = 0;
    U_I(S) = 0;
    U_I(S + 8) = 0;
    U_I(S + 0xc) = 0;
    U_I(S + 0x10) = 0;
    U_I(S + 0x14) = 0;
    U_I(S + 0x18) = 0;
    U_I(S + 0x1c) = 0;
    U_I(S + 0x20) = -1;
    d2_CloseView(U_I(S + 0x24));
    U_I(S + 0x24) = -1;
    d2_CloseView(U_I(S + 0x34));
    U_I(S + 0x34) = -1;
    U_I(S + 0x14) = 0;
    U_I((unsigned)&full_data_6f265c) = 0;
    U_I((unsigned)&full_data_6f2660) = 0;
}
