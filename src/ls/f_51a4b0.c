/* 0x51a4b0, writes the characters (the counterpart of 0x51de70): the player character, the count of the used
   records of 0x770b80 (0x21c bytes each, a record is used when its id is not -1), the two judges 0x6f1bfc /
   0x6f1c00 and the eight slots of 0x6f1c04 as ids (+4 of the record, -1 for none), then each used record in
   pieces (the pointers at +0x16c / +0x170 / +0x17c / +0x184 as ids, the words at +0x54 / +0x18c stored with an
   offset of -0x59a / -0x639). The record pointer runs at +2 (the original's bias). Returns 1.
   @deep draft: 836 differing instructions, every one read in the asmdiff/diffclass: the record field list and its order match the original at 0x51a5b4..0x51ad15 exactly (offsets c-2, c+0, c+2, c+4, c+6, c+7, c+8, c+0xa, c+0xb, c+0xe, c+0x12 .. c+0x20a, sizes 1/2/4/5/0x10/0x18/0x20/0xa8, the four reference fields with +1 / +4 and -1, the two -0x59a / -0x639 offsets), so no meaning error is left; what differs is stack slot choice (the original keeps local_c, cnt and local_4 in the top local slot [esp+0x1c], ours puts local_c in [esp+0x20] and cnt in [esp+0xc] and uses the dead argument slot differently) and push scheduling (the original hoists push esi / push 0x1 / push 0x4 above the je of the table loop and of the three pointer-field blocks, ours merges the branches and schedules the pushes after); needs vargen slot / order rounds. The two manual vfs_fwrite checks are fixed to the original's unsigned jbe (LS_W); the record loop now uses the natural unbiased pointer and VC6 biases the induction variable to +2 as the original does. Readability Q2/Q4 need AICHAR field names for the record (the save writes raw slices) + the judge/slot globals named. */


int unk_Unk51a4b0(int f)
{
  int cnt;
  char *c;
  short *psVar3;
  int *puVar4;
  int *piVar5;
  int local_c;
  int local_8;
  int local_4;

  local_c = 0xffffffff;
  LS_W(&ai_PlayerId, 2)
  cnt = 0;
  psVar3 = (short *)ai_Chars;
  do {
    if (*psVar3 != -1) {
      cnt = cnt + 1;
    }
    psVar3 = psVar3 + 0x10e;
  } while ((int)psVar3 < (int)&ai_Chars[768]);
  LS_W(&cnt, 4)
  if (ai_JudgeCatholic != 0) {
    LS_W(&ai_JudgeCatholic->netId, 4)
  }
  else {
    LS_W(&local_c, 4)
  }
  if (ai_JudgeProtestant != 0) {
    LS_W(&ai_JudgeProtestant->netId, 4)
  }
  else {
    LS_W(&local_c, 4)
  }
  piVar5 = (int *)ai_KindBChars;
  do {
    local_4 = 0xffffffff;
    if (*piVar5 != 0) {
      puVar4 = (int *)(*piVar5 + 4);
    }
    else {
      puVar4 = &local_4;
    }
    LS_W(puVar4, 4)
    piVar5 = piVar5 + 1;
  } while ((int)piVar5 < (int)&ai_KindBChars[8]);
  c = (char *)ai_Chars;
  do {
    if (*(short *)(c) != -1) {
      LS_W(c, 2)
      LS_W((c + 0x20c), 4)
      LS_W((c + 2), 1)
      LS_W((c + 4), 4)
      LS_W((c + 8), 1)
      LS_W((c + 9), 1)
      LS_W((c + 0xa), 2)
      LS_W((c + 0xc), 1)
      LS_W((c + 0xd), 1)
      LS_W((c + 0x10), 4)
      LS_W((c + 0x14), 4)
      LS_W((c + 0x18), 4)
      LS_W((c + 0x1c), 4)
      LS_W((c + 0x20), 4)
      LS_W((c + 0x24), 4)
      LS_W((c + 0x28), 2)
      LS_W((c + 0x2c), 4)
      LS_W((c + 0x30), 0x10)
      LS_W((c + 0x40), 0x10)
      LS_W((c + 0x50), 2)
      cnt = *(int *)((c + 0x54)) + -0x59a;
      LS_W(&cnt, 4)
      LS_W((c + 0x58), 1)
      LS_W((c + 0x5c), 0x20)
      LS_W((c + 0x7c), 4)
      LS_W((c + 0x80), 5)
      LS_W((c + 0x88), 0xa8)
      LS_W((c + 0x164), 1)
      LS_W((c + 0x165), 1)
      LS_W((c + 0x166), 1)
      LS_W((c + 0x167), 1)
      LS_W((c + 0x168), 1)
      LS_W((c + 0x169), 1)
      if (*(int *)((c + 0x16c)) != 0) {
        puVar4 = (int *)(*(int *)((c + 0x16c)) + 1);
      }
      else {
        puVar4 = &local_4;
        local_4 = 0xffffffff;
      }
      LS_W(puVar4, 4)
      if (*(int *)((c + 0x170)) != 0) {
        puVar4 = (int *)(*(int *)((c + 0x170)) + 1);
      }
      else {
        local_4 = 0xffffffff;
        puVar4 = &local_4;
      }
      LS_W(puVar4, 4)
      LS_W((c + 0x174), 4)
      if (*(int *)((c + 0x17c)) != 0) {
        puVar4 = (int *)(*(int *)((c + 0x17c)) + 4);
      }
      else {
        puVar4 = &local_4;
        local_4 = 0xffffffff;
      }
      LS_W(puVar4, 4)
      LS_W((c + 0x180), 1)
      cnt = *(int *)((c + 0x18c)) + -0x639;
      LS_W(&cnt, 4)
      LS_W((c + 0x190), 4)
      LS_W((c + 0x194), 4)
      LS_W((c + 0x198), 4)
      LS_W((c + 0x19c), 4)
      LS_W((c + 0x1a0), 4)
      LS_W((c + 0x1a4), 4)
      LS_W((c + 0x1a8), 4)
      LS_W((c + 0x1ac), 4)
      LS_W((c + 0x1b0), 1)
      LS_W((c + 0x1b1), 1)
      LS_W((c + 0x1b4), 0x10)
      LS_W((c + 0x1c8), 4)
      LS_W((c + 0x1cc), 4)
      LS_W((c + 0x1d0), 4)
      LS_W((c + 0x1d4), 0x10)
      LS_W((c + 0x1e4), 4)
      LS_W((c + 0x1e8), 4)
      LS_W((c + 0x1ec), 4)
      LS_W((c + 0x1f0), 4)
      LS_W((c + 0x1c5), 1)
      if (*(int **)((c + 0x184)) != 0) {
        local_8 = **(int **)((c + 0x184));
      }
      else {
        local_8 = 0xffffffff;
      }
      LS_W(&local_8, 4)
      LS_W((c + 0x1f4), 0x18)
      LS_W((c + 0x210), 4)
      LS_W((c + 0x214), 1)
      LS_W((c + 0x215), 1)
      LS_W((c + 0x216), 1)
      LS_W((c + 0x217), 1)
      LS_W((c + 0x218), 1)
    }
    c = c + 0x21c;
  } while ((int)c < (int)&ai_Chars[768]);
  return 1;
}
