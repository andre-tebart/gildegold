/* 0x431ae0, poses the child objects (bones) from the skeleton animations of the object's slots, weighted;
   without a running animation, the bones get the mesh's rest pose.
   @deep B draft, 1158/1712 bytes: 551 code hunks over the whole body - the frame is 0x35c vs the original's 0x34c and the bone-accumulator stack slots and expression order differ throughout, a deep matching job (D3BONEACC weighting layout plus scheduling), not a base meaning fix. Readability also needs Q1 (evidence for d3anim_PoseBones: poses the child objects from the skeleton animations of the object's slots) and Q2 (d3_Lerp3 lerp, nextFrame/subFrame/animCtl). */

typedef struct D3BONEACC {          /* 0xa4 bytes, a bone blended over the running skeleton animations */
    char name[0x40];
    int n;                          /* 0x40, animations added */
    D3VECTOR4 pos;                  /* 0x44 */
    D3VECTOR4 base;                 /* 0x54, rest position */
    D3VECTOR4 m[4];                 /* 0x64, rotation matrix (rows) */
} D3BONEACC;

void d3anim_PoseBones(D3OBJECT *obj)
{
    D3BONEACC acc[4];
    D3VECTOR4 m1[4], m0[4];
    D3VECTOR4 v, rot;
    D3ANIMCTL *ctl;
    D3ANIMSLOT *s;
    D3ANIMDATA *data;
    D3ANIMMESH *mesh;
    D3OBJECT *c;
    char *name;
    float t;
    int n, i, k, b, bone, r;

    if (obj && obj->material && obj->material->ops) {
        n = 0;
        if (((D3ANIMCTL *)obj->material->animCtl)->active) {
            D3_ZERO(acc, sizeof(acc));
            for (k = 0; k < 3; k++) {
                s = &((D3ANIMCTL *)obj->material->animCtl)->slots[k];
                if (data = s->data) {
                    for (b = 0; b < 4; b++) {
                        if (s->map[b] != 0xff) {
                            bone = s->map[b];
                            name = (char *)data + (bone + 1) * 0x40;
                            for (i = 0; i < n; i++)
                                if (!strcmp(acc[i].name, name))
                                    break;
                            if (i >= n) {
                                if (n < 4) {
                                    strcpy(acc[n].name, name);
                                    mesh = ((D3ANIMCTL *)obj->material->animCtl)->mesh;
                                    for (i = 0; i < mesh->nbones; i++) {
                                        if (!strcmp(mesh->bones[i].name, name)) {
                                            acc[n].base.x = mesh->bones[i].pos.x;
                                            acc[n].base.y = mesh->bones[i].pos.y;
                                            acc[n].base.z = mesh->bones[i].pos.z;
                                            break;
                                        }
                                    }
                                    t = (float)s->subFrame / data->frames[s->frame].count;
                                    d3_Lerp3((D3VECTOR4 *)&data->frames[s->frame].bone[bone].pos,
                                                 (D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].pos, t, &v);
                                    v.x -= acc[n].base.x;
                                    v.y -= acc[n].base.y;
                                    v.z -= acc[n].base.z;
                                    acc[n].pos.x = s->weight * v.x;
                                    acc[n].pos.y = s->weight * v.y;
                                    acc[n].pos.z = s->weight * v.z;
                                    acc[n].pos.x += acc[n].base.x;
                                    acc[n].pos.y += acc[n].base.y;
                                    acc[n].pos.z += acc[n].base.z;
                                    d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->frame].bone[bone].rot, (D3MATRIX *)m0);
                                    d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].rot, (D3MATRIX *)m1);
                                    for (r = 0; r < 3; r++)
                                        d3_Lerp3(&m0[r], &m1[r], t, &acc[n].m[r]);
                                    acc[n].n = 1;
                                    n++;
                                }
                            } else {
                                t = (float)s->subFrame / data->frames[s->frame].count;
                                d3_Lerp3((D3VECTOR4 *)&data->frames[s->frame].bone[bone].pos,
                                             (D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].pos, t, &v);
                                v.x -= acc[i].pos.x;
                                v.y -= acc[i].pos.y;
                                v.z -= acc[i].pos.z;
                                acc[i].pos.x += s->weight * v.x;
                                acc[i].pos.y += s->weight * v.y;
                                acc[i].pos.z += s->weight * v.z;
                                d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->frame].bone[bone].rot, (D3MATRIX *)m0);
                                d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].rot, (D3MATRIX *)m1);
                                for (r = 0; r < 3; r++) {
                                    d3_Lerp3(&m0[r], &m1[r], t, &v);
                                    acc[i].m[r].x += v.x;
                                    acc[i].m[r].y += v.y;
                                    acc[i].m[r].z += v.z;
                                }
                                acc[i].n++;
                            }
                        }
                    }
                }
            }
            if (n) {
                for (i = 0; i < n; i++) {
                    t = 1.0f / acc[i].n;
                    for (r = 0; r < 3; r++) {
                        acc[i].m[r].x *= t;
                        acc[i].m[r].y *= t;
                        acc[i].m[r].z *= t;
                    }
                    d3_MatToAngles(acc[i].m, &rot);
                    for (c = obj->child; c; c = c->next) {
                        if (c->material && !strcmp(acc[i].name, (char *)c->material + 0xb4)) {
                            d3_SetObjectPos(c, (D3VECTOR *)&acc[i].pos);
                            d3_SetObjectRot(c, (D3VECTOR *)&rot);
                        }
                    }
                }
                return;
            }
        }
        mesh = ((D3ANIMCTL *)obj->material->animCtl)->mesh;
        for (c = obj->child; c; c = c->next) {
            if (c->material && *(name = (char *)c->material + 0xb4)) {
                for (i = 0; i < mesh->nbones; i++)
                    if (!strcmp(mesh->bones[i].name, name))
                        break;
                if (i < mesh->nbones) {
                    d3_SetObjectPos(c, &mesh->bones[i].pos);
                    d3_SetObjectRot(c, &mesh->bones[i].rot);
                }
            }
        }
    }
}
