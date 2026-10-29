/* 0x479150, copies the part (sx, sy, w, h) of src to (x, y) of dst, clipped to both; mode 1: with the colour key @deep B3 far draft: 223 code hunks (681/816 bytes, 125 differing instructions of 289). Meaning read against 0x479150..0x47947d: the clip chain (y/dst->top, x/dst->left, sy/src->top, sx/src->left, then the four bottom/right clamps), so/d from bpp>>3 and rowbytes, the manual memcpy path when a surface lacks a DirectDraw surface or manualBlit is set, the Blt with DDBLT_WAIT / DDBLT_WAIT|DDBLT_KEYSRC, the 0x80004001 fallback that marks both surfaces manualBlit and memcpies, and the lock restore at the end - no meaning error. What differs is register/slot assignment and scheduling: the original keeps dst in ebx, src in ebp and y in edx, hoists the first sy/top load before the cmp and keeps sy/h in slots 0x3c/0x44; ours swaps those roles, reorders the loads and splits so/d differently. spell.py: cmp-test at 0x479430 tried (branch polarity dstLocked: the original is 'if (dstLocked != 0) Lock else Unlock', source flipped - kept, 225 -> 223 code hunks); the dup-tail entries at 0x479162 / 0x4792c6 / 0x47947a are the load/store scheduling of the clip blocks and of so/d (the source has no shared tail to pull), no concrete source form left. Readability done: Q1 named (blits a clipped part of a surface with optional colour key and manual fallback), Q2 D2SURF.unk3c -> manualBlit (set when the hardware Blt returns 0x80004001, checked to force the memcpy path), Q3 0x1000000/0x1008000 -> DDBLT_WAIT / DDBLT_WAIT|DDBLT_KEYSRC. */

int d2_BlitPartMode(int sx, int sy, int w, int h, D2SURF *src, int x, int y, D2SURF *dst, unsigned char mode)
{
    RECT rs, rd;
    int so, d, i, srcLocked, dstLocked, n, bf;
    unsigned int len;
    unsigned int *ps, *pd;
    unsigned char *bs, *bd;

    if (y < dst->top) {
        sy = sy + (dst->top - y);
        h = h + (y - dst->top);
        y = dst->top;
    }
    if (x < dst->left) {
        sx = sx + (dst->left - x);
        w = w + (x - dst->left);
        x = dst->left;
    }
    if (sy < src->top) {
        h = h + (sy - src->top);
        sy = src->top;
    }
    if (sx < src->left) {
        w = w + (sx - src->left);
        sx = src->left;
    }
    if (h + y > dst->bottom)
        h = dst->bottom - y;
    if (h + sy > src->bottom)
        h = src->bottom - sy;
    if (w + x > dst->right)
        w = dst->right - x;
    if (w + sx > src->right)
        w = src->right - sx;
    if (h < 1 || w < 1)
        return 0;
    so = (src->bpp >> 3) * sx + src->rowbytes * sy;
    d = (dst->bpp >> 3) * x + dst->rowbytes * y;
    srcLocked = src->locked;
    dstLocked = dst->locked;
#ifdef REA_FRAME_DUMP
    if (w > 300)
        REA_DUMP_TRACE((rd_f, "%u BlitPart %dx%d from %d,%d to %d,%d srcbpp=%d dstbpp=%d dd=%d,%d manual=%d,%d mode=%d\n",
                        GetTickCount(), w, h, sx, sy, x, y, src->bpp, dst->bpp, src->DDSurface != 0, dst->DDSurface != 0,
                        src->manualBlit, dst->manualBlit, mode));
#endif
    if (dst->DDSurface == 0 || src->DDSurface == 0 || dst->manualBlit != 0 || src->manualBlit != 0) {
        d2_LockSurface(dst);
        d2_LockSurface(src);
        {
            for (i = 0; i < h; i++) {
                len = (src->bpp >> 3) * w;
                bs = (unsigned char *)src->bits + src->rowbytes * i + so;
                bd = (unsigned char *)dst->bits + dst->rowbytes * i + d;
                memcpy(bd, bs, len);
            }
        }
    } else {
        if (dstLocked != 0)
            d2_UnlockSurface(dst);
        if (src->locked != 0)
            d2_UnlockSurface(src);
        rs.top = sy;
        rs.bottom = sy + h;
        rs.right = sx + w;
        rd.right = w + x;
        rd.bottom = h + y;
        rd.left = x;
        rs.left = sx;
        rd.top = y;
        bf = DDBLT_WAIT;
        if (mode & 1)
            bf = DDBLT_WAIT | DDBLT_KEYSRC;
        if (dst->DDSurface->lpVtbl->Blt(dst->DDSurface, &rd, src->DDSurface, &rs, bf, 0) != 0)
            return 0;
        if (d3d_hr == 0x80004001) {
            dst->manualBlit = 1;
            src->manualBlit = 1;
            d2_LockSurface(dst);
            d2_LockSurface(src);
            {
                for (i = 0; i < h; i++) {
                    len = (src->bpp >> 3) * w;
                    bs = (unsigned char *)src->bits + src->rowbytes * i + so;
                    bd = (unsigned char *)dst->bits + dst->rowbytes * i + d;
                    memcpy(bd, bs, len);
                }
            }
        }
    }
    if (dstLocked != 0)
        d2_LockSurface(dst);
    else
        d2_UnlockSurface(dst);
    if (srcLocked != 0) {
        d2_LockSurface(src);
        return 1;
    }
    d2_UnlockSurface(src);
    return 1;
}
