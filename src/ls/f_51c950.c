/* 0x51c950 @deep B3 code hunks 282 (+ order 24, regs 20): the draft is the Ghidra && chain re-spelled - the version bounds are written with the neighbouring constant (fixed 0x1005b/0x10025 to the original's 0x1005a/0x10026; 0x10021 must become 0x10022 with the original's jl, 0x1002f -> 0x10030, ...) and the original compares the version memory operand directly (cmp DWORD PTR ds:0x623650,0x10022) where ours loads it into edi first, and keeps the file in esi with edi=0 (xor edi,edi at 0x51c969) while ours allocates ebp/ebx; the check chain is one && in the original with every failure jumping to the shared 0x51cecf/0x51ced8 blocks, the draft splits it into if-blocks with close/return. Read the header check, the fixed globals and version gates (0x10021/0x1002f/0x1003d), the subsystem reader chain (0x51d3d0 .. 0x4c0040) and the tail (0x528770, 0x511790, 0x513740, 0x513590, 0x76f9bc -> 0x75e640). Needs the rewrite in the original's shape. Q1/Q2/Q3/Q5 open */


short ai_Unk75e13c = 0;  /* 0x75e13c, its 2-byte last field ( */
char ls_SavePath[640] = {0};  /* 0x75e640 */
unsigned short ai_ScreenImage[19200] = {0};  /* 0x766320, the */
int ai_Unk76f920[1] = {0};  /* 0x76f920, the header of the loaded save game (id, flag byte, name, ..) */
/* 0x51c950, loads a save game: opens the file (0x430470, mode 0x5f1084), resets the game (0x503e80, 0x40b240, 0x50d490), reads the header (0x51cee0, versions 0x10026 .. 0x1005a only) and the fixed globals (each from its version: 0x6f1bcc from 0x10022, 0x61a658 from 0x10030, 0x6f1b88 from 0x1003d), then the readers of the subsystems in the order 0x51d3d0, 0x51d550, 0x51db00, 0x51de70, 0x51f080, 0x4ea7a0 (from 0x10045), 0x51d2d0, [unless the header flag +4 has bit 2 (a map only save): 0x4ea7a0, 0x4f2b30, 0x51f650 (from 0x10016), 0x51f720, 0x4ed270 (from 0x1002a), 0x51fb90, 0x520000, 0x520200, 0x520310, 0x5205f0 (from 0x1002e), 0x5206b0, then the references (0x511790 / 0x511840 / 0x51e900), the character pointers 0x6f1bfc / 0x6f1c00 (0x505df0), 0x4b63b0, 0x589b30 (from 0x10029), 0x5a8a20 (from 0x10047), 0x4ec140 (from 0x10049), 0x4ada80 (from 0x10052), 0x4c0040 (from 0x10053)]. The first one that fails closes the file, frees the buffer 0xf31140 and returns 0. At the end 0x528770, 0x511790, 0x513740, 0x513590(0x633738, 1) and the path 0x76f9bc is kept in 0x75e640. Returns 1. ROUGH */
int ai_Unk76f924 = 0;  /* 0x76f924 (the flag byte of the header) */
int ai_Unk76f9b8 = 0;  /* 0x76f9b8 */
char ai_Unk76f9bc[100] = {0};  /* 0x76f9bc */
extern char * ai_Unkf31140;                /* 0xf31140 */
extern int ai_Unk75e134;  /* 0x75e134, the clock copy */
extern char ai_Unk75e130[4];  /* 0x75e130 */
extern int ai_Unk75e138;  /* 0x75e138 */
#define LS_RD(p, sz) ls_Read((int)(p), (sz), 1, h)
int unk_Unk51c950(int name)
{
    int h, v, i;

    h = vfs_Tfopen(name, "rb", vfs_Root);
    if (h == 0)
        return 0;
    gm_ResetCharacters();
    ch_DeleteAll();
    ai_PlayerCharC = 0;
    ai_JudgeCatholic = 0;
    ai_JudgeProtestant = 0;
    init_FreeScenes();
    if (ls_ReadSaveHeader(h, ai_Unk76f920) == 0 || ai_Unk76f920[0] > 0x1005a || ai_Unk76f920[0] < 0x10026
        || !LS_RD(&ai_NextObjectId, 4) || !LS_RD(&ai_Unk6f2618, 4) || !LS_RD(ai_Time, 0xe)
        || !LS_RD(&ai_OwnPlayer, 1) || !LS_RD(&ai_PlayerCharC, 4) || !LS_RD(&ai_Unk6f1bb8, 4)
        || !LS_RD(&ai_Unk6f1bbc, 4) || !LS_RD(&ai_CharCount, 4)) {
        vfs_fclose(h);
        return 0;
    }
    v = ls_Version;
    if (v > 0x10021)
        LS_RD(&ai_Unk6f1bcc, 4);
    LS_RD(ai_UpdateStart, 0x18);
    ai_Unk75e134 = *(int *)(ai_Time + 4);
    ai_Unk75e130I = *(int *)ai_Time;
    ai_Unk75e138 = *(int *)(ai_Time + 8);
    ai_Unk75e13c = *(short *)(ai_Time + 0xc);
    if ((ai_Unk76f924 & 2) == 0)
        ai_Difficulty = ai_Unk76f9b8;
    if (v > 0x1002f)
        LS_RD(&ai_Unk61a658, 4);
    if (v < 0x1003d)
        ai_Unk6f1b88 = 1000000;
    else
        LS_RD(&ai_Unk6f1b88, 4);
    if (ls_ReadObjects(h) == 0 || ls_ReadBuildings(h) == 0 || ls_ReadPlayerRecords(h) == 0 || ls_ReadCharacters(h) == 0
        || unk_Unk51f080(h) == 0 || (v >= 0x10045 && amt_LoadAemter(h, v) == 0) || unk_Unk51d2d0(h) == 0) {
        vfs_fclose(h);
        return 0;
    }
    if ((ai_Unk76f924 & 2) != 0) {
        vfs_fclose(h);
        ai_JudgeCatholic = 0;
        ai_JudgeProtestant = 0;
        return ls_FixupAfterLoad() != 0;
    }
    if (!((v > 0x10044 || amt_LoadAemter(h, v) != 0) && unk_Unk4f2b30(h) != 0 && (v < 0x10016 || ls_ReadRelations(h) != 0)
          && unk_Unk51f720(h) != 0 && (v < 0x1002a || gv_ReadAvatars((void *)h) != 0) && unk_Unk51fb90(h) != 0
          && unk_Unk520000(h) != 0 && unk_Unk520200(h) != 0)) {
        vfs_fclose(h);
        return 0;
    }
    if (unk_Unk520310(h) == 0) {
        vfs_fclose(h);
        ls_FixupAfterLoad();
        return 0;
    }
    if (!((v < 0x1002e || unk_Unk5205f0(h) != 0) && ls_FixupAfterLoad() != 0)) {
        vfs_fclose(h);
        return 0;
    }
    d3if_SelectScene(0, 1);
    unk_Unk511790();
    unk_Unk511840();
    if (unk_Unk51e900(h) == 0)
        goto fail;
    ai_JudgeCatholic = gm_FindChar(ai_JudgeCatholic);
    ai_JudgeProtestant = gm_FindChar(ai_JudgeProtestant);
    if (ai_Unk4b63b0(h) == 0)
        goto fail;
    if (!(v < 0x10029 || ai_Unk589b30(h) != 0))
        goto fail;
    if (v > 0x10046 && ai_Unk5a8a20(h) == 0)
        goto fail;
    if (v > 0x10048 && ahm_LoadWalk(h) == 0)
        goto fail;
    if (!(v < 0x10052 || hmt_Load(h, v) != 0))
        goto fail;
    if (v > 0x10052 && rt_LoadRoutes(h) == 0)
        goto fail;
    vfs_fclose(h);
    sv_Unk528770();
    unk_Unk511790();
    unk_Unk513740();
    init_SetDetailLevel((int)ai_Unk633738, 1);
    i = 0;
    do {
        ls_SavePath[i] = ai_Unk76f9bc[i];
    } while (ai_Unk76f9bc[i++] != 0);
    return 1;
fail:
    vfs_fclose(h);
    mem_Free(ai_Unkf31140);
    ai_Unkf31140 = 0;
    return 0;
}
