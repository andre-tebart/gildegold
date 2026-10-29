/* 0x45cd60, d3_ForEachObject callback: frees the shadows of obj cast by light */

char d3s_FreeLightCB(D3OBJECT *obj, D3OBJECT *light)
{
    int i;

    if ((obj->objectFlags & 4) && obj->material) {
        for (i = 0; i < 4; i++) {
            if (D3S_SLOTS(obj->material)[i].tex && D3S_SLOTS(obj->material)[i].light == light) {
                d3s_FreeSlot(&D3S_SLOTS(obj->material)[i]);
                D3S_SLOTS(obj->material)[i].light = 0;
                D3S_SLOTS(obj->material)[i].dir.x = 0;
                D3S_SLOTS(obj->material)[i].dir.y = 0;
                D3S_SLOTS(obj->material)[i].dir.z = 0;
            }
        }
    }
    return 1;
}
