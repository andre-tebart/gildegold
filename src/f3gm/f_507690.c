/* 0x507690, creates a character of the type `type` (the slot is the first free one, id -1; with none left, the first of type 0xf is reused) and returns its id (0xffff when full): clears the record (0x21c bytes), sets
   type / ids (+4 a running number 0x61a654) / defaults (+8 = 100, +0x1e4 = 1.0, +0x190 = 4), the kind byte +0x164 = a6 and +0x165 = a7, the age +0xa = a4, the player avatar for type 0x10 (0x4ed170: names, skills, floats
   copied from the avatar record), the home objects (+0x16c / +0x170 from the building a5 by its class), and either rolls the 14 float skills (+0x88 + 12 * i: value, 0.001 * value, 900 + rand) from the table 0x5ee160
   by the kind (0x504ad0), or inherits them from the two parents a2 / a3 (a random mix, the parents' child slots +0x5c ..) and picks names (0x143ac5c / 0x143af70 / 0x143b178, 0xf characters) and the portrait
   (+0x50). ROUGH */

extern int ai_Difficulty;
/* ai_Unk143c860: view of txt_Ptrs (build/mk_txt_views.py) */
int ai_GameFlags = 0;  /* 0x6f23f8 */
extern float ai_Unk617830[], ai_Unk617840[];  /* 0x617830 / 0x617840 */
char * ahm_OwnerChar();                     /* 0x4eaa50 */
int ahm_ObjectValue();                     /* 0x4ec3d0 */
int gm_ListAmount();                     /* 0x50aaf0 */
int gm_ListStock();                     /* 0x50ab50 */
int gm_ClockSeason();                     /* 0x4fe350 */
int gm_CompareTime();                     /* 0x4fe210 */
int gv_GetTableRecord(int i, int *out);                     /* 0x4f2720 */
float gm_ItemPrice(int item, unsigned char n);
int gm_Unk509e30(void *list, unsigned char p);

#define PLW54(k) (*(unsigned short *)(ai_PlayerSlots[(unsigned char)(k)] + 0x54))
#define PLTAB(i) (((int *)ai_Unk61a8ac)[i])
#define OPROTO(i) (ai_Goods + (unsigned)(i) * 0x41)

extern int ai_CharCount, ai_Unk6f1bb8, ai_Unk6f1bbc, ai_Unk6f2448, ai_NextObjectId;  /* 0x6f1bc0, 0x6f1bb8, 0x6f1bbc, 0x6f2448, 0x61a654 */

extern char ai_Unk61a8c8[];  /* 0x61a8c8 */
const float ai_Unk5ee160[476] = { 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 16.0f, 24.0f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.0f, 3.0f, 2.29999995f, 2.9000001f, 2.4000001f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 2.70000005f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.0f, 3.0f, 2.29999995f, 2.9000001f, 2.4000001f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 2.70000005f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.0f, 3.0f, 2.29999995f, 2.9000001f, 2.4000001f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 2.70000005f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 2.9000001f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 2.79999995f, 2.20000005f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 2.9000001f, 2.29999995f, 2.9000001f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 2.79999995f, 2.20000005f, 2.79999995f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.29999995f, 2.79999995f, 2.4000001f, 3.0f, 2.20000005f, 2.9000001f, 2.20000005f, 2.70000005f, 2.5f, 3.0f, 2.5f, 3.0f, 2.4000001f, 2.70000005f, 2.29999995f, 2.9000001f, 2.20000005f, 2.79999995f, 2.0f, 3.0f, 16.0f, 24.0f, 2.4000001f, 3.0f, 2.5f, 3.0f, 2.0f, 3.0f, 2.0999999f, 2.9000001f, 2.5f, 3.0f, 2.0f, 2.5f, 2.5f, 3.0f, 2.5f, 3.0f, 2.5f, 3.0f, 2.20000005f, 3.0f, 2.0f, 2.5999999f, 2.4000001f, 3.0f, 2.20000005f, 3.0f, 16.0f, 24.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.9000001f, 2.0f, 3.0f, 2.0999999f, 2.9000001f, 2.20000005f, 2.70000005f, 2.0f, 2.79999995f, 2.0999999f, 2.5999999f, 2.20000005f, 2.70000005f, 2.20000005f, 2.70000005f, 2.20000005f, 2.70000005f, 2.29999995f, 2.79999995f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 16.0f, 24.0f, 2.5f, 2.9000001f, 2.4000001f, 2.9000001f, 2.0f, 3.0f, 2.0999999f, 2.9000001f, 2.0f, 2.9000001f, 2.0f, 2.5999999f, 2.5999999f, 2.9000001f, 2.29999995f, 2.79999995f, 2.4000001f, 2.79999995f, 2.20000005f, 2.79999995f, 2.29999995f, 2.79999995f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 16.0f, 24.0f, 2.5f, 2.9000001f, 2.29999995f, 2.79999995f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 2.9000001f, 2.0999999f, 2.9000001f, 2.0999999f, 2.79999995f, 2.0999999f, 2.9000001f, 2.5f, 2.79999995f, 2.5999999f, 2.9000001f, 2.5f, 3.0f, 2.29999995f, 2.79999995f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.29999995f, 2.9000001f, 2.29999995f, 3.0f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 3.0f, 2.0f, 2.70000005f, 2.0f, 2.9000001f, 2.29999995f, 2.79999995f, 2.0f, 2.29999995f, 2.5999999f, 3.0f, 2.0f, 2.4000001f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.29999995f, 2.9000001f, 2.0999999f, 2.70000005f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.0f, 2.79999995f, 2.0999999f, 2.29999995f, 2.5999999f, 3.0f, 2.0f, 2.4000001f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.29999995f, 2.9000001f, 2.0999999f, 2.70000005f, 2.0f, 3.0f, 2.4000001f, 2.9000001f, 2.5f, 3.0f, 2.0999999f, 3.0f, 2.0f, 2.70000005f, 2.29999995f, 2.9000001f, 2.0f, 2.79999995f, 2.0999999f, 2.29999995f, 2.5999999f, 3.0f, 2.0f, 2.4000001f, 2.29999995f, 2.70000005f, 16.0f, 24.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 16.0f, 24.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };  /* 0x5ee160 */
void hmt_Create(int type, int unk, unsigned short v, struct AICHAR *c);                     /* 0x4adbb0 */
void ai_ResetJob(struct AICHAR *c);                     /* 0x47e940 */
int sprintf(char *buf, const char *fmt, ...);                     /* 0x5de652 */

#define CF(o, T) (*(T *)(c + (o)))

unsigned short gm_Unk507690(int type, int a2, int a3, int a4, unsigned char *a5, unsigned a6, int a7, int a8)
{
    int idx, i, n, kind, w;
    char *c;
    unsigned short *p1;
    unsigned short *p2;
    char *av;
    float f;
    unsigned short r, rr;
    char buf[0x400];

    idx = 0;
    while (*(short *)ai_Chars[idx] != -1) {
        idx++;
        if (idx >= 0x300)
            break;
    }
    if (idx == 0x300) {
        idx = 0;
        while (ai_Chars[idx][2] != 0xf) {
            idx++;
            if (idx >= 0x300)
                break;
        }
    }
    c = ai_Chars[idx];
    if (c[2] == 0xf)
        ai_CharCount--;
    if (idx == 0x300)
        return 0xffff;
    memset(c, 0, 0x21c);
    ai_CharCount++;
    CF(4, int) = -1;
    CF(0, unsigned short) = 0xffff;
    for (i = 0; i < 8; i++)
        CF(0x5c + i * 4, int) = -1;
    CF(0, unsigned short) = (unsigned short)idx;
    c[2] = (char)type;
    CF(0x170, int) = 0;
    CF(0x16c, int) = 0;
    CF(0x1f0, int) = 0;
    CF(0x210, int) = -1;
    c[0x215] = 0;
    c[0x216] = 0;
    c[0x217] = 0;
    c[0x218] = 1;
    sprintf(buf, "-----> SpID = %i, playermode = %i", ai_NextObjectId, type);
    CF(4, int) = ai_NextObjectId;
    ai_NextObjectId++;
    c[0x164] = (char)a6;
    CF(0x18c, int) = -1;
    CF(0x20c, int) = -1;
    CF(0x1e4, float) = 1.0f;
    c[8] = 100;
    CF(0xa, unsigned short) = (unsigned short)a4;
    c[0x166] = 0;
    c[0x165] = (char)a7;
    CF(0x1f4, char) = 0;
    CF(0x190, int) = 4;
    CF(0x194, int) = 0;
    c[0xd] = 1;
    kind = c[2];
    if (kind == 3 || kind == 0x10 || kind == 0x13) {
        CF(0x1e8, int) = 1;
        c[0xd] = 2;
    }
    if (kind == 0x10) {
        av = gv_AllocAvatar();
        c[2] = 3;
        if (av != 0) {
            r = util_RandomU(8);
            CF(0x54, int) = 0x59a;
            CF(0x1f0, int) = *(int *)av;
            c[0x164] = av[0xd4];
            c[9] = av[0xd5];
            c[0xc] = av[0xd6];
            strcpy(c + 0x30, av + 4);
            strcpy(c + 0x40, av + 0x14);
            strcpy(c + 0x1f4, av + 0xd7);
            CF(0x80, int) = *(int *)(av + 0x24);
            c[0x84] = av[0x28];
            memcpy(c + 0x88, av + 0x2c, 0xa8);
            CF(0x1d0, float) = util_RandomF() * 0.5f + 0.25f;
            CF(0x18c, int) = unk_Unk4fb770(av + 0xd7) + 0x639;
            CF(0x1c, float) = 1000.0f;
            CF(0x50, unsigned short) = (unsigned short)ai_Unk6f1bb8++;
            CF(0x20, float) = 12.0f;
            CF(0x14, float) = util_RandomSq() * 7.0f + 30.0f;
            rr = util_RandomU((int)CF(0x14, float));
            CF(0x18, float) = CF(0x1c, float);
            CF(0x10, float) = (CF(0x1c, float) * 0.33f + rr) - CF(0x1c, float) * 0.1f;
            if (r < 2)
                c[0x217] = 0x58;
            else if (r < 4)
                c[0x217] = 0x5c;
            c[0x217] = (char)((((r > 5) - 1) & 0x86) + 0x3c);
            if (CF(0x20, float) < (float)CF(0xa, unsigned short)) {
                n = (int)CF(0x14, float);
                f = CF(0x20, float);
                for (; n > 0; n--)
                    f = (f - CF(0x14, float)) + CF(0x14, float) / f;
                if (f <= 1.0f)
                    f = 1.0f;
                CF(0x1c, float) = f;
                return CF(0, unsigned short);
            }
            return CF(0, unsigned short);
        }
    }
    if (c[2] == 0x15 || c[2] == 0x16) {
        hmt_Create(c[2], a2, a4, c);
        return CF(0, unsigned short);
    }
    if (a5 != 0) {
        kind = (unsigned char)ai_BTypes[a5[0] * 0x24d];
        if (kind == 1)
            CF(0x170, unsigned char *) = a5;
        else if (kind > 3 && kind < 0x17 && kind != 10 && kind != 0xf && kind != 0x11)
            CF(0x16c, unsigned char *) = a5;
    }
    c[0xd] = (c[0x166] != 0) + 1;
    if (type != 6 && type != 7 && type < 10)
        c[0x1b0] = (char)util_RandomU(8);
    if (a8 == 2)
        a8 = util_RandomU(2);
    c[9] = (char)a8;
    if (type < 4 || type == 0xb) {
        if (a7 == 0) {
            if (a6 == 0)
                goto skip;
            w = gm_Unk503f30(a6);
        } else
            w = gm_Unk503f50(a7);
        if (w != 2)
            c[9] = (char)w;
    }
skip:
    CF(0x54, int) = 0x59a;
    if (ai_Unk6f2448 != 0) {
        CF(0x194, int) = 10;
        CF(0x190, int) = 10;
    }
    p1 = (unsigned short *)gm_FindChar(a2);
    p2 = (unsigned short *)gm_FindChar(a3);
    if (p1 == 0 || p2 == 0) {
        c[0x164] = (char)a6;
        c[0x166] = 0;
        c[0x165] = (char)a7;
        w = gm_TitleGroup(a6 & 0xff);
        r = util_RandomU(8);
        if (c[2] == 3) {
            if (w == 0xb || w == 0xc) {
                if (r > 2)
                    c[0x217] = (char)(((r > 5) - 1 & 0x9a) - 0x3e);
                else
                    c[0x217] = 0x58;
            } else if (r < 3)
                c[0x217] = 0x3c;
            else
                c[0x217] = (char)((r < 6) * 4 + 0x58);
        } else if (w == 0xb || w == 0xc)
            c[0x217] = (char)((r > 3) * 8 + 0x4e);
        else if (r < 3)
            c[0x217] = 0x3e;
        else
            c[0x217] = (char)((r < 6) * 8 + 0x4e);
        for (i = 0; i < 14; i++) {
            float lo = ai_Unk5ee160[w * 0x1c + i * 2], hi = ai_Unk5ee160[w * 0x1c + i * 2 + 1];
            CF(0x88 + i * 12, float) = (hi - lo) * util_RandomF() + lo;
            rr = util_RandomU(10);
            CF(0x90 + i * 12, float) = (float)rr + 900.0f;
            CF(0x8c + i * 12, float) = CF(0x88 + i * 12, float) * 0.001f;
        }
        f = (util_RandomF() + 2.0f) * 8.0f;
        CF(0x124, float) = f;
        CF(0x128, float) = f * 0.001f;
        for (i = 0; i < 0x300; i++) {
            ai_RelationMemory[i][CF(0, unsigned short)] = 0;
            ai_Relations[i][CF(0, unsigned short)] = (char)(util_RandomU(0x40) - 0x20);
            ai_Relations[CF(0, unsigned short)][i] = (char)(util_RandomU(0x40) - 0x20);
            ai_RelationMemory[CF(0, unsigned short)][i] = 0;
        }
        c[0xc] = (char)util_RandomU(2);
        CF(0x1d0, float) = util_RandomF() * 0.5f + 0.25f;
        for (i = 0; i < 5; i++)
            c[0x80 + i] = (char)gm_RandomAge(ai_Unk61a688[(unsigned char)c[0x164] * 6 + i]);
        if (type != 6 && type != 7)
            c[0xd] = ai_Unk61a688[(unsigned char)c[0x164] * 6 + 4];
        for (i = 0; i < 8; i++)
            CF(0x5c + i * 4, int) = -1;
        CF(0x64, int) = a3;
        c[0x58] = 1;
        if (type == 6 || type == 7 || type == 5) {
            if (ai_Unk6f1bbc > 0xf)
                return 0xffff;
            CF(0x50, unsigned short) = (unsigned short)ai_Unk6f1bbc | 0x8000;
            ai_Unk6f1bbc++;
        } else if (type < 10)
            CF(0x50, unsigned short) = (unsigned short)ai_Unk6f1bb8++;
        else
            CF(0x50, unsigned short) = 0x7fff;
        CF(0x1c, float) = 1000.0f;
        CF(0x20, float) = 12.0f;
        CF(0x14, float) = util_RandomSq() * 7.0f + 30.0f;
        rr = util_RandomU((int)CF(0x14, float));
        CF(0x18, float) = CF(0x1c, float);
        CF(0x10, float) = (CF(0x1c, float) * 0.33f + rr) - CF(0x1c, float) * 0.1f;
        strncpy(c + 0x40, ai_Unk143b178[util_RandomU(0x95)], 0xf);
    } else {
        c[0xc] = (char)p1[6];
        CF(0x1d0, float) = *(float *)(p1 + 0xe8);
        c[0x217] = *((char *)p1 + 0x217);
        for (i = 0; i < 14; i++) {
            f = util_RandomF();
            CF(0x88 + i * 12, float) = f * *(float *)((char *)p1 + 0x88 + i * 12) + (1.0f - f) * *(float *)((char *)p2 + 0x88 + i * 12);
            rr = util_RandomU(10);
            CF(0x90 + i * 12, float) = (float)rr + 900.0f;
            CF(0x8c + i * 12, float) = CF(0x88 + i * 12, float) * 0.001f;
        }
        f = (util_RandomF() + 2.0f) * 8.0f;
        CF(0x124, float) = f;
        CF(0x128, float) = f * 0.001f;
        CF(0x60, int) = a2;
        CF(0x64, int) = a3;
        CF(0x68, int) = -1;
        CF(0x6c, int) = -1;
        CF(0x70, int) = -1;
        CF(0x74, int) = -1;
        CF(0x78, int) = -1;
        CF(0x7c, int) = 0;
        CF(0x50, unsigned short) = p1[0x28];
        c[0x58] = (char)p1[0x2c] + 1;
        CF(0x54, int) = *(int *)(p1 + 0x2a);
        CF(0x1c, float) = 1000.0f;
        CF(0x20, float) = 12.0f;
        CF(0x14, float) = (*(float *)(p2 + 10) + *(float *)(p1 + 10)) * 0.2f + (util_RandomSq() * 7.0f + 30.0f) * 0.6f;
        rr = util_RandomU((int)CF(0x14, float));
        CF(0x18, float) = CF(0x1c, float);
        CF(0x10, float) = (CF(0x1c, float) * 0.33f + rr) - CF(0x1c, float) * 0.1f;
        strcpy(c + 0x40, (char *)(p1 + 0x20));
    }
    if (c[9] == 0)
        strncpy(c + 0x30, ai_Unk143ac5c[util_RandomU(0xbf)], 0xf);
    else
        strncpy(c + 0x30, ai_Unk143af70[util_RandomU(0x70)], 0xf);
    if (CF(0x20, float) < (float)CF(0xa, unsigned short)) {
        n = (int)CF(0x14, float);
        f = CF(0x20, float);
        for (; n > 0; n--)
            f = (f - CF(0x14, float)) + CF(0x14, float) / f;
        if (f <= 1.0f)
            f = 1.0f;
        CF(0x1c, float) = f;
    }
    gm_ChooseHeadIcon(c);
    CF(0x1b4, int) = 0;
    CF(0x1b8, int) = 0;
    CF(0x1bc, int) = 0;
    CF(0x1c0, int) = 0;
    ai_ResetJob(c);
    kind = c[2];
    if (kind == 3 || kind == 4 || kind == 2 || kind == 0x13)
        c[0x82 + (util_RandomU(3) & 0xffff)] += 0x2a;
    sprintf(buf, "-----> PlayerID = %i, PlayerName = %s", ai_NextObjectId, c + 0x30);
    return (unsigned short)idx;
}
