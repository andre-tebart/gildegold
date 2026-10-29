/* 0x4497f0, converts a world-space position and rotation into the coordinate system of the object's parent
   (the parent's inverse world matrix applied to `pos`, `rot` multiplied by the transposed parent matrix); an
   object without parent copies the position and takes the angles of `rot` */

void d3_WorldToParent(D3OBJECT *obj, float *pos, D3MATRIX *rot, D3VECTOR *outpos, D3VECTOR *outrot)
{
    D3OBJECT *parent;
    D3MATRIX m, w;

    parent = obj->parent;
    if (!parent) {
        outpos->x = pos[0];
        outpos->y = pos[1];
        outpos->z = pos[2];
        d3_MatToAngles((D3VECTOR4 *)rot, (D3VECTOR4 *)outrot);
        return;
    }
    d3_WorldMatrix(parent, 0, 0, &m);
    d3_MatInverse(&m, &w);
    outpos->x = w._31 * pos[2] + w._11 * pos[0] + w._21 * pos[1] + w._41;
    outpos->y = w._32 * pos[2] + w._12 * pos[0] + w._22 * pos[1] + w._42;
    outpos->z = w._33 * pos[2] + w._13 * pos[0] + w._23 * pos[1] + w._43;
    d3_WorldMatrix(parent, 0, 1, &m);
    d3_MatTranspose(&m, &w);
    d3_MatMul43(rot, &w, &m);
    d3_MatToAngles((D3VECTOR4 *)&m, (D3VECTOR4 *)outrot);
}
