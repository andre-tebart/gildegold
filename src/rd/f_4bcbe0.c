/* 0x4bcbe0, hides the plants */

void veg_HidePlants(G15PLANT *p)
{
    int i;

    if (p == 0)
        return;
    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1 && p[i].obj != 0)
            d3if_ShowTree(p[i].obj, 0);
    }
}
