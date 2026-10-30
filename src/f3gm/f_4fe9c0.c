/* 0x4fe9c0, frees the buildings of the group (1, 6) and the four tables (building prototypes, building instances, object prototypes, object instances) */

void gm_Free(void)
{
    int o;

    o = gm_BuildQuery(1, 6);
    while (o != 0) {
        gm_FreeBuilding(o);
        o = gm_BuildNext();
    }
    if (ai_BTypes != 0) {
        mem_Free(ai_BTypes);
        ai_BTypes = 0;
    }
    if (ai_Blds != 0) {
        mem_Free(ai_Blds);
        ai_Blds = 0;
    }
    if (ai_Goods != 0) {
        mem_Free(ai_Goods);
        ai_Goods = 0;
    }
    if (ai_ObjectsV != 0) {
        mem_Free(ai_ObjectsV);
        ai_ObjectsV = 0;
    }
}
