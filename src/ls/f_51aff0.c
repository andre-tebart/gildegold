/* 0x51aff0 @deep draft: the loop structure is now read against the original at 0x51aff0..0x51b340 and the record pieces fixed - the draft wrote rec+0x70 / rec+0x2c / rec+0x38, the original at 0x51b262 / 0x51b29f / 0x51b2b9 writes rec+0xf0 (0xa0 bytes) / rec+0x24 / rec+0x28, exactly what the reader in unk_Unk51e900 reads back (t+0x3c, t+9, t+10), so three wrong offsets were a meaning error and are corrected; the flag/rec reset moved before the fourth loop (the original zeroes them once at 0x51b165..0x51b177), the tab local removed so both table loops re-read ls_ActionQueue per iteration as the original does. Remaining 209 code hunks: register allocation and scheduling - the original's first count loop runs in eax/ecx with no callee-saved register and the pushes ebx/ebp/esi/edi come only after it at 0x51b013, ours pushes them at entry (so cnt is memory-incremented in [esp+0x14] and the zero sits in edi), then the whole function is shifted (ebp/esi/ebx/edi roles and the esp-relative offsets of the rec pieces). Readability Q2/Q4 unchanged: the CHACTION typed view (ch.h) is blocked in ls.c by gv.h's K&R d3sm_GetVertex; the record is handled as int*. */


/* 0x51aff0, the characters' scene records and the table 0x631050 (0x194-byte records, 0x800 of them with the used flag at +9): the count and the records of the characters (0x770d04, 0x21c each) through 0x51ad80, the same for the objects of the kind 0x1d in the list 0x7e3f34, then the table's pointers are turned into indices (+0x14 the first word of the pointed record, +0x24 / +0x28 through 0x51ad30), each used record whose scene object (the list 0x1468ae0) exists is written in pieces (the flag byte, the vectors, the names, a 0xa0-byte block and a byte), and the pointers are restored (0x51ad60). Returns 1. ROUGH: the pieces of the record copy (local_188 .. local_4) are the fields of the 0x65-word copy */
extern int ai_Unk1468ae0[];             /* 0x1468ae0, the scene objects (0x300) */
extern char *ai_Goods;              /* 0x91aaf0, the object prototypes (0x41 bytes) */
int mem_IsValid();                     /* 0x4225e0 */
int unk_Unk51aff0(int f)
{
    int cnt = 0, i, k, n;
    int *p;
    int rec[0x65];
    char flag;

    for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182); p += 0x87) {
        if (*p)
            cnt++;
    }
    LS_W(&cnt, 4)
    for (p = (int *)((unsigned)&ai_Unk770cec + 0x18); (int)p < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x182); p += 0x87) {
        if (*p != 0 && unk_Unk51ad80(*p, p[-0x60], f) == 0)
            return 0;
    }
    cnt = 0;
    {
        unsigned short *q = (unsigned short *)ai_Objects;

        for (i = 0x5008; i != 0; i--) {
            if (*(char *)(ai_Goods + *q * 0x41) == 0x1d)
                cnt++;
            q = (unsigned short *)((char *)q + 0x43);
        }
    }
    LS_W(&cnt, 4)
    for (k = 0; k < 0x14f218; k += 0x43) {
        char *q = (char *)ai_Objects + k;

        if (*(char *)(ai_Goods + *(unsigned short *)q * 0x41) == 0x1d
            && unk_Unk51ad80(*(int *)(q + 0x3b), *(int *)(q + 2), f) == 0)
            return 0;
    }
    for (k = 0; k < 0xc5440; k += 0x194) {
        int *r = (int *)(ls_ActionQueue + k);

        if (ls_ActionQueue[k + 9] == 0) {
            memset(r, 0, 0x194);
        } else {
            r[5] = *(int *)r[5];
            r[9] = ls_ActionIndex((char *)r[9]);
            r[10] = ls_ActionIndex((char *)r[10]);
        }
    }
    flag = 0;
    memset(rec, 0, sizeof(rec));
    for (k = 0; k < 0xc5440; k += 0x194) {
        int *r = (int *)(ls_ActionQueue + k);

        n = *(int *)(ls_ActionQueue + k + 0x14);
        if (n > -1 && n < 0x300) {
            n = ai_Unk1468ae0[n];
            if (mem_IsValid(n) == 0)
                __asm int 3
            if (n != 0 && *(int *)(*(int *)(n + 0x34) + 0x200) != 0) {
                flag = *((char *)r + 9);
                memcpy(rec, r, 0x194);
            }
            LS_W(&flag, 1)
            LS_W((char *)rec + 0xc, 4)
            LS_W((char *)rec + 0x14, 4)
            LS_W((char *)rec + 0x30, 0x20)
            LS_W((char *)rec + 0x50, 0x20)
            LS_W((char *)rec + 0xf0, 0xa0)
            LS_W((char *)rec + 0x190, 1)
            LS_W((char *)rec + 0x24, 4)
            LS_W((char *)rec + 0x28, 4)
        }
    }
    for (k = 0; k < 0xc5440; k += 0x194) {
        int *r = (int *)(ls_ActionQueue + k);

        if (ls_ActionQueue[k + 9] != 0) {
            r[5] = ai_Unk1468ae0[r[5]];
            r[9] = ls_ActionAt(r[9]);
            r[10] = ls_ActionAt(r[10]);
        }
    }
    return 1;
}
