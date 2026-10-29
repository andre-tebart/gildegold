/* 0x51b340, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep draft: the draft's addressing is wrong in four places, read against the original at 0x51b375/0x51b3de and 0x51b543/0x51b55c: the four block header writes use 0x7d6a60 + k*0xaa4 but the original computes edi = 0x7d6a60 + k*0x2a90 (eax=k; shl 4; add k; lea *5; *8; shl 4 = 0x2a90) and writes edi+0/+4/+8/+0xc; the player name +0x40 uses 0x7e3f80 + i*0xbd and the word +0x54 uses 0x7e3f94 + i*0x17a, the original uses the slot base + 0x40 / +0x54 (0x7e3f40 + i*0x2f4, lea [edi+0x40] / lea [edi+0x54] at 0x51b543 / 0x51b55c); the four multipliers are Ghidra's lost pointer scale (0xaa4 = 0x2a90/4, 0xbd = 0x2f4/4, 0x17a = 0x2f4/2). Fixing only these made unitdiff WORSE (261 -> 318 differing instructions, the corrected addressing changes the register/scheduling shape of the whole block), so the function needs the rewrite with the fix, not a base patch. The first write check is signed in the draft (if (iVar1 != 0)) where the original uses unsigned ja; the ROUGH comment stays. */


int unk_Unk51b340(int f)
{
  int iVar1;
  int iVar2;
  int iVar3;
  char *puVar4;
  int local_4;
  local_4 = 0x55;
  iVar1 = vfs_fwrite(&local_4,4,1,f);
  if (iVar1 != 0) {
    local_4 = 0;
    do {
      iVar1 = local_4;
      iVar3 = local_4 * 0x2a90;
      LS_W(((char *)(unsigned)&ai_Unk7d6a60) + local_4 * 0xaa4, 4)
      LS_W(((char *)(unsigned)&ai_Unk7d6a64) + iVar1 * 0xaa4, 4)
      LS_W(((char *)((unsigned)&ai_Unk7d6a64 + 0x4)) + iVar1 * 0xaa4, 4)
      LS_W(((char *)((unsigned)&ai_Unk7d6a64 + 0x8)) + iVar1 * 0xaa4, 4)
      iVar1 = 0;
      puVar4 = ((char *)((unsigned)ai_Unk7d6a60 + 0x18)) + iVar3;
      do {
        LS_W(puVar4 + -8, 2)
        LS_W(puVar4 + -4, 4)
        LS_W(puVar4, 4)
        LS_W(puVar4 + 4, 4)
        LS_W(puVar4 + 0x18, 4)
        LS_W(puVar4 + 0x1c, 4)
        LS_W(puVar4 + 0x28, 4)
        LS_W(puVar4 + 0x2c, 4)
        LS_W(puVar4 + 0x30, 4)
        LS_W(puVar4 + 0x34, 2)
        LS_W(puVar4 + 0x24, 4)
        iVar1 = iVar1 + 1;
        puVar4 = puVar4 + 0x80;
      } while (iVar1 < 0x55);
      local_4 = local_4 + 1;
    } while (local_4 < 5);
    local_4 = 0;
    while( 1 ) {
      iVar2 = local_4;
      iVar1 = local_4 * 0x2f4;
      iVar3 = vfs_fwrite(((char *)(unsigned)&ai_PlayerSlots) + iVar1,0x20,1,f);
      if (iVar3 == 0) break;
      LS_W(((char *)(unsigned)&ai_Unk7e3f80) + iVar2 * 0xbd, 8)
      LS_W(((char *)((unsigned)&ai_PlayerSlots + 0x48)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_PlayerSlots + 0x4c)) + iVar1, 4)
      LS_W(((char *)((unsigned)&ai_PlayerSlots + 0x50)) + iVar1, 4)
      LS_W(((char *)(unsigned)&ai_Unk7e3f94) + iVar2 * 0x17a, 2)
      LS_W(((char *)((unsigned)&ai_Unk7e3f94 + 0x4)) + iVar1, 8)
      LS_W(((char *)((unsigned)&ai_Unk7e3f94 + 0xc)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e3f94 + 0xd)) + iVar1, 1)
      iVar2 = 0;
      puVar4 = ((char *)((unsigned)&ai_Unk7e3f94 + 0x10)) + iVar1;
      do {
        LS_W(puVar4, 8)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 8;
      } while (iVar2 < 10);
      iVar2 = 0;
      puVar4 = ((char *)((unsigned)&ai_Unk7e3f94 + 0x60)) + iVar1;
      do {
        LS_W(puVar4, 8)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 8;
      } while (iVar2 < 10);
      LS_W(((char *)((unsigned)&ai_Unk7e3ff8 + 0x4c)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e3ff8 + 0x4d)) + iVar1, 1)
      iVar2 = 0;
      puVar4 = ((char *)(unsigned)&ai_Unk7e4048) + iVar1;
      do {
        LS_W(puVar4, 4)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 4;
      } while (iVar2 < 4);
      iVar2 = 0;
      puVar4 = ((char *)(unsigned)&ai_Unk7e4058) + iVar1;
      do {
        LS_W(puVar4, 4)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 4;
      } while (iVar2 < 4);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x10)) + iVar1, 4)
      iVar2 = 0;
      do {
        LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x14)) + iVar2 + iVar1, 1)
        iVar2 = iVar2 + 1;
      } while (iVar2 < 0xb);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x1f)) + iVar1, 1)
      iVar2 = 0;
      do {
        LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x20)) + iVar2 + iVar1, 1)
        iVar2 = iVar2 + 1;
      } while (iVar2 < 7);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x27)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x28)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x29)) + iVar1, 1)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0x2a)) + iVar1, 1)
      iVar2 = 0;
      puVar4 = ((char *)((unsigned)&ai_Unk7e4058 + 0x2c)) + iVar1;
      do {
        LS_W(puVar4, 0x12)
        iVar2 = iVar2 + 1;
        puVar4 = puVar4 + 0x12;
      } while (iVar2 < 8);
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0xbc)) + iVar1, 4)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0xc0)) + iVar1, 4)
      LS_W(((char *)((unsigned)&ai_Unk7e4058 + 0xc4)) + iVar1, 0xd0)
      LS_W(((char *)(unsigned)&ai_Unk7e422c) + iVar1, 8)
      local_4 = local_4 + 1;
      if (3 < local_4) {
        return 1;
      }
    }
  }
  return 0;
}
