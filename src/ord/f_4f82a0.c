/* 0x4f82a0, resets the statistics at the start of a game (the only caller is main_Unk537de0, 0x537e3f, after
   the graveyard clear): clears the message record 0x91b480 (0x24 bytes, then its first int = -1 = no event),
   the income history gm_IncomeHistory (0x154), the character statistics 0x91b500 (0x68), the 8 x 8 grid
   ord_GridCells (0x600), the statistics history (0x140), the last statistics record (0x28) and the base rating
   0x6f1b9c. No result. The -1 store goes through the alias ord_MessageRecordEnabled: through
   ord_MessageRecord the compiler drops the whole clear as a dead store. */

void ord_ResetStatistics(void)
{
    memset(ord_MessageRecord, 0, 0x24);
    memset(gm_IncomeHistory, 0, 0x154);
    memset(gm_CharacterStatistics, 0, 0x68);
    memset(ord_GridCells, 0, 0x600);
    memset(ord_StatisticsHistory, 0, 0x140);
    memset(ord_StatisticsRecord, 0, 0x28);
    ord_MessageRecordEnabled = -1;
    ai_RatingBase = 0;
}
