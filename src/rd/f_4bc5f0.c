/* 0x4bc5f0, the plant at cell x / z, 0 if none */

G15PLANT *veg_FindPlantAt(G15PLANT *p, int x, int z)
{
    int i;

    for (i = 0; i < 0x40; i++) {
        if (p[i].x == x && p[i].z == z && p[i].state != -1)
            return &p[i];
    }
    return 0;
}
