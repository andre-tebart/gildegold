/* 0x458da0, the world matrix of obj, in ref's space when ref is given (orientOnly 1: rotation only, no
   translation) */

void d3_WorldMatrix(XOBJ *obj, XOBJ *ref, char orientOnly, D3MATRIX *out)
{
    D3MATRIX t, id;

    d3_MatIdentity(&id);
    if (ref) {
        d3_MatToWorld(obj, orientOnly, &id, &t);
        t._41 = t._41 - ref->p4c.x;
        t._42 = t._42 - ref->p4c.y;
        t._43 = t._43 - ref->p4c.z;
        t._41 = t._41 - ref->p78.x;
        t._42 = t._42 - ref->p78.y;
        t._43 = t._43 - ref->p78.z;
        d3_MatMul43(&t, &ref->m, out);
        return;
    }
    d3_MatToWorld(obj, orientOnly, &id, out);
}
