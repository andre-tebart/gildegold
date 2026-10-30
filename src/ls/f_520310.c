/* 0x520310, reads the two record pools 0x9367e0 / 0xa69140 (0x99-byte records: the references at +0x91 / +0x95 are indices turned into pointers (-1 = NULL)), the pool heads 0xa68d24 / 0xa68c00 / 0xf81160, the reference table 0xf31160 (10 bytes each, 0x2000 or 0x8000 of them from 0x1003c) and the words 0xf81168, 0xf8121c, 0xa68c04, 0x60bd88 .. 0x60bd90. Returns true when all was read @deep diff read against the source: meaning verified - the 0x99-byte pool loop 0x9367e0 .. (refs at +0x91/+0x95, -1 -> NULL, else pool + n*0x99), the pool heads a68d24 / a68c00, the 0x2000/0x8000 table from 0x1003c, the reference table 0xf31160 (10-byte entries, p[1]=0, *p=-1, byte -2 = 1), the second pool loop 0xa69140, the f81160 head and the final reads (0xf8121c, 0xa68c04, 0x60bd88 / 0x60bd8c / 0x60bd90) all match. Left is codegen: the IV base - ours keeps esi at p+4 (reads [esi-0x4] / [esi] and the record at esi-0x95), the original keeps esi at p (record at esi-0x91, refs at [esi] / [esi+0x4]) - same addresses, and the loop bound n in a stack slot vs ecx; plus the shared vs inline return-0 epilogue (xor eax,eax at 0x52039e). Readability: Q1, Q2 (ai_Unka68c00 / a68d24 / f81160 / f81168), Q3 (12 image literals), Q4. */


int unk_Unk520310(int f)
{
    int *p;
    int i, n;
    char *q;

    for (p = (int *)((unsigned)&ai_Unk9367e0 + 0x91); (int)p < ((unsigned)&ai_Unka68800 + 0x71); p = (int *)((char *)p + 0x99)) {
        LS_R((char *)p - 0x91, 0x99)
        if (*p == -1)
            *p = 0;
        else
            *p = (unsigned)&ai_Unk9367e0 + *p * 0x99;
        if (p[1] == -1)
            p[1] = 0;
        else
            p[1] = (unsigned)&ai_Unk9367e0 + p[1] * 0x99;
    }
    LS_R(&n, 4)
    ai_Unka68d24I = n == -1 ? 0 : (unsigned)&ai_Unk9367e0 + n * 0x99;
    i = 0;
    LS_R(&n, 4)
    ai_Unka68c00I = n == -1 ? 0 : (unsigned)&ai_Unk9367e0 + n * 0x99;
    n = ((ls_Version < 0x1003c) - 1 & 0x6000) + 0x2000;
    for (p = (int *)((unsigned)&ai_Unkf31160 + 0x2); (int)p < ((unsigned)((unsigned)ai_Unkf31160 + 0x50000) + 0x2); p = (int *)((char *)p + 10)) {
        p[1] = 0;
        *p = -1;
        *((char *)p - 2) = 1;
    }
    if (n != 0) {
        for (p = (int *)((unsigned)&ai_Unka69140 + 0x91); i < n; p = (int *)((char *)p + 0x99)) {
            LS_R((char *)p - 0x91, 0x99)
            if (*p == -1)
                *p = 0;
            else
                *p = (unsigned)&ai_Unka69140 + *p * 0x99;
            if (p[1] == -1)
                p[1] = 0;
            else
                p[1] = (unsigned)&ai_Unka69140 + p[1] * 0x99;
            i++;
        }
    }
    i = 0;
    if (ls_Read((int)&n, 4, 1, f) == 0)
        return 0;
    cm_SendQueueI = n == -1 ? 0 : (unsigned)&ai_Unka69140 + n * 0x99;
    LS_R(&ai_Unkf81168, 4)
    n = ((ls_Version < 0x1003c) - 1 & 0x6000) + 0x2000;
    if (n != 0) {
        q = (char *)(unsigned)&ai_Unkf31160;
        do {
            LS_R(q, 10)
            i++;
            q += 10;
        } while (i < n);
    }
    if (ls_Read((int)(unsigned)&ai_Unkf8121c, 4, 1, f) == 0)
        return 0;
    LS_R((unsigned)&ai_Unka68c04, 4)
    *(int *)(unsigned)&ai_Unk60bd88 = 0;
    if (ls_Read((int)(unsigned)&ai_Unk60bd88, 4, 1, f) == 0)
        return 0;
    LS_R((unsigned)&ai_Unk60bd8c, 4)
    return ls_Read((int)(unsigned)&ai_Unk60bd90, 4, 1, f) != 0;
}
