/* 0x520000, reads the party records (0x925520, 0x114 bytes; the counterpart of 0x51c2f0): the count, then per record the words and bytes (+0x20 is 16 bytes of 0x40-byte pieces below 0x10059, else 16 words; the clock at +4 is set from 0x770ae0 and 0x4fe140 below 0x1003f; +0x14 = -1 below 0x10040). Returns 1 @deep read 0x520000..0x5201f1 against the source; two real fixes: the temp buffer is 0x1000 bytes (the original's frame is 0x1008 with __chkstk, ours was 0x408 for tmp[0x400]) and the three version branches are written from the >= side so the non-zero arm is inline (the original jl's to the <0x10059 / <0x1003f / <0x10040 arm); 131 -> 95 code hunks, 100 -> 70 asm lines. Meaning verified: the count, the per-record reads p-0x14 / p-0xc / p / p+0x12 / p+0x1c, the tmp[0x40] + memcpy to p+0x20 vs the 4-byte p+0x20 from 0x10059, the +0x60 / +0x68 (0x98) reads, the 0xe memcpy from 0x770ae0 + 0x4fe140(p+4, i+1, 0, 0) vs the 0xe read from 0x1003f, the -1 to p-8 below 0x10040, the 0x114 stride and count loop. Left is codegen: the IV base - the original's ebx = 0x925534, ours = 0x925540 (0xc higher, every displacement differs by 0xc), and i in esi (the original) vs edi (ours) which is entangled with the memcpy's esi restore. Readability: Q1, Q2 (gm_AddTime, ai_Time), Q3 (0x925534), Q4, Q7 (gm_AddTime has no prototype). */


int unk_Unk520000(int f)
{
    char tmp[0x1000];
    int n, i;
    char *p;

    LS_R(&n, 4)
    i = 0;
    p = (char *)((unsigned)&ai_Unk925520 + 0x14);
    for (; i < n; i++) {
        LS_R(p - 0x14, 4)
        LS_R(p - 0xc, 1)
        LS_R(p, 4)
        LS_R(p + 0x12, 1)
        LS_R(p + 0x1c, 1)
        if (ls_Version >= 0x10059) {
            LS_R(p + 0x20, 4)
        } else {
            LS_R(tmp, 0x40)
            memcpy(p + 0x20, tmp, 0x40);
        }
        LS_R(p + 0x60, 1)
        LS_R(p + 0x68, 0x98)
        if (ls_Version >= 0x1003f) {
            LS_R(p + 4, 0xe)
        } else {
            memcpy(p + 4, ai_Time, 0xe);
            gm_AddTime(p + 4, i + 1, 0, 0);
        }
        if (ls_Version >= 0x10040)
            LS_R(p - 8, 4)
        else
            *(int *)(p - 8) = -1;
        p += 0x114;
    }
    return 1;
}
