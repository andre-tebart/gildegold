/* 0x4c9280, handler of type 0x27: a cutscene object from the joined message (0x114 bytes) */

int ai_Unk4c9280(G17MSG *m, G17STAT *s)
{
    char buf[0x114];
    char *o;
    int ret = 0;

    memcpy(buf, ai_Unka68d40, 0x114);
    if (ai_Unk1460ae0 != -1)
        ai_Unk61a658 = G17_I(*m, 0);
    o = ai_Unk4e5150(buf);
    if (o != 0) {
        if (ai_GameFlags & 4)
            o[0x26] |= 0x80;
        *(int *)(o + 0x78) = G17_I(*m, 4);
        if (s != 0) {
            s->state = 1;
            s->unk01 = 9;
            s->value = (int)o;
        }
    } else
        ret = 1;
    if (o[8] == 5) {
        if (ai_CharInts[ai_PlayerId][0] != *(int *)(o + 0x34) && !(ai_GameFlags & 4))
            o[0x26] &= 0x7f;
    }
    ai_Unk61a658++;
    return ret;
}
