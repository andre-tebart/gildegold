/* 0x58ad20, waits for the player list (128 B): 0x4cd610 and its answer 0x4c3ae0; the list 0x4cd5d0 -> 0x6f267c;
   the entry 0x6f267c[1 .. 8] with a player (0x4cd820) is awaited (0x4c3ae0)
   @deep Q1 (unit unnamed, @named_by -); B: code 28 (the entry scan of the player list 0x6f267c, 0x4cd820); Q2 (5 callees), Q3 (0x6f267c), Q4 U_I; needs the player-list API and the record typed */

void ai_Unk58ad20(void)
{
    int h;
    int r;
    int i;

    h = ((int (*)())ai_Unk4cd610)();
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    U_I((unsigned)&full_data_6f267c) = ai_Unk4cd5d0();
    r = -1;
    for (i = 4; i < 0x24; i += 4)
        if (U_I(U_I((unsigned)&full_data_6f267c) + i) != 0)
            r = ((int (*)(int))ai_Unk4cd820)(U_I(U_I((unsigned)&full_data_6f267c) + i));
    if (r != -1)
        while (cm_GetSlotState(r) == 0)
            sv_PumpNet();
}
