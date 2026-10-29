/* 0x5768a0, when `arg` is 0, finds a record (0x501190 (1, 5, 0xf)) and writes 0x10e into its word +0x29; no return
   value (0x501190 returns the record) */

void ai_Unk5768a0(int arg)
{
    char *r;

    if (arg)
        return;
    r = (char *)gm_BuildQuery(1, 5, 0xf);
    if (r)
        *(unsigned short *)(r + 0x29) = 0x10e;
}
