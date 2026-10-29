/* 0x458ce0, a matrix in the object's space in world space: the object's matrix, and then those of its
   parents, multiplied onto m. Side effects: rewrites the translation row of obj->m; with orientOnly 0 it
   also offsets m's translation row by the object's p6c (orientOnly 1: rotation only, no translation). */

void d3_MatToWorld(XOBJ *obj, char orientOnly, D3MATRIX *m, D3MATRIX *out)
{
    D3MATRIX t;
    XOBJ *p = obj->parent;

    if (orientOnly) {
        obj->m._41 = 0;
        obj->m._42 = 0;
        obj->m._43 = 0;
    } else {
        m->_41 = m->_41 + obj->p6c.x;
        m->_42 = m->_42 + obj->p6c.y;
        m->_43 = m->_43 + obj->p6c.z;
        obj->m._41 = (obj->p4c.x - obj->p6c.x) + obj->p78.x;
        obj->m._42 = (obj->p4c.y - obj->p6c.y) + obj->p78.y;
        obj->m._43 = (obj->p4c.z - obj->p6c.z) + obj->p78.z;
    }
    if (p) {
        d3_MatMul43(m, &obj->m, &t);
        d3_MatToWorld(p, orientOnly, &t, out);
        return;
    }
    d3_MatMul43(m, &obj->m, out);
}
