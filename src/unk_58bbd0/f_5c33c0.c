/* 0x5c33c0, the master letter eligibility test (992 B): 0 when param_2 (the index) is param_1, has no
   record, is the +0x1b1 flagged character, is the own character 0x6f1bf8 or leaves the filter record
   param_3 (+4 the mask, +0x14 bit 2 with the four +0x1c ids, +0x40 .. +0x48 the relation range 0x50c550).
   The mask bits: 1 / 2 the +9 byte, 4 / 8 the +0x50 / +0x5c pair against param_1's, 0x10 the +0x165 byte
   with kind 0, 0x20 the +0x1c8 0x40000000, 0x40 kind 1, 0x80 the +0x1f0 int, 0x100 the +0x166 / +0x169
   bytes outside the kinds 5 / 6 / 7, 0x200 / 0x400 0x4fd7e0, 0x800 the +0x166 / +0x169 pair, 0x1000 the
   +0x5c handle over 0x505df0, 0x2000 the +0x184 int, 0x4000 the kinds 5 / 6 / 7, 0x8000 the same, 0x10000
   the +0x170 int, 0x20000 the +0x1c8 0x800000, 0x40000 the +0x1cc 0x3000, 0x100000 kinds 6 / 7, 0x200000
   kind 5, 0x400000 kind != 5 with 0x47dd70, 0x800000 kind 0 (one of the four must hold) */

int unk_Unk4fd7e0(); int ai_WorksForMaster();

int ai_Unk5c33c0(unsigned short param_1, unsigned short param_2, unsigned int *param_3, int param_4)
{
    char *rec;
    char *other;
    char *p3;
    unsigned int idx;
    unsigned int u;
    unsigned char kind;
    unsigned char c;
    int v;
    int i;
    int b2;
    int b3;
    int b4;
    int b5;
    float f;

    p3 = (char *)param_3;
    if (param_1 == (unsigned short)param_2)
        return 0;
    idx = param_2 & 0xffff;
    rec = (char *)(unsigned)&ai_Chars + idx * 0x21c;
    other = (char *)(unsigned)&ai_Chars + param_1 * 0x21c;
    if (*(unsigned short *)rec == 0xffff)
        return 0;
    if (*(unsigned char *)(rec + 0x1b1) != 0)
        return 0;
    if (rec == (char *)U_I((unsigned)&ai_PlayerChar))
        return 0;
    if (p3 == 0)
        return 1;
    u = *(unsigned int *)(p3 + 4);
    if (u == 0)
        return 1;
    kind = *(unsigned char *)(rec + 2);
    if (kind > 9)
        return 0;
    if (*(unsigned char *)(p3 + 0x14) & 2) {
        for (i = 0; i < 4; i++) {
            if ((unsigned short)param_2 == *(unsigned short *)(p3 + 0x1c + i * 2) &&
                *(unsigned short *)(p3 + 0x1c + i * 2) != 0xffff)
                return 0;
        }
    }
    if ((u & 0x3ffff) != 0) {
        if ((u & 1) && *(unsigned char *)(rec + 9) == 1)
            return 0;
        if ((u & 2) && *(unsigned char *)(rec + 9) == 0)
            return 0;
        if ((u & 0x1000) && U_I(rec + 0x5c) != -1 &&
            (v = gm_FindChar(U_I(rec + 0x5c))) != 0 && *(char *)(v + 8) != 0)
            return 0;
        u = *(unsigned int *)(p3 + 4);
        if ((u & 4) && U_I(rec + 0x50) != U_I(other + 0x50) && U_I(rec + 0x5c) != U_I(other + 4))
            return 0;
        if (u & 8) {
            if (U_I(rec + 0x50) == U_I(other + 0x50))
                return 0;
            if (U_I(rec + 0x5c) == U_I(other + 4))
                return 0;
        }
        if ((u & 0x40) && kind != 1)
            return 0;
        if (u & 0x10) {
            if (*(unsigned char *)(rec + 0x165) == 0)
                return 0;
            if (kind != 0)
                return 0;
        }
        if ((u & 0x100) && kind != 6 && kind != 7 &&
            (kind != 5 && *(unsigned char *)(rec + 0x166) == 0) &&
            *(unsigned char *)(rec + 0x169) == 0)
            return 0;
        if ((u & 0x200) && unk_Unk4fd7e0(param_2) == 0)
            return 0;
        if ((u & 0x400) && unk_Unk4fd7e0(param_2) != 0)
            return 0;
        u = *(unsigned int *)(p3 + 4);
        if ((u & 0x2000) && U_I(rec + 0x184) != 0)
            return 0;
        if (u & 0x4000) {
            c = kind;
            if (c == 6 || c == 7 || c == 5)
                return 0;
        }
        if ((char)(u >> 8) < 0 && kind != 6 && kind != 7 && kind != 5)
            return 0;
        if ((u & 0x800) && *(unsigned char *)(rec + 0x166) == 0 &&
            *(unsigned char *)(rec + 0x169) == 0)
            return 0;
        if ((u & 0x10000) && U_I(rec + 0x170) == 0)
            return 0;
        if ((u & 0x20000) && (U_I(rec + 0x1c8) & ((unsigned)&ai_Relations + 0x1aca0)) != 0)
            return 0;
        if ((u & 0x20) && (U_I(rec + 0x1c8) & 0x40000000) != 0)
            return 0;
        if ((char)u < 0 && U_I(rec + 0x1f0) == 0)
            return 0;
        if ((u & 0x40000) == 0 && (U_I(rec + 0x1cc) & 0x3000) != 0)
            return 0;
    }
    if ((u & ((unsigned)&ai_Unka69140 + 0x496ec0)) != 0) {
        b4 = (u & 0x100000) != 0 && (kind == 6 || kind == 7);
        b5 = (u & 0x200000) != 0 && kind == 5;
        b3 = (u & 0x400000) != 0 && kind != 5 && ai_WorksForMaster((int)rec) != 0;
        b2 = (u & ((unsigned)&ai_Relations + 0x1aca0)) != 0 && kind == 0;
        if (!b2 && !b4 && !b5 && !b3)
            return 0;
    }
    if (*(int *)(p3 + 0x40) != 0) {
        f = gm_Liking(param_1, idx, 1);
        if (f < *(float *)(p3 + 0x44))
            return 0;
        if (*(float *)(p3 + 0x48) < f)
            return 0;
    }
    return 1;
}
