/* 0x4319e0, maps the bones of the object's skeleton animations to its child objects (by name). */

char d3anim_MapBones(D3OBJECT *obj)
{
    D3BONESLOT *bs;
    D3BONEANIM *anim;
    D3OBJECT *c;
    D3MATERIAL *mat;
    int i, k, n;

    if (obj && (mat = obj->material)) {
        for (i = 0; i < 3; i++) {
            bs = (D3BONESLOT *)&mat->tex[i];
            if (anim = bs->anim) {
                bs->mapAll = -1;
                n = 0;
                for (k = 0; n < 4; k++) {
                    if (k >= anim->nbones)
                        break;
                    for (c = obj->child; c; c = c->next)
                        if (!strcmp((char *)c->material + 0xb4, anim->names[k]))
                            bs->map[n++] = k;
                }
            }
        }
    }
    return 0;
}
