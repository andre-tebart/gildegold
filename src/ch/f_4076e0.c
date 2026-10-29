/* 0x4076e0, height map cell nearest to pos */

int ch_PosToCell(D3SM *h, D3VECTOR *pos, int *x, int *z)
{
    D3VECTOR4 d;
    float f;

    d.x = pos->x - h->origin.x;
    d.z = pos->z - h->origin.z;
    f = 1.0f / h->cell.x;
    d.x *= f;
    d.z *= f;
    *x = (int)(d.x + 0.5);
    *z = (int)(d.z + 0.5);
    return 1;
}
