/* 0x454560, scales v to unit length in place; a zero vector is left as the zero vector */

void d3_Normalize(D3VECTOR4 *v)
{
    float l = (float)sqrt(v->z * v->z + v->y * v->y + v->x * v->x);

    if (l != 0.0f) {
        l = 1.0f / l;
        v->x = l * v->x;
        v->y = l * v->y;
        v->z = l * v->z;
    } else {
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
    }
}
