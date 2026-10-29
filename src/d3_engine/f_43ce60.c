/* 0x43ce60 @deep B3 draft: the whole original 0x43ce60..0x43cf95 was read; only the same instructions in another schedule/encoding remain. All 6 orders of the state/stateFlags/flags RMWs were built (state,stateFlags,flags = 10/336 B best; a stateFlags-first order explodes to 233+ B). Left: or cl,0x65 and and bl,0xc1 swapped, the 0x212 store before 0x210, and the light-entry loop addressed as [eax+ebx-4]/[eax+ebx-8] (original) vs [ebx+eax-4]/[ebx+eax-8] (ours); i[e], (e+i)-> and byte-offset spellings were tried. Readability debt left: Q2 on the D3OBJX/D3LOPS fields (offTime unk044 unk06c unk078 unk09c unk198 unk19c unk1a0 unk1a4, light e[] unk30/unk34), Q6. */

void d3_InitObject(D3OBJECT *obj)
{
    unsigned int i;

    for (i = 0; i < 10; i++) {
        D3_X(obj)->unk09c[i].v[0] = 0;
        D3_X(obj)->unk09c[i].v[1] = 0;
        D3_X(obj)->unk09c[i].v[2] = 0;
        D3_X(obj)->unk09c[i].v[3] = 0;
        D3_X(obj)->unk09c[i].v[4] = 0;
        D3_X(obj)->unk09c[i].v[5] = 0;
    }
    obj->objectFlags = obj->objectFlags & 0xa2 | 2;
    D3_X(obj)->unk214 = 0;
    obj->prev = 0;
    obj->next = 0;
    obj->child = 0;
    obj->parent = 0;
    obj->sound = 0;
    obj->state = obj->state & 0x18 | 0x65;
    obj->stateFlags &= 0xc1;
    obj->flags &= 0xcf;
    obj->type = 1;
    obj->kind = 0;
    obj->mask = 0;
    D3_X(obj)->offTime = 0;
    D3_X(obj)->path = 0;
    D3_X(obj)->events = 0;
    D3_X(obj)->unk044 = 0;
    if (D3_X(obj)->light) {
        D3_X(obj)->light->unk198 = 0;
        D3_X(obj)->light->unk1a0 = 15;
        D3_X(obj)->light->unk19c = 0.3f;
        D3_X(obj)->light->unk1a4 = 0;
        for (i = 0; i < 7; i++) {
            D3_X(obj)->light->e[i].unk34 = 15;
            D3_X(obj)->light->e[i].unk30 = 0.3f;
        }
    }
    D3_X(obj)->unk06c.x = 0;
    D3_X(obj)->unk06c.y = 0;
    D3_X(obj)->unk06c.z = 0;
    D3_X(obj)->unk078.x = 0;
    D3_X(obj)->unk078.y = 0;
    D3_X(obj)->unk078.z = 0;
    d3_SetObjectRotXYZ(obj, 0, 0, 0);
    d3_SetObjectPosXYZ(obj, 0, 0, 0);
}
