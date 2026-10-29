/* 0x455a20, moves the position of the system's embedded object (the particles are placed relative to it) */

void par_SetPos(D3PARTSYS *ps, D3VECTOR *pos)
{
    if (ps)
        d3_SetObjectPos((D3OBJECT *)ps->obj, pos);
}
