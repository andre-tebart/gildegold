/* 0x524770, 1 when the object o stands at the door of the group record g's destination (+0x184): the door position of o (the dummy "TUER", 0x449690) is within 0x2eee (0x443b8000 = 750.0) of the destination's
   position (+0x34 + 0x4c) and the destination is not busy (+0x128); or when the destination's owner (+0x2c) is o's id */

int sv_Unk524770(char *g, char *o)
{
    int dst, d;
    char pos[16];

    dst = *(int *)(g + 0x184);
    if (*(int *)(o + 0x61) != 0) {
        if (dst == 0)
            return 0;
        d = d3_FindObject(*(int *)(o + 0x61), 0x100, 0, "dummy_TUER");
        if (d == 0)
            d = *(int *)(o + 0x61);
        d3_PointToWorld(d, d + 0x4c, pos);
        if (d3_VecEqual(*(int *)(dst + 0x34) + 0x4c, pos, 750.0f) != 0 && *(int *)(dst + 0x128) == 0)
            return 1;
    }
    if (dst != 0 && *(int *)(dst + 0x2c) == *(int *)(o + 1))
        return 1;
    return 0;
}
