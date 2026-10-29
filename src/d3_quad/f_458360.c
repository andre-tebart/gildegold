/* 0x458360, the 8 corners of the node's box relative to pos, rotated by m (the first one also moved by m's translation) */

D3BOXVERT *d3q_BoxCorners(D3QNODE *n, float *pos, float *m)
{
    float x0 = n->min.x - pos[0];
    float y0 = n->min.y - pos[1];
    float z0 = n->min.z - pos[2];
    D3VECTOR4 h;

    h.x = n->max.x - pos[0];
    h.y = n->max.y - pos[1];
    h.z = n->max.z - pos[2];

    d3_QBox[0].x = y0 * m[4] + z0 * m[8] + x0 * m[0] + m[12];
    d3_QBox[0].y = y0 * m[5] + z0 * m[9] + x0 * m[1] + m[13];
    d3_QBox[0].z = x0 * m[2] + y0 * m[6] + z0 * m[10] + m[14];
    d3_QBox[1].x = y0 * m[4] + z0 * m[8] + h.x * m[0];
    d3_QBox[1].y = y0 * m[5] + z0 * m[9] + h.x * m[1];
    d3_QBox[1].z = h.x * m[2] + y0 * m[6] + z0 * m[10];
    d3_QBox[2].x = z0 * m[8] + h.y * m[4] + x0 * m[0];
    d3_QBox[2].y = z0 * m[9] + h.y * m[5] + x0 * m[1];
    d3_QBox[2].z = x0 * m[2] + z0 * m[10] + h.y * m[6];
    d3_QBox[3].x = z0 * m[8] + h.y * m[4] + h.x * m[0];
    d3_QBox[3].y = z0 * m[9] + h.y * m[5] + h.x * m[1];
    d3_QBox[3].z = h.x * m[2] + z0 * m[10] + h.y * m[6];
    d3_QBox[4].x = y0 * m[4] + h.z * m[8] + x0 * m[0];
    d3_QBox[4].y = y0 * m[5] + h.z * m[9] + x0 * m[1];
    d3_QBox[4].z = x0 * m[2] + y0 * m[6] + h.z * m[10];
    d3_QBox[5].x = y0 * m[4] + h.z * m[8] + h.x * m[0];
    d3_QBox[5].y = y0 * m[5] + h.z * m[9] + h.x * m[1];
    d3_QBox[5].z = h.x * m[2] + y0 * m[6] + h.z * m[10];
    d3_QBox[6].x = h.y * m[4] + h.z * m[8] + x0 * m[0];
    d3_QBox[6].y = h.y * m[5] + h.z * m[9] + x0 * m[1];
    d3_QBox[6].z = x0 * m[2] + h.y * m[6] + h.z * m[10];
    d3_QBox[7].x = h.y * m[4] + h.z * m[8] + h.x * m[0];
    d3_QBox[7].y = h.y * m[5] + h.z * m[9] + h.x * m[1];
    d3_QBox[7].z = h.x * m[2] + h.y * m[6] + h.z * m[10];
    return d3_QBox;
}
