/* 0x453b70, rows 0..2 of m: right, up, forward (forward = normalized dir, up = (0,1,0) rolled by roll) */

void d3_MakeBasis(D3VECTOR *dir, float roll, D3MATRIX *m)
{
    D3VECTOR4 u;
    float s, c, d, t;

    s = (float)sin(roll);
    u.z = d3_YAxis.z;
    c = (float)cos(roll);
    u.x = d3_YAxis.x * c - d3_YAxis.y * s;
    u.y = d3_YAxis.y * c + d3_YAxis.x * s;
    m->_31 = dir->x;
    m->_32 = dir->y;
    m->_33 = dir->z;
    d3_Normalize((D3VECTOR4 *)&m->_31);
    d = -(u.x * m->_31 + u.z * m->_33 + u.y * m->_32);
    m->_21 = d * m->_31 + u.x;
    m->_22 = d * m->_32 + u.y;
    m->_23 = d * m->_33 + u.z;
    if ((float)sqrt(m->_21 * m->_21 + m->_22 * m->_22 + m->_23 * m->_23) < 1e-7f) {
        t = -m->_32;
        m->_21 = t * m->_31 + d3_YAxis.x;
        m->_22 = t * m->_32 + d3_YAxis.y;
        m->_23 = t * m->_33 + d3_YAxis.z;
        if ((float)sqrt(m->_22 * m->_22 + m->_21 * m->_21 + m->_23 * m->_23) < 1e-7f) {
            t = -m->_33;
            m->_21 = t * m->_31 + d3_ZAxis.x;
            m->_22 = t * m->_32 + d3_ZAxis.y;
            m->_23 = t * m->_33 + d3_ZAxis.z;
            if ((float)sqrt(m->_22 * m->_22 + m->_21 * m->_21 + m->_23 * m->_23) < 1e-7f) {
                d3_MatIdentity(m);
                return;
            }
        }
    }
    d3_Normalize((D3VECTOR4 *)&m->_21);
    m->_34 = 0.0f;
    m->_24 = 0.0f;
    m->_14 = 0.0f;
    m->_44 = 1.0f;
    m->_11 = m->_22 * (m->_33) - (m->_32) * m->_23;
    m->_12 = m->_31 * m->_23 - (m->_21) * m->_33;
    m->_13 = m->_21 * m->_32 - m->_31 * m->_22;
}
