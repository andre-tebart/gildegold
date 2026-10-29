/* 0x4d8c70, draws the next picture 0x4df of a stack (o + 0x2c counts them, 10 up each) on the window 0x6ed938 */

/* ---- cut.c (joined) ---- */

void ai_Unk4d8c70(int unused, char *o)
{
    ((int (*)(short, short, int, int))d2_WinOpenPic)(0x8c, (short)(0x44 - *(short *)(o + 0x2c) * 10), 0x4df, d2_LastWindow);
    *(int *)(o + 0x2c) = *(int *)(o + 0x2c) + 1;
}
