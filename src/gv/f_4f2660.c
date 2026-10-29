/* 0x4f2660, a rate of the object o: 30 (item 0x20f) + 10 (0x21f) + 15 (0x222) + 25 (0x223), times 0.0125 * 0.5 + 0.5 */

float unk_Unk4f2660(char *o)
{
    float x;

    x = 0.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x20f) != 0)
        x = 30.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x21f) != 0)
        x = x + 10.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x222) != 0)
        x = x + 15.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x223) != 0)
        x = x + 25.0f;
    return x * 0.0125f * 0.5f + 0.5f;
}
