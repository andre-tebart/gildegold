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
    float end, start;
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
                d3if_AmbR = v.x;
                d3if_AmbG = v.y;
                d3if_AmbB = v.z;
                if (version >= 0x3a6c00b5) {
                    bio_rd_v3(in, (int *)&v);
                    d3if_Dir[0] = v.x;
                    d3if_Dir[1] = v.y;
                    d3if_Dir[2] = v.z;
                }
                if (version >= 0x3a6c00b3) {
                    x = 0;
                    if (version >= 0x3a6c00b7)
                        bio_rd_i(in, &x);
                    D3IO_SCENEB4(d3_DefaultScene) = x;
                    bio_rd_i(in, &col);
                    bio_rd_i(in, (int *)&start);
                    bio_rd_i(in, (int *)&end);
                    d3_SetFog(*(int *)&start, *(int *)&end, col);
                }
                for (x = 0; x < 7; x++) {
                    D3IO_SETS[x].amb.x = d3if_AmbR;
                    D3IO_SETS[x].amb.y = d3if_AmbG;
                    D3IO_SETS[x].amb.z = d3if_AmbB;
                    D3IO_SETS[x].dir.x = 0;
                    D3IO_SETS[x].dir.y = 0;
                    D3IO_SETS[x].dir.z = 0;
                }
                if (version >= 0x3a6c00a2) {
                    nsets = d3_SceneVersion(version);
                    for (x = 0; x < nsets; x++) {
                        bio_rd_v3(in, (int *)&v);
                        D3IO_SETS[x].amb.x = v.x;
                        D3IO_SETS[x].amb.y = v.y;
                        D3IO_SETS[x].amb.z = v.z;
                        if (version >= 0x3a6c00b3) {
                            if (version >= 0x3a6c00b5) {
                                bio_rd_v3(in, (int *)&v);
                                D3IO_SETS[x].dir.x = v.x;
                                D3IO_SETS[x].dir.y = v.y;
                                D3IO_SETS[x].dir.z = v.z;
                            }
                            for (n = 0; n < 6; n++) {
                                bio_rd_i(in, &fcol);
                                bio_rd_i(in, (int *)&t);
                                D3IO_SETS[x].fog[n].col = fcol;
                                D3IO_SETS[x].fog[n].start = t;
                                bio_rd_i(in, (int *)&t);
                                D3IO_SETS[x].fog[n].end = t;
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
                            d3_SetObjectPos(o, &o->unk05c);
                            d3_SetObjectRot(o, &o->unk090);
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
                if (d3sm_ShadowV)
                    d3sm_Build(d3sm_ShadowV, 0);
                else
                    d3sm_ShadowV = d3sm_Create(0, -d3if_Unk633dcc, 1);
            } else if (d3sm_ShadowV) {
                d3sm_Destroy(d3sm_ShadowV);
                d3sm_ShadowV = 0;
            }
            if (!D3IO_SCENEB4(d3_DefaultScene))
                D3IO_SCENEB4(d3_DefaultScene) = d3_Terrain ? 0x80 : 0x40;
            if (d3_Terrain)
                D3IO_SCENEAC(d3_DefaultScene) = d3_Terrain;
            tex_ReloadAll(0, (int)progress);
            d3_Busy--;
            if (name)
                strncpy((char *)d3_DefaultScene, name, 0x7f);
            d3_DefaultScene->loaded = 1;
            D3IO_SCENE3D8(d3_DefaultScene) = 0;
            return 1;
        }
        if (!f)
            vfs_fclose(in);
    }
    return 0;
}
