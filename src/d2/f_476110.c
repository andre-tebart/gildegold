/* 0x476110, one step of the dissolve of a picture onto the screen */

void d2_DissolveStep(D2DISSOLVE *d)
{
    int i;
    unsigned short *dst;

    if (d != 0 && d->mask != 0) {
        d->step += 8;
#ifdef REA_FRAME_DUMP
        REA_DUMP_TRACE((rd_f, "%u Dissolve step=%d %dx%d at %d,%d pitchpix=%d\n", GetTickCount(), d->step, d->w, d->h,
                        d->x, d->y, dd_PitchPixels));
#endif
        if (d->step <= 0xff) {
            d2_SetBlitParams(((D2SURF *)d2_ScreenSurf)->bits + d->y * dd_PitchPixels + d->x, d->src, d->mask,
                         (short)d->w, (short)d->h, (short)d->step);
        } else {
            for (i = 0; i < d->h; i++) {
                dst = ((D2SURF *)d2_ScreenSurf)->bits + (d->y + i) * dd_PitchPixels + d->x;
                memcpy(dst, (unsigned short *)d->src + d->w * i, d->w * 2);
            }
        }
        if (d->step > 0x120) {
            if (d->mask != 0) {
                mem_Free(d->mask);
                d->mask = 0;
            }
            if (d->src != 0) {
                mem_Free(d->src);
                d->src = 0;
            }
            d2_RemoveObj(d->obj);
            mem_Free(d);
            d2_CurrentDissolve = 0;
        }
    }
}
