/* 0x45efc0, the cell nearest to (x, z) (in rings of growing distance) with a real ground type */

char d3sm_FindGround(D3SM *h, int x, int z, int *ox, int *oz)
{
    int r = 0, xx, zz, x1, z1, w;

    if (h && h->entries && ox && oz && h->size > 0) {
        do {
            z1 = min(h->size - 1, z + r);
            for (zz = max(0, z - r); zz <= z1; zz++) {
                w = r - abs(z - zz);
                x1 = min(h->size - 1, x + w);
                for (xx = max(0, x - w); xx <= x1; xx++) {
                    if (zz >= 0 && zz < h->size && xx >= 0 && xx < h->size
                        && h->entries[zz * h->size + xx].type != 0 && h->entries[zz * h->size + xx].type != 0xd) {
                        *ox = xx;
                        *oz = zz;
                        return 1;
                    }
                }
            }
            r++;
        } while (r < h->size);
    }
    return 0;
}
