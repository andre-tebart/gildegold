/* 0x458910, angles of the object's world matrix */

void d3_WorldAngles(XOBJ *obj, D3VECTOR4 *angles)
{
    D3MATRIX m;

    d3_WorldMatrix(obj, (obj->flags & 0x80) ? 0 : (XOBJ *)d3_Camera, 1, &m);
    d3_MatToAngles((D3VECTOR4 *)&m, angles);
}
