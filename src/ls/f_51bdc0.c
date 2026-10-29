/* 0x51bdc0, ROUGH: converted from the Ghidra pseudocode, absolute data addresses; save writer @deep B3 code hunks 762: the draft copies Ghidra element indices as byte offsets - the player table 0x6fab40 has 0x400 records of 0x14c bytes (original add eax,0x14c at 0x51bdde) but the draft writes 0x6fab48 + i*0xa6, 0x6fab50 + i*0x53, 0x6fabb0 + i*0x53, 0x6fabbc[i*0x53], 0x6fabc0 + i*0x53 instead of base+8/0x10/0x70/0x7c/0x80; the 0x100 records at 0x74e1e0 have stride 0xa4 (add eax,0xa4 at 0x51c040; index*4 at 0x51c07c) and the draft steps a char* by 0x29; the 0x100 records at 0x758ca0 have stride 0x50 (add eax,0x50 at 0x51c1bd; fields +8/+0xc/+0x10, 0x40 bytes at +0x10) and the draft steps by 0x14; so the whole function needs the rewrite with corrected bases (the 0x51b340 note: patching addresses alone makes unitdiff worse because every block's register shape changes). Also the prologue: the original runs the first count loop in eax/ecx and pushes ebx/ebp/esi/edi only at 0x51bde8, ours pushes at entry (same shape as 0x51aff0). Q1/Q2/Q3/Q5 open, no evidence for the three record tables' names */

int unk_Unk51bdc0(int f)
{
  int uVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  char *puVar8;
  int *puVar9;
  int uVar10;
  int local_8;
  int local_4;
  uVar1 = f;
  local_4 = 0;
  pcVar2 = ((char *)(unsigned)&he_Events);
  local_8 = 0x400;
  do {
    if (*pcVar2 != '\0') {
      local_4 = local_4 + 1;
    }
    pcVar2 = pcVar2 + 0x14c;
  } while ((int)pcVar2 < (unsigned)((unsigned)he_Events + 0x53000));
  LS_W(&local_4, 4)
  local_8 = 0xa0;
  LS_W(&local_8, 4)
  local_8 = 0;
  do {
    iVar6 = local_8;
    iVar3 = local_8 * 0x14c;
    if ((((char *)(unsigned)&he_Events))[local_8 * 0x14c] != '\0') {
      LS_W(((char *)(unsigned)&he_Events) + iVar3, 1)
      LS_W(((char *)((unsigned)&he_Events + 0x4)) + iVar3, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x8)) + iVar6 * 0xa6, 2)
      LS_W(((char *)((unsigned)&he_Events + 0xc)) + iVar3, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x10)) + iVar6 * 0x53, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x14)) + iVar3, 0x30)
      LS_W(((char *)((unsigned)&he_Events + 0x44)) + iVar3, 0xe)
      LS_W(((char *)((unsigned)&he_Events + 0x52)) + iVar3, 0xe)
      LS_W(((char *)((unsigned)&he_Events + 0x60)) + iVar3, 0xe)
      LS_W(((char *)((unsigned)&he_Events + 0x70)) + iVar6 * 0x53, 4)
      LS_W(((char *)((unsigned)&he_Events + 0x78)) + iVar3, 1)
      iVar4 = 0;
      puVar8 = ((char *)((unsigned)&he_Events + 0x8c)) + iVar3;
      do {
        LS_W(puVar8, 4)
        iVar4 = iVar4 + 1;
        puVar8 = puVar8 + 4;
      } while (iVar4 < 8);
      if (((((char *)((unsigned)&he_Events + 0x7c)))[iVar6 * 0x53] == 0) || ((((char *)((unsigned)&he_Events + 0x80)))[iVar6 * 0x53] == 0)) {
        puVar9 = &f;
        uVar10 = 4;
        f = 0;
      }
      else {
        LS_W(((char *)((unsigned)&he_Events + 0x80)) + iVar6 * 0x53, 4)
        uVar10 = (((char *)((unsigned)&he_Events + 0x80)))[iVar6 * 0x53];
        puVar9 = (int *)(((char *)((unsigned)&he_Events + 0x7c)))[iVar6 * 0x53];
      }
      iVar6 = vfs_fwrite(puVar9,uVar10,1,uVar1);
      if (iVar6 == 0) {
        return 0;
      }
      LS_W(((char *)((unsigned)&he_Events + 0xac)) + iVar3, 0xa0)
    }
    local_8 = local_8 + 1;
    if (0x3ff < local_8) {
      local_4 = 0;
      piVar7 = ((char *)((unsigned)&he_QuickList + 0x6a0));
      local_8 = 0x100;
      do {
        if (*piVar7 != 0) {
          local_4 = local_4 + 1;
        }
        piVar7 = piVar7 + 0x29;
      } while ((int)piVar7 < ((unsigned)&he_QuickList + 0xaaa0));
      LS_W(&local_4, 4)
      local_8 = 0;
      do {
        if ((((char *)((unsigned)&he_QuickList + 0x6a0)))[local_8 * 0x29] != 0) {
          LS_W((((char *)((unsigned)&he_QuickList + 0x6a0)))[local_8 * 0x29] + 4, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6a4)) + local_8 * 0x29, 1)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6a8)) + local_8 * 0x29, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6ac)) + local_8 * 0x29, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6b0)) + local_8 * 0x29, 4)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6b4)) + local_8 * 0x29, 0xe)
          LS_W(((char *)((unsigned)&he_QuickList + 0x6c2)) + local_8 * 0xa4, 0x80)
        }
        local_8 = local_8 + 1;
        if (0xff < local_8) {
          local_4 = 0;
          piVar7 = ((char *)((unsigned)&he_RunHandlers + 0x6ac));
          local_8 = 0x100;
          do {
            if (((piVar7[-1] != 0) || (*piVar7 != 0)) || ((char)piVar7[1] != '\0')) {
              local_4 = local_4 + 1;
            }
            piVar7 = piVar7 + 0x14;
          } while ((int)piVar7 < ((unsigned)((unsigned)he_RunHandlers + 0x56a0) + 0xc));
          iVar3 = vfs_fwrite(&local_4,4,1,uVar1);
          if (iVar3 != 0) {
            local_8 = 0;
            while (((((((char *)((unsigned)&he_RunHandlers + 0x6a8)))[local_8 * 0x14] == 0 && ((((char *)((unsigned)&he_RunHandlers + 0x6ac)))[local_8 * 0x14] == 0)
                     ) && ((((char *)((unsigned)&he_RunHandlers + 0x6b0)))[local_8 * 0x50] == '\0')) ||
                   (((iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6a0)) + local_8 * 0x14,1,1,uVar1), iVar3 != 0 &&
                     (iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6a4)) + local_8 * 0x14,4,1,uVar1), iVar3 != 0))
                    && ((iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6a8)) + local_8 * 0x14,4,1,uVar1), iVar3 != 0
                        && ((iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6ac)) + local_8 * 0x14,4,1,uVar1),
                            iVar3 != 0 &&
                            (iVar3 = vfs_fwrite(((char *)((unsigned)&he_RunHandlers + 0x6b0)) + local_8 * 0x50,0x40,1,uVar1),
                            iVar3 != 0))))))))) {
              local_8 = local_8 + 1;
              if (0xff < local_8) {
                return 1;
              }
            }
          }
          return 0;
        }
      } while( 1 );
    }
  } while( 1 );
}
