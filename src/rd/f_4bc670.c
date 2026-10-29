/* 0x4bc670, the plant with id id, 0 if none */

G15PLANT *veg_FindPlantId(G15PLANT *p, int id)
{
    int i;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1 && p[i].id == id)
            return &p[i];
    }
    return 0;
}
