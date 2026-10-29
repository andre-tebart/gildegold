/* 0x4539f0, signed angle around the y axis from a to b (xz plane) */

float d3_VecAngle(D3VECTOR *a, D3VECTOR *b)
{
    D3VECTOR4 va, vb, nb;
    float c[3];

    va.x = a->x;
    va.z = a->z;
    va.y = 0.0f;                     /* dead store: ranks va.y before vb.y for VC6's operand order in the acos dot product (matching trick) */
    vb.y = 0.0f;
    va.y = 0.0f;
    vb.x = b->x;
    vb.z = b->z;
    d3_Normalize(&va);
    d3_Normalize(&vb);
    if (d3_VecEqual((D3VECTOR *)&va, (D3VECTOR *)&vb, 1e-7f))
        return 0.0f;
    nb.x = -vb.x;
    nb.y = -vb.y;
    nb.z = -vb.z;
    if (d3_VecEqual((D3VECTOR *)&va, &nb, 1e-7f))
        return -3.1415927f;
    c[0] = vb.z * va.y - va.z * vb.y;
    c[1] = vb.x * va.z - vb.z * va.x;
    c[2] = vb.y * va.x - va.y * vb.x;
    if (d3_YAxis.x * c[0] + d3_YAxis.y * c[1] + d3_YAxis.z * c[2] > 0.0f)
        /* the (float) cast on the first product orders the three products like the original (matching trick) */
        return -acos((float)(vb.z * va.z) + vb.y * va.y + vb.x * va.x);
    return acos((float)(vb.z * va.z) + vb.y * va.y + vb.x * va.x) - 6.2831855f;
}
