/* 0x479f40, makes a run length shape from 24 bit pixels @deep 679 code hunks, the unit note's Open work records this function as unsolved (0x479f40 / 0x47a460 / 0x47a7d0: the original computes the row pointer from scratch every row, ours strength-reduces it into a running pointer plus a [pitch*3] slot); the byte numbers are unchanged since that round (1159/1328). Read against the original 0x479f40..0x47a45f: the 24 bit pixel walk, the bounds scan (minx/maxx/miny/maxy), the key compare, the segment runs and the d2_SetLineTable upload correspond; what differs is the per-row pointer computation / register webs and the r/g/b local homes. Readability: Q1 name, Q2 d2_CompareColour / d2_SetColour / d2_EndMarkerColour / d2_StartMarkerColour / d2_TransparentColour. */

D2SHP *d2_GrabShape24Runs(int x, int y, int w, int h, unsigned char *pix, unsigned char flag, unsigned short pitch)
{
    D2SHP *shp, *ns;
    unsigned int minx, maxx, miny, maxy;
    unsigned char key[3];
    int tab[864];
    int r, c, nseg;
    unsigned char *p;
    int *seg, *rowp;
    int base, base2;

    r = 0;
    key[0] = 0;
    key[1] = 0;
    key[2] = 0;
    maxx = 0;
    minx = w;
    maxy = 0;
    miny = h;
    nseg = 0;
    shp = mem_Alloc((w * 0xc + 4) * h + 0x32, "shp_t:GrabBit24:ShapeHeader");
    for (r = 0; r < h; r++) {
        p = pix + ((r + y) * (pitch & 0xffff) + x) * 3;
        for (c = 0; c < w; c++) {
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c > (int)(maxx & 0xffff))
                maxx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && c < (int)(minx & 0xffff))
                minx = c;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r > (int)(maxy & 0xffff))
                maxy = r;
            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0 && r < (int)(miny & 0xffff))
                miny = r;
            p += 3;
        }
    }
    if ((unsigned short)minx > (unsigned short)maxx || (unsigned short)miny > (unsigned short)maxy) {
        maxx = 0;
        minx = 0;
        maxy = 0;
        miny = 0;
    }
    shp->x0 = (unsigned short)minx;
    shp->ax0 = (unsigned short)minx;
    shp->w = (unsigned short)maxx + 1;
    shp->y0 = (unsigned short)miny;
    shp->h = (unsigned short)maxy + 1;
    shp->ay0 = (unsigned short)miny;
    shp->ax1 = (unsigned short)maxx + 1;
    shp->ay1 = (unsigned short)maxy + 1;
    if (d2_CompareColour((unsigned char *)&d2_EndMarkerColour, key) != 0) {
        for (r = 0; r < h; r++) {
            p = pix + ((y + r) * (pitch & 0xffff) + x) * 3;
            for (c = 0; c < w; c++) {
                if (d2_CompareColour((unsigned char *)&d2_StartMarkerColour, key) != 0 && d2_CompareColour(p, (unsigned char *)&d2_StartMarkerColour) == 0) {
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                    minx = c;
                    miny = r;
                }
                if (d2_CompareColour(p, (unsigned char *)&d2_EndMarkerColour) == 0) {
                    d2_SetColour(p, d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
                    maxy = r;
                    maxx = c;
                }
                p += 3;
            }
        }
    }
    shp->by0 = (unsigned short)miny;
    shp->bx0 = (unsigned short)minx;
    shp->bx1 = (unsigned short)maxx;
    shp->by1 = (unsigned short)maxy;
    shp->size = 0x32;
    miny = 0;
    if (shp->h != 0) {
        do {
            maxx = 0;
            rowp = (int *)((char *)shp + shp->size);
            shp->size += 4;
            tab[miny] = (char *)rowp - (char *)shp;
            minx = 0;
            if (shp->w != 0) {
                base = (miny + y) * (pitch & 0xffff);
                base2 = x + base;
                do {
                    seg = (int *)((char *)shp + shp->size);
                    seg[0] = 0;
                    seg[1] = 0;
                    if ((int)minx < shp->w) {
                        p = pix + (minx + base + x) * 3;
                        do {
                            if (d2_CompareColour(p, (unsigned char *)&d2_TransparentColour) != 0)
                                break;
                            minx++;
                            p += 3;
                            seg[0]++;
                        } while ((int)minx < shp->w);
                    }
                    seg[0] *= 3;
                    if (seg[1] + minx < shp->w) {
                        do {
                            if (d2_CompareColour(pix + (minx + base + x + seg[1]) * 3, (unsigned char *)&d2_TransparentColour) == 0)
                                break;
                            seg[1]++;
                        } while (seg[1] + minx < shp->w);
                    }
                    memcpy(seg + 2, pix + (base2 + minx) * 3, seg[1] * 3);
                    shp->npix += seg[1];
                    minx += seg[1];
                    shp->size = shp->size + seg[1] * 3 + 8;
                    nseg++;
                    maxx++;
                } while ((int)minx < shp->w);
            }
            miny++;
            *rowp = maxx;
        } while ((int)miny < shp->h);
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
