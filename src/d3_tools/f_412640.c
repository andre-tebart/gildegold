/* 0x412640, does the triangle's bounding box overlap the box a..b in x/z and reach below b.y? */

int d3t_InTriBox(D3VECTOR **tri, D3VECTOR *a, D3VECTOR *b)
{
    D3VECTOR4 mn, mx;

    mn.x = min(min(tri[0]->x, tri[1]->x), tri[2]->x);
    mn.y = min(min(tri[0]->y, tri[1]->y), tri[2]->y);
    mn.z = min(min(tri[0]->z, tri[1]->z), tri[2]->z);
    mx.x = max(max(tri[0]->x, tri[1]->x), tri[2]->x);
    mx.y = max(max(tri[0]->y, tri[1]->y), tri[2]->y);
    mx.z = max(max(tri[0]->z, tri[1]->z), tri[2]->z);
    if (mx.x < a->x || mn.x > b->x || mn.y > b->y || mx.z < a->z || mn.z > b->z)
        return 0;
    return 1;
}
