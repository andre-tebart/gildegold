/* 0x47c0e0, converts a 24 bit shape into a 16 bit one @deep B3, 39 code + 30 order hunks after the unsigned guard fix (0 < src[1], jbe now matches): the rest is scheduling (npix/size loads hoisted before the branch, the lea placed before vs after the stores, xor edi,edi hoisted) and one addressing form (the original keeps pix+1 and reads -1/+0/+1, ours pix with 0/+1/+2), no meaning error found in the read hunks */

D2SHP *d2_ShpConvert24To16(D2SHP *s)
{
    unsigned short buf[0x480];
    int lines[864];
    int i, j;
    unsigned int k, m;
    unsigned int *src, *srccnt, *dst, *dstcnt;
    unsigned char *pix;
    unsigned short c;
    D2SHP *r;

    if (s->nruns != -1) {
        r = mem_Alloc(s->size - s->npix, "d2:shp:NewShape");
        memcpy(r, s, 0x32);
        r->size = 0x32;
        r->type = 1;
        r->nruns = 0;
        r->npix = 0;
        src = (unsigned int *)((char *)s + 0x36);
        dst = (unsigned int *)((char *)r + 0x32);
        dstcnt = (unsigned int *)((char *)r + 0x36);
        srccnt = (unsigned int *)((char *)s + 0x32);
        for (i = 0; i < s->h; i++) {
            *dst = *srccnt;
            lines[i] = (char *)dst - (char *)r;
            r->size += 4;
            m = 0;
            if (0 < *srccnt) {
                do {
                    *dstcnt = (*src * 2) / 3;
                    dstcnt[1] = src[1];
                    if (0 < src[1]) {
                        pix = (unsigned char *)src + 8;
                        k = 0;
                        do {
                            c = dd_RGB(pix[0], pix[1], pix[2]);
                            buf[k] = c;
                            if (c == 0)
                                buf[k] = dd_RGB(5, 5, 5);
                            if (s->mode == 1)
                                buf[k] = buf[k] >> 1 & d2_HighBitMask;
                            k++;
                            pix += 3;
                        } while (k < src[1]);
                    }
                    memcpy(dstcnt + 2, buf, src[1] * 2);
                    r->nruns++;
                    r->npix += src[1];
                    r->size += src[1] * 2 + 8;
                    src = (unsigned int *)((char *)src + src[1] * 3 + 8);
                    dstcnt = (unsigned int *)((char *)dstcnt + dstcnt[1] * 2 + 8);
                    m++;
                } while (m < *srccnt);
            }
            dst = dstcnt;
            dstcnt = dstcnt + 1;
            srccnt = src;
            src = src + 1;
        }
        r->lineoff = r->size;
        r->size += r->h * 4;
        d2_SetLineTable(lines, r);
#ifdef REA_FRAME_DUMP
        if (r->w > 300) {
            char rd_path[64];
            REA_DUMP_TRACE((rd_f, "%u Conv24 %dx%d mode=%d nruns=%d npix=%d size=%u -> nruns=%d size=%u\n", GetTickCount(),
                            s->w, s->h, s->mode, s->nruns, s->npix, s->size, r->nruns, r->size));
            sprintf(rd_path, "dump/shape24src_%dx%d_%u.bin", s->w, s->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, s, s->size);
            sprintf(rd_path, "dump/shape16_%dx%d_%u.bin", r->w, r->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, r, r->size);
        }
#endif
        return r;
    } else {
        r = mem_Alloc(s->size - s->npix, "d2:shp:NewShape");
        memcpy(r, s, 0x32);
        r->nruns = -1;
        r->size = 0x32;
        r->type = 1;
        r->npix = 0;
        dst = (unsigned int *)((char *)r + 0x32);
        for (i = 0; i < s->h; i++) {
            for (j = 0; j < s->w; j++) {
                pix = (unsigned char *)s + 0x32 + (s->w * i + j) * 3;
                buf[j] = dd_RGB(pix[0], pix[1], pix[2]);
                if (s->mode == 1)
                    buf[j] = buf[j] >> 1 & d2_HighBitMask;
            }
            memcpy(dst, buf, s->w * 2);
            dst = (unsigned int *)((char *)dst + s->w * 2);
            r->size += s->w * 2;
        }
        r->lineoff = 0;
#ifdef REA_FRAME_DUMP
        if (r->w > 300) {
            char rd_path[64];
            REA_DUMP_TRACE((rd_f, "%u Conv24 %dx%d mode=%d nruns=%d npix=%d size=%u -> nruns=%d size=%u\n", GetTickCount(),
                            s->w, s->h, s->mode, s->nruns, s->npix, s->size, r->nruns, r->size));
            sprintf(rd_path, "dump/shape24src_%dx%d_%u.bin", s->w, s->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, s, s->size);
            sprintf(rd_path, "dump/shape16_%dx%d_%u.bin", r->w, r->h, GetTickCount());
            REA_DUMP_BYTES(rd_path, r, r->size);
        }
#endif
        return r;
    }
}
