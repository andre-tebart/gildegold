/* 0x44a2c0, writes obj (if it is saved: objectFlags bit 1), then its first saved child and its next saved
   sibling, recursively ("wr_object")
   @deep Q2/Q4: read the function against its mirror reader rd_object (d3_int_io.c 0x44abb0) and the other users of every field. Q2 fields with no evidenced meaning anywhere: D3OBJX.unk09c (10 x 6 ints, only zeroed in d3_InitObject, D3E_COPY3'd in d3_CloneObject and serialized for types 2/3, no reader of the values), D3OBJX.unk214 (serialized for mesh types, only init 0 and shadow_fake = 3), D3LIGHTKEYS.unk19c / unk1a0 and D3LIGHTKEY.unk30 / unk34 (written/read by the scene file only, no other use); unk05c/unk090 do have views (D3IFLIGHT, light colour / range / power / atten), the rest do not. Q4: 9 x *(int *)&float serialization casts (radius, the three light params, three key params, pad200) - a typed union view has not been tried; both need match-deep */

void d3_WriteObject(VFSFILE *f, D3OBJECT *obj)
{
    char buf[256];
    char *p;
    D3OBJECT *c;
    int type, i;

    bio_wr_c(f, obj->objectFlags >> 1 & 1);
    if (obj->objectFlags & 2) {
        type = D3_X(obj)->type;
        if (type == 1 && (type = D3_X(obj)->type0, obj->name[0] == '!'))
            bio_wr_s(f, obj->name + 1);
        else
            bio_wr_s(f, obj->name);
        bio_wr_i(f, *(int *)obj->pad200);
        bio_wr_i(f, obj->kind);
        bio_wr_i(f, type);
        bio_wr_c(f, D3_X(obj)->type == 1);
        switch (type) {
        case 1:
        case 4:
            bio_wr_c(f, obj->state >> 7);
            bio_wr_c(f, obj->state >> 5 & 1);
            bio_wr_c(f, obj->objectFlags >> 2 & 1);
            bio_wr_c(f, obj->stateFlags >> 2 & 3);
            bio_wr_c(f, obj->stateFlags >> 4 & 1);
            bio_wr_c(f, obj->stateFlags >> 5 & 1);
            bio_wr_c(f, obj->stateFlags >> 6 & 1);
            bio_wr_i(f, D3_X(obj)->unk214);
            if (D3_X(obj)->mesh) {
                bio_wr_i(f, ((unsigned char *)D3_X(obj)->mesh)[0x96c]);
                if (((unsigned char *)D3_X(obj)->mesh)[0x96c] > 0) {
                    strcpy(buf, *(char **)((char *)D3_X(obj)->mesh + 0x104));
                    p = strrchr(buf, '_');
                    if (p && strlen(p) == 2 && isdigit(p[1]))
                        *p = 0;
                    bio_wr_s(f, buf);
                }
            } else
                bio_wr_i(f, 0);
        case 2:
        case 3:
            bio_wr_v3(f, (int *)&D3_X(obj)->pos);
            bio_wr_v3(f, (int *)&D3_X(obj)->rot);
            bio_wr_c(f, obj->objectFlags & 1);
            if (obj->objectFlags & 1) {
                bio_wr_v3(f, (int *)&D3_X(obj)->unk05c);
                bio_wr_v3(f, (int *)&D3_X(obj)->unk090);
            }
            for (i = 0; i < 10; i++) {
                bio_wr_v3(f, &D3_X(obj)->unk09c[i].v[0]);
                bio_wr_v3(f, &D3_X(obj)->unk09c[i].v[3]);
            }
            break;
        case 0:
            bio_wr_c(f, obj->objectFlags & 1);
            bio_wr_i(f, *(int *)&D3_X(obj)->radius);
            bio_wr_v3(f, (int *)&D3_X(obj)->pos);
            bio_wr_v3(f, (int *)&D3_X(obj)->unk05c);
            bio_wr_v3(f, (int *)&D3_X(obj)->unk090);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            bio_wr_c(f, obj->state >> 5 & 1);
            bio_wr_c(f, obj->objectFlags >> 4 & 1);
            bio_wr_i(f, *(int *)&D3_X(obj)->unk090.x);
            bio_wr_i(f, *(int *)&D3_X(obj)->unk090.y);
            bio_wr_i(f, *(int *)&D3_X(obj)->unk090.z);
            bio_wr_v3(f, (int *)&D3_X(obj)->unk05c);
            bio_wr_v3(f, (int *)&D3_X(obj)->pos);
            bio_wr_v3(f, (int *)&D3_X(obj)->rot);
            bio_wr_i(f, D3O_LIGHT(obj)->unk19c);
            bio_wr_i(f, D3O_LIGHT(obj)->unk1a0);
            for (i = 0; i < 7; i++) {
                bio_wr_i(f, *(int *)&D3O_LIGHT(obj)->k[i].range);
                bio_wr_i(f, *(int *)&D3O_LIGHT(obj)->k[i].intensity);
                bio_wr_i(f, *(int *)&D3O_LIGHT(obj)->k[i].atten);
                bio_wr_v3(f, (int *)&D3O_LIGHT(obj)->k[i].color);
                bio_wr_v3(f, (int *)&D3O_LIGHT(obj)->k[i].pos);
                bio_wr_v3(f, (int *)&D3O_LIGHT(obj)->k[i].rot);
                bio_wr_i(f, D3O_LIGHT(obj)->k[i].unk30);
                bio_wr_i(f, D3O_LIGHT(obj)->k[i].unk34);
            }
            break;
        default:
            err_Message("Unknown Object-Type in wr_object()!");
        }
    }
    for (c = obj->child; c; c = c->next)
        if (c->objectFlags & 2)
            break;
    bio_wr_c(f, c != 0);
    if (c)
        d3_WriteObject(f, c);
    for (c = obj->next; c; c = c->next)
        if ((c->state & 1) != 1 && (c->objectFlags & 2))
            break;
    bio_wr_c(f, c != 0);
    if (c)
        d3_WriteObject(f, c);
    d3e_SaveEvents(f, (D3EVENT *)D3_X(obj)->events);
}
