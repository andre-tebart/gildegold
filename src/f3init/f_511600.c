/* 0x511600, in the scene s: for every "ub_FACKEL_" object (0x5115b0 collects them, up to 0x20) the children whose kind is above 4 and whose name contains "rLicht" are handed on (0x449960), then the object is released (0x447190) */


void unk_Unk511600(int s)
{
    int list[33];
    int i, c, next;

    i = 0;
    list[32] = 0;
    d3_ForEachObject(d3_DefaultScene, s, 0x2c0, unk_Unk5115b0, list);
    if (0 < list[32]) {
        do {
            c = *(int *)(list[i] + 0x1fc);
            while (c != 0) {
                next = *(int *)(c + 0x1f0);
                if (*(int *)(c + 0x218) >= 5 && strstr((char *)c, "rLicht") != 0)
                    d3_SetParent(c, s);
                c = next;
            }
            d3if_Delete((struct D3OBJECT *)list[i]);
            i++;
        } while (i < list[32]);
    }
}
