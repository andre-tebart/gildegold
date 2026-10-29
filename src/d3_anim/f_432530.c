/* 0x432530, spline tangents at k1 from the mean slope of the segments k0-k1 and k1-k2. */

void d3path_Tangents(D3PATHKEY *k0, D3PATHKEY *k1, D3PATHKEY *k2)
{
    D3VECTOR4 a, b;
    float t, f;

    a.x = k1->pos.x - k0->pos.x;
    a.y = k1->pos.y - k0->pos.y;
    a.z = k1->pos.z - k0->pos.z;
    f = 1.0f / k0->time;
    a.x *= f;
    a.y *= f;
    a.z *= f;
    b.x = k2->pos.x - k1->pos.x;
    b.y = k2->pos.y - k1->pos.y;
    b.z = k2->pos.z - k1->pos.z;
    t = (float)k1->time;
    f = 1.0f / t;
    b.x *= f;
    b.y *= f;
    b.z *= f;
    a.x = b.x + a.x;
    a.y = b.y + a.y;
    a.z = b.z + a.z;
    a.x *= 0.5f;
    a.y *= 0.5f;
    a.z *= 0.5f;
    k1->posin.x = t * a.x;
    k1->posin.y = t * a.y;
    k1->posin.z = t * a.z;
    f = (float)k0->time;
    k0->posout.x = f * a.x;
    k0->posout.y = f * a.y;
    k0->posout.z = f * a.z;

    a.x = k1->rot.x - k0->rot.x;
    a.y = k1->rot.y - k0->rot.y;
    a.z = k1->rot.z - k0->rot.z;
    f = 1.0f / f;
    a.x *= f;
    a.y *= f;
    a.z *= f;
    b.x = k2->rot.x - k1->rot.x;
    b.y = k2->rot.y - k1->rot.y;
    b.z = k2->rot.z - k1->rot.z;
    t = (float)k1->time;
    f = 1.0f / t;
    b.x *= f;
    b.y *= f;
    b.z *= f;
    a.x = b.x + a.x;
    a.y = b.y + a.y;
    a.z = b.z + a.z;
    a.x *= 0.5f;
    a.y *= 0.5f;
    a.z *= 0.5f;
    k1->rotin.x = t * a.x;
    k1->rotin.y = t * a.y;
    k1->rotin.z = t * a.z;
    f = (float)k0->time;
    k0->rotout.x = f * a.x;
    k0->rotout.y = f * a.y;
    k0->rotout.z = f * a.z;
}
