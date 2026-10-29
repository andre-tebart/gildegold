/* 0x47bd70, draws a run-length shape at x / y onto the surface, dispatching on the shape type: 8 bit to the
   palette blitters, 16 bit to d2_ShpBlit16ClipXY (clipped horizontally) or d2_ShpBlit16ClipY, 24 bit is not
   drawn at all; sets d2_SurfPitch from the surface for the blitters, restores it on the not-drawn path.
   Returns 1 if the shape was drawn (or nothing was to be drawn), 0 otherwise. */

int d2_ShpBlit(int x, int y, D2SURF *surf, D2SHP *s)
{
    int right, old, w;

#ifdef REA_FRAME_DUMP
    if (s->w > 300)
        REA_DUMP_TRACE((rd_f, "%u ShpBlit %dx%d at %d,%d type=%d mode=%d nruns=%d size=%u surf %dx%d bpp=%d pitch=%d clip %d,%d-%d,%d lte=%d\n",
                        GetTickCount(), s->w, s->h, x, y, s->type, s->mode, s->nruns, s->size, surf->w, surf->h,
                        surf->bpp, surf->pitch, d2_ClipLeft, d2_ClipTop, d2_ClipRight, d2_ClipBottom, d2_LightTableExternal));
#endif
    if (d2_LightTableExternal == 0) {
        if (s->nruns != -1) {
            w = s->w;                     /* the local keeps the original's register layout (w in edx) */
            if (x <= d2_ClipRight && w + x >= d2_ClipLeft) {
                right = w + x;            /* the original computes the sum once for the check and keeps it in right */
                old = d2_SurfPitch;
                d2_SurfPitch = surf->pitch;
                switch (s->type) {
                case 0:
                    if (d2_PaletteDepth == 0) {
                        d2_ShpBlit8(x, y, surf->bits, s);
                        return 1;
                    }
                    d2_ShpBlit8To16(x, y, surf->bits, s);
                    return 1;
                case 1:
                    if (x < d2_ClipLeft) {
                        d2_ShpBlit16ClipXY(x, y, surf->bits, s);
                        return 1;
                    }
                    if (right > d2_ClipRight) {
                        d2_ShpBlit16ClipXY(x, y, surf->bits, s);
                        return 1;
                    }
                    d2_ShpBlit16ClipY(x, y, surf->bits, s);
                    return 1;
                case 2:
                    return 1;
                }
                d2_SurfPitch = old;
            }
        } else {
            switch (s->type) {
            case 0:
                return 0;
            case 2:
                return 1;
            case 1:
                d2_ShpBlit16Rows(x, y, surf, s);
                return 1;
            }
        }
    }
    return 0;
}
