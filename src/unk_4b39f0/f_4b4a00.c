/* 0x4b4a00, mission condition: 0x4b51f0 values 0x15, 0x14 and 0x12 of c: at least 3 each, all reaching 1 */

int ai_Unk4b4a00(AICHAR *c, G13TYPE *t)
{
    G13STAT s;

    ai_CharBuildingShare(c, 0x15, &s);
    if (s.n < 3 || s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 0x14, &s);
    if (s.n < 3 || s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 0x12, &s);
    if (s.n < 3 || s.v < 1.0f)
        return 0;
    return 1;
}
