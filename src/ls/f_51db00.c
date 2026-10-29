/* 0x51db00, reads the 16 player records gm_PlayerRecords (0xac bytes each; the counterpart of the writer 0x51a1a0): the fields, each version gated (+0x26 from 0x1002c, +0x52 .. +0x6a from 0x10014, +0x5e = 1.0 below 0x10056, the 0x11 (0x0e below 0x10046) name bytes at +0x72 from 0x10015, +0x6e from 0x10018). Returns 1, 0 when a read fails. @deep byte + readable blocked on two Q rules. Q3: the loop bound 0x7d6a42 (the end of gm_PlayerRecords in the +2 byte view, = 0x7d5f80 + 16 * 0xac + 2) is an image literal; spelling it as an extern at that address (gm_PlayerRecordsEnd, tried) changes the codegen to 84 differing asm lines instead of 0, and (int)gm_PlayerRecordBytes + 0xac0 doubles the frame - the literal form is byte-identical (the do-while over the pointer is what the original does: add esi,0xac; cmp esi,0x7d6a42; jl). Q4: the one raw access left is *(int *)(p + 0x5e) = 0x3f800000 (the 1.0 float default for the versions below 0x10056) - it needs the 0xac-byte player-record fields, ai.h blocked in ls.c (gv.h type debt). Everything else fixed: v removed so every version check reads ls_Version directly (287 instructions became equal), the '> 0x1002b/0x10013/0x10014/0x10017/0x10045' bounds written as the original's '>= 0x1002c/0x10014/0x10015/0x10018/0x10046', the 0x5e branch laid out with the read inline. */


int ls_ReadPlayerRecords(int f)
{
    char *p = gm_PlayerRecordBytes;
    int i, n;

    do {
        LS_R(p - 2, 2)
        LS_R(p, 0x10)
        LS_R(p + 0x12, 4)
        if (ls_Version >= 0x1002c)
            LS_R(p + 0x26, 4)
        LS_R(p + 0x1a, 4)
        LS_R(p + 0x1e, 4)
        LS_R(p + 0x2a, 4)
        LS_R(p + 0x2e, 4)
        LS_R(p + 0x32, 4)
        LS_R(p + 0x36, 4)
        LS_R(p + 0x3e, 4)
        LS_R(p + 0x42, 4)
        LS_R(p + 0x46, 4)
        LS_R(p + 0x4a, 4)
        LS_R(p + 0x4e, 4)
        if (ls_Version >= 0x10014) {
            LS_R(p + 0x52, 4)
            LS_R(p + 0x56, 4)
            LS_R(p + 0x5a, 4)
            LS_R(p + 0x3a, 4)
            LS_R(p + 0x62, 4)
            LS_R(p + 0x66, 4)
            LS_R(p + 0x6a, 4)
        }
        if (ls_Version >= 0x10056) {
            LS_R(p + 0x5e, 4)
        } else {
            *(int *)(p + 0x5e) = 0x3f800000;
        }
        if (ls_Version >= 0x10015) {
            n = (((ls_Version >= 0x10046) - 1) & 0xfffffffd) + 0x11;
            for (i = 0; i < n; i++)
                LS_R(p + i + 0x72, 1)
        }
        LS_R(p + 0x86, 4)
        LS_R(p + 0x8a, 0x10)
        LS_R(p + 0x9a, 0xc)
        LS_R(p + 0xa6, 4)
        if (ls_Version >= 0x10018)
            LS_R(p + 0x6e, 4)
        p += 0xac;
    } while ((int)p < (int)((char *)gm_PlayerRecords + 0xac2));  /* one past the 16th record (gm_PlayerRecordBytes + 0xac0; a byte offset, the array is int[]); the original's literal 0x7d6a42 is an absolute address that does not hold in the linked image, where it made the loop read ~70 records and the .cty load fail */
    return 1;
}
