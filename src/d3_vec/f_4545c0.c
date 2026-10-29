/* 0x4545c0, angles to matrix */

void d3_MatFromAngles(D3VECTOR4 *v, D3MATRIX *m)
{
    double sx = sin(v->x), sy = sin(v->y), sz = sin(v->z);
    float cx = (float)cos(v->x), cy = (float)cos(v->y), t;
    double cz = cos(v->z), a;

    a = cz * sy;
    t = (float)(sz * sy);
    m->_34 = 0.0f; m->_24 = 0.0f; m->_14 = 0.0f;
    m->_43 = 0.0f; m->_42 = 0.0f; m->_41 = 0.0f;
    m->_44 = 1.0f;
    /* the (float) casts on cz here and on a in _13 set VC6's operand order of the mixed products (matching trick) */
    m->_11 = (float)((float)cz * cy);
    m->_12 = (float)(a * sx - cx * sz);
    m->_13 = (float)((float)a * cx + sz * sx);
    m->_21 = (float)(cy * sz);
    m->_22 = (float)(t * sx + cx * cz);
    m->_23 = (float)(t * cx - cz * sx);
    m->_31 = (float)-sy;
    m->_32 = (float)(cy * sx);
    m->_33 = cy * cx;
}
