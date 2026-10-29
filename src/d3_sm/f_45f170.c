/* 0x45f170, cell of pos and (h != 0) the interpolated height there @deep behaviour: 18 code + 6 order hunks from two scheduling differences only, the whole asmdiff read (28 lines): (1) our fdiv [esi+0x18] is emitted before and ebx,eax / imul ebx,edx, the original after them; (2) around the second corner height the original pops ebp and loads 'mov al,[edi+ecx]' earlier and takes the int temp from [esp+0x14] where we use [esp+0x18] - the same local, other slot, the same byte values and the same interpolation (y + dz*(top-y) + dx*(right-y)) in both; no statement missing, readability already readable; needs FPU scheduling / slot work */

char d3_GetHeightAt(D3SM *h, D3VECTOR *pos, int *px, int *pz, float *py)
{
    int x, z;
    float y;

    if (h) {
        x = (int)((pos->x - h->origin.x) / h->cell.x);
        z = (int)((pos->z - h->origin.z) / h->cell.z);
        if (x >= 0 && x < h->size && z >= 0 && z < h->size && px && pz) {
            *px = x;
            *pz = z;
            if (py) {
                y = (h->data[h->size * z + x] + 0.5f) * h->cell.y + h->origin.y;
                *py = ((h->data[(z + 1 & h->size - 1) * h->size + x] + 0.5f) * h->cell.y + h->origin.y - y)
                        * ((pos->z - (z * h->cell.z + h->origin.z)) / h->cell.z)
                    + ((h->data[h->size * z + (x + 1 & h->size - 1)] + 0.5f) * h->cell.y + h->origin.y - y)
                        * ((pos->x - (x * h->cell.x + h->origin.x)) / h->cell.x)
                    + y;
            }
            return 1;
        }
    }
    return 0;
}
