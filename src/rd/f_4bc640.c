/* 0x4bc640, the plant with 3D object obj, 0 if none */

G15PLANT *veg_FindPlantObject(G15PLANT *p, char *obj)
{
    int i;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1 && p[i].obj == obj)
            return &p[i];
    }
    return 0;
}
