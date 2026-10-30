/* 0x51f080, reads the five player blocks of 0x7d6a60 (0x2a90 bytes: four words and n records of 0x80 bytes, n = 0x3e or the count read first from 0x10048; the counterpart of 0x51b340) and the player slots of 0x7e3f40 (0x2f4 bytes: the name, the 8 bytes +0x40, the fields up to +0x2a4 + 0x48 in the order of the writer, from version 0x10037 the 8 bytes +0x2ec); the town name of the first slot is looked up in the text table ("_STADTAUSWAHL_%s_INFO+0", 0x42e770) and copied to +0x20 (else the slot's own name). Returns 1. ROUGH @deep read 0x51f080..0x51f64f against the source, meaning verified: the 4 header words, n = 0x3e or the count read (version >= 0x10048), the 11 reads of the 0x80-byte records, the slot loop with s = 0x7e3f88 and p = s - 0x48 (the original keeps s in the stack slot [esp+0x10] and reloads ebx every iteration, p in edi; all reads up to s+0x19 and the first 10x8 loop use s, everything from p+0x104 on uses p - our draft used s everywhere and the rewrite to p for that half took 180 -> 94 code hunks, 156 -> 74 asm lines), the read order/offsets, the 10x8 / 4x4 / 0xb / 7 / 8x0x12 loops, the 0xd0 tail, the >= 0x10037 8-byte +0x2ec read, the bound s < 0x7e4b58 (4 slots; the p-based bound I first wrote read 5 and was reverted), the town lookup. Version bounds written as the original's >= 0x10048 / >= 0x10037. No meaning error left. Remaining 94 code hunks are shape: s in a stack slot vs our registers, the original's xor eax,eax in the shared return-0 block, the loop rotation at the bottom (ours duplicates the loop head after the jg exit test), and the two strcpy calls inlined with a dst-src bias (sub edx,eax) in the original vs our [eax+0x20] form; a do-while restructure of the tail was tried and made it WORSE (177 asm lines, reverted). Readability left: Q1 name, Q2 ai_PlayerSlots, Q3 image literals 0x7d6a68 / 0x7e3f38 / 0x7e3f88 / 0x7e4b57, Q5 ROUGH, Q4 on the typed records (ai.h blocked in ls.c). */


int unk_Unk51f080(int f)
{
    int n = 0x3e, i, j;
    int *p5, *p7;
    char *p, *s, *q;
    char buf[256];
    int idx;

    if (ls_Version >= 0x10048 && ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    for (p5 = (int *)((unsigned)&ai_Unk7d6a64 + 0x4); (int)p5 < (int)((unsigned)&ai_Unk7d6a64 + 0xd4d4); p5 += 0xaa4) {  /* the bound is the address + 0xd4d4 (the end of the five 0x2a90-byte blocks); ls.c declares ai_Unk7d6a64 as a scalar, so the plain `(unsigned)ai_Unk7d6a64` read the byte value and skipped the loop */
        LS_R(p5 - 2, 4)
        LS_R(p5 - 1, 4)
        LS_R(p5, 4)
        LS_R(p5 + 1, 4)
        i = 0;
        if (n > 0) {
            p7 = p5 + 4;
            do {
                LS_R(p7 - 2, 2)
                LS_R(p7 - 1, 4)
                LS_R(p7, 4)
                LS_R(p7 + 1, 4)
                LS_R(p7 + 6, 4)
                LS_R(p7 + 7, 4)
                LS_R(p7 + 10, 4)
                LS_R(p7 + 11, 4)
                LS_R(p7 + 12, 4)
                LS_R(p7 + 13, 2)
                LS_R(p7 + 9, 4)
                i++;
                p7 += 0x20;
            } while (i < n);
        }
    }
    s = (char *)((unsigned)&ai_PlayerSlots + 0x48);
    for (;;) {
        p = s - 0x48;
        LS_R(p, 0x20)
        LS_R(s - 8, 8)
        LS_R(s, 1)
        LS_R(s + 4, 4)
        LS_R(s + 8, 4)
        LS_R(s + 0xc, 2)
        LS_R(s + 0x10, 8)
        LS_R(s + 0x18, 1)
        LS_R(s + 0x19, 1)
        i = 0;
        q = s + 0x1c;
        do {
            LS_R(q, 8)
            i++;
            q += 8;
        } while (i < 10);
        i = 0;
        q = p + 0xb4;
        do {
            LS_R(q, 8)
            i++;
            q += 8;
        } while (i < 10);
        LS_R(p + 0x104, 1)
        LS_R(p + 0x105, 1)
        i = 0;
        q = p + 0x108;
        do {
            LS_R(q, 4)
            i++;
            q += 4;
        } while (i < 4);
        i = 0;
        q = p + 0x118;
        do {
            LS_R(q, 4)
            i++;
            q += 4;
        } while (i < 4);
        LS_R(p + 0x128, 4)
        i = 0;
        do {
            LS_R(p + i + 0x12c, 1)
            i++;
        } while (i < 0xb);
        LS_R(p + 0x137, 1)
        i = 0;
        do {
            LS_R(p + i + 0x138, 1)
            i++;
        } while (i < 7);
        if (ls_Read((int)(p + 0x13f), 1, 1, f) == 0)
            break;
        LS_R(p + 0x140, 1)
        LS_R(p + 0x141, 1)
        LS_R(p + 0x142, 1)
        i = 0;
        q = p + 0x144;
        do {
            LS_R(q, 0x12)
            i++;
            q += 0x12;
        } while (i < 8);
        LS_R(p + 0x1d4, 4)
        LS_R(p + 400 + 0x48, 4)
        LS_R(p + 0x1dc, 0xd0)
        if (ls_Version >= 0x10037 && ls_Read((int)(p + 0x2ec), 8, 1, f) == 0)
            return 0;
        s += 0x2f4;
        if ((int)s > ((unsigned)((unsigned)ai_PlayerSlots + 0xbe0) + 0x37)) {
            sprintf(buf, "_STADTAUSWAHL_%s_INFO+0", ai_PlayerSlots[0]);
            _strupr(buf);
            idx = txt_FindKey(buf);
            if (idx == -1) {
                strcpy(ai_PlayerSlots[0] + 0x20, ai_PlayerSlots[0]);
                return 1;
            }
            strcpy(ai_PlayerSlots[0] + 0x20, txt_Ptrs[idx]);
            return 1;
        }
    }
    return 0;
}
