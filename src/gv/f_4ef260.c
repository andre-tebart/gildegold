/* 0x4ef260, two rates of the object o: 0.02 / 0.2 (0.017 / 0.17 with item 0x25c), minus 0.003 / 0.03 with item 0x25f and minus 0.004 / 0.04 with item 0x260 */

void unk_Unk4ef260(char *o, float *a, float *b)
{
    float x, y;

    x = 0.02f;
    y = 0.2f;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x25c) != 0) {
        x = 0.017f;
        y = 0.17f;
    }
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x25f) != 0) {
        x = x - 0.003f;
        y = y - 0.03f;
    }
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x260) != 0) {
        x = x - 0.004f;
        y = y - 0.04f;
    }
    *a = x;
    *b = y;
}
