/* 0x47a7d0, grabs a rectangle of a 16 bit surface into a run length coded shape (memory tag
   "shp_t:GrabBit8:ShapeHeader"): scans the rectangle for the bounding box of the non-zero pixels, then stores
   every row as runs of the transparent colour (skip runs) alternating with literal runs, builds the table of
   row offsets, and returns an exactly sized copy of the shape (allocated with mem_Alloc; the scratch shape is
   freed).
   @deep behaviour draft (466 code hunks, 771/848 bytes): the bounds scan strength-reduces like 0x47a460 (ours hoists pitch*2 and keeps the row pointer running, the original recomputes pix + (y+r)*(pitch&0xffff)+x each row, 0x47a830..0x47a859) and keeps maxx/maxy in registers where the original keeps them in the slots 0x10/0x14 (minx/miny the other way round); the run-length loop differs the same way (base/base+x slots, seg pointer, the key in di vs a slot). Read against the original 47a7d0..47ab2f: the bbox reset, the header stores, the dd_RGB key, the two scan guards, the run skip/count/seg[0]*=2/memcpy/npix/size updates, the line table upload and the tail all correspond. Tried: loop-form variants on the sibling 0x47a460 (fewer locals, comma increment, mask removal) changed no byte */

D2SHP *d2_GrabShape16Runs(int x, int y, int w, int h, unsigned short *pix, unsigned char flag, unsigned short pitch)
{
    D2SHP *shp, *ns;
    unsigned int minx, maxx, miny, maxy;
    unsigned short ux, uy;
    unsigned short key;
    short w1, h1;
    int tab[864];
    int r, c, nseg;
    unsigned short *p;
    int *seg, *rowp;
    int base;

    maxx = 0;
    maxy = 0;
    nseg = 0;
    shp = mem_Alloc((w * 8 + 4) * h + 0x32, "shp_t:GrabBit8:ShapeHeader");
    r = 0;
    minx = w;
    miny = h;
    for (r = 0; r < h; r++) {
        p = pix + (y + r) * (pitch & 0xffff) + x;
        for (c = 0; c < w; c++) {
            if (*p != 0) {
                if (c > (int)(maxx & 0xffff))
                    maxx = c;
                if (c < (int)(minx & 0xffff))
                    minx = c;
                if (r > (int)(maxy & 0xffff))
                    maxy = r;
                if (r < (int)(miny & 0xffff))
                    miny = r;
            }
            p++;
        }
    }
    uy = (unsigned short)miny;
    ux = (unsigned short)minx;
    if ((unsigned short)maxx < ux || (unsigned short)maxy < uy) {
        ux = 0;
        uy = 0;
        maxy = 0;
        maxx = 0;
    }
    shp->x0 = ux;
    w1 = (short)(maxx + 1);
    shp->w = w1;
    shp->y0 = uy;
    h1 = (short)(maxy + 1);
    shp->h = h1;
    shp->ax0 = ux;
    shp->ay0 = uy;
    shp->ax1 = w1;
    shp->ay1 = h1;
    shp->bx0 = ux;
    shp->by0 = uy;
    shp->bx1 = (short)maxx;
    shp->by1 = (short)maxy;
    shp->size = 0x32;
    key = (unsigned short)dd_RGB(d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
    r = 0;
    if (shp->h != 0) {
        do {
            maxx = 0;
            rowp = (int *)((char *)shp + shp->size);
            minx = shp->size + 4;
            shp->size = minx;
            tab[r] = (char *)rowp - (char *)shp;
            miny = 0;
            if (shp->w != 0) {
                base = (r + y) * (pitch & 0xffff);
                do {
                    seg = (int *)(minx + (int)shp);
                    seg[0] = 0;
                    seg[1] = 0;
                    if (pix[x + base + miny] == key) {
                        p = pix + base + miny + x;
                        do {
                            if ((int)miny >= shp->w)
                                break;
                            miny++;
                            p++;
                            seg[0]++;
                        } while (*p == key);
                    }
                    seg[0] *= 2;
                    if (miny < shp->w) {
                        do {
                            if (pix[base + seg[1] + miny + x] == key)
                                break;
                            c = seg[1] + 1;
                            seg[1] = c;
                        } while (c + miny < shp->w);
                    }
                    minx = seg[1];
                    memcpy(seg + 2, pix + x + base + miny, minx * 2);
                    c = seg[1];
                    if (c != 0)
                        miny--;
                    shp->npix += c;
                    minx = shp->size + seg[1] * 2 + 8;
                    shp->size = minx;
                    nseg++;
                    maxx++;
                    miny += c + 1;
                } while ((int)miny < shp->w);
            }
            *rowp = maxx;
            r++;
        } while (r < shp->h);
    }
    shp->lineoff = shp->size;
    shp->size += shp->h * 4;
    d2_SetLineTable(tab, shp);
    shp->mode = flag;
    shp->nruns = nseg;
    ns = mem_Alloc(shp->size, "shp_t:ShapeHeader");
    memcpy(ns, shp, shp->size);
    mem_Free(shp);
    return ns;
}
