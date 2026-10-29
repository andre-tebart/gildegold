/* 0x4bcb90, shows the plants (loading missing models) */

void veg_ShowPlants(G15PLANT *p)
{
    int i;

    if (p == 0)
        return;
    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1) {
            if (p[i].obj == 0) {
                veg_LoadModel(&p[i], 0);
                if (p[i].obj == 0)
                    continue;
            }
            d3if_ShowTree(p[i].obj, 1);
        }
    }
}
