/* 0x49c570, decide callback 0x25: spying on a character (0x49c700 sends "spionage"), if the character can pay for it */

int ai_Unk49c570(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned int flags;
    unsigned short id;
    int r, money, v, x;
    G5CH *c;

    flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
    r = util_Random(3);
    money = gm_Unk509ee0((AIPER *)p, ai_OwnPlayer);
    if (f != 0)
        return 0;
    if (ai_Unk4a58d0((AIPER *)p, 1, (int *)&flags, 0, (float)util_Random(0x19) + 10.0, &id) == 0)
        return 0;
    c = &G5_CHARS[id];
    x = (int)(gm_GetWorth(c->id) * 0.005f + gm_GetWorth(p->id) * 0.005f);
    v = (int)(x * ((float)r * 0.33f + 1.0f));
    if (!((float)v <= money * 0.22f))
        return 0;
    if (he_FindEvent(2, 0, 0x18, 2, (int)p->id) != 0)
        return 0;
    o1->kind = 7;
    o1->unk04 = c->netId;
    o1->unk08 = 1;
    o1->unk10 = v;
    o2->kind = 0;
    o2->unk04 = 0;
    o2->unk08 = 0;
    o2->unk10 = 0;
    return 0x25;
}
