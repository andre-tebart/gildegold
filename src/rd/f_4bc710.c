/* 0x4bc710, 1 if a plant of size cells fits at x / z (a free record, no plant there or in the area) */

int veg_Fits(G15PLANT *p, int x, int z, int size)
{
    int free;
    int i, j, k;

    free = 0;
    for (i = 0; i < 0x40; i++) {
        if (p[i].state == -1)
            free = 1;
        else if (p[i].x == x && p[i].z == z)
            return 0;
    }
    if (!free)
        return 0;
    for (j = 0; j < size; j++) {
        for (k = 0; k < size; k++) {
            if (veg_FindPlantArea(p, x - size / 2 + k, z - size / 2 + j) != 0)
                return 0;
        }
    }
    return 1;
}
