/* 0x4522b0, adds the point lights and then the directional lights (type 7) of set to the vertex colours of obj */

char d3_LightApply(D3OBJECT *obj, D3LCTX *set)
{
    D3LINFO *info = D3_L(obj)->info;
    D3OBJECT *l;
    D3LVERT *v;
    D3LTRI *t;
    D3VECTOR4 d, c, nrm, acc;
    D3MATRIX m;
    D3VECTOR *p;
    unsigned int i, j;
    int k, iv;
    float a, s, r, r2, dist, f, sc;

    if (!info)
        return 1;
    {
        for (i = 0; i < set->count; i++) {
            l = set->list[i];
            if (l->type != 7) {
                s = D3_L(obj)->mesh->lightScale * D3_L(l)->power * 10.0f;
                a = D3_L(l)->atten;
                r = D3_L(l)->range;
                r2 = r * r;
                if (set->normals) {
                    for (k = 0, v = info->keys; k < info->nkeys; k++, v++) {
                        d.x = v->x - D3_L(l)->cam.x;
                        d.y = v->y - D3_L(l)->cam.y;
                        d.z = v->z - D3_L(l)->cam.z;
                        dist = d.x * d.x + d.y * d.y + d.z * d.z;
                        if (dist < r2) {
                            d3_Normalize(&d);
                            f = d.x * v->nx + d.y * v->ny + d.z * v->nz;
                            if (f < 0.0) {
                                d3l_FtoI2(f * -1023.0f, &iv);
                                f = ((float)(1.0f / (dist * a)) * s) * d3fl_Shade[iv];
                                c.x = f * D3_L(l)->color.x;
                                c.y = f * D3_L(l)->color.y;
                                c.z = f * D3_L(l)->color.z;
                                v->r = c.x + v->r;
                                v->g = c.y + v->g;
                                v->b = c.z + v->b;
                            }
                        }
                    }
                } else {
                    for (k = 0, v = info->keys; k < info->nkeys; k++, v++) {
                        d.x = v->x - D3_L(l)->cam.x;
                        d.y = v->y - D3_L(l)->cam.y;
                        d.z = v->z - D3_L(l)->cam.z;
                        dist = d.x * d.x + d.y * d.y + d.z * d.z;
                        if (dist < r2) {
                            f = (float)(1.0f / (dist * a)) * s;
                            c.x = f * D3_L(l)->color.x;
                            c.y = f * D3_L(l)->color.y;
                            c.z = f * D3_L(l)->color.z;
                            v->r = c.x + v->r;
                            v->g = c.y + v->g;
                            v->b = c.z + v->b;
                        }
                    }
                }
            }
        }
        for (i = 0; i < set->count; i++)
            if (set->list[i]->type == 7)
                break;
        if (i < set->count) {
            if (obj->kind == 1) {
                d3_WorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 1, &m);
                sc = 20.0f / info->ops->radius;
                acc.y = info->ops->originY;
                for (j = i; j < set->count; j++) {
                    l = set->list[j];
                    if (l->type == 7) {
                        s = D3_L(obj)->mesh->lightScale * D3_L(l)->power * 0.01f;
                        nrm.x = D3_L(l)->light->dir.x;
                        nrm.y = D3_L(l)->light->dir.y;
                        nrm.z = D3_L(l)->light->dir.z;
                        for (k = 0, v = info->keys; k < info->nkeys; k++, v++) {
                            p = v->src;
                            d.x = p->x;
                            d.y = p->y;
                            d.z = p->z;
                            d.y = (d.y - acc.y) * sc;
                            c.x = m._11 * d.x + m._21 * d.y + m._31 * d.z;
                            c.y = m._12 * d.x + m._22 * d.y + m._32 * d.z;
                            c.z = m._13 * d.x + m._23 * d.y + m._33 * d.z;
                            d3_Normalize(&c);
                            f = nrm.x * c.x + nrm.y * c.y + nrm.z * c.z;
                            if (f < 0.0) {
                                d3l_FtoI2(f * -1023.0f, &iv);
                                f = s * d3fl_Shade[iv];
                                c.x = f * D3_L(l)->color.x;
                                c.y = f * D3_L(l)->color.y;
                                c.z = f * D3_L(l)->color.z;
                                v->r = c.x + v->r;
                                v->g = c.y + v->g;
                                v->b = c.z + v->b;
                            }
                        }
                    }
                }
            } else {
                for (k = 0, t = info->tris; k < info->ntris; k++, t++) {
                    d3_TriNormal(t->v[0], t->v[1], t->v[2], &nrm);
                    acc.x = 0.0f;
                    acc.y = 0.0f;
                    acc.z = 0.0f;
                    for (j = i; j < set->count; j++) {
                        l = set->list[j];
                        if (l->type == 7) {
                            s = D3_L(obj)->mesh->lightScale * D3_L(l)->power * 0.001f;
                            f = nrm.x * D3_L(l)->light->dir.x + nrm.y * D3_L(l)->light->dir.y + nrm.z * D3_L(l)->light->dir.z;
                            if (f < 0.0f) {
                                d3l_FtoI2(f * -1023.0f, &iv);
                                f = s * d3fl_Shade[iv];
                                c.x = f * D3_L(l)->color.x;
                                c.y = f * D3_L(l)->color.y;
                                c.z = f * D3_L(l)->color.z;
                                acc.x = acc.x + c.x;
                                acc.y = acc.y + c.y;
                                acc.z = acc.z + c.z;
                            }
                        }
                    }
                    for (j = 0; j < 3; j++) {
                        t->v[j]->r = acc.x + t->v[j]->r;
                        t->v[j]->g = acc.y + t->v[j]->g;
                        t->v[j]->b = acc.z + t->v[j]->b;
                    }
                }
            }
        }
    }
    return 1;
}
