/* 0x453e80, all components within eps */

int d3_VecEqual(D3VECTOR *a, D3VECTOR *b, float eps)
{
    if (fabs(b->x - a->x) <= eps && fabs(b->y - a->y) <= eps && fabs(b->z - a->z) <= eps)
        return 1;
    return 0;
}
