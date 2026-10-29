/* 0x454510, identity */

void d3_MatIdentity(D3MATRIX *m)
{
    D3_ZERO_AT(m, sizeof(D3MATRIX));
    m->_44 = 1.0f;
    m->_33 = 1.0f;
    m->_22 = 1.0f;
    m->_11 = 1.0f;
}
