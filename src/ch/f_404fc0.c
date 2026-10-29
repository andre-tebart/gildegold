/* 0x404fc0, sets the ground type of a diamond of radius r around (x, z) */

int ch_SetGroundDiamond(int x, int z, unsigned char scene, int type, int r)
{
    D3SM *h;
    int i = 0;
    int d, xx, zz, x1, z1;

    h = (D3SM *)d3_Scenes[scene].hmap;
    if (d3_Scenes[0].hmap != (struct D3HMAP *)h && d3_DefaultScene->hmap != (struct D3HMAP *)h)
        return 0;
    if (!h)
        return 0;
    for (; i < h->size; i++) {
        if (i >= r)
            break;
        z1 = min(h->size - 2, z + i);
        for (zz = max(1, z - i); zz <= z1; zz++) {
            d = i - abs(z - zz);
            x1 = min(h->size - 2, x + d);
            for (xx = max(1, x - d); xx <= x1; xx++)
                if (zz >= 1 && zz < h->size - 1 && xx >= 1 && xx < h->size - 1)
                    h->entries[h->size * zz + xx].type = type;
        }
    }
    if (x - r < ch_DirtyX0)
        ch_DirtyX0 = x - r;
    if (x + r > ch_DirtyX1)
        ch_DirtyX1 = x + r + 1;
    if (z - r < ch_DirtyZ0)
        ch_DirtyZ0 = z - r;
    if (z + r > ch_DirtyZ1)
        ch_DirtyZ1 = z + r + 1;
    return 1;
}
