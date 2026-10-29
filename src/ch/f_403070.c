/* 0x403070, name is <...>_<body>[_<variant>] */

int ch_CreateMesh(CHARACTER *ch, char *name, D3VECTOR *pos)
{
    D3VECTOR4 rot = {0.0f, 0.0f, 0.0f, 0.0f};
    char buf[0x100];
    char msg[0x200];
    char *p, *q;

    if (!ch)
        return 0;
    ch->obj = d3_CreateObject(0, pos, (D3VECTOR *)&rot, name);
    ch->scriptId = -1;
    if (!ch->obj) {
        sprintf(msg, "ch_CreateMesh(): Could not load 3D-Character: %s", name);
        return 0;
    }
    strcpy(buf, name);
    if (p = strrchr(buf, '_')) {
        p++;
        if (q = strrchr(p, '_'))
            *q = 0;
        strcpy(ch->body, p);
        p[-1] = 0;
        strcpy(ch->fullname, buf);
    } else
        strcpy(ch->body, "");
    ch->scene = d3_DefaultScene;
    ch->obj->stateFlags |= 0xc;
    ch->obj->objectFlags &= ~2;
    ch->obj->kind = 2;
    ch->obj->mask = 1;
    ch->obj->flags &= ~4;
    D3MATERIAL_UNK958(ch->obj->material) = 1.8f;
    ch->unk1a0 = 1.0f;
    ch->group = -1;
    ch->scriptGroup = -1;
    strcpy(ch->name, name);
    ch->obj->objectFlags |= 4;
    if (!d3t_SceneIndex(ch->scene))
        D3OBJ_BITS(ch->obj, 0x211)->b3 = ch_ShadowEnabled;
    else
        D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
    D3OBJ_UNK048(ch->obj) = 0x3000000;
    ch_PlaceOnGround(ch, 0);
    d3_RelightObject(ch->obj);
    d3_UpdateObject(ch->obj, 1);
    ch->kind = 1;
    if (strstr(ch->name, "RATTE") || strstr(ch->name, "HUND") || strstr(ch->name, "KATZE")
        || strstr(ch->name, "PFERD")) {
        ch->kind = 2;
        if (strstr(ch->name, "RATTE")) {
            D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
            ch->obj->objectFlags &= ~4;
        }
        if (strstr(ch->name, "PFERD")) {
            ch->kind |= 8;
            D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
        }
    }
    ch_PreloadAnims(ch, 2, "bewegung/gehen", "stehen/stehen_newnoise");
    if (ch_LowPoly && ch->lowPolyBody) {
        ch_LoadLowAnims(ch, 1, "gehen");
        ch_UpdateLowPoly(ch);
    }
    ch->flags.unk12 = 1;
    return 1;
}
