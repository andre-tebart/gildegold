/* 0x4794c0, blits the part (sx, sy, w, h) of the surface src to (x, y) of the DirectDraw surface dst @deep B3 20 code + 6 slots hunks (136/336 bytes), one hunk area: the RECT stores before the Blt. Original stores rs.left/rs.top (and rd.left/rd.top) early - mov [esp+0x10],esi; mov [esp+0x14],ebp - so sx/sy are dead when rs.right = sx+w / rs.bottom = sy+h are computed, and it adds them in place (add esi,edi); ours stores in the current order and recomputes with lea (lea edx,[esi+edi*1]); the 6 slot hunks are the RECT local homes ([esp+0x30]/0x34 vs 0x40). notes/units/gfx.md (d2q) records this exact case: the old session brute-forced all 70 orders of the 8 rect stores, all identical, so store order is not the lever. Meaning read against 0x4794c0..0x47960e: the four negative-offset clips, the src bottom/right clamps, src->locked save + restore around the Blt, the DDSurface/manualBlit guard, rs/rd fill, Blt(dst, &rd, src->DDSurface, &rs, DDBLT_WAIT, 0) and the 0/1 returns - no meaning error. Readability done: Q1 named (blits a clipped part of a D2SURF onto a raw DirectDraw surface, the d2.c texture uploads call it), Q3 0x1000000 -> DDBLT_WAIT. */

int d2_BlitPartToDD(int sx, int sy, int w, int h, D2SURF *src, int x, int y, LPDIRECTDRAWSURFACE4 dst)
{
    RECT rs, rd;
    int locked;

    if (sy < 0) {
        h = h + sy;
        sy = 0;
    }
    if (sx < 0) {
        w = w + sx;
        sx = 0;
    }
    if (x < 0) {
        sx = sx - x;
        w = w + x;
        x = 0;
    }
    if (y < 0) {
        sy = sy - y;
        h = h + y;
        y = 0;
    }
    if (h + sy > src->bottom)
        h = src->bottom - sy;
    if (sx + w > src->right)
        w = src->right - sx;
    if (h != 0 && w != 0) {
        locked = src->locked;
        if (src->DDSurface != 0 && src->manualBlit == 0) {
            if (locked != 0)
                d2_UnlockSurface(src);
            rs.right = sx + w;
            rd.left = x;
            rd.right = w + x;
            rs.bottom = sy + h;
            rd.bottom = h + y;
            rs.left = sx;
            rs.top = sy;
            rd.top = y;
            if (dst->lpVtbl->Blt(dst, &rd, src->DDSurface, &rs, DDBLT_WAIT, 0) != 0)
                return 0;
        }
        if (locked != 0) {
            d2_LockSurface(src);
            return 1;
        }
        d2_UnlockSurface(src);
        return 1;
    }
    return 0;
}
