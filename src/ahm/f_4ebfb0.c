/* 0x4ebfb0, writes the walk state to the save file f (the save game 0x5190b0 in ls.c calls it): the five ints 0x6f0dc8 .. 0x6f0dd8, the seed word 0x6f0ddc
   (2 bytes), then the three steps (flag, x, y, the player-prototype view of the grid entry: five attribute bytes + its id); 1 when all was written, 0 when a
   write failed.
   Matching notes: the unsigned `<= 0` checks (test eax,eax; jbe + an inline `pop edi; pop esi; ret`) are what the original's early-return epilogues need;
   the loop is the direct index `ai_WalkSteps[i]`, which is what puts VC6's induction variable at the step + 0x10 (`mov edi,0x6f0df0`, `cmp edi,0x6f0e38`) -
   every pointer spelling (char* byte offsets, AHMSTEP *p with a pointer or an index loop) re-centred it at +0x14, so the index is the original's source
   shape (notes/vc6-matching.md, the struct-array loop lesson). Q1..Q7 pass. */

int ahm_SaveWalk(void *f)
{
    int i;

    if (vfs_fwrite(&ai_WalkX, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkDX, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkY, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkDY, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkIndex, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_WalkSeed, 2, 1, f) <= 0)
        return 0;
    for (i = 0; i < 3; i++) {
        if (vfs_fwrite(&ai_WalkSteps[i].on, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].x, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].y, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[0], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[1], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[2], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[3], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.attr[4], 1, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&ai_WalkSteps[i].grid.plr.id, 4, 1, f) <= 0)
            return 0;
    }
    return 1;
}
