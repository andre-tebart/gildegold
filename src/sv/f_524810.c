/* 0x524810, like 0x524770 for the position record at +0x3b of g (only when its +0x88 is the table 0xfd7ba0): the radius is 650.0 (0x44228000), 1950.0 (0x44f3c000) for the building class 10, and 3250.0
   (0x454b2000) when the object has no door dummy */

int sv_Unk524810(char *g, unsigned char *o)
{
    float r;
    int dst, d;
    char pos[16];

    r = 650.0f;
    if (ai_BTypes[*o * 0x24d] == 10)
        r = 1950.0f;
    dst = *(int *)(g + 0x3b);
    if (*(int *)(o + 0x61) != 0) {
        if (dst == 0)
            return 0;
        if (*(int *)(dst + 0x88) == (unsigned)&d3_Scenes) {
            d = d3_FindObject(*(int *)(o + 0x61), 0x100, 0, "dummy_TUER");
            if (d == 0) {
                d3_PointToWorld(*(int *)(o + 0x61), *(int *)(o + 0x61) + 0x4c, pos);
                r = 3250.0f;
            } else
                d3_PointToWorld(d, d + 0x4c, pos);
            if (d3_VecEqual(*(int *)(dst + 0x34) + 0x4c, pos, r) != 0)
                return 1;
        }
    }
    if (dst != 0 && *(int *)(dst + 0x2c) == *(int *)(o + 1))
        return 1;
    return 0;
}
