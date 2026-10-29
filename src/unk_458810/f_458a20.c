/* 0x458a20, a point of the object in world space */

void d3_PointToWorld(D3OBJECT *obj_, D3VECTOR *in, D3VECTOR *out)
{
    XOBJ *obj = (XOBJ *)obj_;
    XOBJ *p;
    float x, y, z;

    out->x = obj->p78.x + in->x;
    out->y = obj->p78.y + in->y;
    out->z = obj->p78.z + in->z;
    for (p = obj->parent; p; p = p->parent) {
        x = p->p6c.x + out->x;
        y = p->p6c.y + out->y;
        z = p->p6c.z + out->z;
        out->x = x * p->m._11 + y * p->m._21 + z * p->m._31;
        out->y = x * p->m._12 + y * p->m._22 + z * p->m._32;
        out->z = x * p->m._13 + y * p->m._23 + z * p->m._33;
        out->x = out->x - p->p6c.x;
        out->y = out->y - p->p6c.y;
        out->z = out->z - p->p6c.z;
        out->x = p->p4c.x + out->x;
        out->y = p->p4c.y + out->y;
        out->z = p->p4c.z + out->z;
        out->x = p->p78.x + out->x;
        out->y = p->p78.y + out->y;
        out->z = p->p78.z + out->z;
    }
}
