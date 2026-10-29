/* 0x413960, moves obj along the n objects in path (null entries are skipped) within time @deep B: all 6 diffclass hunks read; the 79 code instructions are the same statements with i and k in swapped registers (ours i=ecx/k=edx, original k=ecx/i=edx; asmdiff --regs ecx=edx,edx=ecx leaves only the k store/reload placement and a 6-byte-longer tail) plus nop fill, no missing/wrong statement, constant or field */

D3ANIM *d3t_MoveObjectPath(D3OBJECT *obj, int time, int n, D3OBJECT **path)
{
    D3ANIMKEY keys[64];
    D3ANIMFLAGS af;
    D3OBJECT *o;
    int i, k, step;

    D3_ZERO(keys, sizeof(keys));
    D3_ZERO(&af, sizeof(af));
    af.b[1] |= 0x14;
    af.b[8] |= 2;
    af.b[1] |= 8;
    step = time / n;
    keys[0].rot.x = 0.0f;
    keys[0].rot.y = 0.0f;
    keys[0].rot.z = 0.0f;
    keys[0].pos.x = 0.0f;
    keys[0].pos.y = 0.0f;
    keys[0].pos.z = 0.0f;
    keys[0].time = step;
    k = 1;
    for (i = 0; i < n; i++) {
        if (o = path[i]) {
            d3_PointToWorld(o, &o->pos, (D3VECTOR *)&keys[k].pos);
            keys[k - 1].time = step;
            keys[k].rot.x -= obj->rotation.x;
            keys[k].rot.y -= obj->rotation.y;
            keys[k].rot.z -= obj->rotation.z;
            keys[k].pos.x -= obj->pos.x;
            keys[k].pos.y -= obj->pos.y;
            keys[k].pos.z -= obj->pos.z;
            k++;
        }
    }
    keys[k].time = 0;
    return d3anim_CreatePath(obj, af, n + 1, keys);
}
