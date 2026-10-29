/* 0x54fc00, a Ghidra miss: creates the effect 0x177 on the scene record of the object p (0x415ad0 (scene, 0x5f36ec, 1, colour 0x64ffa0.., .., 0x12c, 1, ..)); returns its handle (ROUGH, the float arguments are raw) */

int he_Unk54fc00(int o)
{
    unsigned char col[4];
    int v[3];

    col[0] = 0x64;
    col[1] = 0xff;
    col[2] = 0xa0;
    v[0] = 0;
    v[1] = 0x43480000;
    v[2] = 0;
    return d3t_CreateEmitter(*(int *)(o + 0x61), "Rauch", 1, *(int *)col, v, 0x12c, 1, 0x41f00000, 0x40000000, 0x40f00000, 0x3ecccccd, 0x3f800000);
}
