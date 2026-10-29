/* 0x44d1c0, finishes a loaded mesh: always computes its bounds (radius, diagonal, centre, box corners),
   and its normals when bounds is set; returns nothing */

void d3iot_Finish(D3BGF *m, char bounds)
{
    d3iot_CalcBounds(m);
    if (bounds)
        d3_CalcNormals(m);
}
