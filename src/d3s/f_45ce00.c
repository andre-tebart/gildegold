/* 0x45ce00 */

void d3s_ResetShadows(D3OBJECT *obj)
{
    int i;

    if ((obj->objectFlags & 4) && obj->material) {
        for (i = 0; i < 4; i++) {
            if (D3S_SLOTS(obj->material)[i].tex) {
                D3S_SLOTS(obj->material)[i].dir.x = 0;
                D3S_SLOTS(obj->material)[i].dir.y = 0;
                D3S_SLOTS(obj->material)[i].dir.z = 0;
            }
        }
    }
}
