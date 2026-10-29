/* 0x404ed0, writes the saved ground types of the changed rectangle back into the height map */

void ch_RestoreGround(struct D3HMAP *hmap)
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
    ch_DirtyX0 = max(0, ch_DirtyX0);
    if (h->size < ch_DirtyX1)
        ch_DirtyX1 = h->size;
    ch_DirtyZ0 = max(0, ch_DirtyZ0);
    if (h->size < ch_DirtyZ1)
        ch_DirtyZ1 = h->size;
    for (i = ch_DirtyX0; i < ch_DirtyX1; i++)
        for (j = ch_DirtyZ0; j < ch_DirtyZ1; j++)
            h->entries[h->size * j + i].type = buf[h->size * j + i];
    ch_DirtyX0 = h->size;
    ch_DirtyX1 = 0;
    ch_DirtyZ0 = h->size;
    ch_DirtyZ1 = 0;
}
