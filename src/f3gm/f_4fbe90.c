/* 0x4fbe90, one daily step of the character c (the value is the new health, -1 for no step): free slots (id -1) -1; the kind 0xf (0x5075c0 true: message 0x4c4e00 (id, 1)); kind 0x14 (0x4f1660 == 1: removed through the message 0x72, the message 0x414a to the master player); then the flags word +0x2c is compacted (each field of 1 .. 4 bits is halved: 1 -> 0, n -> n | n >> 1 of the field's upper bits) and sent (0x4c4a60 (id, 0x2c, 4, flags, -1)); a character with flag 2 (+0x1cc) loses 0x11 + random 0x10 of its float +0x1c and dies (message 0x72) below 0; otherwise the health float +0x1c moves toward +0x10 (a share of +0x20 by the age (+10) / the 0x5092e0 skill of the master's building, +0x14 as the target) and when it plus the int +0x24 falls below 0 the character dies (a master gets the message 0x1c51 / 0x4080 with "NACHRICHTEN\DOMIZIL_05"; with flag 1 and kinds other than 6 / 7 it is reset to 100 and the 0x1cc flags = 2); else 0x4fccc0 and the age (+10) + 1 is stored. ROUGH, the float code is guessed */

extern int cm_FieldObject;                /* 0xa68d2c */
extern char ch_EmptyName[];             /* 0x631090 */
int gm_Unk5075c0();                     /* 0x5075c0 */
void cm_SendKillCharacter(int a, int b);                     /* 0x4c4e00 */
int unk_Unk4f1660();                     /* 0x4f1660 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
void cm_SendFieldValues(void);                     /* 0x4c49e0 */
int cm_SendLongMessage(char *msg);                     /* 0x4c4b70 */
void txt_Format(char *out, int id0, ...);  /* 0x5176d0 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);                     /* 0x53dda0 */
void cm_SendSetField(int a, int b, int c, int d, int e);                     /* 0x4c4a60 */
void unk_Unk4fccc0();                   /* 0x4fccc0 */

/* the 0xf8-byte message 0x72 (kind at +4, the character id at +8 and +0x58, -1 at +0xc, the clock 0x770ae0 at +0x28 .. +0x34, 2 at +0x36) of 0x4fbe90 */
#define G_MSG72(m, c)                                                  \
    memset(m, 0, 0xf8);                                                \
    *(int *)((m) + 8) = *(int *)((c) + 4);                             \
    *(int *)((m) + 0x28) = *(int *)ai_Time;                       \
    *(int *)((m) + 0x2c) = *(int *)(ai_Time + 4);                 \
    (m)[4] = 0x72;                                                     \
    *(int *)((m) + 0xc) = -1;                                          \
    (m)[0x36] = 2;                                                     \
    *(int *)((m) + 0x30) = *(int *)(ai_Time + 8);                 \
    *(short *)((m) + 0x34) = *(short *)(ai_Time + 0xc);           \
    *(int *)((m) + 0x58) = *(int *)((m) + 8);                          \
    cm_SendLongMessage(m)

float unk_Unk4fbe90(unsigned short *c)
{
    char m[0xf8];
    char buf[512];
    unsigned f, g;
    char b;
    float h, lo, t, fl;
    double d;
    int a;
    char *p = (char *)c;
    char *mst;
    int k;

    if (*c == 0xffff)
        return -1.0f;
    if (p[2] == 0x0f) {
        if (gm_Unk5075c0(c) != 0) {
            cm_SendKillCharacter(*(int *)(p + 4), 1);
            ai_Unk6f1b98++;
        }
        return -1.0f;
    }
    if (p[2] == 0x14) {
        if (unk_Unk4f1660(c) == 1) {
            b = 0;
            cm_BeginFieldBlock(c, *(int *)(p + 4));
            cm_AddFieldValue(1, 1, (int)p + (8 - cm_FieldObject), &b);
            cm_SendFieldValues();
            G_MSG72(m, p);
            mst = *(char **)(p + 0x16c);
            if (mst != 0 && *(unsigned short *)(mst + 0x27) != 0xffff && (ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 6 || ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 7)) {
                txt_Format(buf, 0x414a, *c, mst);
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, ch_EmptyName);
            }
        }
        return 0.0f;
    }
    f = *(unsigned *)(p + 0x2c);
    if (f != 0) {
        if ((f & 0xf) != 0) {
            if ((f & 0xf) == 1)
                f &= 0xfffffff0;
            else
                f = (f & 0xfffffff0) | ((f >> 1) & 7);
        }
        g = (f >> 4) & 0xf;
        if (g != 0) {
            if (g == 1)
                f &= 0xffffff0f;
            else
                f = (f & 0xffffff0f) | ((f >> 1) & 0x70);
        }
        g = (f >> 8) & 0xf;
        if (g != 0) {
            if (g == 1)
                f &= 0xfffff0ff;
            else
                f = (f & 0xfffff0ff) | ((f >> 1) & 0x700);
        }
        g = (f >> 0xc) & 3;
        if (g != 0) {
            if (g == 1)
                f &= 0xffffcfff;
            else
                f = (f & 0xffffcfff) | ((f >> 1) & 0x1000);
        }
        g = (f >> 0xe) & 7;
        if (g != 0) {
            if (g == 1)
                f &= 0xfffe3fff;
            else
                f = (f & 0xfffe3fff) | ((f >> 1) & 0xc000);
        }
        g = (f >> 0x11) & 7;
        if (g != 0) {
            if (g == 1)
                f &= 0xfff1ffff;
            else
                f = (f & 0xfff1ffff) | ((f >> 1) & 0x60000);
        }
        g = (f >> 0x14) & 7;
        if (g != 0) {
            if (g == 1)
                f &= 0xff8fffff;
            else
                f = (f & 0xff8fffff) | ((f >> 1) & 0x300000);
        }
        g = (f >> 0x17) & 3;
        if (g != 0) {
            if (g == 1)
                f &= 0xfe7fffff;
            else
                f = (f & 0xfe7fffff) | ((f >> 1) & ((unsigned)&ai_Relations + 0x1aca0));
        }
        g = (f >> 0x19) & 0xf;
        if (g != 0) {
            if (g == 1)
                f &= 0xe1ffffff;
            else
                f = (f & 0xe1ffffff) | ((f >> 1) & 0xe000000);
        }
        cm_SendSetField(*(int *)(p + 4), 0x2c, 4, f, -1);
        ai_Unk6f1b98++;
    }
    if (p[0x1cc] & 2) {
        fl = (float)((util_Random(0x10) & 0xffff) + 0x11);
        t = *(float *)(p + 0x1c) - (float)(int)fl;
        cm_BeginFieldBlock(c, *(int *)(p + 4));
        cm_AddFieldDelta(4, 1, 0x1c, &t);
        cm_SendFieldDeltas();
        ai_Unk6f1b98++;
        if (t <= 0.0f) {
            b = 0;
            cm_BeginFieldBlock(c, *(int *)(p + 4));
            cm_AddFieldValue(1, 1, (int)p + (8 - cm_FieldObject), &b);
            cm_SendFieldValues();
            ai_Unk6f1b98++;
            G_MSG72(m, p);
            ai_Unk6f1b98++;
            mst = *(char **)(p + 0x16c);
            if (mst != 0 && *(unsigned short *)(mst + 0x27) != 0xffff && (ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 6 || ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 7)) {
                txt_Format(buf, 0x1c51, mst, *c);
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, ch_EmptyName);
                ai_Unk6f1b98++;
            }
        }
        return t;
    }
    fl = (float)*(unsigned short *)(p + 10);
    h = fl;
    if (h > *(float *)(p + 0x20)) {
        lo = *(float *)(p + 0x14);
        if (*(int *)(p + 0x170) != 0) {
            a = gm_BuildingSkill(*(int *)(p + 0x170), 8, 1);
            lo = (float)a * 0.0125f * -3.0f + lo;
        }
        t = (*(float *)(p + 0x1c) - lo) + lo / *(float *)(p + 0x1c);
        cm_BeginFieldBlock(c, *(int *)(p + 4));
        cm_AddFieldDelta(4, 1, 0x1c, &t);
        cm_SendFieldDeltas();
        ai_Unk6f1b98++;
    } else {
        t = ((*(float *)(p + 0x1c) - *(float *)(p + 0x10)) / *(float *)(p + 0x20)) * h + *(float *)(p + 0x10);
    }
    fl = (float)*(int *)(p + 0x24) + t;
    if (fl <= 0.0f) {
        b = 0;
        if ((p[0x1cc] & 1) != 0 && p[2] != 6 && p[2] != 7) {
            d = 0.0;
            t = 100.0f;
            cm_BeginFieldBlock(c, *(int *)(p + 4));
            cm_AddFieldDelta(4, 1, 0x1c, &t);
            cm_AddFieldDelta(4, 1, 0x24, &d);
            cm_SendFieldDeltas();
            ai_Unk6f1b98++;
            cm_SendSetField(*(int *)(p + 4), 0x1cc, 4, 2, 0);
            ai_Unk6f1b98++;
            return t;
        }
        cm_BeginFieldBlock(c, *(int *)(p + 4));
        cm_AddFieldValue(1, 1, (int)p + (8 - cm_FieldObject), &b);
        cm_SendFieldValues();
        ai_Unk6f1b98++;
        G_MSG72(m, p);
        ai_Unk6f1b98++;
        mst = *(char **)(p + 0x16c);
        if (mst != 0 && *(unsigned short *)(mst + 0x27) != 0xffff && (ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 6 || ai_Chars[*(unsigned short *)(mst + 0x27)][2] == 7)) {
            if ((p[0x1cc] & 4) == 0) {
                txt_Format(buf, 0x1c51, mst, *c);
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, ch_EmptyName);
            } else {
                txt_Format(buf, 0x4080, p + 0x30, p + 0x40, *(unsigned short *)(p + 10));
                he_ShowNews(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), -1, buf, 0, 0x60a, "_NACHRICHTEN_DOMIZIL_05");
            }
            ai_Unk6f1b98++;
        }
        return 0.0f;
    }
    unk_Unk4fccc0(c);
    if (fl <= 0.0f)
        d = 0.0;
    else
        d = (double)fl;
    k = *(unsigned short *)(p + 10) + 1;
    cm_BeginFieldBlock(c, *(int *)(p + 4));
    cm_AddFieldDelta(2, 1, 10, &k);
    cm_SendFieldDeltas();
    ai_Unk6f1b98++;
    return (float)d;
}
