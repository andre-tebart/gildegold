/* 0x577690, when `arg` is 0 and 0x4e8160() answers non-zero: builds a 0x114-byte record (zeroed) out of the
   character records 0x770b80 + i * 0x21c (the kind byte +2: 6 -> its id to +0xc, 7 -> the ids +0x34, count +0x30,
   byte), copies the game time 0x770ae0 .. 0x770aec, sets the bytes +0x10 = 1 / +0x2e = 0xa0 / the dword +0x1c = -1
   and calls 0x4c4fa0 (record). ROUGH: the record's field offsets are the disassembly's; its type is unknown.
   Ends 0x57773f; the next start 0x577740 has no nop padding */

void ai_Unk577690(int arg)
{
    char rec[0x114];
    char *p;
    int i, n;

    if (arg)
        return;
    if (!amt_CheckOfficeColumns())
        return;
    memset(rec, 0, sizeof rec);
    n = 0;
    for (p = (char *)(unsigned)&ai_CharInts; p < (char *)((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x2); p += 0x21c) {
        if (*(unsigned char *)(p - 2) == 6)
            *(int *)(rec + 0xc) = *(int *)p;
        else if (*(unsigned char *)(p - 2) == 7) {
            *(int *)(rec + 0x34 + n * 4) = *(int *)p;
            n++;
        }
    }
    rec[0x30] = (char)n;
    *(int *)(rec + 0x18) = U_I((unsigned)&ai_Time);
    *(int *)(rec + 0x1c) = -1;
    *(int *)(rec + 0x28) = U_I(((unsigned)&ai_Time + 0x8));
    *(unsigned short *)(rec + 0x2c) = *(unsigned short *)((unsigned)&ai_Time + 0xc);
    rec[0x10] = 1;
    rec[0x2e] = (char)0xa0;
    ai_Unk4c4fa0(rec);
}
