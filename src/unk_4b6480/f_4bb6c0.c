/* 0x4bb6c0, sends message m (from the 8th day on): -1 without m, 0x400 without its character or with unk1b1 set,
   -2 for an unknown type */

int ai_Unk4bb6c0(G14MSG *m)
{
    AICHAR *c;

    if (ai_Time.year < 8)
        return 0;
    if (m == 0)
        return -1;
    c = gm_FindChar(m->id);
    if (c == 0 || c->unk1b1 > 0)
        return 0x400;
    if (m->type < 0x45)
        return ai_Unk602570[m->type](m);
    return -2;
}
