/* 0x4ef170, two levels of the object o by the items in its list (0x4ffe90 (+0x5d, 2, 6, 0, id)): *a = 2 with item 0x265, else 1 with 0x263, else 0; *b = 4 with 0x264, 3 with 0x262, 2 with 0x25e, 1 with 0x25a, else 0 */

void unk_Unk4ef170(char *o, unsigned *a, unsigned *b)
{
    *b = 0;
    *a = 0;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x265) == 0)
        *a = (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x263) != 0);
    else
        *a = 2;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x264) != 0) {
        *b = 4;
        return;
    }
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x262) != 0) {
        *b = 3;
        return;
    }
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x25e) != 0) {
        *b = 2;
        return;
    }
    *b = (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x25a) != 0);
}
