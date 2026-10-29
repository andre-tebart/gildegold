/* 0x451f10, callback: flickering light, adds its contribution to the vertices it touches */

#define D3L_RGB_MASK ((unsigned)&txt_Keys + 0x181bf)   /* the r/g/b bits of D3COLOR (white = all three set) */

void d3_LightFlicker(D3OBJECT *obj)
{
    unsigned long now = d3_Time;
    unsigned long t = now - D3_L(obj)->light->time;
    unsigned int iv = D3_L(obj)->light->interval;
    D3LREC *n;
    D3OBJECT *o;
    D3LVERT *v;
    D3LHIT *p;
    D3COLOR c;
    unsigned int i, cnt;
    int j, k;
    float s, f, m, r, g, b, a, q;

    if (t >= iv) {
        m = D3_L(obj)->color.y;
        for (j = 0; j < 7; j++)
            m = max(m, D3_L(obj)->light->keys[j].val);
        s = (D3_L(obj)->light->flicker * D3_L(obj)->color.y) / m;
        if (!D3_L(obj)->light->time)
            D3_L(obj)->flickerStart = (float)((float)rand() / RAND_MAX) * s * 2 - s + 1.0f;
        else
            D3_L(obj)->flickerStart = D3_L(obj)->flickerEnd;
        D3_L(obj)->flickerEnd = (float)((float)rand() / RAND_MAX) * s * 2 - s + 1.0f;
        D3_L(obj)->light->time = now;
        t = 0;
    }
    f = (float)t / (float)iv;
    a = (1.0f - f) * D3_L(obj)->flickerStart + f * D3_L(obj)->flickerEnd;
    for (n = D3_L(obj)->light->list; n; n = n->next) {
        o = n->obj;
        if (o && D3_L(o)->mesh && !(D3_L(o)->mesh->flags & 0x40) && D3_L(o)->info) {
            cnt = n->n;
            if (D3_L(o)->info->nkeys >= (int)cnt) {
                if (d3_Hardware) {
                    for (i = 0, p = n->v; i < cnt; i++, p++) {
                        v = p->v;
                        if ((v->color & D3L_RGB_MASK) != D3L_RGB_MASK) {
                            c = p->c;
                            r = c.r * a + v->lit.r;
                            g = c.g * a + v->lit.g;
                            b = c.b * a + v->lit.b;
                            m = max(max(r, g), b);
                            if (m <= 255.0f) {
                                d3_FtoI(&k, r);
                                v->lit.r = k;
                                d3_FtoI(&k, g);
                                v->lit.g = k;
                                d3_FtoI(&k, b);
                                v->lit.b = k;
                            } else {
                                q = 255.0f / m;
                                r *= q;
                                d3_FtoI(&k, r);
                                v->lit.r = k;
                                g *= q;
                                d3_FtoI(&k, g);
                                v->lit.g = k;
                                d3_FtoI(&k, b * q);
                                v->lit.b = k;
                            }
                        }
                    }
                } else {
                    a *= 0.25;
                    for (i = 0, p = n->v; i < cnt; i++, p++) {
                        v = p->v;
                        if (v->lit.r != 0x3f) {
                            c = p->c;
                            d3l_FtoI2(c.r * a, &k);
                            v->lit.r = (unsigned)k < 0x3f ? k : 0x3f;
                        }
                    }
                }
            }
        }
    }
}
