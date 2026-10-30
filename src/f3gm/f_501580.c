/* 0x501580, creates a building of the type k for the owner o (the next free instance, 0x5012b0): the id (0x61a654 counter), the owner / place words, the default values (rank 100, 0x7d00 ..), a name from the names of its type that no building has yet (up to 12 candidates of 0x20 characters: "Building name too long: %s" is the error text), the objects of its prototype (0x500190 / 0x5031b0 for the kinds 2 / 6), the main object (0x501ce0) and the per-type extras: dates (the clock at +0x69 .. +0x75) for the types 4 .. 7, an empty owner for 0xb .. 0xd, the counters of 0xe .. 0x10, the object 0x1b5 for 0x14 .. 0x16, the "f3_gm:PlantMap" of 0x1e (0x600 bytes of 0x18-byte cells), 0x4efc10 / 0x4ef9c0 for 0x48 .. 0x4a, "AHM_INFO" (0x3c bytes) for 0x4b .. 0x4d. ROUGH */

int unk_Unk4ef9c0();                     /* 0x4ef9c0 */
void gm_GetGebaeudeHauptraum(unsigned char *b);                     /* 0x501ce0 */
int gm_BuildingClass();                     /* 0x501d80 */
/* ai_BuildingNames: view of txt_Ptrs (build/mk_txt_views.py) */
extern char ai_Time[];             /* 0x770ae0 */
void * mem_Alloc(int size, char *tag);                   /* 0x4223c0 */
unsigned short * unk_Unk5031b0(unsigned char *b, unsigned n);

unsigned char *gm_CreateBuilding(unsigned char k, unsigned short own)
{
    unsigned char *b;
    char *base, *proto;
    char dbg[0x100];
    char names[12][0x40];
    char cand[0x100];
    char err[0x200];
    int i, n, j, found, any;
    unsigned short *w;
    char *src, *u;
    short *s;
    int *q;
    int ch;
    char c;
    char *bl;

    base = ai_BTypes;
    if (base == 0)
        return 0;
    any = 0;
    b = (unsigned char *)gm_FindFreeBuilding();
    if (b == 0)
        return 0;
    *b = k;
    proto = base + k * 0x24d;
    sprintf(dbg, "----->GebID= %i GebProt= %i", ai_NextObjectId, k);
    *(int *)(b + 1) = ai_NextObjectId;
    ai_NextObjectId++;
    *(unsigned short *)(b + 0x25) = own;
    *(unsigned short *)(b + 0x27) = own;
    for (i = 0x29; i <= 0x2f; i++)
        b[i] = 0;
    for (i = 0x5d; i <= 0x64; i++)
        b[i] = 0;
    *(int *)(b + 0x30) = 0;
    *(int *)(b + 0x39) = 0x7d00;
    *(int *)(b + 0x3d) = 100;
    *(int *)(b + 0x41) = 2;
    *(int *)(b + 0x45) = 100;
    *(int *)(b + 0x49) = 0;
    *(float *)(b + 0x4b) = 1.0f;
    b[0x5c] = 100;
    *(int *)(b + 0x95) = -1;
    if (own != 0xffff && ai_Chars[own][2] == 6)
        ai_Unk5a8560K(b);
    *(unsigned *)(b + 0x99) = 0;
    for (i = 0x9d; i <= 0xa8; i++)
        b[i] = 0;
    *(int *)(b + 0xa5) = -1;
    *(unsigned *)(b + 0x99) |= 1;
    memset(b + 0x65, 0, 0x30);
    n = 0;
    if (**(char **)(&ai_BuildingNames[k * 0xe]) == 0) {
        b[5] = ch_EmptyName[0];
    } else {
        strcpy((char *)b + 5, *(char **)(&ai_BuildingNames[k * 0xe - 2]));
    }
    for (i = 0; i < 0xc; i++) {
        src = *(char **)(&ai_BuildingNames[k * 0xe + i]);
        if (*src != 0) {
            strcpy(cand, src);
            bl = ai_Blds;
            for (j = 0; j < 0x100; j++, bl += 0xa9) {
                if (*bl != 0 && strcmp(bl + 5, cand) == 0)
                    break;
            }
            if (j < 0x100)
                continue;
            if (strlen(cand) < 0x20) {
                strcpy(names[n], cand);
                n++;
            } else
                sprintf(err, "Building name too long: %s", cand);
        }
    }
    if (n != 0) {
        j = util_RandomU(n) & 0xffff;
        strcpy((char *)b + 5, names[j]);
    }
    w = (unsigned short *)(proto + 0x23);
    for (i = 0; i < 0x40; i++, w++) {
        if (*w == 0)
            break;
        if (*w != 0xffff) {
            c = ai_Goods[(*w & 0x7fff) * 0x41];
            if (c == 2 || c == 6)
                any = 1;
            if ((short)*w < 0) {
                if (c == 2 || c == 6) {
                    any = 1;
                    unk_Unk5031b0(b, *w & 0x7fff);
                } else if (any == 0)
                    gm_CreateObject(*(int *)(b + 1), *w & 0x7fff, 1);
            }
        }
    }
    s = (short *)gm_Query(*(void **)(b + 0x5d), 1, 4, 2);
    while (s != 0) {
        if (*s != 0xfd) {
            *(short *)(b + 0x29) = *s;
            break;
        }
        s = (short *)gm_QueryNext();
    }
    s = (short *)((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(b);
    if (s != 0)
        *(short *)(b + 0x29) = *s;
    if (gm_IsHomeType(b) != 0)
        *(int *)(b + 0x30) = 0x1388;
    switch (k) {
    case 4:
    case 5:
    case 6:
    case 7:
        *(int *)(b + 0x69) = *(int *)ai_Time;
        *(int *)(b + 0x6d) = *(int *)(ai_Time + 4);
        *(int *)(b + 0x71) = *(int *)(ai_Time + 8);
        *(short *)(b + 0x75) = *(short *)(ai_Time + 12);
        if (own != 0xffff) {
            *(int *)(b + 0x65) = *(int *)(ai_Chars[own] + 4);
            break;
        }
    case 0xb:
    case 0xc:
    case 0xd:
        *(int *)(b + 0x65) = -1;
        break;
    case 0xe:
    case 0xf:
    case 0x10:
        *(int *)(b + 0x65) = 10;
        *(int *)(b + 0x69) = 3;
        *(int *)(b + 0x6d) = 5;
        break;
    case 0x14:
    case 0x15:
    case 0x16:
        q = (int *)gm_CreateObject(*(int *)(b + 1), 0x1b5, 1);
        if (q != 0) {
            *((char *)q + 0x22) = 0;
            *((char *)q + 0x1c) = -1;
            *((char *)q + 0x1d) = -1;
            *(int *)((char *)q + 0x1e) = -1;
        }
        break;
    case 0x1e:
        *(void **)(b + 0x71) = mem_Alloc(0x600, "f3_gm:PlantMap");
        for (i = 0; i < 0x600; i += 0x18) {
            *((char *)*(void **)(b + 0x71) + i + 0xd) = -1;
            *(int *)((char *)*(void **)(b + 0x71) + i + 0x14) = 0;
        }
    case 0x1f:
    case 0x20:
        *(int *)(b + 0x30) = 100;
        *(float *)(b + 0x49) = 0.8f;
        break;
    case 0x35:
    case 0x36:
    case 0x37:
        *(int *)(b + 0x65) = 0;
        *(int *)(b + 0x69) = 0;
        *(int *)(b + 0x6d) = 0;
        break;
    case 0x47:
        *(int *)(b + 0x30) = 0x13880;
        break;
    case 0x48:
    case 0x49:
    case 0x4a:
        *(int *)(b + 0x65) = gv_AllocGraveyard(b);
        unk_Unk4ef9c0(b, 3);
        break;
    case 0x4b:
    case 0x4c:
    case 0x4d:
        q = (int *)mem_Alloc(0x3c, "AHM_INFO");
        *(int **)(b + 0x65) = q;
        *q = -1;
    }
    if (*b == 0x26) {
        b[0x29] = 0xe;
        b[0x2a] = 1;
    }
    gm_SetCapacities(b);
    gm_BuildingClass(*b);
    gm_StockBuildingObjects(b);
    return b;
}
