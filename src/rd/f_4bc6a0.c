/* 0x4bc6a0, the plant whose area (size cells around it) covers cell x / z, 0 if none */

G15PLANT *veg_FindPlantArea(G15PLANT *p, int x, int z)
{
    int i;
    int r;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1) {
            r = p[i].size / 2;
            if (x >= p[i].x - r && x <= p[i].x + r && z >= p[i].z - r && z <= p[i].z + r)
                return &p[i];
        }
    }
    return 0;
}
