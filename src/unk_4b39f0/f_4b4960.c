/* 0x4b4960, mission condition: 0x4b51f0 values 0x13, 4 and 0x10 of c reach 1 (4 / 0x10 only if present) */

int ai_Unk4b4960(AICHAR *c, G13TYPE *t)
{
    G13STAT s;

    ai_CharBuildingShare(c, 0x13, &s);
    if (s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 4, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    ai_CharBuildingShare(c, 0x10, &s);
    if (s.n > 0 && s.v < 1.0f)
        return 0;
    return 1;
}
