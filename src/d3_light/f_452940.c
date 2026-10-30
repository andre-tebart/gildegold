/* 0x452940, adds the light ctx->light to the vertices of obj it reaches and records them in the light's list */

char d3_LightAddObject(D3OBJECT *obj, D3LCTX2 *ctx)
{
    D3LINFO *info = D3_L(obj)->info;
    D3OBJECT *light;
    D3LVERT *v;
    D3LREC *rec, *p;
    D3LHIT *hit;
    D3VECTOR4 d, c;
    float dd, rr, z, k, t, m, f, s;
    int n, i, idx;

    if (info) {
        d3_PointToRefD(obj, d3_Camera, &obj->pos, (D3VECTOR4 *)&D3_L(obj)->cam);
        D3_L(obj)->radius2 = (d3_LightRadiusMargin + info->ops->radius) * (d3_LightRadiusMargin + info->ops->radius);
        n = 0;
        light = ctx->light;
        d.x = D3_L(obj)->cam.x - D3_L(light)->cam.x;
        d.y = D3_L(obj)->cam.y - D3_L(light)->cam.y;
        d.z = D3_L(obj)->cam.z - D3_L(light)->cam.z;
        if (sqrt(d.x * d.x + d.y * d.y + d.z * d.z) < (D3_L(light)->radius2) + D3_L(obj)->radius2) {
            rr = D3_L(light)->range * D3_L(light)->range;
            k = D3_L(obj)->mesh->lightScale * D3_L(light)->power;
            z = D3_L(light)->atten;
            if (ctx->normals) {
                v = info->keys;
                for (i = 0; i < info->nkeys; i++, v++) {
                    v->flags &= 0x7f;
                    d.x = v->x - D3_L(light)->cam.x;
                    d.y = v->y - D3_L(light)->cam.y;
                    d.z = v->z - D3_L(light)->cam.z;
                    s = d.x * d.x + d.y * d.y + d.z * d.z;   /* s, not dd: the original reuses s (scale below) */
                    if (s < rr) {
                        d3_Normalize((D3VECTOR4 *)&d);
                        f = d.x * v->nx + d.y * v->ny + d.z * v->nz;
                        if (f < 0.0) {
                            f = f * -1023.0f;
                            v->flags |= 0x80;
                            d3_FtoI(&idx, f);
                            t = z;   /* through t: VC6 then loads atten before s, as the original */
                            t = (float)((float)(1.0f / (t * s)) * k * 10.0f) * d3fl_Shade[idx];
                            n++;
                            v->light.x = t * D3_L(light)->color.x;
                            v->light.y = t * D3_L(light)->color.y;
                            v->light.z = t * D3_L(light)->color.z;
                        }
                    }
                }
            } else {
                v = info->keys;
                for (i = 0; i < info->nkeys; i++, v++) {
                    v->flags &= 0x7f;
                    d.x = v->x - D3_L(light)->cam.x;
                    d.y = v->y - D3_L(light)->cam.y;
                    d.z = v->z - D3_L(light)->cam.z;
                    dd = d.x * d.x + d.y * d.y + d.z * d.z;
                    if (dd < rr) {
                        v->flags |= 0x80;
                        n++;
                        t = (float)(1.0f / (z * dd)) * k * 10.0f;
                        v->light.x = t * D3_L(light)->color.x;
                        v->light.y = t * D3_L(light)->color.y;
                        v->light.z = t * D3_L(light)->color.z;
                    }
                }
            }
            if (n) {
                if (D3_L(light)->light->list) {
                    rec = D3_L(light)->light->list;
                    while (rec->next)
                        rec = rec->next;
                    p = rec->next = (D3LREC *)mem_Alloc(n * 8 + 0xc, "d3_light:rlight_object");
                } else {
                    D3_L(light)->light->list = (D3LREC *)mem_Alloc(n * 8 + 0xc, "d3_light:rlight_object");
                    p = D3_L(light)->light->list;
                }
                p->obj = obj;
                p->n = n;
                v = info->keys;
                hit = p->v;
                for (i = 0; i < info->nkeys; i++, v++) {
                    if (v->flags & 0x80) {
                        hit->v = v;
                        c.x = v->light.x;
                        c.y = v->light.y;
                        c.z = v->light.z;
                        m = c.x;
                        if (c.y > m)
                            m = c.y;
                        if (c.z > m)
                            m = c.z;
                        if (m > 255.0f) {
                            s = 255.0f / m;
                            c.x *= s;
                            c.y *= s;
                            c.z *= s;
                        }
                        if (d3_Hardware) {
                            hit->c.r = (int)c.x;
                            hit->c.g = (int)c.y;
                            hit->c.b = (int)c.z;
                        } else {
                            hit->c.r = (int)(c.x * 0.3f + c.y * 0.59f + c.z * 0.11f);
                        }
                        hit++;
                    }
                }
            }
        }
    }
    return 1;
}
