/* 0x459ab0, transforms the 8 corners of the bounding box that follow the vertices of the mesh by the matrix m,
   from the corners of the source (no animation) or of the union of the boxes of the key frames in use.
   Returns the corners (0 if the mesh has no vertices). info is a D3ANIMCTL (d3_anim.h) here. */

D3BOXVERT *d3_GetBoxVerts(D3OBJECT *obj, D3OBJINFO *info, void *unk)
{
    D3ANIMCTL *ctl = (D3ANIMCTL *)info;
    D3MATRIX *m = (D3MATRIX *)unk;
    D3LVERT *v;
    D3VECTOR4 mn, mx;
    D3VECTOR4 c[8];
    D3ANIMFRAME *fr;
    int i;
    int n = info->nkeys;

    if (n == 0)
        return 0;
    {
        if (ctl->active) {
            mn.x = 1e10f;
            mn.y = 1e10f;
            mn.z = 1e10f;
            mx.x = -1e10f;
            mx.y = -1e10f;
            mx.z = -1e10f;
            for (i = 0; i < 3; i++) {
                if (ctl->slots[i].data) {
                    fr = &ctl->slots[i].data->frames[ctl->slots[i].frame];
                    mn.x = min(mn.x, fr->min.x);
                    mn.y = min(mn.y, fr->min.y);
                    mn.z = min(mn.z, fr->min.z);
                    mx.x = max(mx.x, fr->max.x);
                    mx.y = max(mx.y, fr->max.y);
                    mx.z = max(mx.z, fr->max.z);
                }
            }
            c[0].x = mn.x; c[0].y = mn.y; c[0].z = mn.z;
            c[1].x = mx.x; c[1].y = mn.y; c[1].z = mn.z;
            c[2].x = mn.x; c[2].y = mx.y; c[2].z = mn.z;
            c[3].x = mx.x; c[3].y = mx.y; c[3].z = mn.z;
            c[4].x = mn.x; c[4].y = mn.y; c[4].z = mx.z;
            c[5].x = mx.x; c[5].y = mn.y; c[5].z = mx.z;
            c[6].x = mn.x; c[6].y = mx.y; c[6].z = mx.z;
            c[7].x = mx.x; c[7].y = mx.y; c[7].z = mx.z;
            v = (D3LVERT *)info->keys + n;
            for (i = 0; i < 8; i++, v++) {
                v->x = c[i].z * m->_31 + c[i].y * m->_21 + c[i].x * m->_11 + m->_41;
                v->y = c[i].z * m->_32 + c[i].y * m->_22 + c[i].x * m->_12 + m->_42;
                v->z = c[i].z * m->_33 + c[i].y * m->_23 + c[i].x * m->_13 + m->_43;
            }
    
        } else {
            v = (D3LVERT *)info->keys + n;
            for (i = 0; i < 8; i++, v++) {
                v->x = v->src->y * m->_21 + m->_11 * v->src->x + v->src->z * m->_31 + m->_41;
                v->y = v->src->y * (m->_22) + (m->_32) * (v->src->z) + (m->_12) * v->src->x + m->_42;
                v->z = v->src->y * m->_23 + m->_33 * (v->src->z) + m->_13 * v->src->x + m->_43;
            }
        }
        return (D3BOXVERT *)((D3LVERT *)info->keys + info->nkeys);
    }
}
