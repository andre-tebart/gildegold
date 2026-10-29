/* 0x455a40, orientation from a direction and a roll angle */

void par_SetDir(D3PARTSYS *ps, D3VECTOR *dir, float roll)
{
    D3MATRIX m;
    D3VECTOR rot;

    if (ps) {
        d3_MakeBasis(dir, roll, &m);
        d3_MatToAngles((D3VECTOR4 *)&m, (D3VECTOR4 *)&rot);
        d3_SetObjectRot((D3OBJECT *)ps->obj, &rot);
    }
}
