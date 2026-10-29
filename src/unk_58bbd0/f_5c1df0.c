/* 0x5c1df0, opens the special-amount menu 0x5c1b40 with the callback 0x5c1e10 and the caption record
   0x143f030 (36 B), then exits through 0x52d7a0 */

int ai_Unk5c1df0(void)
{
    ai_Unk5c1b40((int)ai_Unk5c1e10, U_I((unsigned)&full_data_143f030), 1);
    sv_CameraFollow();
    return 0;
}
