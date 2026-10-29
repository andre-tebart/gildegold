/* 0x54fc70, a Ghidra miss: creates the effect 0x178 on the scene record of the object p (like 0x54fc00, other colour 0x321e50 and size) */

int he_Unk54fc70(int o)
{
    unsigned char col[4];
    int v[3];

    col[0] = 0x32;
    col[1] = 0x1e;
    col[2] = 0x50;
    v[0] = 0;
    v[1] = 0x42480000;
    v[2] = 0;
    return d3t_CreateEmitter(*(int *)(o + 0x61), "punkte_nm_2", 1, *(int *)col, v, 0x12c, 1, 0x42480000, 0x3f800000, 0x40000000, 0x3f000000, 0x3f000000);
}
