/* 0x47a460, grabs a rectangle of a 24 bit surface into a shape without run lengths (memory tag
   "shp_t:GrabBit24NoRS:ShapeHeader"): scans the rectangle for the bounding box of the pixels that are not the
   transparent colour, then (when the end marker colour is set) replaces the start / end marker pixels with the
   transparent colour and takes the box from those, copies the plain pixel rows into the shape, and returns an
   exactly sized copy of it (allocated with mem_Alloc; the scratch shape is freed).
   @deep behaviour draft (446 code hunks, 774/912 bytes): every hunk is the per-row pixel pointer - ours strength-reduces both scan loops (hoists (pitch&0xffff)*y, keeps pitch*3 in a slot and advances p by it per row), the original recomputes pix + ((y+r)*(pitch&0xffff)+x)*3 with an imul every row (0x47a4c2..0x47a4e5, 0x47a623..0x47a645); the rest is the frame/register shape it causes (frame 0x1c vs 0x14, r/c/p in other registers); reset test, bbox+header stores, marker loop, the two rep-movs copies and the tail order all correspond. Tried: fewer locals (w1/h1 removed), comma-increment inner loop, mask on pitch removed - identical bytes */

D2SHP *d2_GrabShape24NoRuns(int x, int y, int w, int h, unsigned char *pix, unsigned char flag, unsigned short pitch)
{
    D2SHP *shp, *ns;
    unsigned int minx, maxx, miny, maxy;
    unsigned char key[3];
    int r, c, n;
    unsigned char *p, *src, *dst;

    r = 0;
    key[0] = 0;
    key[1] = 0;
    key[2] = 0;
    maxx = 0;
    minx = w;
    maxy = 0;
    miny = h;
    shp = mem_Alloc((w * 0xc + 4) * h + 0x32, "shp_t:GrabBit24NoRS:ShapeHeader");
    for (r = 0; r < h; r++) {
        p = pix + ((r + y) * (pitch & 0xffff) + x) * 3;
        for (c = 0; c < w; c++, p += 3) {
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c > (int)(maxx & 0xffff))
                maxx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c < (int)(minx & 0xffff))
                minx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r > (int)(maxy & 0xffff))
                maxy = r;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r < (int)(miny & 0xffff))
                miny = r;
        }
    }
    if ((unsigned short)minx > (unsigned short)maxx || (unsigned short)miny > (unsigned short)maxy) {
        maxx = 0;
        minx = 0;
        maxy = 0;
        miny = 0;
    }
    shp->x0 = (short)minx;
    shp->ax0 = (short)minx;
    shp->w = (short)maxx + 1;
    shp->y0 = (short)miny;
    shp->h = (short)maxy + 1;
    shp->ay0 = (short)miny;
    shp->ax1 = (short)maxx + 1;
    shp->ay1 = (short)maxy + 1;
    if (d2_CompareColour((unsigned char *)&d2_EndMarkerColour, key) != 0) {
        for (r = 0; r < h; r++) {
            p = pix + ((y + r) * (pitch & 0xffff) + x) * 3;
            for (c = 0; c < w; c++, p += 3) {
                if (d2_CompareColour(p, (unsigned char *)&d2_StartMarkerColour) == 0) {
                    minx = c;
                    miny = r;
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                }
                if (d2_CompareColour(p, (unsigned char *)&d2_EndMarkerColour) == 0) {
                    maxx = c;
                    maxy = r;
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                }
            }
        }
    }
    shp->bx0 = (unsigned short)minx;
    shp->by0 = (unsigned short)miny;
    shp->bx1 = (unsigned short)maxx;
    shp->by1 = (unsigned short)maxy;
    shp->size = 0x32;
    dst = (unsigned char *)shp + 0x32;
    w = 0;
    if (shp->h != 0) {
        n = shp->w * 3;
        src = pix + ((pitch & 0xffff) * y + x) * 3;
        do {
            memcpy(dst, src, n);
            n = shp->w * 3;
            shp->npix += shp->w;
            src += (pitch & 0xffff) * 3;
            shp->size += n;
            dst += n;
            w++;
        } while (w < shp->h);
    }
    shp->lineoff = 0;
    shp->mode = flag;
    shp->nruns = -1;
    ns = mem_Alloc(shp->size, "shp_t:ShapeHeader");
    memcpy(ns, shp, shp->size);
    mem_Free(shp);
    return ns;
}
