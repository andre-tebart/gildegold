/* 0x433b50, spline tangents of all keys (flags.b[1] & 4: smooth); b[8] & 1 wraps the angles first. @deep B draft: 46 code hunks, dominated by the strength-reduced key offset in the first loop ([edi+ecx+0x20] ours vs [ecx+edi+0x20] original); the recorded attempts (notes/units/d3_anim.md: (p->keys + i)->, i[p->keys], 0.0f vs 0) failed - deep matching. Q2: the callee 0x453d60 is d3_UnwrapAngles in d3_vec.c, but d3.h declares the D3VECTOR* view as d3_UnwrapAngles, so naming it needs a shared-header decision. */

void d3path_Init(D3PATH *p)
{
    int i;

    if (p && (p->flags.b[1] & 4)) {
        if (p->nkeys <= 2) {
            for (i = 0; i < p->nkeys; i++) {
                if ((p->flags.b[8] & 1) && i < p->nkeys - 1)
                    d3_UnwrapAngles(&p->keys[i + 1].rot, &p->keys[i].rot, &p->keys[i + 1].rot, &p->keys[i].rot);
                p->keys[i].posin.x = 0;
                p->keys[i].posin.y = 0;
                p->keys[i].posin.z = 0;
                p->keys[i].posout.x = 0;
                p->keys[i].posout.y = 0;
                p->keys[i].posout.z = 0;
                p->keys[i].rotin.x = 0;
                p->keys[i].rotin.y = 0;
                p->keys[i].rotin.z = 0;
                p->keys[i].rotout.x = 0;
                p->keys[i].rotout.y = 0;
                p->keys[i].rotout.z = 0;
            }
        } else {
            for (i = 0; i < p->nkeys; i++) {
                if ((p->flags.b[8] & 1) && i < p->nkeys - 1)
                    d3_UnwrapAngles(&p->keys[i + 1].rot, &p->keys[i].rot, &p->keys[i + 1].rot, &p->keys[i].rot);
                if (i == 0) {
                    p->keys[0].posin.x = 0;
                    p->keys[0].posin.y = 0;
                    p->keys[0].posin.z = 0;
                    p->keys[0].rotin.x = 0;
                    p->keys[0].rotin.y = 0;
                    p->keys[0].rotin.z = 0;
                } else if (i < p->nkeys - 1) {
                    if (i == p->nkeys - 2 && ((p->flags.b[1] & 1) || (p->flags.b[1] & 0x10))) {
                        p->keys[i].posout.x = 0;
                        p->keys[i].posout.y = 0;
                        p->keys[i].posout.z = 0;
                        p->keys[i].rotout.x = 0;
                        p->keys[i].rotout.y = 0;
                        p->keys[i].rotout.z = 0;
                    }
                    d3path_Tangents(&p->keys[i - 1], &p->keys[i], &p->keys[i + 1]);
                } else if (!(p->flags.b[1] & 1) && !(p->flags.b[1] & 0x10))
                    d3path_Tangents(&p->keys[i - 1], &p->keys[i], &p->keys[0]);
                else {
                    p->keys[i].posin.x = 0;
                    p->keys[i].posin.y = 0;
                    p->keys[i].posin.z = 0;
                    p->keys[i].posout.x = 0;
                    p->keys[i].posout.y = 0;
                    p->keys[i].posout.z = 0;
                    p->keys[i].rotin.x = 0;
                    p->keys[i].rotin.y = 0;
                    p->keys[i].rotin.z = 0;
                    p->keys[i].rotout.x = 0;
                    p->keys[i].rotout.y = 0;
                    p->keys[i].rotout.z = 0;
                }
            }
        }
    }
}
