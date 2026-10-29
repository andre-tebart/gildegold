/* 0x51d550, reads the buildings ai_Blds from the save file f (the counterpart of the writer 0x519d20): for each of n records the fields (+0 .. +0x5c), for the type 0x1e the plant map ("f3g:PlantMap", 0x600 bytes of 0x18-byte plants, its pointer at +0x71), for the types 0x48 .. 0x4a / 0x4b .. 0x4d the type specific record at +0x65 (read by gv_ReadGraveyard / ahm_ReadRecord, or built by gv_AllocGraveyard / a new "AHM_INFO" record for the versions below 0x10047 / 0x10049), then the 0x10 flag bytes at +0x99 (for the versions below 0x10043: zero, 0xffffffff at +0xa5, bit 0 set) and the fixed +0x30 = 0x1388 and +0x95 = -1; at the end the building sites ai_Bauplaetze (ai_NBauplaetze records of 0x60 bytes). Returns 1, 0 when a read fails. @deep Q4: the 13 raw accesses need the AIBLD fields at +0x65 / +0x71 / +0x41 / the 0x18-byte plant record (ai.h; ls.c cannot include ai.h: gv.h declares ai_Blds/ai_Chars/... with other types, the same type debt as 0x51d4f0). B: the meaning is verified instruction by instruction (all hunks read against 0x51d550..0x51daff); the remaining code hunks are source shape: the off=0 / k=0 preheader init and the k / off stack slots ([esp+0x10] / [esp+0x20] swapped), the K/L/M and H/I/J version branches laid out with the old-version arm inline where the original puts the read inline, and the i / j register pair of the building-site loop. The plant map loop now re-reads the pointer field (+0x71) every read like the original (1200 -> 935 differing bytes). */

int ls_ReadBuildings(int f)
{
    int n, k, off = 0, i, j, r;
    char *p;
    char *q;
    int *pi;
    unsigned *flags;
    int dummy;

    gm_ClearBuildings();
    LS_R(&n, 4)
    for (k = 0; k < n; k++) {
        p = ai_Blds + off;
        LS_R(p, 1)
        LS_R(p + 1, 4)
        LS_R(p + 5, 0x20)
        LS_R(p + 0x25, 2)
        LS_R(p + 0x27, 2)
        LS_R(p + 0x29, 2)
        LS_R(p + 0x2b, 4)
        LS_R(p + 0x2f, 1)
        LS_R(p + 0x34, 1)
        LS_R(p + 0x35, 4)
        LS_R(p + 0x39, 4)
        LS_R(p + 0x3d, 4)
        pi = (int *)(p + 0x41);
        LS_R(pi, 4)
        if (*pi > 4)
            *pi = 2;
        LS_R(p + 0x45, 4)
        if (ls_Version >= 0x10028)
            LS_R(p + 0x49, 4)
        if (ls_Version < 0x10032)
            LS_R(&dummy, 4)
        if (ls_Version >= 0x10057) {
            LS_R(p + 0x4d, 4)
            LS_R(p + 0x51, 4)
            LS_R(p + 0x55, 4)
        }
        LS_R(p + 0x5a, 2)
        LS_R(p + 0x5c, 1)
        if (*p == 0x1e) {
            *(char **)(p + 0x71) = mem_Alloc(0x600, "f3g:PlantMap");
            for (i = 0; i < 0x600; i += 0x18) {
                LS_R(*(char **)(p + 0x71) + i, 4)
                LS_R(*(char **)(p + 0x71) + i + 4, 4)
                LS_R(*(char **)(p + 0x71) + i + 8, 1)
                LS_R(*(char **)(p + 0x71) + i + 9, 1)
                LS_R(*(char **)(p + 0x71) + i + 10, 2)
                LS_R(*(char **)(p + 0x71) + i + 0x10, 4)
                LS_R(*(char **)(p + 0x71) + i + 0xc, 1)
                LS_R(*(char **)(p + 0x71) + i + 0xd, 1)
                *(int *)(*(char **)(p + 0x71) + i + 0x14) = 0;
            }
        } else {
            if (*p == 'H' || *p == 'I' || *p == 'J') {
                if (ls_Version < 0x10047) {
                    LS_R(p + 0x65, 0x30)
                    *(int *)(p + 0x65) = (int)gv_AllocGraveyard(p);
                    goto tail;
                }
                r = gv_ReadGraveyard(f, p);
            } else if (*p == 'K' || *p == 'L' || *p == 'M') {
                if (ls_Version < 0x10049) {
                    int *a;

                    LS_R(p + 0x65, 0x30)
                    a = (int *)mem_Alloc(0x3c, "AHM_INFO");
                    *(int **)(p + 0x65) = a;
                    *a = -1;
                    goto tail;
                }
                r = ahm_ReadRecord(f, p);
            } else {
                r = ls_Read((int)(p + 0x65), 0x30, 1, f);
            }
            if (r == 0)
                return 0;
        }
tail:
        flags = (unsigned *)(p + 0x99);
        if (ls_Version < 0x10043) {
            memset(p + 0x99, 0, 0x10);
            p[0xa5] = -1;
            p[0xa6] = -1;
            p[0xa7] = -1;
            p[0xa8] = -1;
            *flags |= 1;
        } else {
            LS_R(flags, 0x10)
        }
        p[0x30] = 0x88;
        p[0x31] = 0x13;
        p[0x32] = 0;
        p[0x33] = 0;
        p[0x95] = -1;
        p[0x96] = -1;
        p[0x97] = -1;
        p[0x98] = -1;
        off += 0xa9;
    }
    LS_R(&ai_NBauplaetze, 4)
    ai_Bauplaetze = realloc(ai_Bauplaetze, ai_NBauplaetze * 0x60);
    for (i = 0, j = 0; i < ai_NBauplaetze; i++) {
        LS_R(ai_Bauplaetze + j, 0x40)
        LS_R(ai_Bauplaetze + j + 0x40, 0x10)
        LS_R(ai_Bauplaetze + j + 0x50, 0x10)
        j += 0x60;
    }
    return 1;
}
