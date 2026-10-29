/* 0x51c2f0, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep B3 code hunks 116, both are VC6 register spellings, not meaning: the count loop at 0x51c2f9 keeps the counter in ecx and stores it once at 0x51c31e (push ecx frame), ours keeps it in the stack slot (inc DWORD PTR [esp]); the record loop at 0x51c334 keeps the loop pointer in esi at 0x925534 (record base +0x14, accesses [esi-0x14]/[esi-0xc]/[esi]/[esi+0x12]/[esi+0x1c]/[esi+0x20]/[esi+0x60]/[esi+0x68]/[esi+4]/[esi-8]), ours bases esi at 0x925520 with shifted offsets, same addresses. Read the whole function: stride 0x114 (add eax,0x114), 96 records, field order/sizes/args match the original; tried char* byte offsets, int* element offsets, for and do-while spellings, VC6 canonicalises to the lowest base and spills the counter either way. Readability Q1/Q2/Q3/Q5 open (the 0x925520 table is unnamed) */


int unk_Unk51c2f0(int f)
{
  char *piVar1;
  int iVar2;
  char *puVar3;
  int local_4;
  local_4 = 0;
  piVar1 = ((char *)(unsigned)&ai_Unk925520);
  do {
    if (*(int *)piVar1 != -1) {
      local_4 = local_4 + 1;
    }
    piVar1 = piVar1 + 0x114;
  } while ((int)piVar1 < (unsigned)((unsigned)ai_Unk925520 + 0x6780));
  LS_W(&local_4, 4)
  puVar3 = ((char *)((unsigned)&ai_Unk925520 + 0x14));
  do {
    if (*(int *)(puVar3 - 0x14) != -1) {
      LS_W(puVar3 - 0x14, 4)
      LS_W(puVar3 - 0xc, 1)
      LS_W(puVar3, 4)
      LS_W(puVar3 + 0x12, 1)
      LS_W(puVar3 + 0x1c, 1)
      iVar2 = vfs_fwrite(puVar3 + 0x20,4,0x10,f);
      if (iVar2 == 0) {
        return 0;
      }
      LS_W(puVar3 + 0x60, 1)
      LS_W(puVar3 + 0x68, 0x98)
      LS_W(puVar3 + 4, 0xe)
      LS_W(puVar3 - 8, 4)
    }
    puVar3 = puVar3 + 0x114;
    if (((unsigned)&ai_Unk925520 + 0x6793) < (int)puVar3) {
      return 1;
    }
  } while( 1 );
}
