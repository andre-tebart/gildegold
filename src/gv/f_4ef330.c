/* 0x4ef330, a factor of the object o: 1.0 (1.05 with item 0x259), plus 0.05 with item 0x25b and plus 0.05 with item 0x261 */

float unk_Unk4ef330(char *o)
{
    float x;

    x = 1.0f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x259) != 0)
        x = 1.05f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x25b) != 0)
        x = x + 0.05f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x261) != 0)
        return x + 0.05f;
    return x;
}
