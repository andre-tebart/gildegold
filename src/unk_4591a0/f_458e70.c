/* 0x458e70, environment map coordinates of the vertices of a part that has a reflecting texture @deep behaviour: the only code hunk is an instruction order/stack-slot artifact (the original loads [esp+0x7c] before the call's add esp,0x4 where ours loads [esp+0x78] after it - the same local, same value); readability: Q2 d3_WorldMatrix (callee in unk_458810.c, fills the object's world matrix) and state (object flag byte, bit 0x80 = no camera), Q4 the XENTRY macro and the three *(unsigned char **)(entry + 0xb8) reads of the animation entry */
void d3_EnvMapCoords(XOBJ *obj, XPART *part)
{
    D3MATRIX m;
    D3VECTOR4 b;
    struct { float x, y; volatile float z; float w; } nv;
    volatile float d;
    int n, i, k;
    int *slot, entry;
    XVERT *v;

    if (obj && part && part->ops && (n = part->ops->count) > 0) {
        for (i = 0; i < n; i++) {
            if (part->tex[i] && (part->tex[i]->flags & 1))
                break;
        }
        if (i < n) {
            d3_WorldMatrix((D3OBJECT *)obj, (obj->flags & 0x80) ? 0 : d3_Camera, 1, &m);
            slot = d3anim_GetMainSlot(part);
            if (slot && (entry = XENTRY(slot)) != 0) {
                v = part->keys;
                for (i = 0; i < part->nkeys; i++, v++) {
                    if (v->used) {
                        b.x = ((float)(*(unsigned char **)(entry + 0xb8))[i * 3] - 128.0f) * 0.007843138f;
                        b.y = ((float)(*(unsigned char **)(entry + 0xb8))[i * 3 + 1] - 128.0f) * 0.007843138f;
                        b.z = ((float)(*(unsigned char **)(entry + 0xb8))[i * 3 + 2] - 128.0f) * 0.007843138f;
                        nv.x = m._11 * b.x + m._21 * b.y + m._31 * b.z;
                        nv.y = m._12 * b.x + m._22 * b.y + m._32 * b.z;
                        nv.z = m._13 * b.x + m._23 * b.y + m._33 * b.z;
                        d = (nv.x * v->x + nv.y * v->y + nv.z * v->z) * -2.0f;
                        b.x = d * nv.x + v->x;
                        b.y = d * nv.y + v->y;
                        b.z = nv.z * d + v->z;
                        d3_Normalize(&b);
                        v->u = (b.x + 1.0f) * 0.5f;
                        v->v = (b.y + 1.0f) * 0.5f;
                    }
                }
                return;
            }
            v = part->keys;
            for (i = 0; i < part->nkeys; i++, v++) {
                if (v->used) {
                    nv.x = (m._11 * v->src->nx) + m._31 * v->src->nz + m._21 * v->src->ny;
                    nv.y = (m._12 * v->src->nx) + m._32 * v->src->nz + m._22 * v->src->ny;
                    nv.z = (m._13 * v->src->nx) + m._33 * v->src->nz + m._23 * v->src->ny;
                    d = (nv.x * v->x + nv.y * v->y + nv.z * v->z) * -2.0f;
                    b.x = d * nv.x + v->x;
                    b.y = d * nv.y + v->y;
                    b.z = nv.z * d + v->z;
                    d3_Normalize(&b);
                    v->u = (b.x + 1.0f) * 0.5f;
                    v->v = (b.y + 1.0f) * 0.5f;
                }
            }
        }
    }
}
