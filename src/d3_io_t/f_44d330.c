/* 0x44d330, loads a BGF mesh (or finds it among the loaded ones) @deep behaviour: 558 code hunks are the frame/register cascade of a 3 KB function - frame 0x2d0 vs ours 0x2b0 (the 14 local slots of which 6 stay unused, note's block-by-block reading: all blocks present and in the original order), zero register ebx vs edi, dead-parameter slots and pointer forms (hunks at 181/207/303/333); Q2 d3_Unk4506f0 (callee reads the BGF header flags, bit meaning unknown) and the unk1e4 parameter (the field is D3BGF.nsets now, the parameter name stays) */

D3BGF *d3iot_LoadBGF(char *path, char *name, char *texnames, int unk1e4, int ntex)
{
    D3BGF *m = 0;
    char buf[256];
    D3BGFDESC desc;
    void *f;
    int i, j, k, n, mi;
    float s1, c1, s2, c2, t2, u, t1;
    float *py, *pz;
    D3VECTOR4 av, bv;
    char *used, *used2;
    char *tmp;

    strcpy(buf, path);
    _strupr(buf);
    if (strstr(buf, ".BGF")) {
        m = d3_FindBGF(buf, name, texnames, unk1e4, ntex);
        if (m) {
            d3iot_Finish(m, 0);
            return m;
        }
        d3_AddFLChunk(buf, d3_Unk4506f0(buf));
        m = d3_FindBGF(buf, name, texnames, unk1e4, ntex);
        if (m) {
            d3iot_Finish(m, 0);
            return m;
        }
        D3_ZERO(&desc, sizeof desc);
        f = vfs_Tfopen(buf, "rb", vfs_Root);
        if (f) {
            d3_ReadBGF(f, &desc);
            vfs_fclose(f);
            if (desc.npoly > 0 && desc.npts > 0) {
                m = mem_Alloc(0x20c, "d3_io:LoadAGI(obj)");
                strncpy(m->name, name, 0x3f);
                m->name[0x3f] = 0;
                if (desc.flag01 == 0) {
                    for (i = 0; i < desc.npts; i++) {
                        for (j = 0; j < desc.npts; ) {
                            if (i == j || !d3_VecEqual((D3VECTOR *)(desc.pts + i * 6), (D3VECTOR *)(desc.pts + j * 6), 0.001f)) {
                                j++;
                            } else {
                                for (k = 0; k < desc.npoly; k++) {
                                    for (n = 0; n < 3; n++) {
                                        if (desc.polys[k].v[n] == j)
                                            desc.polys[k].v[n] = i;
                                        else if (desc.polys[k].v[n] > j)
                                            desc.polys[k].v[n]--;
                                    }
                                }
                                desc.npts--;
                                memmove(desc.pts + j * 6, desc.pts + (j + 1) * 6, (desc.npts - j) * 0x18);
                            }
                        }
                    }
                }
                m->nverts = desc.npts;
                m->verts = mem_Alloc((desc.npts * 3 + 0x18) * 8, "d3_io:LoadAGF(poi)");
                s1 = (float)sin(-1.5707963705062866);
                c1 = (float)cos(-1.5707963705062866);
                s2 = (float)sin(3.1415927410125732);
                c2 = (float)cos(3.1415927410125732);
                for (k = 0; k < m->nverts; k++) {
                    av.x = desc.pts[k * 6];
                    py = &desc.pts[k * 6 + 1];
                    pz = &desc.pts[k * 6 + 2];
                    t1 = c1 * *py - s1 * *pz;
                    t2 = c1 * *pz + s1 * *py;
                    u = av.x * c2 + s2 * t2;
                    bv.z = t2 * c2 - av.x * s2;
                    u = u * -1.0f;
                    m->verts[k * 6] = bv.z * s2 + c2 * u;
                    m->verts[k * 6 + 1] = t1;
                    m->verts[k * 6 + 2] = bv.z * c2 - u * s2;
                }
                if (desc.nmat > 0) {
                    used = mem_Alloc(desc.nmat, "d3_io:RemoveDoubleMaterials");
                    n = desc.nmat;
                    for (i = 1; i - 1 < desc.nmat; i++) {
                        if (!used[i - 1]) {
                            used[i - 1] = 1;
                            for (j = i; j < desc.nmat; j++) {
                                if (!memcmp(&desc.mats[i - 1], &desc.mats[j], 0xe0)) {
                                    for (k = 0; k < desc.npoly; k++)
                                        if (desc.polys[k].mat == j)
                                            desc.polys[k].mat = i - 1;
                                    used[j] = 1;
                                }
                            }
                        }
                    }
                    mem_Free(used);
                    used = mem_Alloc(desc.nmat, "d3_io:RemoveUnusedMaterials");
                    n = desc.nmat;
                    for (k = 0; k < desc.npoly; k++)
                        if (desc.polys[k].mat >= 0)
                            used[desc.polys[k].mat] = 1;
                    for (i = 0; i < n; ) {
                        if (!used[i]) {
                            for (k = 0; k < desc.npoly; k++)
                                if (i < desc.polys[k].mat)
                                    desc.polys[k].mat--;
                            n--;
                            memmove(&desc.mats[i], &desc.mats[i + 1], (n - i) * 0xe0);
                            memmove(used + i, used + i + 1, n - i);
                        } else {
                            i++;
                        }
                    }
                    desc.nmat = n;
                    mem_Free(used);
                }
                m->npolys = desc.npoly;
                m->polys = mem_Alloc(desc.npoly * 0x38, "d3_io:LoadAGF(pol)");
                D3_COPY(m->polys, desc.polys, m->npolys * 0x38);
                used = mem_Alloc(desc.nmat, "d3_io:Materials(bool)");
                tmp = mem_Alloc(desc.nmat * 0xe0, "d3_io:TempMat[ReOrder]");
                D3_COPY(tmp, desc.mats, desc.nmat * 0xe0);
                i = 0;
                for (k = 0; k < m->npolys; k++) {
                    mi = desc.polys[k].mat;
                    if (mi >= 0 && !used[mi]) {
                        used[mi] = 1;
                        for (j = 0; j < m->npolys; j++)
                            if (desc.polys[j].mat == mi)
                                m->polys[j].mat = i;
                        {
                            char *src = tmp + mi * 0xe0;
                            D3_COPY((char *)desc.mats + i * 0xe0, src, 0xe0);
                        }
                        i++;
                    }
                }
                mem_Free(tmp);
                mem_Free(used);
                for (k = 0; k < m->npolys; k++) {
                    mi = m->polys[k].mat;
                    if (mi >= 0) {
                        D3BGFMAT *mt = &desc.mats[mi];
                        for (n = 0; n < 3; n++) {
                            float a, b;
                            a = (m->polys[k].uv[n][0] * mt->su + 0.5f) - mt->su * 0.5f;
                            b = (m->polys[k].uv[n][1] * mt->sv + 0.5f) - mt->sv * 0.5f;
                            m->polys[k].uv[n][0] = (float)cos(mt->rot) * a - (float)sin(mt->rot) * b + mt->ou;
                            m->polys[k].uv[n][1] = (float)sin(mt->rot + 3.1415927f) * a + (float)cos(mt->rot + 3.1415927f) * b + mt->ov;
                        }
                    }
                }
                if (!texnames || unk1e4 < 1 || ntex != desc.nmat) {
                    m->ntex = desc.nmat;
                    m->nsets = 1;
                    m->texnames = mem_Alloc(desc.nmat << 6, "d3_io:LoadAGF(texset)");
                    for (k = 0; k < desc.nmat; k++) {
                        D3BGFMAT *mt = &desc.mats[k];
                        if (mt->name[2][0])
                            strcpy(m->texnames + k * 0x40, mt->name[2]);
                        else if (mt->name[0][0])
                            strcpy(m->texnames + k * 0x40, mt->name[0]);
                        else if (mt->name[1][0])
                            strcpy(m->texnames + k * 0x40, mt->name[1]);
                    }
                } else {
                    m->ntex = ntex;
                    m->nsets = unk1e4;
                    m->texnames = texnames;
                }
                used2 = mem_Alloc(m->npolys, "d3_io:LoadAGF(texref)");
                for (k = 0; k < m->ntex; k++) {
                    D3BGFFLAGS fl;
                    D3BGFMAT *mt = &desc.mats[k];
                    D3TEXHANDLE *tex;
                    fl.all = 0;
                    fl.s.b0 = 0xff;
                    if (mt->kind) {
                        fl.s.b0 = mt->c1;
                        if (mt->kind == 2)
                            fl.s.c0 |= 1;
                    }
                    fl.s.b1 = mt->c7;
                    fl.s.c1 = mt->c2;
                    if (mt->name[2][0] == 0)
                        tex = D3BGF_TEXLOAD(m->texnames + k * 0x40, fl, ((mt->c3 << 4 | mt->c6) * 2 | mt->c5) * 2, mt->c8 << 4 | mt->c9);
                    else
                        tex = D3BGF_TEXLOAD(m->texnames + k * 0x40, fl, (((mt->c3 << 4 | mt->c6) * 2 | mt->c5) * 2) | 1, mt->c8 << 4 | mt->c9);
                    mi = tex ? tex - (D3TEXHANDLE *)d3_Textures : -1;
                    for (j = 0; j < m->npolys; j++) {
                        if (!used2[j] && m->polys[j].mat == k) {
                            m->polys[j].tex = mi;
                            used2[j] = 1;
                        }
                    }
                }
                mem_Free(used2);
                for (k = 0; k < desc.nattach; k++) {
                    strncpy(m->attach[k].name, desc.attach[k].name, 0x3f);
                    m->attach[k].name[0x3f] = 0;
                    m->attach[k].f[0] = desc.attach[k].f[0];
                    m->attach[k].f[1] = desc.attach[k].f[1];
                    m->attach[k].f[2] = desc.attach[k].f[2];
                    m->attach[k].f[3] = desc.attach[k].f[3];
                    m->attach[k].f[4] = desc.attach[k].f[4];
                    m->attach[k].f[5] = desc.attach[k].f[5];
                }
                d3iot_Finish(m, 1);
            }
            if (desc.nmat > 0 && desc.mats) {
                mem_Free(desc.mats);
                desc.mats = 0;
            }
            if (desc.npoly > 0 && desc.polys) {
                mem_Free(desc.polys);
                desc.polys = 0;
            }
            if (desc.npts > 0 && desc.pts)
                mem_Free(desc.pts);
        }
    }
    return m;
}
