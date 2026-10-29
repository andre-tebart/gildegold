/* 0x451250, loads the BGF file path, cleans it up as d3iot_LoadBGF does and appends the converted mesh to
   the file as a fast-load chunk (0x2d ... 0x2f)
   @deep behaviour: the 377 code hunks are register allocation and stack slots only (seen in the hunks: ebx where the original uses edi as the zero register, [ebp-0x198] slot reuse), no meaning difference; Q2 the unk1e4 parameter (the field is D3BGF.nsets now, the parameter name stays); Q4 7 raw access casts in the writer loop */

/* 0x451250: the fast-load chunk writer (was unk_451250.c, worker mesh) */
#define D3_COPY_AT(dvar, svar, size) {                              \
    int _cn = (size);                                               \
    __asm mov edi, dvar                                             \
    __asm mov esi, svar                                             \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

void d3_AddFLChunk(char *path, char weld)
{
    char buf[256];
    D3VECTOR4 bv;
    D3BGFDESC desc;
    void *f, *out;
    D3BGF *m;
    D3VECTOR4 v;
    int i, j, k, n, mi, size;
    float s1, c1, s2, c2, t1, t2, u, a, b;
    float *py, *pz;
    char *used, *tmp, *data;

    strcpy(buf, path);
    D3_ZERO(&desc, sizeof desc);
    f = vfs_Tfopen(path, "rb", vfs_Root);
    if (f) {
        d3_ReadBGF(f, &desc);
        if (desc.npoly <= 0 || desc.npts <= 0) {
            vfs_fclose(f);
            return;
        }
        m = mem_Alloc(0x20c, "d3_io_fl:AddFLChunk(obj)");
        if (desc.flag01 == 0 && weld) {
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
        m->verts = mem_Alloc((desc.npts * 3 + 0x18) * 8, "d3_io_fs:AddFLChunk(poi)");
        s1 = (float)sin(-1.5707963705062866);
        c1 = (float)cos(-1.5707963705062866);
        s2 = (float)sin(3.1415927410125732);
        c2 = (float)cos(3.1415927410125732);
        for (k = 0; k < m->nverts; k++) {
            v.x = desc.pts[k * 6];
            py = &desc.pts[k * 6 + 1];
            pz = &desc.pts[k * 6 + 2];
            t1 = c1 * *py - s1 * *pz;
            t2 = s1 * *py + c1 * *pz;
            u = v.x * c2 + s2 * t2;
            bv.z = t2 * c2 - v.x * s2;
            u = u * -1.0;
            m->verts[k * 6] = bv.z * s2 + c2 * u;
            m->verts[k * 6 + 1] = t1;
            m->verts[k * 6 + 2] = bv.z * c2 - u * s2;
        }
        if (desc.nmat > 0) {
            used = mem_Alloc(desc.nmat, "d3_io_fs:RemoveDoubleMaterials");
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
            D3FL_ZERO_AT(used, desc.nmat);
            n = desc.nmat;
            for (k = 0; k < desc.npoly; k++)
                if (desc.polys[k].mat >= 0)
                    used[desc.polys[k].mat] = 1;
            for (i = 0; i < n; ) {
                if (!used[i]) {
                    for (k = 0; k < desc.npoly; k++)
                        if (desc.polys[k].mat > i)
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
        m->polys = mem_Alloc(desc.npoly * 0x38, "d3_io_fs:AddFLChunk(pol)");
        D3FL_COPY(m->polys, desc.polys, m->npolys * 0x38);
        used = mem_Alloc(desc.nmat, "d3_io_fs:Materials(bool)");
        tmp = mem_Alloc(desc.nmat * 0xe0, "d3_io_fs:TempMat[ReOrder]");
        D3_COPY_AT(tmp, desc.mats, desc.nmat * 0xe0);
        k = 0;
        for (i = 0; i < m->npolys; i++) {
            mi = desc.polys[i].mat;
            if (mi > -1 && !used[mi]) {
                used[mi] = 1;
                for (j = 0; j < m->npolys; j++)
                    if (desc.polys[j].mat == mi)
                        m->polys[j].mat = k;
                D3FL_COPY3((char *)desc.mats + k * 0xe0, tmp + mi * 0xe0, 0xe0);
                k++;
            }
        }
        mem_Free(tmp);
        mem_Free(used);
        for (i = 0; i < m->npolys; i++) {
            mi = m->polys[i].mat;
            if (mi >= 0) {
                for (n = 0; n < 3; n++) {
                    a = (m->polys[i].uv[n][0] * desc.mats[mi].su + 0.5f) - desc.mats[mi].su * 0.5f;
                    b = (m->polys[i].uv[n][1] * desc.mats[mi].sv + 0.5f) - desc.mats[mi].sv * 0.5f;
                    m->polys[i].uv[n][0] = (float)cos(desc.mats[mi].rot) * a - (float)sin(desc.mats[mi].rot) * b + desc.mats[mi].ou;
                    m->polys[i].uv[n][1] = (float)sin(desc.mats[mi].rot + 3.1415927f) * a + (float)cos(desc.mats[mi].rot + 3.1415927f) * b + desc.mats[mi].ov;
                }
            }
        }
        m->ntex = desc.nmat;
        m->nsets = 0;
        d3_CalcBounds(m);
        d3_CalcNormals(m);
        size = vfs_ftell(f);
        vfs_fseek(f, 0, 0);
        data = mem_Alloc(size, "d3_io_fl:NotMyChunkBufferStart");
        vfs_fread(data, size, 1, f);
        vfs_fclose(f);
        out = vfs_Tfopen(buf, "wb", vfs_Root);
        vfs_fwrite(data, size, 1, out);
        mem_Free(data);
        bio_wr_c(out, 0x2d);
        bio_wr_i(out, d3_FLChunkSize(m, &desc));
        bio_wr_i(out, 0xfab50005);
        bio_wr_i(out, m->ntex);
        bio_wr_i(out, m->nverts);
        bio_wr_i(out, m->npolys);
        if (m->nverts > 0)
            for (i = 0; i < m->nverts + 8; i++) {
                bio_wr_v3(out, &m->verts[i * 6]);
                bio_wr_v3(out, &m->verts[i * 6 + 3]);
            }
        bio_wr_i(out, *(int *)&m->radius);
        for (i = 0; i < m->npolys; i++) {
            bio_wr_i(out, m->polys[i].v[0]);
            bio_wr_i(out, m->polys[i].v[1]);
            bio_wr_i(out, m->polys[i].v[2]);
            ((int *)&v)[0] = *(int *)&m->polys[i].uv[0][0];
            ((int *)&v)[1] = *(int *)&m->polys[i].uv[1][0];
            ((int *)&v)[2] = *(int *)&m->polys[i].uv[2][0];
            bio_wr_v3(out, &v);
            ((int *)&v)[0] = *(int *)&m->polys[i].uv[0][1];
            ((int *)&v)[1] = *(int *)&m->polys[i].uv[1][1];
            ((int *)&v)[2] = *(int *)&m->polys[i].uv[2][1];
            bio_wr_v3(out, &v);
            bio_wr_v3(out, m->polys[i].pad2c);
            if (m->ntex > 0xfe) {
                bio_wr_i(out, m->polys[i].mat);
            } else {
                if (m->polys[i].mat == -1)
                    bio_wr_c(out, 0xff);
                else
                    bio_wr_c(out, (char)m->polys[i].mat);
            }
        }
        for (i = 0; i < m->ntex; i++) {
            bio_wr_s(out, desc.mats[i].name[0]);
            bio_wr_s(out, desc.mats[i].name[2]);
            bio_wr_s(out, desc.mats[i].name[1]);
            bio_wr_c(out, desc.mats[i].kind);
            bio_wr_c(out, desc.mats[i].c1);
            bio_wr_c(out, ((desc.mats[i].c3 << 4 | desc.mats[i].c6) * 2 | desc.mats[i].c5) * 2 | desc.mats[i].c2);
            bio_wr_c(out, desc.mats[i].c7 & 0xfe | desc.mats[i].pad4);
            bio_wr_c(out, desc.mats[i].c8);
            bio_wr_c(out, desc.mats[i].c9);
        }
        bio_wr_i(out, desc.nattach);
        for (i = 0; i < desc.nattach; i++) {
            bio_wr_s(out, desc.attach[i].name);
            bio_wr_v3(out, &desc.attach[i].f[0]);
            bio_wr_v3(out, &desc.attach[i].f[3]);
        }
        bio_wr_c(out, 0x2f);
        vfs_ftell(out);
        vfs_fclose(out);
        if (desc.nmat > 0 && desc.mats) {
            mem_Free(desc.mats);
            desc.mats = 0;
        }
        if (m->npolys > 0 && m->polys) {
            mem_Free(m->polys);
            m->polys = 0;
        }
        if (desc.npoly > 0 && desc.polys) {
            mem_Free(desc.polys);
            desc.polys = 0;
        }
        if (m->nverts > 0 && m->verts) {
            mem_Free(m->verts);
            m->verts = 0;
        }
        if (desc.npts > 0 && desc.pts) {
            mem_Free(desc.pts);
            desc.pts = 0;
        }
        mem_Free(m);
    }
}
