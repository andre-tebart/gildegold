/* 0x57fdb0, the cannon's pitch as 0 .. 1 (28 B): (sc + 0x10 + 0.2617994) / (0.7853982 + 0.2617994) */

float ai_Unk57fdb0(char *sc)
{
    return (*(float *)(sc + 0x10) - -0.2617994f) / (0.7853982f - -0.2617994f);
}
