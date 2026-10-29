/* 0x5c1b20, opens the special-amount menu 0x5c1b40 with the callback 0x5c1500 and the caption record
   0x143a3f0 (36 B), then exits through 0x52d7a0 */

int ai_Unk5c1500(); int ai_Unk5c1e10();

int ai_Unk5c1b20(void)
{
    ai_Unk5c1b40((int)ai_Unk5c1500, U_I((unsigned)&full_data_143a3f0), 1);
    sv_CameraFollow();
    return 0;
}
