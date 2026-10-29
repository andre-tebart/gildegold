/* 0x459300 @deep behaviour: 225 fpu + 107 code hunks are the FPU operand ranks and register allocation of the blend/transform loops (the volatile-z / volatile-weight ordering the unit note records, worked to 176 lines before); readability: Q2 objectFlags (object flag byte), Q4 14 raw access casts (frame and slot views) */

/* 0x459300, transforms the vertices of a mesh into camera space with the matrix m. An animated mesh (3 slots, each
   with two key frames) first blends the packed positions of the frames in use; the vertices then also add the
   source point. With D3OBJECT::objectFlags bit 5 the nearest and farthest z are tracked. */
typedef struct D3MVEC {             /* a 16-byte vector local; only z goes through memory */
    float x, y;
    volatile float z;
    float w;
} D3MVEC;

typedef struct D3MVEC2 {
    float x;
    volatile float y, z;
    float w;
} D3MVEC2;

void d3_TransformVerts(D3OBJECT *obj, D3LINFO *info, D3MATRIX *m)
{
    D3ANIMCTL *ctl = (D3ANIMCTL *)info;
    D3LVERT *v;
    D3VECTOR *sp;
    D3ANIMSLOT *sl;
    D3ANIMFRAME *f1, *f2;
    unsigned char *p1, *p2;
    float a, b;
    D3MVEC s1, b1, s2, b2, q1;
    D3MVEC2 q2;
    float z;
    float vx, vy, vz;
    int i, k;
    char first;

    if (ctl->active) {
        first = 1;
        for (k = 0; k < 3; k++) {
            sl = &ctl->slots[k];
            if (sl->data && sl->weight != 0.0f) {
                f1 = &sl->data->frames[sl->frame];
                f2 = &sl->data->frames[sl->nextFrame];
                p1 = f1->packed;
                p2 = f2->packed;
                /* scale / base of the two frames, pre-multiplied by the slot weight; the statement order (z copies
                   are volatile stores, x / y pairs share the FPU loads) is what the bytes need (hill climb) */
                s1.x = f1->scale.x;
                s1.y = f1->scale.y;
                s1.x = *(volatile float *)&sl->weight * s1.x;
                s1.z = f1->scale.z;
                s1.y = *(volatile float *)&sl->weight * s1.y;
                s2.z = f2->scale.z;
                b2.z = f2->base.z;
                s1.z = *(volatile float *)&sl->weight * s1.z;
                b1.x = f1->base.x;
                b1.y = f1->base.y;
                b1.x = *(volatile float *)&sl->weight * b1.x;
                b1.y = *(volatile float *)&sl->weight * b1.y;
                b1.z = f1->base.z;
                b1.z = *(volatile float *)&sl->weight * b1.z;
                s2.x = f2->scale.x;
                s2.y = f2->scale.y;
                s2.x = *(volatile float *)&sl->weight * s2.x;
                s2.y = *(volatile float *)&sl->weight * s2.y;
                s2.z = *(volatile float *)&sl->weight * s2.z;
                b2.x = f2->base.x;
                b2.y = f2->base.y;
                b2.x = *(volatile float *)&sl->weight * b2.x;
                b2.y = *(volatile float *)&sl->weight * b2.y;
                b2.z = *(volatile float *)&sl->weight * b2.z;
                d3_AnimBlendWeights(sl, &a, &b);
                v = info->keys;
                if (first) {
                    for (i = 0; i < info->nkeys; i++, p1 += 3, p2 += 3, v++) {
                        q1.x = ((float)p1[0] * s1.x + b1.x) * b;
                        q1.y = ((float)p1[1] * s1.y + b1.y) * b;
                        q1.z = ((float)p1[2] * s1.z + b1.z) * b;
                        q2.x = ((float)p2[0] * s2.x + b2.x) * a;
                        q2.y = ((float)p2[1] * s2.y + b2.y) * a;
                        q2.z = ((float)p2[2] * s2.z + b2.z) * a;
                        v->y = q2.y + q1.y;
                        v->x = q2.x + q1.x;
                        v->z = q2.z + q1.z;
                    }
                    first = 0;
                } else {
                    for (i = 0; i < info->nkeys; i++, p1 += 3, p2 += 3, v++) {
                        q1.x = ((float)p1[0] * s1.x + b1.x) * b;
                        q1.y = ((float)p1[1] * s1.y + b1.y) * b;
                        q1.z = ((float)p1[2] * s1.z + b1.z) * b;
                        q2.x = ((float)p2[0] * s2.x + b2.x) * a;
                        q2.y = ((float)p2[1] * s2.y + b2.y) * a;
                        q2.z = ((float)p2[2] * s2.z + b2.z) * a;
                        q1.x = q2.x + q1.x;
                        q1.y = q2.y + q1.y;
                        q1.z = q2.z + q1.z;
                        v->x += q1.x;
                        v->y += q1.y;
                        v->z += q1.z;
                    }
                }
            }
        }
        i = 0;
        if (obj->objectFlags & 0x20) {
            v = info->keys;
            if (0 < info->nkeys) {
                do {
                    sp = v->src;
                    vx = sp->x + v->x;
                    vy = sp->y + v->y;
                    vz = sp->z + v->z;
                    v->x = m->_11 * vx + vy * m->_21 + m->_31 * vz + m->_41;
                    v->y = (vz * m->_32) + vy * m->_22 + vx * m->_12 + m->_42;
                    z = (vz * m->_33) + vy * m->_23 + vx * m->_13 + m->_43;
                    v->z = z;
                    if (!(d3_MinZ < z))
                        d3_MinZ = z;
                    if (!(z < d3_MaxZ))
                        d3_MaxZ = *(volatile float *)&v->z; /* matching: re-read of the stored z (cuts 12 differing lines; the original copies it through the FPU) */
                    i++;
                    v++;
                } while (i < info->nkeys);
                D3_ENVMAPCOORDS(obj, info);
                return;
            }
        } else {
            v = info->keys;
            if (0 < info->nkeys) {
                do {
                    sp = v->src;
                    vx = sp->x + v->x;
                    vy = sp->y + v->y;
                    vz = sp->z + v->z;
                    v->x = vx * m->_11 + vz * m->_31 + vy * m->_21 + m->_41;
                    v->y = vz * m->_32 + vy * m->_22 + vx * m->_12 + m->_42;
                    v->z = vz * m->_33 + vy * m->_23 + vx * m->_13 + m->_43;
                    i++;
                    v++;
                } while (i < info->nkeys);
                D3_ENVMAPCOORDS(obj, info);
                return;
            }
        }
    } else {
        v = info->keys;
        i = 0;
        if (obj->objectFlags & 0x20) {
            if (0 < info->nkeys) {
                do {
                    sp = v->src;
                    v->x = sp->z * m->_31 + sp->y * m->_21 + sp->x * m->_11 + m->_41;
                    v->y = sp->x * m->_12 + m->_22 * sp->y + sp->z * m->_32 + m->_42;
                    z = m->_33 * sp->z + m->_23 * sp->y + sp->x * m->_13 + m->_43;
                    v->z = z;
                    if (!(d3_MinZ < z))
                        d3_MinZ = z;
                    if (!(z < d3_MaxZ))
                        d3_MaxZ = *(volatile float *)&v->z; /* matching: re-read of the stored z (cuts 12 differing lines; the original copies it through the FPU) */
                    i++;
                    v++;
                } while (i < info->nkeys);
                D3_ENVMAPCOORDS(obj, info);
                return;
            }
        } else if (0 < info->nkeys) {
            do {
                sp = v->src;
                v->x = sp->z * m->_31 + sp->y * m->_21 + sp->x * m->_11 + m->_41;
                v->y = sp->x * m->_12 + m->_22 * sp->y + sp->z * m->_32 + m->_42;
                v->z = m->_33 * sp->z + m->_23 * sp->y + sp->x * m->_13 + m->_43;
                i++;
                v++;
            } while (i < info->nkeys);
        }
    }
    D3_ENVMAPCOORDS(obj, info);
}
