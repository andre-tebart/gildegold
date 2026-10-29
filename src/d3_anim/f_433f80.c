/* 0x433f80, creates a two-frame morph animation from frame (of mesh) to dframe, or to the shape of dmesh;
   bone transforms are matched by name.
   @deep B draft: 306 code hunks + 18 fpu + 16 order, frame 0x34 vs the original's 0x40, the stack-slot layout, the FPU comparison chains and the inline-asm tail all differ - a deep reconstruction, not a base meaning fix. Readability also needs Q2 (unk00). */

D3ANIMDATA *d3anim_CreateMorphAnim(D3ANIMMESH *mesh, D3ANIMDATA *src, D3ANIMFRAME *frame, D3ANIMDATA *dst,
                                   D3ANIMFRAME *dframe, D3ANIMMESH *dmesh, char *name, int n)
{
    D3ANIMDATA *a = 0;
    int i, k, count;
    D3VECTOR4 *tmp, *t;
    D3VECTOR mn, mx, d;
    float sx, sy, sz, z;

    if (frame && mesh && (dmesh || (dst && dframe)) && n > 1) {
        a = mem_Alloc(sizeof(D3ANIMDATA), "d3_anim:CreateMorphAnim");
        a->nverts = mesh->nverts;
        a->nframes = 2;
        strcpy(a->name, name);
        a->frames = mem_Alloc(a->nframes * sizeof(D3ANIMFRAME), "d3_anim:CreateMorphAnimFrame");
        a->frames[0].time = 0;
        a->frames[0].count = count = n * 3;
        a->frames[0].move.x = frame->move.x;
        a->frames[0].move.y = frame->move.y;
        a->frames[0].move.z = frame->move.z;
        a->frames[0].turn.x = frame->turn.x;
        a->frames[0].turn.y = frame->turn.y;
        a->frames[0].turn.z = frame->turn.z;
        if (frame->pos) {
            a->frames[0].pos = mem_Alloc(a->nverts * sizeof(D3VECTOR), "d3_anim:CreateMorphAnimPoints0");
            D3ANIM_COPY3(a->frames[0].pos, frame->pos, a->nverts * sizeof(D3VECTOR));
            a->frames[1].pos = mem_Alloc(a->nverts * sizeof(D3VECTOR), "d3_anim:CreateMorphAnimPoints1");
        }
        if (frame->packed) {
            a->frames[0].packed = mem_Alloc(a->nverts * 3, "d3_anim:CreateMorphAnimWPoints0");
            D3ANIM_COPY3(a->frames[0].packed, frame->packed, a->nverts * 3);
            a->frames[0].base.x = frame->base.x;
            a->frames[0].base.y = frame->base.y;
            a->frames[0].base.z = frame->base.z;
            a->frames[0].scale.x = frame->scale.x;
            a->frames[0].scale.y = frame->scale.y;
            a->frames[0].scale.z = frame->scale.z;
            a->frames[1].packed = mem_Alloc(a->nverts * 3, "d3_anim:CreateMorphAnimWPoints1");
        }
        a->frames[1].time = n;
        a->frames[1].count = count;
        a->nbones = src->nbones;
        for (i = 0; i < a->nbones; i++) {
            strcpy(D3ANIM_BONENAME(a, i), D3ANIM_BONENAME(src, i));
            a->frames[0].bone[i].pos.x = frame->bone[i].pos.x;
            a->frames[0].bone[i].pos.y = frame->bone[i].pos.y;
            a->frames[0].bone[i].pos.z = frame->bone[i].pos.z;
            a->frames[0].bone[i].rot.x = frame->bone[i].rot.x;
            a->frames[0].bone[i].rot.y = frame->bone[i].rot.y;
            a->frames[0].bone[i].rot.z = frame->bone[i].rot.z;
        }
        if (dmesh) {
            for (i = 0; i < a->nbones; i++) {
                for (k = 0; k < 4; k++) {
                    if (!strcmp(D3ANIM_BONENAME(a, i), dmesh->bones[k].name)) {
                        a->frames[1].bone[i].pos.x = dmesh->bones[k].pos.x;
                        a->frames[1].bone[i].pos.y = dmesh->bones[k].pos.y;
                        a->frames[1].bone[i].pos.z = dmesh->bones[k].pos.z;
                        a->frames[1].bone[i].rot.x = dmesh->bones[k].rot.x;
                        a->frames[1].bone[i].rot.y = dmesh->bones[k].rot.y;
                        a->frames[1].bone[i].rot.z = dmesh->bones[k].rot.z;
                    }
                }
            }
            a->frames[1].move.x = ((D3ANIMMESHX *)dmesh)->pos.x - ((D3ANIMMESHX *)mesh)->pos.x;
            a->frames[1].move.y = ((D3ANIMMESHX *)dmesh)->pos.y - ((D3ANIMMESHX *)mesh)->pos.y;
            a->frames[1].move.z = ((D3ANIMMESHX *)dmesh)->pos.z - ((D3ANIMMESHX *)mesh)->pos.z;
            a->frames[1].move.y = 0.0f;
            a->frames[1].turn.x = ((D3ANIMMESHX *)dmesh)->rot.x - ((D3ANIMMESHX *)mesh)->rot.x;
            a->frames[1].turn.y = ((D3ANIMMESHX *)dmesh)->rot.y - ((D3ANIMMESHX *)mesh)->rot.y;
            a->frames[1].turn.z = ((D3ANIMMESHX *)dmesh)->rot.z - ((D3ANIMMESHX *)mesh)->rot.z;
            if (frame->pos) {
                for (i = 0; i < a->nverts; i++) {
                    a->frames[1].pos[i].x = dmesh->verts[i].pos.x - mesh->verts[i].pos.x;
                    a->frames[1].pos[i].y = dmesh->verts[i].pos.y - mesh->verts[i].pos.y;
                    a->frames[1].pos[i].z = dmesh->verts[i].pos.z - mesh->verts[i].pos.z;
                }
            }
            if (frame->packed) {
                tmp = mem_Alloc(a->nverts * sizeof(D3VECTOR4), "d3_anim:CreateMorph(TempWPoints)");
                mx.z = -1e10f;
                mn.x = 1e10f;
                mn.y = 1e10f;
                mn.z = 1e10f;
                mx.x = -1e10f;
                mx.y = -1e10f;
                for (i = 0, t = tmp; i < a->nverts; i++, t++) {
                    t->x = dmesh->verts[i].pos.x - mesh->verts[i].pos.x;
                    t->y = dmesh->verts[i].pos.y - mesh->verts[i].pos.y;
                    t->z = (z = dmesh->verts[i].pos.z - mesh->verts[i].pos.z);
                    mn.x = mn.x >= t->x ? t->x : mn.x;
                    mn.y = min(mn.y, t->y);
                    if (mn.z >= z)
                        mn.z = z;
                    mx.x = max(t->x, mx.x);
                    mx.y = max(t->y, mx.y);
                    mx.z = max(mx.z, z);
                }
                d.x = mx.x - mn.x;
                a->frames[1].base.x = mn.x;
                d.y = mx.y - mn.y;
                a->frames[1].base.y = mn.y;
                a->frames[1].base.z = mn.z;
                d.z = mx.z - mn.z;
                a->frames[1].scale.x = d.x * (1.0f / 255.0f);
                a->frames[1].scale.y = d.y * (1.0f / 255.0f);
                a->frames[1].scale.z = d.z * (1.0f / 255.0f);
                if (a->nverts > 0) {
                    sx = 1.0f / d.x;
                    sy = 1.0f / d.y;
                    sz = 1.0f / d.z;
                    for (i = 0, t = tmp; i < a->nverts; i++, t++) {
                        a->frames[1].packed[i * 3] = (unsigned char)((t->x - mn.x) * 255.0f * sx);
                        a->frames[1].packed[i * 3 + 1] = (unsigned char)((t->y - mn.y) * 255.0f * sy);
                        a->frames[1].packed[i * 3 + 2] = (unsigned char)((t->z - mn.z) * 255.0f * sz);
                    }
                }
                mem_Free(tmp);
            }
        } else {
            for (i = 0; i < a->nbones; i++) {
                for (k = 0; k < 4; k++) {
                    if (!strcmp(D3ANIM_BONENAME(a, i), D3ANIM_BONENAME(dst, k))) {
                        a->frames[1].bone[i].pos.x = dframe->bone[k].pos.x;
                        a->frames[1].bone[i].pos.y = dframe->bone[k].pos.y;
                        a->frames[1].bone[i].pos.z = dframe->bone[k].pos.z;
                        a->frames[1].bone[i].rot.x = dframe->bone[k].rot.x;
                        a->frames[1].bone[i].rot.y = dframe->bone[k].rot.y;
                        a->frames[1].bone[i].rot.z = dframe->bone[k].rot.z;
                    }
                }
            }
            a->frames[1].move.x = dframe->move.x;
            a->frames[1].move.y = dframe->move.y;
            a->frames[1].move.z = dframe->move.z;
            a->frames[1].turn.x = dframe->turn.x;
            a->frames[1].turn.y = dframe->turn.y;
            a->frames[1].turn.z = dframe->turn.z;
            if (frame->pos)
                D3ANIM_COPY3(a->frames[1].pos, dframe->pos, a->nverts * sizeof(D3VECTOR));
            if (frame->packed) {
                D3ANIM_COPY3(a->frames[1].packed, dframe->packed, a->nverts * 3);
                a->frames[1].base.x = dframe->base.x;
                a->frames[1].base.y = dframe->base.y;
                a->frames[1].base.z = dframe->base.z;
                a->frames[1].scale.x = dframe->scale.x;
                a->frames[1].scale.y = dframe->scale.y;
                a->frames[1].scale.z = dframe->scale.z;
            }
        }
        a->prev = d3anim_Tail.prev;
        d3anim_Tail.prev = a;
        a->next = &d3anim_Tail;
        a->prev->next = a;
    }
    return a;
}
