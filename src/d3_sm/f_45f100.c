/* 0x45f100, world position of map vertex (x, z) */
char d3sm_GetVertex(D3SM *h, int x, int z, D3VECTOR *out)
{
    if (h && x >= 0 && x < h->size && z >= 0 && z < h->size) {
        out->x = x * h->cell.x + h->origin.x;
        out->y = h->data[h->size * z + x] * h->cell.y + h->origin.y;
        out->z = z * h->cell.z + h->origin.z;
        return 1;
    }
    return 0;
}
