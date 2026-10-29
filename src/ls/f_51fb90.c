/* 0x51fb90, reads the 0x14c-byte records of 0x6fab40 (the counterpart of 0x51bdc0): the counts, then each record in pieces (the 8 words +0x8c from 0x1002f), the text buffer of +0x7c (size +0x80, "messagebox", 0x400 bytes) and the type specific block of +0xac (the kind 0xc gets a 0x58-byte "f3_alwin_t:d" record at +0xbc, the kind 0xd -1 and, for the type 6, +0xd0 = 0), the records flagged 8 are listed in 0x74db40; then the 0x29-word table 0x74e1e0 (0x2c-byte entries) and the 0x14-word table 0x758ca0. Returns 1. ROUGH @deep diff read against the source (97 lines, 70 code / 6 regs / 4 order): meaning verified - the three 4-byte counts, the record loop (rec = 0x6fab40 + idx*0x14c, the 11 pieces incl. the version-gated 8 words +0x8c, the text buffer +0x7c / +0x80 via mem_Alloc and the direct read, the kind 0xc / 0xd block of +0xac and the +0xbc pointer / -1 / +0xd0 clear), the flagged records into 0x74db40, then the 0x29-int table 0x74e1e0 and the 0x14-int table 0x758ca4. Left is codegen: a store-order pair (the +0x7c store before/after the following load) and the induction-variable base - the original keeps the middle-loop pointer 8 bytes above ours (its edi with 0/+4/+8/... vs our edi-8 with -8/-4/0/...; both reach the same 0x74e1e8 start), the next recorded attempt here ('pointer one induction step ahead', notes/units/ls.md Open work). Readability: Q3 on every table literal (0x6fab40, 0x74db40, 0x74e1e0, 0x758ca4), Q2 on he_Unk53d590 / ai_Unk7585e4 / ai_Unk6f261c / ai_Unk6f2620, Q1/Q5. */

int unk_Unk51fb90(int f)
{
    int n, size2, i, idx;
    char *rec;
    int *pi;
    char *tb;

    he_Unk53d590();
    LS_R(&n, 4)
    LS_R(&size2, 4)
    ai_Unk6f261c = 0;
    ai_Unk6f2620 = 0;
    ai_Unk7585e4 = 0;
    if (n > 0) {
        do {
            idx = ai_Unk7585e4;
            rec = (char *)(unsigned)&he_Events + idx * 0x14c;
            LS_R(rec, 1)
            LS_R(rec + 4, 4)
            LS_R(rec + 8, 2)
            LS_R(rec + 0xc, 4)
            LS_R(rec + 0x10, 4)
            LS_R(rec + 0x14, 0x30)
            LS_R(rec + 0x44, 0xe)
            LS_R(rec + 0x52, 0xe)
            LS_R(rec + 0x60, 0xe)
            LS_R(rec + 0x70, 4)
            LS_R(rec + 0x78, 1)
            if (ls_Version >= 0x1002f) {
                for (i = 0; i < 8; i++)
                    LS_R(rec + 0x8c + i * 4, 4)
            }
            pi = (int *)(rec + 0x80);
            *(int *)(rec + 0x7c) = 0;
            *pi = 0;
            LS_R(pi, 4)
            if (*pi != 0) {
                tb = (char *)mem_Alloc(0x400, "messagebox");
                *(char **)(rec + 0x7c) = tb;
                if (ls_Read((int)tb, *pi, 1, f) == 0)
                    return 0;
            }
            if (*rec == 0xc) {
                LS_R(rec + 0xac, size2)
                *(char **)(rec + 0xbc) = (char *)mem_Alloc(0x58, "f3_alwin_t:d");
            } else if (*rec == 0xd) {
                LS_R(rec + 0xac, size2)
                *(int *)(rec + 0xbc) = -1;
                if (*(int *)(rec + 0x70) == 6)
                    rec[0xd0] = 0;
            } else {
                LS_R(rec + 0xac, size2)
            }
            if ((rec[0x78] & 8) != 0) {
                he_QuickList[ai_Unk6f2620] = rec;
                ai_Unk6f2620++;
            }
            ai_Unk7585e4++;
        } while (ai_Unk7585e4 < n);
    }
    ai_Unk6f261c = ai_Unk7585e4;
    LS_R(&n, 4)
    i = 0;
    if (n > 0) {
        pi = (int *)((unsigned)&he_QuickList + 0x6a0);
        do {
            *pi = 0;
            LS_R(pi, 4)
            LS_R(pi + 1, 1)
            LS_R(pi + 2, 4)
            LS_R(pi + 3, 4)
            LS_R(pi + 4, 4)
            LS_R(pi + 5, 0xe)
            LS_R((int)pi + 0x22, 0x80)
            i++;
            pi += 0x29;
        } while (i < n);
    }
    LS_R(&n, 4)
    if (n > 0) {
        i = 0;
        pi = (int *)((unsigned)&he_RunHandlers + 0x6a4);
        do {
            LS_R(pi - 1, 1)
            LS_R(pi, 4)
            LS_R(pi + 1, 4)
            LS_R(pi + 2, 4)
            LS_R(pi + 3, 0x40)
            i++;
            pi += 0x14;
        } while (i < n);
    }
    return 1;
}
