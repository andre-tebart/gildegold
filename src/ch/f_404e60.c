/* 0x404e60, saves the ground types of a scene's height map */

void ch_SaveGround(struct D3HMAP *hmap)
{
    D3SM *h = (D3SM *)hmap;
    int *buf;
    int i, j;

    if (d3_Scenes[0].hmap == hmap)
        buf = ch_Ground0;
    else if (d3_DefaultScene->hmap == hmap)
        buf = ch_Ground1;
    else
        return;
    for (i = 0; i < h->size; i++)
        for (j = 0; j < h->size; j++)
            buf[h->size * j + i] = h->entries[h->size * j + i].type;
}
