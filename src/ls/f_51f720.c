/* 0x51f720, reads the global tables 0x91b4c0 .. 0x91b690 (the counterpart of the writer 0x51b910). ROUGH: converted from the pseudocode @deep whole 358-line diff read against the source; meaning equal: the 26 leading 4-byte reads, the 2-byte + two 4-byte reads, the 0x18-byte block, the 5x4 loop (base 0x91ab04, 16 iterations to 0x91ac44), the four 0x44 reads and the gate on the 0x91b64c read, the 8x(2,2,1,4) record loop from 0x91ae62 to 0x91b461 (our '0x91b461 < p' is the original's 'jl 0x91b461' - equivalent, the stride 0x60 never hits the bound), the reads and sizes match. The remaining 327 code hunks are the known epilogue shape: the original ends every failed read with an inline 'xor eax,eax; pop edi; pop esi; pop ebp; pop ebx; ret' (31 of them), ours merges all into one shared return-0 block, so each check differs by je/jne + the 5-instruction epilogue; the loop base bias (-4 vs ours -8) is the same IV re-based. Readability: Q3 blocks it entirely - every read is an image literal (0x91b4c0 .. 0x91b690), needing the ~45 global table symbols the writer 0x51b910 writes named first; Q1 name, Q5 ROUGH. */


int unk_Unk51f720(int f)
{
  int iVar1;
  int iVar2;
  short *puVar3;
  int *puVar4;
  short *puVar5;
  LS_R(((char *)(unsigned)&ord_Statistics), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x4)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x8)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0xc)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x10)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x14)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x18)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x1c)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x20)), 4)
  LS_R(((char *)((unsigned)&ord_Statistics + 0x24)), 4)
  LS_R(((char *)(unsigned)&ord_RatingRate), 4)
  LS_R(((char *)(unsigned)&ord_StatisticsClock), 4)
  LS_R(((char *)((unsigned)&ord_StatisticsClock + 0x4)), 2)
  LS_R(((char *)((unsigned)&ord_StatisticsClock + 0x6)), 4)
  LS_R(((char *)((unsigned)&ord_StatisticsClock + 0xa)), 4)
  LS_R(((char *)(unsigned)&ai_RatingBase), 4)
  LS_R(((char *)(unsigned)&ai_Rating), 4)
  LS_R(((char *)(unsigned)&ord_MessageRecord), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x4)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x8)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0xc)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x10)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x14)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x18)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x1c)), 4)
  LS_R(((char *)((unsigned)&ord_MessageRecord + 0x20)), 4)
  puVar4 = ((char *)((unsigned)&ord_StatisticsHistory + 0x4));
  do {
    LS_R(puVar4 + -1, 4)
    LS_R(puVar4, 4)
    LS_R(puVar4 + 1, 4)
    LS_R(puVar4 + 2, 4)
    LS_R(puVar4 + 3, 4)
    puVar4 = puVar4 + 5;
  } while ((int)puVar4 < ((unsigned)((unsigned)ord_StatisticsHistory + 0x140) + 0x4));
  LS_R(((char *)(unsigned)&gm_IncomeHistory), 0x44)
  LS_R(((char *)(unsigned)&gm_IncomeHistory + 0x44), 0x44)
  LS_R(((char *)(unsigned)&gm_IncomeHistory + 0x88), 0x44)
  iVar1 = ls_Read((int)((unsigned)&gm_IncomeHistory + 0xcc), 0x44, 1, f);
  if (iVar1 != 0) {
    LS_R(((char *)(unsigned)&gm_IncomeHistory + 0x110), 0x44)
    puVar3 = ((char *)((unsigned)&ai_Grid + 0x2));
    do {
      iVar1 = 0;
      puVar5 = puVar3;
      do {
        LS_R(puVar5 + -1, 2)
        LS_R(puVar5, 2)
        LS_R(puVar5 + 1, 1)
        LS_R(puVar5 + 3, 4)
        iVar1 = iVar1 + 1;
        puVar5 = puVar5 + 0xc;
      } while (iVar1 < 8);
      puVar3 = puVar3 + 0x60;
      if (((unsigned)&ord_RatingBytes + 0x1) < (int)puVar3) {
        return 1;
      }
    } while( 1 );
  }
  return 0;
}
