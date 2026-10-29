/* 0x49a020, decide: action 0x19 for a character with flag 0x4000 and little money */

int ai_Unk49a020(int a, AIPER *p, int b, AIARG *o1, AIARG *o2)
{
    int m;

    if (b != 1) {
        if (b != 0)
            return 0;
    } else if (o1->kind != 0xd || o1->unk04 != 2)
        return 0;
    {
        if (p->unk00d < 2 && (p->flags1c8 & 0x4000)) {
            m = gm_Unk509ee0(p, ai_OwnPlayer);
            if (m >= 0 && !(m * 0.44f < 16000.0f)) {
                o1->kind = 0xd;
                o1->unk04 = 2;
                o1->unk08 = 1;
                return 0x19;
            }
        }
    }
    return 0;
}
