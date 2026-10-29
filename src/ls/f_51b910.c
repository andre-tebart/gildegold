/* 0x51b910, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep B3 code hunks 81, all four hunks are two loops' induction-variable base: 0x51bc0f/0x51bd2e VC6 bases the IV at p+4 (lea [edi-8]..[edi+8], cmp edi-4) / row+2, the original keeps p (lea [edi-4]..[edi+0xc], cmp edi) / row; the 372 instructions before them are equal after the LS_W early-return rewrite (the if(r!=0){...} return 0 form made VC6 merge every return block, 361 hunks); read both loops and tried char* byte offsets, int*, int address, while and for spellings, VC6 canonicalizes the same; the loops' data/offsets are identical, only the base register spelling differs. Readability Q1/Q2/Q3/Q5 open: 34 save-image globals have no role evidence yet */


int unk_Unk51b910(int f)
{
  int iVar1;
  char *puVar3;
  char *puVar4;
  char *puVar5;
  LS_W(((char *)(unsigned)&ord_Statistics), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x4)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x8)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0xc)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x10)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x14)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x18)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x1c)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x20)), 4)
  LS_W(((char *)((unsigned)&ord_Statistics + 0x24)), 4)
  LS_W(((char *)(unsigned)&ord_RatingRate), 4)
  LS_W(((char *)(unsigned)&ord_StatisticsClock), 4)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0x4)), 2)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0x6)), 4)
  LS_W(((char *)((unsigned)&ord_StatisticsClock + 0xa)), 4)
  LS_W(((char *)(unsigned)&ai_RatingBase), 4)
  LS_W(((char *)(unsigned)&ai_Rating), 4)
  LS_W(((char *)(unsigned)&ord_MessageRecord), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x4)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x8)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0xc)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x10)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x14)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x18)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x1c)), 4)
  LS_W(((char *)((unsigned)&ord_MessageRecord + 0x20)), 4)
  puVar4 = ((char *)((unsigned)&ord_StatisticsHistory + 0x4));
  do {
    LS_W(puVar4 - 4, 4)
    LS_W(puVar4, 4)
    LS_W(puVar4 + 4, 4)
    LS_W(puVar4 + 8, 4)
    LS_W(puVar4 + 0xc, 4)
    puVar4 = puVar4 + 0x14;
  } while ((int)puVar4 < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0x4));
  LS_W(((char *)(unsigned)&gm_IncomeHistory), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x44), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x88), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0xcc), 0x44)
  LS_W(((char *)(unsigned)&gm_IncomeHistory + 0x110), 0x44)
  puVar3 = ((char *)((unsigned)&ai_Grid + 0x2));
  do {
    iVar1 = 0;
    puVar5 = puVar3;
    do {
      LS_W(puVar5 - 2, 2)
      LS_W(puVar5, 2)
      LS_W(puVar5 + 2, 1)
      LS_W(puVar5 + 6, 4)
      iVar1 = iVar1 + 1;
      puVar5 = puVar5 + 0x18;
    } while (iVar1 < 8);
    puVar3 = puVar3 + 0xc0;
  } while ((int)puVar3 < ((unsigned)((unsigned)ai_Grid + 0x600) + 0x2));
  return 1;
}
