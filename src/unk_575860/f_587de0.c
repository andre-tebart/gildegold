/* 0x587de0, clears the 0x4000-byte table 0x6f5f60 (17 B), called from main.c 0x532160 (the per-turn reset) */

void ai_ClearTurnTable(void)
{
    memset((char *)(unsigned)&full_data_6f5f60, 0, 0x4000);
}
