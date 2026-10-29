/* 0x45ccd0, frees the four shadow slots of obj and forgets their lights and directions */

void d3s_FreeShadows(D3OBJECT *obj)
{
    int i;

    if ((obj->objectFlags & 4) && obj->material) {
        for (i = 0; i < 4; i++) {
            if (D3S_SLOTS(obj->material)[i].tex) {
                d3s_FreeSlot(&D3S_SLOTS(obj->material)[i]);
                D3S_SLOTS(obj->material)[i].light = 0;
                D3S_SLOTS(obj->material)[i].dir.x = 0;
                D3S_SLOTS(obj->material)[i].dir.y = 0;
                D3S_SLOTS(obj->material)[i].dir.z = 0;
            }
        }
    }
}
