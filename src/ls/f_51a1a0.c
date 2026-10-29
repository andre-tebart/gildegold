/* 0x51a1a0, the records of 0x7d5f80 .. 0x7d6a42 (0xac bytes each): the id, name, ids, the floats and ints at the offsets below; the fields +0x52 .. +0x6a only when the save game's version 0x623650 is above 0x10013 @deep B: 135 code hunks, read at the start and sampled: the base pointer 0x7d5f82 lands in eax in ours and in esi in the original, so the first block is sub eax,0x2 vs lea eax,[esi-0x2] and every later block's lea/sub and pairing shifts; the offsets (-2, 0, 0x12, 0x26, 0x1a, 0x1e, 0x2a, 0x2e, 0x32, 0x36, 0x3e, 0x42, 0x46, 0x4a, 0x4e and the 0x10013 version tail +0x52 .. +0x6a), the sizes and vfs_fwrite calls match the original at 0x51a1a0 exactly. Shape is match-deep's. Q2/Q7: gm_PlayerRecordBytes and ls_Version unnamed; Q6 described. */


int unk_Unk51a1a0(int f)
{
    char *p = gm_PlayerRecordBytes;

    for (;;) {
        LS_W(p - 2, 2)
        LS_W(p, 0x10)
        LS_W(p + 0x12, 4)
        LS_W(p + 0x26, 4)
        LS_W(p + 0x1a, 4)
        LS_W(p + 0x1e, 4)
        LS_W(p + 0x2a, 4)
        LS_W(p + 0x2e, 4)
        LS_W(p + 0x32, 4)
        LS_W(p + 0x36, 4)
        LS_W(p + 0x3e, 4)
        LS_W(p + 0x42, 4)
        LS_W(p + 0x46, 4)
        LS_W(p + 0x4a, 4)
        LS_W(p + 0x4e, 4)
        if (ls_Version > 0x10013) {
            LS_W(p + 0x52, 4)
            LS_W(p + 0x56, 4)
            LS_W(p + 0x5a, 4)
            LS_W(p + 0x3a, 4)
            LS_W(p + 0x62, 4)
            LS_W(p + 0x66, 4)
            LS_W(p + 0x6a, 4)
        }
        LS_W(p + 0x5e, 4)
        LS_W(p + 0x72, 0x11)
        LS_W(p + 0x86, 4)
        LS_W(p + 0x8a, 0x10)
        LS_W(p + 0x9a, 0xc)
        LS_W(p + 0xa6, 4)
        LS_W(p + 0x6e, 4)
        p += 0xac;
        if ((int)p > ((unsigned)&ai_Unk7d6a40 + 0x1))
            return 1;
    }
}
