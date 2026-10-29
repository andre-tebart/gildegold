/* 0x453b40, d3_VecAngle wrapped to [-pi, pi) */

float d3_VecAngle2(D3VECTOR *a, D3VECTOR *b)
{
    float r = d3_VecAngle(a, b);

    if (r < -3.1415927f)
        r += 6.2831855f;
    return r;
}
