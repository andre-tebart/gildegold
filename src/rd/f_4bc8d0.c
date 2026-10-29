/* 0x4bc8d0, the plants grow one state (their objects are rebuilt by 0x4bcb90) */

void veg_GrowPlants(G15PLANT *p)
{
    int i;
    G15TREE *t;

    for (i = 0; i < 0x40; i++) {
        if (p[i].state != -1) {
            if (p[i].obj != 0) {
                d3if_Delete((struct D3OBJECT *)p[i].obj);
                p[i].obj = 0;
            }
            t = veg_FindType(p[i].type);
            if (t == 0)
                t = veg_FindType(p[i].type);
            if (p[i].state < t->lastState)
                p[i].state++;
        }
    }
    veg_ShowPlants(p);
}
