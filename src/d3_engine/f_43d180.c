/* 0x43d180, copies an object (name + "_"), without its children. @deep Q2 only, the body is byte: D3OBJX.unk09c (10 x 6 ints at 0x9c) has no evidenced meaning anywhere - zeroed in d3_InitObject, D3E_COPY3'd here and serialized for object types 2/3 (d3_int_io.c 0x44abb0 reader / d3_interface.c writer), no reader of any value in the exe (all 0x9c accesses in the Ghidra exports checked). The other five fields have views for the deep rename: unk058 = D3LOBJ.flickerEnd (d3_light.h 0x58), unk05c = D3LOBJ.color / D3IFLIGHT.color (light colour), unk06c = the pivot (XOBJ.p6c, d3_PointToWorld subtracts/adds it around the parent matrix), unk078 = D3OBJECT.posOffset, unk090 = D3LOBJ.range/power/atten. */

D3OBJECT *d3_CloneObject(D3OBJECT *src)
{
    D3OBJECT *obj = 0;

    if (src) {
        obj = mem_Alloc(0x228, "d3:CloneObject");
        if (src->type >= 5)
            D3_X(obj)->light = mem_Alloc(0x1ac, "d3:CloneObject(lightinfo)");
        d3_InitObject(obj);
        strcpy(obj->name, src->name);
        strcat(obj->name, "_");
        D3_X(obj)->pos.x = D3_X(src)->pos.x;
        D3_X(obj)->pos.y = D3_X(src)->pos.y;
        D3_X(obj)->pos.z = D3_X(src)->pos.z;
        D3_X(obj)->unk058 = D3_X(src)->unk058;
        D3_X(obj)->unk05c.x = D3_X(src)->unk05c.x;
        D3_X(obj)->unk05c.y = D3_X(src)->unk05c.y;
        D3_X(obj)->unk05c.z = D3_X(src)->unk05c.z;
        D3_X(obj)->radius = D3_X(src)->radius;
        D3_X(obj)->rot.x = D3_X(src)->rot.x;
        D3_X(obj)->rot.y = D3_X(src)->rot.y;
        D3_X(obj)->rot.z = D3_X(src)->rot.z;
        D3_X(obj)->unk090.x = D3_X(src)->unk090.x;
        D3_X(obj)->unk090.y = D3_X(src)->unk090.y;
        D3_X(obj)->unk090.z = D3_X(src)->unk090.z;
        D3_X(obj)->unk06c.x = D3_X(src)->unk06c.x;
        D3_X(obj)->unk06c.y = D3_X(src)->unk06c.y;
        D3_X(obj)->unk06c.z = D3_X(src)->unk06c.z;
        D3_X(obj)->unk078.x = D3_X(src)->unk078.x;
        D3_X(obj)->unk078.y = D3_X(src)->unk078.y;
        D3_X(obj)->unk078.z = D3_X(src)->unk078.z;
        D3_X(obj)->path = 0;
        D3_X(obj)->info = 0;
        if (D3_X(src)->light)
            D3E_COPY3(D3_X(obj)->light, D3_X(src)->light, 0x188);
        D3E_COPY3(D3_X(obj)->unk09c, D3_X(src)->unk09c, 0xf0);
        D3E_COPY3(&obj->state, &src->state, 0x18);
        if (src->material) {
            d3_AddObjectDrawData(obj);
            D3_MESH(obj)->groundOffsetRaw = D3_MESH(src)->groundOffsetRaw;
            D3_MESH(obj)->lightScale = D3_MESH(src)->lightScale;
            if (D3_MESH(src)->nsurf > 0)
                d3_AttachMesh(obj, 0, (char *)D3_MESH(src)->surf[0].info.mesh);
        }
        obj->objectFlags &= 0x9f;
        obj->state = obj->state & 0xfc | 4;
        d3_SetObjectPos(obj, &D3_X(obj)->pos);
        d3_SetObjectRot(obj, &D3_X(obj)->rot);
    }
    return obj;
}
