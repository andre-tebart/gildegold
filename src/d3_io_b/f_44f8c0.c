/* 0x44f8c0, reads an animation stream (chunk 0x30): per frame the vertices, uv faces, position / angles and link
   points; then makes everything relative to frame 0 and packs the points into bytes */

/* 0x44f8c0..0x450858 (was unk_44f8c0.c, worker anim) */

D3BASTREAM *d3_LoadBinaryAnimStream(char *path, char *name, char relative)
{
    int first = -1;
    int last = -1;
    int nvfix = -1;
    int nuvfix = -1;
    D3BASTREAM *stream = 0;          /* result, created at the frame count chunk (0x23) */
    int nframes = 0;
    int nsub = 1;
    int nlinks = 0;
    char readuv = 0;
    char readverts = 1;
    VFSFILE *f;
    int hdr, tag, i, j, frame, pt;   /* frame: index of the frame being read, pt: point index */
    int time, nv, nuv, ofs;
    D3BAVEC v, w, mn, mx;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fread(&hdr, 4, 1, f);
        tag = bgi_ReadId(f);
        if (tag == 0x30) {
            bio_rd_i(f, &frame);
            tag = bgi_ReadId(f);
            if (tag == 1) {
                bio_rd_i(f, &frame);
                if ((unsigned)frame > 0xabcd0001) {
                    vfs_fclose(f);
                    return 0;
                }
            }
            tag = bgi_ReadId(f);
            if (tag == 0x23) {
                bio_rd_i(f, &nframes);
                if (nframes > 0) {
                    stream = mem_Alloc(sizeof(D3BASTREAM), "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream)");
                    strncpy(stream->name, name, 0x3f);
                    stream->nframes = nframes;
                    stream->first = 0;
                    stream->unk168 = 0;
                    stream->last = stream->nframes - 1;
                    stream->relative = relative;
                    stream->frames = mem_Alloc(stream->nframes * sizeof(D3BAFRAME),
                                          "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata)");
                }
                tag = bgi_ReadId(f);
            }
            if (tag == 0x33) {
                bio_rd_c(f, &readverts);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x24) {
                bio_rd_c(f, &readuv);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x37) {
                bio_rd_i(f, &nlinks);
                if (stream)
                    stream->nlinks = nlinks;
                tag = bgi_ReadId(f);
            }
            if (tag == 0x36) {
                bio_rd_i(f, &nsub);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x34) {
                bio_rd_i(f, &nvfix);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x35) {
                bio_rd_i(f, &nuvfix);
                tag = bgi_ReadId(f);
            }
            if (tag == 0x29) {
                bio_rd_i(f, &first);
                if (stream)
                    stream->first = first;
                tag = bgi_ReadId(f);
            }
            if (tag == 0x2a) {
                bio_rd_i(f, &last);
                if (stream)
                    stream->last = last;
                tag = bgi_ReadId(f);
            }
            if (stream && stream->first > stream->last) {
                frame = stream->last;
                stream->last = stream->first;
                stream->first = frame;
            }
            for (frame = 0; frame < nframes; frame++) {
                time = -1;
                ofs = 0;
                for (j = 0; j < nsub; j++) {
                    if (tag == 0x18) {
                        bio_rd_i(f, &time);
                        if (stream)
                            stream->frames[frame].time = time;
                        tag = bgi_ReadId(f);
                    }
                    nv = 0;
                    if (readverts && tag == 0x19) {
                        bio_rd_i(f, &nv);
                        if (stream) {
                            if (stream->nverts == 0) {
                                if (nvfix >= 0)
                                    stream->nverts = nvfix;
                                else
                                    stream->nverts = nv;
                            } else if (nvfix == -1 && stream->nverts != nv)
                                err_Message("Inconsistent number of vertices in d3_LoadBinaryAnimStream()");
                        }
                        tag = bgi_ReadId(f);
                    }
                    if (tag == 0x21) {
                        if (stream && ofs == 0) {
                            if (nvfix > 0)
                                stream->frames[frame].points = mem_Alloc(nvfix * sizeof(D3BAPOINT),
                                    "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata->points)");
                            else
                                stream->frames[frame].points = mem_Alloc(nv * sizeof(D3BAPOINT),
                                    "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata->points)");
                        }
                        for (i = 0; i < nv; i++) {
                            bio_rd_v3(f, (int *)&v);
                            if (stream) {
                                if (frame == 0) {
                                    stream->frames[frame].points[ofs + i].x = v.x;
                                    stream->frames[frame].points[ofs + i].y = v.y;
                                    stream->frames[frame].points[ofs + i].z = v.z;
                                } else {
                                    stream->frames[frame].points[ofs + i].x = v.x - stream->frames[0].points[ofs + i].x;
                                    stream->frames[frame].points[ofs + i].y = v.y - stream->frames[0].points[ofs + i].y;
                                    stream->frames[frame].points[ofs + i].z = v.z - stream->frames[0].points[ofs + i].z;
                                }
                            }
                        }
                        ofs += nv;
                        tag = bgi_ReadId(f);
                        if (tag != 0x28)
                            err_Message("Expected EXP_DONE in d3_LoadBinaryAnimStream(): ReadVertices");
                        else
                            tag = bgi_ReadId(f);
                    }
                    if (readuv && tag == 0x1a) {
                        bio_rd_i(f, &nuv);
                        tag = bgi_ReadId(f);
                        if (tag == 0x22) {
                            for (i = 0; i < nuv; i++) {
                                bio_rd_v3(f, (int *)&v);
                                bio_rd_v3(f, (int *)&w);
                            }
                            tag = bgi_ReadId(f);
                        }
                        if (tag != 0x28)
                            err_Message("Expected EXP_DONE in d3_LoadBinaryAnimStream(): ReadUVFaces");
                        else
                            tag = bgi_ReadId(f);
                    }
                }
                if (tag == 0x31) {
                    bio_rd_v3(f, (int *)&v);
                    bio_rd_v3(f, (int *)&w);
                    if (stream) {
                        stream->frames[frame].pos[0] = v.x;
                        stream->frames[frame].pos[1] = v.y;
                        stream->frames[frame].pos[2] = v.z;
                        stream->frames[frame].rot[0] = w.x;
                        stream->frames[frame].rot[1] = w.y;
                        stream->frames[frame].rot[2] = w.z;
                    }
                    tag = bgi_ReadId(f);
                }
                for (i = 0; i < nlinks; i++) {
                    if (tag == 0x38) {
                        bio_rd_s(f, stream->links[i]);
                        tag = bgi_ReadId(f);
                    }
                    if (tag == 0x39) {
                        bio_rd_v3(f, (int *)stream->frames[frame].links[i].a);
                        tag = bgi_ReadId(f);
                    }
                    if (tag == 0x3a) {
                        bio_rd_v3(f, (int *)stream->frames[frame].links[i].b);
                        tag = bgi_ReadId(f);
                    }
                }
            }
            if (tag == 0x32) {
                bio_rd_v3(f, (int *)&v);
                bio_rd_v3(f, (int *)&w);
                tag = bgi_ReadId(f);
            }
            if (tag != 0x2f)
                err_Message("Expected EXP_EOCHUNK in d3_LoadBinaryAnimStream(): EndOfFrameLoop");
            if (stream) {
                if (nvfix >= 0)
                    nv = nvfix;
                if (nuvfix >= 0)
                    nuv = nuvfix;
                if (stream->first < 0)
                    stream->first = 0;
                if (stream->first >= stream->nframes)
                    stream->first = stream->nframes - 1;
                if (stream->last < 0)
                    stream->last = 0;
                if (stream->last >= stream->nframes)
                    stream->last = stream->nframes - 1;
                for (frame = 0; frame < nframes - 1; frame++)
                    stream->frames[frame].dt = stream->frames[frame + 1].time - stream->frames[frame].time;
                stream->frames[nframes - 1].dt = stream->frames[nframes - 2].dt;
                for (frame = 1; frame < nframes; frame++) {
                    stream->frames[frame].pos[0] -= stream->frames[0].pos[0];
                    stream->frames[frame].pos[1] -= stream->frames[0].pos[1];
                    stream->frames[frame].pos[2] -= stream->frames[0].pos[2];
                    stream->frames[frame].rot[0] -= stream->frames[0].rot[0];
                    stream->frames[frame].rot[1] -= stream->frames[0].rot[1];
                    stream->frames[frame].rot[2] -= stream->frames[0].rot[2];
                }
                stream->frames[0].pos[0] = 0.0f;
                stream->frames[0].pos[1] = 0.0f;
                stream->frames[0].pos[2] = 0.0f;
                stream->frames[0].rot[0] = 0.0f;
                stream->frames[0].rot[1] = 0.0f;
                stream->frames[0].rot[2] = 0.0f;
                for (frame = 0; frame < stream->nverts; frame++) {
                    stream->frames[0].points[frame].x = 0.0f;
                    stream->frames[0].points[frame].y = 0.0f;
                    stream->frames[0].points[frame].z = 0.0f;
                }
                for (frame = 0; frame < nframes; frame++) {
                    if (stream->relative) {
                        for (i = 0; i < stream->nverts; i++) {
                            stream->frames[frame].points[i].x -= stream->frames[frame].pos[0];
                            stream->frames[frame].points[i].y -= stream->frames[frame].pos[1];
                            stream->frames[frame].points[i].z -= stream->frames[frame].pos[2];
                        }
                        for (i = 0; i < nlinks; i++) {
                            stream->frames[frame].links[i].a[0] -= stream->frames[frame].pos[0];
                            stream->frames[frame].links[i].a[1] -= stream->frames[frame].pos[1];
                            stream->frames[frame].links[i].a[2] -= stream->frames[frame].pos[2];
                        }
                    }
                    mn.x = 1e10f;
                    mn.y = 1e10f;
                    mn.z = 1e10f;
                    mx.x = -1e10f;
                    mx.y = -1e10f;
                    mx.z = -1e10f;
                    if (nv > 0) {
                        D3BAPOINT *p = stream->frames[frame].points;
                        for (i = nv; i; i--, p++) {
                            mn.x = (((p->x) > (mn.x)) ? (mn.x) : (p->x * 1.0f));
                            mn.y = (((p->y) > (mn.y)) ? (mn.y) : (p->y));
                            mn.z = (((p->z) > (mn.z)) ? (mn.z) : (p->z));
                            mx.x = (((p->x) < (mx.x)) ? (mx.x) : (p->x * 1.0f));
                            mx.y = (((p->y) < (mx.y)) ? (mx.y) : (p->y));
                            mx.z = (!((mx.z) > (p->z)) ? (p->z) : (mx.z));
                        }
                    }
                    stream->frames[frame].min[0] = mn.x;
                    stream->frames[frame].min[1] = mn.y;
                    stream->frames[frame].min[2] = mn.z;
                    v.x = mx.x - mn.x;
                    v.y = mx.y - mn.y;
                    v.z = mx.z - mn.z;
                    stream->frames[frame].scale[0] = v.x / 255.0f;
                    stream->frames[frame].scale[1] = v.y / 255.0f;
                    stream->frames[frame].scale[2] = v.z / 255.0f;
                    stream->frames[frame].dt *= 3;
                    stream->frames[frame].packed = mem_Alloc(nv * 3,
                        "d3_io_b:d3_LoadBinaryAnimation(tp_veranimstream->framedata->wpoints)");
                    for (pt = 0; pt < nv; pt++) {
                        w.x = stream->frames[frame].points[pt].x - mn.x;
                        w.y = stream->frames[frame].points[pt].y - mn.y;
                        w.z = stream->frames[frame].points[pt].z - mn.z;
                        stream->frames[frame].packed[pt * 3] = (unsigned char)((w.x * 255.0f) / v.x);
                        stream->frames[frame].packed[pt * 3 + 1] = (unsigned char)((w.y * 255.0f) / v.y);
                        stream->frames[frame].packed[pt * 3 + 2] = (unsigned char)((w.z * 255.0f) / v.z);
                    }
                    mem_Free(stream->frames[frame].points);
                    stream->frames[frame].points = 0;
                }
            }
        }
        vfs_fclose(f);
    }
    return stream;
}
