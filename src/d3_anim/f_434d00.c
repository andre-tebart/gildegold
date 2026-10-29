/* 0x434d00, the largest vector length. */

float d3anim_MaxRadius(D3VECTOR4 *v, int n)
{
    float r = -1.0f;
    float d;

    int i;
    for (i = 0; i < n; i++) {
        d = v[i].x * v[i].x + v[i].y * v[i].y + v[i].z * v[i].z;
        r = max(r, d);
    }
    return (float)sqrt(r);
}
