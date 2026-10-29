/* 0x454700, normal of the triangle a, b, c */

void d3_TriNormal(D3VECTOR4 *a, D3VECTOR4 *b, D3VECTOR4 *c, D3VECTOR4 *n)
{
    D3VECTOR4 u, v;

    u.x = b->x - a->x; u.y = b->y - a->y; u.z = b->z - a->z;
    v.x = c->x - a->x; v.y = c->y - a->y; v.z = c->z - a->z;
    n->x = v.z * u.y - v.y * u.z;
    n->y = v.x * u.z - v.z * u.x;
    n->z = v.y * u.x - v.x * u.y;
    d3_Normalize(n);
}
