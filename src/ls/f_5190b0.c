/* 0x5190b0, loads / writes the save game: opens the file (0x430470, mode 0x5f8324), remembers the path in 0x75e640, makes the references plain (0x519a00), the header (0x5194c0) and the fixed globals, then the writers of the subsystems in this order: 0x519bf0 (buildings), 0x519d20, 0x51a1a0, 0x51a4b0, 0x51b340, 0x4ea690, 0x519b50 and, when (flags & 2) == 0, 0x4f28d0, 0x51b890, 0x51b910, 0x4ed1e0, 0x51bdc0, 0x51c2f0, 0x51c450, 0x51c560, 0x51c870, 0x51aff0, 0x4b62e0, 0x589aa0, 0x5a8940, 0x4ebfb0, 0x4adb20, 0x4bff40; the first one that fails closes the file (0x430aa0), restores the references (0x519aa0) and returns 0. Returns 1. ROUGH: the callee of the file functions is 0x430e30(data, size, count, file) @deep B: the 14 code hunks are one scheduling pattern - the original hoists 'push esi' (the file handle argument of the next call) above the preceding jne (disasm 0x5190fd, 0x5191db, 0x51910e); ours pushes at the call site, the calls and arguments themselves match (read all 3 code hunk sites; a rewrite of this shape is match-deep's). The other hunks are fpu. Q2/Q7: 45 callees and globals of other units are unnamed (vfs_Tfopen, vfs_fclose, vfs_fwrite, 0x519bf0 ... 0x51c870); Q5 ROUGH. */


int ls_SaveGame(int name, char *path, unsigned flags)
{
    int f;

    f = vfs_Tfopen(name, "wb", vfs_Root);
    if (f == 0)
        return 0;
    strcpy(ls_SavePath, path);
    unk_Unk519a00();
    if (ls_WriteSaveHeader(f, flags) == 0) {
        ((int (*)())vfs_fclose)();
        unk_Unk519aa0();
        return 0;
    }
    vfs_fwrite(&ai_NextObjectId, 4, 1, f);
    vfs_fwrite(&ai_Unk6f2618, 4, 1, f);
    vfs_fwrite(ai_Time, 0xe, 1, f);
    vfs_fwrite(&ai_OwnPlayer, 1, 1, f);
    vfs_fwrite(ai_PlayerCharC + 4, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1bb8, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1bbc, 4, 1, f);
    vfs_fwrite(&ai_CharCount, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1bcc, 4, 1, f);
    vfs_fwrite(ai_UpdateStart, 0x18, 1, f);
    vfs_fwrite(&ai_Unk61a658, 4, 1, f);
    vfs_fwrite(&ai_Unk6f1b88, 4, 1, f);
#define LS_STEP(fn) if (fn(f) == 0) { ((void (*)())vfs_fclose)(); unk_Unk519aa0(); return 0; }
    LS_STEP(ls_WriteObjects)
    LS_STEP(unk_Unk519d20)
    LS_STEP(unk_Unk51a1a0)
    LS_STEP(unk_Unk51a4b0)
    LS_STEP(unk_Unk51b340)
    LS_STEP(amt_SaveAemter)
    LS_STEP(unk_Unk519b50)
    if ((flags & 2) == 0) {
        LS_STEP(unk_Unk4f28d0)
        LS_STEP(ls_WriteRelations)
        LS_STEP(unk_Unk51b910)
        LS_STEP(gv_WriteAvatars)
        LS_STEP(unk_Unk51bdc0)
        LS_STEP(unk_Unk51c2f0)
        LS_STEP(ls_WriteHistory)
        LS_STEP(unk_Unk51c560)
        LS_STEP(unk_Unk51c870)
        LS_STEP(unk_Unk51aff0)
        LS_STEP(ai_Unk4b62e0)
        LS_STEP(ai_Unk589aa0)
        LS_STEP(ai_Unk5a8940)
        LS_STEP(ahm_SaveWalk)
        LS_STEP(hmt_Save)
        LS_STEP(rt_SaveRoutes)
    }
    vfs_fclose(f);
    unk_Unk519aa0();
    return 1;
}
