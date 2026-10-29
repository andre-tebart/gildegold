/* 0x47ab30, grabs a rectangle of a 16 bit surface into a shape with plain rows (no run lengths; memory tag
   "shp_t:GrabBit8:ShapeHeader"): scans the rectangle for the bounding box of the non-zero pixels, copies the
   plain pixel rows into the shape and returns an exactly sized copy of it (allocated with mem_Alloc; the
   scratch shape is freed).
   @deep behaviour draft (319 code hunks, 493/544 bytes): the pixel scan keeps p in a stack slot (load/add/store per pixel, [esp+0x10]) where the original keeps it in edx (one add edx,2), and the bounds homes differ (original maxx/maxy in the slots 0x10/0x14, minx/miny in esi/edi; ours maxx=edi, minx=edx, miny=esi, maxy=slot); frame 0x10 vs 0x8, 544 vs 528 bytes. Read against 47ab30..47ad3f: the bbox conditions and reset, the header store order (x0,w,y0,h,ax0,ay0,ax1,ay1,bx0,by0,bx1,by1), the unused dd_RGB call, the row copy (rep movs, npix/size/p/dst updates), lineoff/mode/nruns and the alloc-copy-free tail all correspond. Tried: unsigned int bounds with (int)(x & 0xffff) compares (the original's dword stores), declaration orders, tail counter on w instead of i - no byte change */

D2SHP *d2_GrabShape16NoRuns(int x, int y, int w, int h, unsigned short *src, unsigned char mode, unsigned short pitch)
{
    int i, j;
    unsigned int maxx, maxy, minx, miny;
    unsigned short *p;
    D2SHP *s, *r;
    char *dst;

    maxx = 0;
    maxy = 0;
    s = mem_Alloc((w * 8 + 4) * h + 0x32, "shp_t:GrabBit8:ShapeHeader");
    minx = w;
    miny = h;
    for (i = 0; i < h; i++) {
        p = src + (y + i) * (pitch & 0xffff) + x;
        for (j = 0; j < w; j++, p++) {
            if (*p != 0) {
                if (j > (int)(maxx & 0xffff))
                    maxx = j;
                if (j < (int)(minx & 0xffff))
                    minx = j;
                if (i > (int)(maxy & 0xffff))
                    maxy = i;
                if (i < (int)(miny & 0xffff))
                    miny = i;
            }
        }
    }
    if ((unsigned short)maxx < (unsigned short)minx || (unsigned short)maxy < (unsigned short)miny) {
        minx = 0;
        miny = 0;
        maxx = 0;
        maxy = 0;
    }
    s->x0 = (unsigned short)minx;
    s->w = (unsigned short)maxx + 1;
    s->y0 = (unsigned short)miny;
    s->h = (unsigned short)maxy + 1;
    s->ax0 = (unsigned short)minx;
    s->ay0 = (unsigned short)miny;
    s->ax1 = (unsigned short)maxx + 1;
    s->ay1 = (unsigned short)maxy + 1;
    s->bx0 = (unsigned short)minx;
    s->by0 = (unsigned short)miny;
    s->bx1 = (unsigned short)maxx;
    s->by1 = (unsigned short)maxy;
    s->size = 0x32;
    dd_RGB(d2_TransparentColour.r, (unsigned char)d2_TransparentColour.gb, (unsigned char)(d2_TransparentColour.gb >> 8));
    dst = (char *)s + s->size;
    p = src + y * pitch + x;
    w = 0;
    if (s->h != 0) {
        do {
            memcpy(dst, p, s->w * 2);
            s->npix += s->w;
            s->size += s->w * 2;
            p += pitch;
            dst += s->w * 2;
            w++;
        } while (w < s->h);
    }
    s->lineoff = 0;
    s->mode = mode;
    s->nruns = -1;
    r = mem_Alloc(s->size, "shp_t:ShapeHeader");
    memcpy(r, s, s->size);
    mem_Free(s);
    return r;
}
