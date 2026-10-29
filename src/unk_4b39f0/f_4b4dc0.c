/* 0x4b4dc0, mission condition (from year 10): 0x4b5160 with "t->goal is not below value 0x18 of 0x4f9e00" */

typedef struct G13ECON {                /* 0x28 bytes, filled by 0x4f9e00 */
    float unk00;
    float unk04;                        /* 0x04 */
    float unk08;                        /* 0x08 */
    float unk0c;                        /* 0x0c */
    char pad10[8];
    float unk18;                        /* 0x18 */
    char pad1c[0x0c];
} G13ECON;

struct VFSFILE; float unk_Unk4f9e00(int *o);                                                /* 0x4f9e00, defined elsewhere */

int ai_Unk4b4dc0(AICHAR *c, int x, G13TYPE *t)
{
    G13ECON e;

    if (ai_Time.year < 10)
        return 0;
    unk_Unk4f9e00(&e);
    if (t->goal < e.unk18)
        return ai_TimedCondition(x, 0, t->unk18);
    return ai_TimedCondition(x, 1, t->unk18);
}
