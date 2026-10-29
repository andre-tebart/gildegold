/* 0x51c560, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep B3 code hunks 384: the record copy is Ghidra's stack alias, not meaning - the original copies each 0x99-byte pool record to a 0x99-byte buffer at [esp+0x14] (sub esp,0xa0 at 0x51c560; rep movs 0x26 dwords + 1 byte at 0x51c585) and stores the two converted pointers into the buffer's +0x91/+0x95 words (0x51c5a6 [esp+0xa5], 0x51c5d7 [esp+0xb9]); the draft copies into int local_9c[36] (0x90 bytes) and patches separate locals local_b/local_7, which are Ghidra's aliases of those two buffer words, so the written bytes come from the wrong stack slots; the index conversion is the unsigned /0x99 (magic 0xd62b80d7, shr 7) and matches. The fix is the rewrite with a 0x99-byte record type (char pad[0x91]; int target, source), which changes the frame and the register shape of both pool loops. Read both pool loops (0x9367e0 and 0xa69140, 0x2000 / 0x8000 records), the heads 0xa68d24/0xa68c00/0xf81160, the 0xf31160 10-byte table loop and the 0x60bd88..0x60bd90 tail. Q1/Q2/Q3/Q5 open */


int unk_Unk51c560(int f)
{
  int iVar1;
  int *piVar2;
  int *puVar3;
  char *puVar4;
  int *puVar5;
  unsigned local_a0;
  int local_9c [36];
  unsigned local_b;
  unsigned local_7;
  piVar2 = ((char *)((unsigned)&ai_Unk9367e0 + 0x91));
  do {
    puVar3 = (int *)((int)piVar2 + -0x91);
    puVar5 = local_9c;
    for (iVar1 = 0x26; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    iVar1 = *piVar2;
    *(char *)puVar5 = *(char *)puVar3;
    if (iVar1 == 0) {
      local_b = 0xffffffff;
    }
    else {
      local_b = (iVar1 - 0x9367e0U) / 0x99;
    }
    if (piVar2[1] == 0) {
      local_a0 = 0xffffffff;
    }
    else {
      local_a0 = (piVar2[1] - 0x9367e0U) / 0x99;
    }
    local_7 = local_a0;
    LS_W(local_9c, 0x99)
    piVar2 = (int *)((int)piVar2 + 0x99);
  } while ((int)piVar2 < ((unsigned)&ai_Unka68800 + 0x71));
  if ((*(int *)(unsigned)&ai_Unka68d24) == 0) {
    local_a0 = 0xffffffff;
  }
  else {
    local_a0 = ((*(int *)(unsigned)&ai_Unka68d24) - 0x9367e0U) / 0x99;
  }
  iVar1 = vfs_fwrite(&local_a0,4,1,f);
  if (iVar1 != 0) {
    if ((*(int *)(unsigned)&ai_Unka68c00) == 0) {
      local_a0 = 0xffffffff;
    }
    else {
      local_a0 = ((*(int *)(unsigned)&ai_Unka68c00) - 0x9367e0U) / 0x99;
    }
    LS_W(&local_a0, 4)
    piVar2 = ((char *)((unsigned)&ai_Unka69140 + 0x91));
    do {
      puVar3 = (int *)((int)piVar2 + -0x91);
      puVar5 = local_9c;
      for (iVar1 = 0x26; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      iVar1 = *piVar2;
      *(char *)puVar5 = *(char *)puVar3;
      if (iVar1 == 0) {
        local_b = 0xffffffff;
      }
      else {
        local_b = (iVar1 - 0xa69140U) / 0x99;
      }
      if (piVar2[1] == 0) {
        local_a0 = 0xffffffff;
      }
      else {
        local_a0 = (piVar2[1] - 0xa69140U) / 0x99;
      }
      local_7 = local_a0;
      LS_W(local_9c, 0x99)
      piVar2 = (int *)((int)piVar2 + 0x99);
    } while ((int)piVar2 < ((unsigned)&ls_RefTargets + 0x6b));
    if ((*(int *)(unsigned)&cm_SendQueue) == 0) {
      local_a0 = 0xffffffff;
    }
    else {
      local_a0 = ((*(int *)(unsigned)&cm_SendQueue) - 0xa69140U) / 0x99;
    }
    LS_W(&local_a0, 4)
    LS_W(((char *)(unsigned)&ai_Unkf81168), 4)
    puVar4 = ((char *)(unsigned)&ai_Unkf31160);
    while (iVar1 = vfs_fwrite(puVar4,10,1,f), iVar1 != 0) {
      puVar4 = puVar4 + 10;
      if (((unsigned)&ai_Unkf31160 + 0x4ffff) < (int)puVar4) {
        LS_W(((char *)(unsigned)&ai_Unkf8121c), 4)
        LS_W(((char *)(unsigned)&ai_Unka68c04), 4)
        LS_W(((char *)(unsigned)&ai_Unk60bd88), 4)
        iVar1 = vfs_fwrite(((char *)(unsigned)&ai_Unk60bd8c),4,1,f);
        if (iVar1 != 0) {
          iVar1 = vfs_fwrite(((char *)(unsigned)&ai_Unk60bd90),4,1,f);
          return iVar1 != 0;
        }
        return 0;
      }
    }
  }
  return 0;
}
