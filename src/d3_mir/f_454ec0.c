/* 0x454ec0, d3_Mirror.Transform: mirrors camera-space points at the mirror plane and projects them again */

void d3_MirTransform(D3PVERT *v, int n, int unk)
{
    int i;
    float d, z;

    for (i = 0; i < n; i++, v++) {
        d = (d3_Mirror.plane.x * v->x + d3_Mirror.plane.y * v->y + d3_Mirror.plane.z * v->z - d3_Mirror.plane.w) * -2.0f;
        v->x += d * d3_Mirror.plane.x;
        v->y += d * d3_Mirror.plane.y;
        v->z += d * d3_Mirror.plane.z;
        v->rhw = d;
        z = 1.0f / v->z;
        v->sx = d3_ScaleX * v->x * z + d3_CenterX;
        v->sy = d3_ScaleY * v->y * z + d3_CenterY;
    }
}
