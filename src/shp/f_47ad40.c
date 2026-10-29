/* 0x47ad40, grabs a rectangle of an 8 bit surface into a run-length coded shape (memory tag
   "d2:shp:_GrabBit8:ShapeHeader"): scans the rectangle for the bounding box of the non-zero pixels, clears the
   palette pixels of the transparent colour and of the start / end marker colours (the markers move the box),
   then stores every row as skip runs of zero pixels alternating with literal runs, builds the table of row
   offsets, and returns an exactly sized copy of the shape (allocated with mem_Alloc; the scratch shape is
   freed).
   @deep behaviour draft (527 code hunks, 997/1184 bytes, frame 0xda0 on both sides): every meaning was read against 47ad40..47b21f and corresponds - the bbox scan with the four (x & 0xffff) compares, the reset, the header store order (x0,y0,w,h,ay1,ax0,ay0,ax1), the odd transparent-palette loop (src[i], inner j unused), the marker loop (start -> minx/miny, end -> maxx/maxy, same t, pixel cleared), the run loop (skip run with the row recomputed from maxy*pitch+x+col, literal run bounded by run[1]+col<w, memcpy(run+2,row+col,run[1]), the col-- / col+=run[1]+1, npix/size/cnt/runs), the line table, mode/nruns and the alloc-copy-free tail. What differs is codegen: our tail keeps row/col as pointers where the original recomputes src[maxy*pitch+x+col] in the skip loop and keeps the row base in a slot, and the scan variables all have other register/slot homes (1184 vs 1248 bytes). No source meaning fix found */

D2SHP *d2_GrabShape8Runs(int x, int y, int w, int h, unsigned char *src, unsigned char mode, unsigned short pitch)
{
    int i, j, cnt, runs, col, t;
    unsigned short minx, miny, maxx, maxy;
    int lines[864];
    unsigned char *p, *row;
    char c;
    D2SHP *s, *r;
    int *line, *run;

    maxx = 0;
    maxy = 0;
    cnt = 0;
    miny = h;
    s = mem_Alloc((w * 8 + 4) * h + 0x32, "d2:shp:_GrabBit8:ShapeHeader");
    minx = w;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            c = src[(y + i) * pitch + x + j];
            if (c != 0 && j > maxx)
                maxx = j;
            if (c != 0 && j < minx)
                minx = j;
            if (c != 0 && i > maxy)
                maxy = i;
            if (c != 0 && i < miny)
                miny = i;
        }
    }
    if (maxx < minx || maxy < miny) {
        maxx = 0;
        minx = 0;
        maxy = 0;
        miny = 0;
    }
    s->x0 = minx;
    s->y0 = miny;
    s->w = (unsigned short)maxx + 1;
    s->h = (unsigned short)maxy + 1;
    s->ay1 = (unsigned short)maxy + 1;
    s->ax0 = minx;
    s->ay0 = miny;
    s->ax1 = (unsigned short)maxx + 1;
    for (i = 0; i < h; i++) {
        for (j = w; j > 0; j--) {
            t = src[i] * 4;
            if (d2_Palette[0][t] == d2_TransparentColour.r && d2_Palette[0][t + 1] == (unsigned char)d2_TransparentColour.gb
                && d2_Palette[0][t + 2] == (unsigned char)(d2_TransparentColour.gb >> 8))
                src[i] = 0;
        }
    }
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            p = src + (i + y) * pitch + x;
            t = p[j] * 4;
            if (d2_Palette[0][t] == d2_StartMarkerColour.r && d2_Palette[0][t + 1] == (unsigned char)d2_StartMarkerColour.gb
                && d2_Palette[0][t + 2] == (unsigned char)(d2_StartMarkerColour.gb >> 8)) {
                p[j] = 0;
                minx = j;
                miny = i;
            }
            if (d2_Palette[0][t] == d2_EndMarkerColour.r && d2_Palette[0][t + 1] == (unsigned char)d2_EndMarkerColour.gb
                && d2_Palette[0][t + 2] == (unsigned char)(d2_EndMarkerColour.gb >> 8)) {
                p[j] = 0;
                maxy = i;
                maxx = j;
            }
        }
    }
    s->bx1 = (unsigned short)maxx;
    s->bx0 = (unsigned short)minx;
    s->by0 = (unsigned short)miny;
    s->by1 = (unsigned short)maxy;
    s->size = 0x32;
    miny = 0;
    if (s->h != 0) {
        maxy = y;
        do {
            runs = 0;
            line = (int *)((char *)s + s->size);
            s->size += 4;
            lines[miny] = (char *)line - (char *)s;
            col = 0;
            if (s->w != 0) {
                t = maxy * pitch;
                row = src + x + t;
                do {
                    run = (int *)((char *)s + s->size);
                    run[0] = 0;
                    run[1] = 0;
                    if (row[col] == 0) {
                        row = src + maxy * pitch + x;
                        do {
                            if (s->w <= col)
                                break;
                            col++;
                            run[0]++;
                        } while (row[col] == 0);
                    }
                    c = src[t + col + x];
                    while (c != 0 && (unsigned int)(run[1] + col) < s->w) {
                        run[1]++;
                        c = src[run[1] + t + col + x];
                    }
                    memcpy(run + 2, row + col, run[1]);
                    if (run[1] != 0)
                        col--;
                    s->npix += run[1];
                    s->size = s->size + run[1] + 8;
                    cnt++;
                    runs++;
                    col += run[1] + 1;
                } while (col < s->w);
            }
            *line = runs;
            miny++;
            maxy++;
        } while (miny < s->h);
    }
    s->lineoff = s->size;
    s->size += s->h * 4;
    d2_SetLineTable(lines, s);
    s->mode = mode;
    s->nruns = cnt;
    r = mem_Alloc(s->size, "shp_t:ShapeHeader");
    memcpy(r, s, s->size);
    mem_Free(s);
    return r;
}
