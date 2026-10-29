/* 0x44abb0, reads an object with its children and following siblings ("rd_object" in the error message) */
D3OBJECT *rd_object(void *f, D3OBJECT *parent, unsigned int version, char reload)
{
    D3OBJECT *obj = 0;
    char c;
    char dummy = 0;
    char bang;
    char sib;
    int n;
    int i, k;
    int kind = 0;
    int unk200 = 0;
    char name[0x100];
    char buf[0x100];
    int type, cnt;
    D3OPS *ops;
    void **tex;

    if (version >= 0x3a6c000b) {
    bio_rd_c(f, &c);
    if (c) {
        bio_rd_s(f, name);
        if (version >= 0x3a6c00b2)
            bio_rd_i(f, &unk200);
        if (version >= 0x3a6c00ab)
            bio_rd_i(f, &kind);
        bio_rd_i(f, &n);
        if (version < 0x3a6c00a6) {
            bang = 0;
            switch (n) {
            case 0:
                type = 0;
                break;
            case 1:
                type = 1;
                break;
            case 2:
                type = 3;
                break;
            case 3:
            case 4:
                type = 4;
                break;
            case 5:
                type = 5;
                break;
            case 6:
                type = 6;
                break;
            case 7:
                type = 7;
                break;
            case 8:
                type = 8;
                break;
            default:
                type = 2;
                break;
            }
        } else {
            type = n;
            bio_rd_c(f, &bang);
            if (name[0] == '!' && !bang) {
                strcpy(buf, name + 1);
                strcpy(name, buf);
            }
        }
        obj = d3_SpawnObject(type, name);
        type = obj->type;
        D3IO_200(obj) = unk200;
        D3_X(obj)->kind = kind;
        obj->parent = parent;
        if (parent)
            D3IO_FL(obj)->a0 = 0;
        switch (type) {
        case 1:
        case 4:
            bio_rd_c(f, &c);
            D3IO_FL(obj)->a7 = c;
            bio_rd_c(f, &c);
            D3IO_FL(obj)->a5 = c;
            if (version >= 0x3a6c000d) {
                bio_rd_c(f, &c);
                D3IO_FL(obj)->b2 = c;
                if (version >= 0x3a6c00a6) {
                    bio_rd_c(f, &c);
                    D3IO_FL(obj)->c23 = c;
                    if (version <= 0x3a6c00b8)
                        D3IO_FL(obj)->c23 |= (D3IO_FL(obj)->c23 & 1) << 1;
                    bio_rd_c(f, &c);
                    D3IO_FL(obj)->c4 = c;
                    if (version >= 0x3a6c00b4) {
                        bio_rd_c(f, &c);
                        D3IO_FL(obj)->c5 = c;
                    }
                    if (version >= 0x3a6c00b9) {
                        bio_rd_c(f, &c);
                        D3IO_FL(obj)->c6 = c;
                    }
                }
            }
            bio_rd_i(f, &n);
            D3_X(obj)->unk214 = n;
            bio_rd_i(f, &n);
            if (version >= 0x3a6c00a4) {
                if (n > 0) {
                    bio_rd_s(f, name);
                    if (d3io_LoadObject(name, name))
                        d3_AttachMesh(obj, 0, name);
                    else {
                        sprintf(buf, "Missing LOD for Object \"%s\": [\"%s.bgf\"].", obj->name, name);
                        d3_LodMissing = 1;
                    }
                }
            } else {
                if (!obj->material)
                    d3_AddObjectDrawData(obj);
                for (k = 0; k < n; k++) {
                    bio_rd_s(f, name);
                    d3io_LoadObject(name, name);
                    d3_AttachMesh(obj, (int)&D3_MESH(obj)->surf[k], name);
                }
                if (version >= 0x3a6c00a3) {
                    bio_rd_c(f, &c);
                    if (c) {
                        bio_rd_s(f, name);
                        d3io_LoadObject(name, name);
                        d3_AttachMesh(obj, (int)&D3_MESH(obj)->surf[3], name);
                    }
                } else if (D3_OPS(&D3_MESH(obj)->surf[0].info)) {
                    strcpy(name, (char *)D3_OPS(&D3_MESH(obj)->surf[0].info));
                    strcat(name, "_s");
                    if (d3io_LoadObject(name, name))
                        d3_AttachMesh(obj, (int)&D3_MESH(obj)->surf[3], name);
                }
            }
        case 2:
        case 3:
            bio_rd_v3(f, (int *)&D3_X(obj)->pos);
            bio_rd_v3(f, (int *)&D3_X(obj)->rot);
            bio_rd_c(f, &c);
            D3IO_FL(obj)->b0 = c;
            if (c) {
                bio_rd_v3(f, (int *)&D3_X(obj)->unk05c);
                bio_rd_v3(f, (int *)&D3_X(obj)->unk090);
            }
            if (version >= 0x3a6c00af) {
                for (i = 0; i < 10; i++) {
                    bio_rd_v3(f, &D3_X(obj)->unk09c[i].v[0]);
                    bio_rd_v3(f, &D3_X(obj)->unk09c[i].v[3]);
                }
            }
            break;
        case 0:
            bio_rd_c(f, &c);
            D3IO_FL(obj)->b0 = c;
            if (version < 0x3a6c00a0)
                D3IO_FL(obj)->b0 = 1;
            bio_rd_i(f, (int *)&D3_X(obj)->radius);
            bio_rd_v3(f, (int *)&D3_X(obj)->pos);
            bio_rd_v3(f, (int *)&D3_X(obj)->unk05c);
            bio_rd_v3(f, (int *)&D3_X(obj)->unk090);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            bio_rd_c(f, &c);
            D3IO_FL(obj)->a5 = c;
            if (version >= 0x3a6c00a8) {
                cnt = d3_SceneVersion(version);
                if (version >= 0x3a6c00a9) {
                    bio_rd_c(f, &c);
                    D3IO_FL(obj)->b4 = c;
                }
                bio_rd_i(f, (int *)&D3_X(obj)->unk090.x);
                bio_rd_i(f, (int *)&D3_X(obj)->unk090.y);
                bio_rd_i(f, (int *)&D3_X(obj)->unk090.z);
                bio_rd_v3(f, (int *)&D3_X(obj)->unk05c);
                bio_rd_v3(f, (int *)&D3_X(obj)->pos);
                bio_rd_v3(f, (int *)&D3_X(obj)->rot);
                if (version >= 0x3a6c00ac) {
                    bio_rd_i(f, (int *)&D3_X(obj)->light->unk19c);
                    bio_rd_i(f, &D3_X(obj)->light->unk1a0);
                }
                for (i = 0; i < cnt; i++) {
                    bio_rd_i(f, (int *)&D3IO_KEYS(obj)[i].f28);
                    bio_rd_i(f, (int *)&D3IO_KEYS(obj)[i].f24);
                    bio_rd_i(f, (int *)&D3IO_KEYS(obj)[i].f2c);
                    bio_rd_v3(f, (int *)&D3IO_KEYS(obj)[i].v18);
                    bio_rd_v3(f, (int *)&D3IO_KEYS(obj)[i].v00);
                    bio_rd_v3(f, (int *)&D3IO_KEYS(obj)[i].v0c);
                    if (version >= 0x3a6c00ac) {
                        bio_rd_i(f, (int *)&D3IO_KEYS(obj)[i].f30);
                        bio_rd_i(f, &D3IO_KEYS(obj)[i].f34);
                    }
                }
                for (; i < 7; i++) {
                    D3IO_KEYS(obj)[i].f28 = D3IO_KEYS(obj)[cnt - 1].f28;
                    D3IO_KEYS(obj)[i].f24 = D3IO_KEYS(obj)[cnt - 1].f24;
                    D3IO_KEYS(obj)[i].f2c = D3IO_KEYS(obj)[cnt - 1].f2c;
                    D3IO_KEYS(obj)[i].v18.x = D3IO_KEYS(obj)[cnt - 1].v18.x;
                    D3IO_KEYS(obj)[i].v18.y = D3IO_KEYS(obj)[cnt - 1].v18.y;
                    D3IO_KEYS(obj)[i].v18.z = D3IO_KEYS(obj)[cnt - 1].v18.z;
                    D3IO_KEYS(obj)[i].v00.x = D3IO_KEYS(obj)[cnt - 1].v00.x;
                    D3IO_KEYS(obj)[i].v00.y = D3IO_KEYS(obj)[cnt - 1].v00.y;
                    D3IO_KEYS(obj)[i].v00.z = D3IO_KEYS(obj)[cnt - 1].v00.z;
                }
            } else {
                bio_rd_i(f, (int *)&D3_X(obj)->rot.x);
                bio_rd_i(f, (int *)&D3_X(obj)->rot.y);
                if (version >= 0x3a6c000c)
                    bio_rd_i(f, (int *)&D3_X(obj)->rot.z);
                else
                    D3_X(obj)->rot.z = 1.0f;
                D3_X(obj)->unk090.x = D3_X(obj)->rot.x;
                D3_X(obj)->unk090.y = D3_X(obj)->rot.y;
                D3_X(obj)->unk090.z = D3_X(obj)->rot.z;
                D3_X(obj)->rot.x = 0.0f;
                D3_X(obj)->rot.y = 0.0f;
                D3_X(obj)->rot.z = 0.0f;
                bio_rd_v3(f, (int *)&D3_X(obj)->unk05c);
                bio_rd_v3(f, (int *)&D3_X(obj)->pos);
                if (version >= 0x3a6c00a1) {
                    cnt = d3_SceneVersion(version);
                    for (i = 0; i < cnt; i++) {
                        bio_rd_i(f, (int *)&D3IO_KEYS(obj)[i].f28);
                        bio_rd_i(f, (int *)&D3IO_KEYS(obj)[i].f24);
                        bio_rd_i(f, (int *)&D3IO_KEYS(obj)[i].f2c);
                        bio_rd_v3(f, (int *)&D3IO_KEYS(obj)[i].v18);
                        bio_rd_v3(f, (int *)&D3IO_KEYS(obj)[i].v00);
                    }
                    for (; i < 7; i++) {
                        D3IO_KEYS(obj)[i].f28 = D3IO_KEYS(obj)[cnt - 1].f28;
                        D3IO_KEYS(obj)[i].f24 = D3IO_KEYS(obj)[cnt - 1].f24;
                        D3IO_KEYS(obj)[i].f2c = D3IO_KEYS(obj)[cnt - 1].f2c;
                        D3IO_KEYS(obj)[i].v18.x = D3IO_KEYS(obj)[cnt - 1].v18.x;
                        D3IO_KEYS(obj)[i].v18.y = D3IO_KEYS(obj)[cnt - 1].v18.y;
                        D3IO_KEYS(obj)[i].v18.z = D3IO_KEYS(obj)[cnt - 1].v18.z;
                        D3IO_KEYS(obj)[i].v00.x = D3IO_KEYS(obj)[cnt - 1].v00.x;
                        D3IO_KEYS(obj)[i].v00.y = D3IO_KEYS(obj)[cnt - 1].v00.y;
                        D3IO_KEYS(obj)[i].v00.z = D3IO_KEYS(obj)[cnt - 1].v00.z;
                    }
                } else {
                    for (i = 0; i < 7; i++) {
                        D3IO_KEYS(obj)[i].f28 = D3_X(obj)->unk090.x;
                        D3IO_KEYS(obj)[i].f24 = D3_X(obj)->unk090.y;
                        D3IO_KEYS(obj)[i].f2c = D3_X(obj)->unk090.z;
                        D3IO_KEYS(obj)[i].v00.x = D3_X(obj)->pos.x;
                        D3IO_KEYS(obj)[i].v00.y = D3_X(obj)->pos.y;
                        D3IO_KEYS(obj)[i].v00.z = D3_X(obj)->pos.z;
                        D3IO_KEYS(obj)[i].v18.x = D3_X(obj)->unk05c.x;
                        D3IO_KEYS(obj)[i].v18.y = D3_X(obj)->unk05c.y;
                        D3IO_KEYS(obj)[i].v18.z = D3_X(obj)->unk05c.z;
                        D3IO_KEYS(obj)[i].v0c.x = D3_X(obj)->unk090.x;
                        D3IO_KEYS(obj)[i].v0c.y = D3_X(obj)->unk090.y;
                        D3IO_KEYS(obj)[i].v0c.z = D3_X(obj)->unk090.z;
                    }
                }
            }
            break;
        default:
            err_Message("Unkown Object-Type in rd_object()!");
            break;
        }
        d3_SetObjectRot(obj, &D3_X(obj)->rot);
        d3_SetObjectPos(obj, &D3_X(obj)->pos);
        if (D3_X(obj)->kind == 1)
            D3IO_FL(obj)->a5 = 0;
        D3IO_FL(obj)->b1 = 1;
        if (bang) {
            d3if_Show(obj, 0);
            if (obj->name[0] != '!') {
                strcpy(name, obj->name);
                strcpy(obj->name, "!");
                strcat(obj->name, name);
            }
        }
    } else {
        obj = d3_SpawnObject(1, "STRANGEFUCK");
        obj->parent = parent;
        if (parent)
            D3IO_FL(obj)->a0 = 0;
        dummy = 1;
        D3IO_FL(obj)->b1 = 0;
    }
    bio_rd_c(f, &c);
    if (c)
        d3_AddChild(obj, rd_object(f, obj, version, reload));
    if (!obj->parent && !dummy)
        d3_LinkTop(obj);
    bio_rd_c(f, &sib);
    if (sib)
        d3_AppendSibling(obj, rd_object(f, parent, version, reload));
    if (!c && !sib && dummy == 1) {
        mem_Free(obj);
        obj = 0;
    } else if (dummy)
        d3_LinkTop(obj);
    if (version >= 0x3a6c00a7)
        d3e_LoadEvents(f, obj);
    if (obj) {
        if (!obj->type || (k = D3_X(obj)->kind) == 4 || k == 3 || k == 2)
            D3_X(obj)->mask = -1;
        else
            D3_X(obj)->mask = 0;
        if (reload && obj->material) {
            for (i = 0; i < 3; i++) {
                if ((ops = D3_OPS(&D3_MESH(obj)->surf[i].info)) && (tex = (void **)D3_MESH(obj)->surf[i].texHandles)) {
                    cnt = ops->count;
                    for (n = 0; n < cnt; n++)
                        if (tex[n])
                            tex_Reload(tex[n], 0);
                }
            }
        }
    }
    }
    return obj;
}
