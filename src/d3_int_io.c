/* d3_int_io.c -- scene file reader ("d3_int_io:" allocation tag, "rd_object()" in an error message): the object
   tree (0x44abb0) and the floor (0x44b920). Its own TU (see d3_obj.c). The next function,
   d3_LoadScene (0x44be60), calls both. Names are ours except rd_object.
   unit of rea 8cef7d5b (src/game/d3_int_io.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#define d3_SetParent d3_SetParent_void      /* d3.h declares it void; it returns char (d3_interface.h) */
#include "d3.h"
#undef d3_SetParent
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_interface.h"
#include "d3_int_io.h"
#include "bio.h"
#include "d3_io_t.h"
#include "d3_sm.h"
#include "err.h"
#include "mem.h"
#include "ts_texture.h"
#include "vfs.h"
/* 0xfc2ffc is the next field of the 0xfc2e00 head sentinel (0xfc2e00 + 0x1fc), not a separate global:
   d3io_LoadObjectFile links the first stock object there. */
/* err.h's other declarations shift the /O2 register allocation of the byte-matched d3_LoadScene (3 bytes at
   0x44c0df); the file only calls err_Message. Its own prototype keeps the call typed, where a local copy of
   the whole header does not compile byte-identically. */
void d3_SetFog(unsigned long start, unsigned long end, unsigned long color);  /* 0x43a6f0, d3_engine.c; @original: the reader passes the raw dwords of the two floats (no int -> float conversion; the original pushes them unchanged) */
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
    int owner = 0;
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
            bio_rd_i(f, &owner);
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
        obj->owner = (void *)owner;
        obj->kind = kind;
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
            obj->meshMode = n;
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
                bio_rd_v3(f, (int *)&obj->savedPos);
                bio_rd_v3(f, (int *)&obj->savedRot);
            }
            if (version >= 0x3a6c00af) {
                for (i = 0; i < 10; i++) {
                    bio_rd_v3(f, (int *)&obj->views[i].pos);
                    bio_rd_v3(f, (int *)&obj->views[i].dir);
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
            bio_rd_v3(f, (int *)&obj->savedPos);
            bio_rd_v3(f, (int *)&obj->savedRot);
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
                bio_rd_i(f, (int *)&obj->lightParams.x);
                bio_rd_i(f, (int *)&obj->lightParams.y);
                bio_rd_i(f, (int *)&obj->lightParams.z);
                bio_rd_v3(f, (int *)&obj->lightColor);
                bio_rd_v3(f, (int *)&D3_X(obj)->pos);
                bio_rd_v3(f, (int *)&D3_X(obj)->rot);
                if (version >= 0x3a6c00ac) {
                    bio_rd_i(f, (int *)&D3_X(obj)->light->flicker);
                    bio_rd_i(f, &D3_X(obj)->light->interval);
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
                obj->lightParams.x = D3_X(obj)->rot.x;
                obj->lightParams.y = D3_X(obj)->rot.y;
                obj->lightParams.z = D3_X(obj)->rot.z;
                D3_X(obj)->rot.x = 0.0f;
                D3_X(obj)->rot.y = 0.0f;
                D3_X(obj)->rot.z = 0.0f;
                bio_rd_v3(f, (int *)&obj->lightColor);
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
                        D3IO_KEYS(obj)[i].f28 = obj->lightParams.x;
                        D3IO_KEYS(obj)[i].f24 = obj->lightParams.y;
                        D3IO_KEYS(obj)[i].f2c = obj->lightParams.z;
                        D3IO_KEYS(obj)[i].v00.x = D3_X(obj)->pos.x;
                        D3IO_KEYS(obj)[i].v00.y = D3_X(obj)->pos.y;
                        D3IO_KEYS(obj)[i].v00.z = D3_X(obj)->pos.z;
                        D3IO_KEYS(obj)[i].v18.x = obj->lightColor.x;
                        D3IO_KEYS(obj)[i].v18.y = obj->lightColor.y;
                        D3IO_KEYS(obj)[i].v18.z = obj->lightColor.z;
                        D3IO_KEYS(obj)[i].v0c.x = obj->lightParams.x;
                        D3IO_KEYS(obj)[i].v0c.y = obj->lightParams.y;
                        D3IO_KEYS(obj)[i].v0c.z = obj->lightParams.z;
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
        if (obj->kind == 1)
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
/* 0x44b920, reads the floor block and loads the map */

D3FLMAP *d3io_ReadFloor(void *f, unsigned int version)
{
    D3FLMAP *map = 0;
    D3FLDESC desc;
    char buf[0x100];
    D3TEXHANDLE *tex;
    D3VECTOR4 pos;
/* */
    int i;

    if (version >= 0x3a6c0009) {
        D3_ZERO(&desc, sizeof desc);
        bio_rd_s(f, desc.name);
        if (desc.name[0] && version >= 0x3a6c00ad) {
            bio_rd_i(f, &desc.size);
            bio_rd_array_quick(f, desc.size, (void **)&desc.height);
            if (version >= 0x3a6c00bb)
                bio_rd_array_quick(f, desc.size * 4, &desc.lightofs);
        }
        if (version >= 0x3a6c00aa) {
            char c;
            bio_rd_c(f, &c);
            if (c) {
                bio_rd_i(f, &desc.nregions);
                if (desc.nregions > 0) {
                    if (version >= 0x3a6c00ad)
                        bio_rd_array_quick(f, desc.size, (void **)&desc.water);
                    desc.regions = mem_Alloc(desc.nregions * sizeof(D3FLREGION), "d3_int_io:FloorWaterRegions");
                    for (i = 0; i < desc.nregions; i++) {
                        char name[0x40];
                        char c1, c2, c3;
                        D3IOFLAGS fl;
                        tex = 0;
                        bio_rd_s(f, name);
                        bio_rd_c(f, &c1);
                        bio_rd_c(f, &c2);
                        bio_rd_c(f, &c3);
                        if (name[0]) {
                            fl.all = 0;
                            fl.s.b0 = 0x7f;
                            tex = D3IO_TEXLOAD(name, fl, (c2 & 0xf) << 2 | c3 & 1, 0);
                            if (tex)
                                tex_Reload(tex, 0);
                        }
                        fl.all = 0;
                        bio_rd_c(f, &c);
                        fl.s.b0 = c;
                        bio_rd_c(f, &c);
                        fl.s.b1 = c;
                        bio_rd_c(f, &c);
                        fl.s.c0 = c;
                        bio_rd_c(f, &c);
                        fl.s.c1 = c;
                        desc.regions[i].tex2 = (D3FLTEX *)tex;
                        desc.regions[i].tex = (D3FLTEX *)tex;
                        desc.regions[i].flags = fl.f;
                        if (version >= 0x3a6c00b6)
                            bio_rd_i(f, (int *)&desc.regions[i].f14);
                        else
                            desc.regions[i].f14 = 63.0f;
                        bio_rd_i(f, (int *)&desc.regions[i].scrollSpeedU);
                        bio_rd_i(f, (int *)&desc.regions[i].scrollSpeedV);
                        bio_rd_v4(f, (int *)desc.regions[i].speed);
                        if (version < 0x3a6c00ac) {
                            desc.regions[i].speed[0] *= 1.0f / 3.0f;
                            desc.regions[i].speed[1] *= 1.0f / 3.0f;
                            desc.regions[i].speed[2] *= 1.0f / 3.0f;
                            desc.regions[i].speed[3] *= 1.0f / 3.0f;
                        }
                        bio_rd_v4(f, (int *)desc.regions[i].amp);
                        if (version >= 0x3a6c00b9) {
                            int water;
                            bio_rd_i(f, &water);
                            desc.regions[i].water = water;
                        } else
                            desc.regions[i].water = 0;
                    }
                }
            }
        }
        bio_rd_s(f, desc.texname);
        if (version >= 0x3a6c00ad && desc.texname[0])
            bio_rd_array_quick(f, desc.size, (void **)&desc.types);
        if (version <= 0x3a6c000f)
            for (i = 0; i < 5; i++)
                bio_rd_s(f, buf);
        if (version >= 0x3a6c00b0) {
            for (i = 0; i < 8; i++)
                bio_rd_s(f, desc.typenames[i]);
        } else {
            for (i = 0; i < 6; i++)
                bio_rd_s(f, desc.typenames[i]);
        }
        bio_rd_i(f, (int *)&desc.scale);
        bio_rd_i(f, (int *)&desc.hscale);
        if (version <= 0x3a6c000f)
            for (i = 0; i < 0x100; i++)
                bio_rd_s(f, buf);
        map = d3fl_LoadMap(&desc);
        if (version >= 0x3a6c000e) {
            bio_rd_v3(f, (int *)&pos);
            if (map) {
                map->orgx = pos.x;
                map->orgy = pos.y;
                map->orgz = pos.z;
            }
        }
        if (desc.regions) {
            mem_Free(desc.regions);
            desc.regions = 0;
        }
        if (desc.types) {
            mem_Free(desc.types);
            desc.types = 0;
        }
        if (desc.water) {
            mem_Free(desc.water);
            desc.water = 0;
        }
        if (desc.height) {
            mem_Free(desc.height);
            desc.height = 0;
        }
        if (desc.lightofs) {
            mem_Free(desc.lightofs);
            desc.lightofs = 0;
        }
    } else
        err_Message("Incompatible Floor-Versions... Sorry!");
    return map;
}
/* 0x44be60, loads a scene file (or reads it from f): light and fog sets, the objects, the floor, the camera
   and the scene events */

char d3_LoadScene(char *name, void *f, D3IOPROGRESS progress)
{
    int x;
    int version;
    int n, nsets;
    char c;
    float t;
    D3VECTOR4 v;
    int col;
    int fcol;
    int end, start;
    char cam[0x100];
    void *in;
    D3OBJECT *o, *b;

    d3_LodMissing = 0;
    if (f)
        in = f;
    else
        in = vfs_Tfopen(name, "rb", vfs_Root);
    if (in) {
        bio_rd_i(in, &version);
        if ((version & 0xffff0000) == 0x3a6c0000 && version >= 0x3a6c0001) {
            d3_Busy++;
            if (version >= 0x3a6c000b) {
                bio_rd_s(in, cam);
                bio_rd_i(in, (int *)&t);
                bio_rd_v3(in, (int *)&v);
                d3if_Gray = t;
                d3_AmbientColor.x = v.x;
                d3_AmbientColor.y = v.y;
                d3_AmbientColor.z = v.z;
                if (version >= 0x3a6c00b5) {
                    bio_rd_v3(in, (int *)&v);
                    d3_SunColor.x = v.x;
                    d3_SunColor.y = v.y;
                    d3_SunColor.z = v.z;
                }
                if (version >= 0x3a6c00b3) {
                    x = 0;
                    if (version >= 0x3a6c00b7)
                        bio_rd_i(in, &x);
                    ((D3IFSCENE *)d3_DefaultScene)->mapSize = x;
                    bio_rd_i(in, &col);
                    bio_rd_i(in, &start);
                    bio_rd_i(in, &end);
                    d3_SetFog(start, end, col);
                }
                for (x = 0; x < 7; x++) {
                    d3if_Levels[x].amb.x = d3_AmbientColor.x;
                    d3if_Levels[x].amb.y = d3_AmbientColor.y;
                    d3if_Levels[x].amb.z = d3_AmbientColor.z;
                    d3if_Levels[x].sun.x = 0;
                    d3if_Levels[x].sun.y = 0;
                    d3if_Levels[x].sun.z = 0;
                }
                if (version >= 0x3a6c00a2) {
                    nsets = d3_SceneVersion(version);
                    for (x = 0; x < nsets; x++) {
                        bio_rd_v3(in, (int *)&v);
                        d3if_Levels[x].amb.x = v.x;
                        d3if_Levels[x].amb.y = v.y;
                        d3if_Levels[x].amb.z = v.z;
                        if (version >= 0x3a6c00b3) {
                            if (version >= 0x3a6c00b5) {
                                bio_rd_v3(in, (int *)&v);
                                d3if_Levels[x].sun.x = v.x;
                                d3if_Levels[x].sun.y = v.y;
                                d3if_Levels[x].sun.z = v.z;
                            }
                            for (n = 0; n < 6; n++) {
                                bio_rd_i(in, &fcol);
                                bio_rd_i(in, (int *)&t);
                                d3if_Levels[x].fog[n].col.col = fcol;
                                d3if_Levels[x].fog[n].start = t;
                                bio_rd_i(in, (int *)&t);
                                d3if_Levels[x].fog[n].end = t;
                            }
                        }
                    }
                }
                if (d3if_MegaCam)
                    d3_DisposeObject((D3OBJECT *)d3if_MegaCam);
                bio_rd_i(in, &n);
                for (x = 0; x < n; x++) {
                    rd_object(in, 0, version, 0);
                    if (progress)
                        progress(0, x, n);
                }
                bio_rd_c(in, &c);
                if (c)
                    d3_Terrain = d3io_ReadFloor(in, version);
                for (o = d3_TopFirst; o != &d3_ObjEnd; o = o->next)
                    if (o->type == 3 && !_strcmpi(cam, o->name)) {
                        d3if_MegaCam = (int)o;
                        if (o->objectFlags & 1) {
                            d3_SetObjectPos(o, &o->savedPos);
                            d3_SetObjectRot(o, &o->savedRot);
                        }
                        d3_SetCamera(o);
                        break;
                    }
                if (version >= 0x3a6c00a7)
                    d3e_LoadSceneEvents(in);
            } else
                err_Message("Scene too old... Sorry!");
            if (!f)
                vfs_fclose(in);
            if (version < 0x3a6c00b1 && d3_Terrain) {
                for (o = d3_TopFirst; o != &d3_ObjEnd; o = d3_NextGroup(o))
                    if (o->type == 7) {
                        b = d3_CloneObject(o);
                        b->objectFlags |= 0x10;
                        strcat(b->name, "boden");
                        d3_LinkTop(b);
                        break;
                    }
            }
            if (!d3_Terrain) {
                if (d3sm_Shadow)
                    d3sm_Build(d3sm_Shadow, 0);
                else
                    d3sm_Shadow = d3sm_Create(0, -d3s_ShadowMapSize, 1);
            } else if (d3sm_Shadow) {
                d3sm_Destroy(d3sm_Shadow);
                d3sm_Shadow = 0;
            }
            if (!((D3IFSCENE *)d3_DefaultScene)->mapSize)
                ((D3IFSCENE *)d3_DefaultScene)->mapSize = d3_Terrain ? 0x80 : 0x40;
            if (d3_Terrain)
                d3_DefaultScene->map = d3_Terrain;
            tex_ReloadAll(0, (int)progress);
            d3_Busy--;
            if (name)
                strncpy(d3_DefaultScene->name, name, 0x7f);
            d3_DefaultScene->loaded = 1;
            ((D3IFSCENE *)d3_DefaultScene)->sceneFlags = 0;
            return 1;
        }
        if (!f)
            vfs_fclose(in);
    }
    return 0;
}
/* 0x44c3f0, loads an object group file (or reads it from f) below parent; returns the first object */

D3OBJECT *d3_LoadGroup(char *name, void *f, D3OBJECT *parent)
{
    D3OBJECT *first = 0;
    D3OBJECT *obj;
    void *in;
    int version;
    int n, i;

    if (f)
        in = f;
    else
        in = vfs_Tfopen(name, "rb", vfs_Root);
    if (in) {
        bio_rd_i(in, &version);
        if ((version & 0xffff0000) == 0x3a6c0000) {
            if (version >= 0x3a6c00ae) {
                d3_LodMissing = 0;
                bio_rd_i(in, &n);
                for (i = 0; i < n; i++) {
                    obj = rd_object(in, parent, version, 1);
                    if (first && obj && parent)
                        d3_AppendSibling(first, obj);
                    if (obj && !first)
                        first = obj;
                }
            } else
                first = rd_object(in, parent, version, 1);
            if (!f)
                vfs_fclose(in);
            if (first && parent)
                d3_AddChild(parent, first);
        } else if (!f)
            vfs_fclose(in);
    }
    return first;
}
/* 0x44c510, sets the directory the stock objects are searched in */

void d3io_SetStockDir(char *path)
{
    d3io_StockDir = vfs_FindDir(path, vfs_Root);
}
/* 0x44c530, finds the file of a stock object ("*" name ext) in the stock directory */

char *d3io_StockFile(char *name, char *ext)
{
    char buf[0x100];

    strcpy(buf, "*");
    strcat(buf, name);
    strcat(buf, ext);
    return vfs_GetPath(buf, d3io_StockDir, vfs_Root, d3io_StockPath);
}
/* 0x44c5d0, the loaded stock object of that name */

int d3io_FindStock(char *name)
{
    D3BGF *s;

    for (s = d3_StockListStart.next; s != &d3_StockListEnd; s = s->next)
        if (!_strnicmp(name, s->name, 0x3f))
            return (int)s;
    return 0;
}
/* 0x44c610, builds surf from the stock object `name`: vertices and triangles, vertex colours from the colour
   key, the texture set; counts the surface (unless it is the shadow surface 3)
   @deep spell.py menu: both hunks 'no menu rule' (44c737 add ecx,edx / mov esi,ecx vs lea esi,[ecx+edx*1]; 44c9cf is the alignment nop that follows from the 1-byte length difference), diffclass: code. Read the hunk against tools/disasm.py 0x44c720 0x40: same value, only the add/lea spelling differs - VC6 computed '(D3IOTEX *)d3_Textures + tri->src->tex' into a temp (ecx, the dead index register) and copied it to tex (esi) at the store. Tried this session: the double-assignment form 'tri->tex = tex = ...' (byte-identical, reverted). Already tried in the 2026-10-22 session (unit note): int ti = tri->src->tex before the test, ti + (D3IOTEX *)d3_Textures, a D3TEXTURE * local with casts at the uses, &((D3IOTEX *)d3_Textures)[i], tex += i, (int)d3_Textures + (i << 7) / * 0x80 - all identical or worse. Readability done: Q4 via D3IOCOL union (all / b,g,r,a) and D3IOVERT.col40/col44 typed D3IOCOL, D3IO_214 macro body now the typed field D3_X(o)->unk214, now meshMode (byte-neutral, check 2567). Left: the register-allocation choice that makes the sum's target esi instead of the dead operand - match-deep's toolbox. */

void d3io_AttachStock(D3OBJECT *obj, D3SURF *surf, char *name)
{
    D3IOCOL col;
    D3IOSTOCK *stock;
    D3IOSURF *s;
    D3IOTRI *tri;
    D3IOTEX *tex;
    D3MESH *mesh;
    char buf[0x100];
    int i, k;

    stock = (D3IOSTOCK *)d3io_FindStock(name);
    if (stock) {
        s = (D3IOSURF *)surf;
        s->nkeys = stock->npoints;
        s->ntris = stock->npolys;
        s->stock = stock;
        s->f194 = 0;
        d3_AllocObjectPolys((D3OBJINFO *)s);
        col.all = -1;
        if (s->nkeys > 0)
            for (i = 0; i < s->nkeys + 8; i++) {
                s->keys[i].src = stock->points + i * 0x18;
                s->keys[i].f4c = 0;
                s->keys[i].col40.all = -1;
                s->keys[i].col44.all = -1;
            }
        tri = s->tris;
        for (i = 0; i < s->ntris; i++, tri++) {
            tri->f20 = 0;
            tri->fl.b0 = 0;
            tri->f1c = 0;
            tri->f18 = 0;
            tri->src = (D3IOPOLY *)stock->polys + i;
            for (k = 0; k < 3; k++) {
                D3IOVERT *v = &s->keys[tri->src->idx[k]];
                tri->v[k] = v;
                v->f4c |= 0x80;
            }
            tri->f24 = 0;
            if (tri->src->tex >= 0) {
                tex = (D3IOTEX *)d3_Textures + tri->src->tex;
                tri->tex = tex;
                if (tex->colorkey < 0xff || (tex->flags & 3)) {
                    col.a = tex->colorkey;
                    if (tex->flags & 1) {
                        col.b = tex->colorkey;
                        col.g = tex->colorkey;
                        col.r = tex->colorkey;
                    } else {
                        col.b = 0xff;
                        col.g = 0xff;
                        col.r = 0xff;
                    }
                    for (k = 0; k < 3; k++) {
                        tri->v[k]->col40 = col;
                        tri->v[k]->col44 = col;
                    }
                }
                tri->fl.b3 = tex->f68;
                for (k = 0; k < 3; k++)
                    tri->v[k]->f4d = tex->f68 & 1;
            } else
                tri->tex = 0;
        }
        s->texSet = 0;
        s->texset = mem_Alloc(stock->ntex * 4, "d3_io:CurTexSet");
        for (i = 0; i < stock->ntex; i++)
            for (k = 0; k < s->ntris; k++)
                if (s->tris[k].src && s->tris[k].src->slot == i) {
                    if (s->tris[k].tex) {
                        s->texset[i] = s->tris[k].tex;
                        tex_AddRef((D3FLTEX *)s->texset[i], 1);
                    } else
                        sprintf(buf, "stock object %s, texture %s not found", stock, stock->texnames[i]);
                    break;
                }
        for (i = 0; i < stock->ntex; i++)
            if (!s->texset[i]) {
                strcpy(buf, "Error attaching Stock-Object \"");
                strcat(buf, stock->name);
                strcat(buf, "\": Not all Textures used/found!");
                break;
            }
        mesh = D3_MESH(obj);
        if (surf != &mesh->surf[3]) {
            mesh->nsurf++;
            D3IO_214(obj) = 4;
            if (!obj->info) {
                obj->info = d3_SelectSurf(obj);
                if (!obj->info) {
                    k = 0;
                    do {
                        if (k >= 3)
                            break;
                        if (D3_MESH(obj)->surf[k].info.ntris > 0 && D3_MESH(obj)->surf[k].info.nkeys > 0 &&
                            D3_MESH(obj)->surf[k].info.mesh)
                            obj->info = &D3_MESH(obj)->surf[k].info;
                        k++;
                    } while (!obj->info);
                }
            }
        }
        stock->refs++;
    }
}
/* 0x44c9d0, the file name(s) of a stock object's LOD: lod < 0 the "_s" variant, lod 0 the best one found
   (mode 2) or the base name, else "name_<lod - 1>" */

char d3io_LodName(char *name, char *name2, int lod, char *out, char *out2)
{
    if (lod < 0) {
        if (d3io_Lod.shadow) {
            strcpy(out, name);
            strcat(out, "_s");
            if (name2 && out2) {
                strcpy(out2, name2);
                strcat(out2, "_s");
            }
            return 1;
        }
    } else if (lod == 0) {
        if (d3io_Lod.mode == 2) {
            for (lod = 2; lod >= 0; lod--) {
                if (lod > 0) {
                    sprintf(out, "%s_%i", name, lod - 1);
                    if (name2 && out2)
                        sprintf(out2, "%s_%i", name2, lod - 1);
                } else {
                    strcpy(out, name);
                    if (name2 && out2)
                        strcpy(out2, name2);
                }
                if (d3io_StockFile(out, ".bgf"))
                    return 1;
            }
        } else {
            strcpy(out, name);
            if (name2 && out2)
                strcpy(out2, name2);
            return 1;
        }
    } else {
        int k = d3io_Lod.mode == 2 ? 2 - lod : lod;
        sprintf(out, "%s_%i", name, k - 1);
        if (name2 && out2)
            sprintf(out2, "%s_%i", name2, k - 1);
        return 1;
    }
    return 0;
}
/* 0x44cbb0, attaches the stock object `name` to obj: into surf, or (surf 0) its LODs into the mesh's
   surfaces 0..2 and the "_s" variant into surface 3 */

void d3_AttachMesh(D3OBJECT *obj, int unk, char *name)
{
    D3SURF *surf = (D3SURF *)unk;
    char buf[0x100];
    int i, n;

    if (surf) {
        if (d3io_FindStock(name)) {
            if (!obj->material)
                d3_AddObjectDrawData(obj);
            d3io_AttachStock(obj, surf, name);
        }
    } else {
        d3io_LodName(name, 0, 0, buf, 0);
        if (d3io_FindStock(buf)) {
            if (!obj->material)
                d3_AddObjectDrawData(obj);
            d3io_AttachStock(obj, &D3_MESH(obj)->surf[0], buf);
            if (d3io_LodName(name, 0, -1, buf, 0) && d3io_FindStock(buf))
                d3io_AttachStock(obj, &D3_MESH(obj)->surf[3], buf);
            if (d3io_Lod.mode == 1) {
                for (i = 1, n = 1; i < 3; i++) {
                    if (n >= 3)
                        break;
                    if (d3io_LodName(name, 0, i, buf, 0) && d3io_FindStock(buf)) {
                        d3io_AttachStock(obj, &D3_MESH(obj)->surf[n], buf);
                        n++;
                    }
                }
            }
        }
    }
}

char d3_LodMissing = 0;  /* 0x6339e4, set when an object LOD is missing */
D3IOLOD d3io_Lod = { 2 };  /* 0x5fbf78, which stock object LODs are loaded (d3_io_t.c and main.c include this header) */
char d3io_StockPath[256] = {0};  /* 0x633a00 */
