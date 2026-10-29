/* 0x454070, inverse of a 4x4 matrix (cofactor expansion; falls back to the transpose when |det| < 1e-7).
   Each m[k] is spelled either m[k] or src->_ij: the spelling changes VC6's operand order of the products
   (matching trick, found by search; no meaning). */

void d3_MatInverse(D3MATRIX *src, D3MATRIX *dst)
{
    float *m = (float *)src, *o = (float *)dst;
    float det;

    det = (src->_22 * (src->_33 * src->_44 - src->_43 * src->_34) - src->_32 * (m[6] * src->_44 - src->_43 * m[7]) + m[13] * (m[6] * src->_34 - m[7] * src->_33)) * src->_11 - (m[1] * (src->_33 * src->_44 - src->_43 * src->_34) - src->_32 * (src->_44 * src->_13 - src->_43 * m[3]) + m[13] * (src->_34 * src->_13 - m[3] * src->_33)) * src->_21 + (m[1] * (m[6] * src->_44 - src->_43 * m[7]) - src->_22 * (src->_44 * src->_13 - src->_43 * m[3]) + m[13] * (m[7] * src->_13 - m[3] * m[6])) * src->_31 - (m[1] * (m[6] * src->_34 - m[7] * src->_33) - src->_22 * (src->_34 * src->_13 - m[3] * src->_33) + src->_32 * (m[7] * src->_13 - m[3] * m[6])) * src->_41;
    if (fabs(det) < 1e-07) {
        d3_MatTranspose(src, dst);
        return;
    }
    det = 1.0f / det;
    o[0] = (src->_22 * (src->_33 * src->_44 - m[14] * src->_34) - src->_32 * (src->_23 * src->_44 - m[14] * m[7]) + src->_42 * (src->_23 * src->_34 - m[7] * src->_33)) * det;
    o[1] = -(((src->_33 * src->_44 - m[14] * src->_34) * m[1] - (src->_44 * src->_13 - m[14] * m[3]) * src->_32 + (src->_34 * src->_13 - m[3] * src->_33) * src->_42) * det);
    o[2] = ((src->_23 * src->_44 - m[14] * m[7]) * m[1] - (src->_44 * src->_13 - m[14] * m[3]) * src->_22 + (m[7] * src->_13 - m[3] * src->_23) * src->_42) * det;
    o[3] = -(((src->_23 * src->_34 - m[7] * src->_33) * m[1] - (src->_34 * src->_13 - m[3] * src->_33) * src->_22 + (m[7] * src->_13 - m[3] * src->_23) * src->_32) * det);
    o[4] = -(((src->_33 * src->_44 - m[14] * src->_34) * src->_21 - (src->_23 * src->_44 - m[14] * m[7]) * src->_31 + (src->_23 * src->_34 - m[7] * src->_33) * src->_41) * det);
    o[5] = ((src->_33 * src->_44 - m[14] * src->_34) * m[0] - (src->_44 * src->_13 - m[14] * m[3]) * src->_31 + (src->_34 * src->_13 - m[3] * src->_33) * src->_41) * det;
    o[6] = -(((src->_23 * src->_44 - m[14] * m[7]) * m[0] - (src->_44 * src->_13 - m[14] * m[3]) * src->_21 + (m[7] * src->_13 - m[3] * src->_23) * src->_41) * det);
    o[7] = ((src->_23 * src->_34 - m[7] * src->_33) * m[0] - (src->_34 * src->_13 - m[3] * src->_33) * src->_21 + (m[7] * src->_13 - m[3] * src->_23) * src->_31) * det;
    o[8] = ((src->_32 * src->_44 - src->_42 * src->_34) * src->_21 - (src->_44 * src->_22 - m[7] * src->_42) * src->_31 + (src->_34 * src->_22 - src->_32 * m[7]) * src->_41) * det;
    o[9] = -(((src->_32 * src->_44 - src->_42 * src->_34) * m[0] - (m[1] * src->_44 - m[3] * src->_42) * src->_31 + (m[1] * src->_34 - src->_32 * m[3]) * src->_41) * det);
    o[10] = ((src->_44 * src->_22 - m[7] * src->_42) * m[0] - (m[1] * src->_44 - m[3] * src->_42) * src->_21 + (m[7] * m[1] - m[3] * src->_22) * src->_41) * det;
    o[11] = -(((src->_34 * src->_22 - (src->_32) * m[7]) * m[0] - (m[1] * src->_34 - src->_32 * m[3]) * src->_21 + (m[7] * m[1] - m[3] * src->_22) * src->_31) * det);
    o[12] = -(((m[14] * src->_32 - src->_42 * src->_33) * src->_21 - (m[14] * src->_22 - src->_42 * src->_23) * src->_31 + (src->_33 * src->_22 - src->_32 * src->_23) * src->_41) * det);
    o[13] = ((m[14] * src->_32 - src->_42 * src->_33) * m[0] - (m[14] * m[1] - src->_42 * src->_13) * src->_31 + (m[1] * src->_33 - src->_32 * src->_13) * src->_41) * det;
    o[14] = -(((m[14] * src->_22 - src->_42 * src->_23) * m[0] - (m[14] * m[1] - src->_42 * src->_13) * src->_21 + (m[1] * src->_23 - src->_22 * src->_13) * src->_41) * det);
    o[15] = ((src->_33 * src->_22 - src->_32 * src->_23) * m[0] - (m[1] * src->_33 - src->_32 * src->_13) * src->_21 + (m[1] * src->_23 - src->_22 * src->_13) * src->_31) * det;
}
