/* 0x51de70, reads the characters (the counterpart of the header writer 0x5194c0 and the character writer 0x51a4b0): ai_PlayerId, ai_CharCount, the two judge pointers ai_JudgeCatholic / ai_JudgeProtestant and the eight slots of ai_KindBChars (from 0x10016), then ai_CharCount records of the 0x21c bytes of ai_Chars (the id word first, it picks the record): each field from its version (the reference fields +0x54 / +0x18c get their base 0x59a / 0x639 added, +0x174 is 2 instead of 4 below 0x10020), some fields with their defaults for older versions (+0x20c = -1 and +0x190 = 4 below 0x1003e, +0x1cc = 0 below 0x10049); the last chunk +0x210 .. +0x218 from 0x10036. Returns 1, 0 when a read fails. @deep Q4: the 8 raw accesses need the AICHAR fields of ai_Chars (+0x54 / +0x18c the reference fields, +0x20c, +0x190, +0x1cc the defaulted ones, +0x16c / +0x170 / +0x17c zeroed then read); ai.h cannot be included in ls.c (gv.h declares ai_Blds/ai_Chars/... with other types, type debt). B: 467 code hunks = the Ghidra conversion's shape, not a meaning fix: read against 0x51de70..0x51e74f - the order, offsets, sizes and version gates match the disassembly (ai_PlayerId 2, count 4, the two judges 4, the 8 slots from 0x10016, then per record id 2 -> ai_Chars[id & 0xffff] and the field list +2 .. +0x218 with its gates 0x1003e / 0x10030 / 0x10023 / 0x10020 / 0x10029 / 0x10035 / 0x10049 and the +0x59a / +0x639 bases), so this is the rewrite that 0x51a4b0 has as @deep, not a base reading fix. Q1/Q2/Q3/Q6 done: ls_ReadCharacters, ai_CharCount / ai_JudgeCatholic / ai_JudgeProtestant / ai_KindBChars named project-wide, the 0x6f1c04 loop by symbol. */


int ls_ReadCharacters(int f)
{
    int *ref;               /* the reference field read (+0x54 / +0x18c): its saved index plus the base */
    int v, ok;
    unsigned idx;
    int *slots;
    int sz, k, cnt;
    unsigned id;            /* the id word read at the start of a record */
    int word;
    char *c;

  LS_R(&ai_PlayerId, 2)
  LS_R(&cnt, 4)
  ai_CharCount = cnt;
  LS_R(&ai_JudgeCatholic, 4)
  LS_R(&ai_JudgeProtestant, 4)
  if (0x10016 < ls_Version) {
    slots = ai_KindBChars;
    do {
      LS_R(slots, 4)
      slots = slots + 1;
    } while (slots < ai_KindBChars + 8);
  }
  k = 0;
  if (0 < cnt) {
    do {
      ok = ls_Read((int)&id, 2, 1, f);
      v = ls_Version;
      if (ok == 0) {
        return 0;
      }
      idx = id & 0xffff;
      c = ai_Chars[idx];
      *(short *)c = (short)id;
      if (v < 0x1003e) {
        *(int *)(c + 0x20c) = 0xffffffff;
      }
      else {
        LS_R((c + 0x20c), 4)
      }
      LS_R((c + 0x2), 1)
      LS_R((c + 0x4), 4)
      LS_R((c + 0x8), 1)
      LS_R((c + 0x9), 1)
      LS_R((c + 0xa), 2)
      LS_R((c + 0xc), 1)
      LS_R((c + 0xd), 1)
      LS_R((c + 0x10), 4)
      LS_R((c + 0x14), 4)
      LS_R((c + 0x18), 4)
      LS_R((c + 0x1c), 4)
      LS_R((c + 0x20), 4)
      LS_R((c + 0x24), 4)
      if (0x1003a < ls_Version) {
        LS_R((c + 0x28), 2)
        LS_R((c + 0x2c), 4)
      }
      LS_R((c + 0x30), 0x10)
      if ((0x10030 < ls_Version) &&
         ls_Read((int)((c + 0x40)), 0x10, 1, f) == 0) {
        return 0;
      }
      LS_R((c + 0x50), 2)
      ref = (int *)(c + 0x54);
      LS_R(ref, 4)
      *ref = *ref + 0x59a;
      LS_R((c + 0x58), 1)
      LS_R((c + 0x5c), 0x20)
      if ((0x10023 < ls_Version) &&
         ls_Read((int)((c + 0x7c)), 4, 1, f) == 0) {
        return 0;
      }
      LS_R((c + 0x80), 5)
      LS_R((c + 0x88), 0xa8)
      LS_R((c + 0x164), 1)
      LS_R((c + 0x165), 1)
      LS_R((c + 0x166), 1)
      LS_R((c + 0x167), 1)
      LS_R((c + 0x168), 1)
      LS_R((c + 0x169), 1)
      *(int *)(c + 0x16c) = 0;
      LS_R((c + 0x16c), 4)
      *(int *)(c + 0x170) = 0;
      LS_R((c + 0x170), 4)
      if (ls_Version < 0x10020) {
        sz = 2;
      }
      else {
        sz = 4;
      }
      LS_R((c + 0x174), sz)
      *(int *)(c + 0x17c) = 0;
      LS_R((c + 0x17c), 4)
      LS_R((c + 0x180), 1)
      ref = (int *)(c + 0x18c);
      LS_R(ref, 4)
      *ref = *ref + 0x639;
      LS_R((c + 0x190), 4)
      if (ls_Version < 0x1003e) {
        *(int *)((c + 0x190)) = 4;
      }
      LS_R((c + 0x194), 4)
      LS_R((c + 0x198), 4)
      LS_R((c + 0x19c), 4)
      LS_R((c + 0x1a0), 4)
      LS_R((c + 0x1a4), 4)
      LS_R((c + 0x1a8), 4)
      LS_R((c + 0x1ac), 4)
      LS_R((c + 0x1b0), 1)
      LS_R((c + 0x1b1), 1)
      LS_R((c + 0x1b4), 0x10)
      LS_R((c + 0x1c8), 4)
      if (ls_Version < 0x10049) {
        *(int *)(c + 0x1cc) = 0;
      }
      else {
        LS_R((c + 0x1cc), 4)
      }
      LS_R((c + 0x1d0), 4)
      LS_R((c + 0x1d4), 0x10)
      LS_R((c + 0x1e4), 4)
      LS_R((c + 0x1e8), 4)
      LS_R((c + 0x1ec), 4)
      if ((0x10029 < ls_Version) &&
         ls_Read((int)((c + 0x1f0)), 4, 1, f) == 0) {
        return 0;
      }
      if ((0x10020 < ls_Version) &&
         ls_Read((int)((c + 0x1c5)), 1, 1, f) == 0) {
        return 0;
      }
      LS_R(&word, 4)
      *(int *)(c + 0x184) = word;
      if ((0x10020 < ls_Version) &&
         ls_Read((int)((c + 0x1f4)), 0x18, 1, f) == 0) {
        return 0;
      }
      if (0x10035 < ls_Version) {
        LS_R((c + 0x210), 4)
        LS_R((c + 0x214), 1)
        LS_R((c + 0x215), 1)
        LS_R((c + 0x216), 1)
        LS_R((c + 0x217), 1)
        LS_R((c + 0x218), 1)
      }
      k = k + 1;
    } while (k < cnt);
  }
  return 1;
}
