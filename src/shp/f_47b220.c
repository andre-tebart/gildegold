/* 0x47b220, grabs a rectangle of a surface into a shape; the colour depth of the surface selects the grabber */

D2SHP *d2_GrabShape(int x, int y, int w, int h, D2SURF *surf, unsigned char mode, char grab8)
{
    D2SHP *r;
    int type;

    r = 0;
    if (grab8 != 0) {
        switch (*((char *)surf + 0x14)) {
        case 8:
            r = d2_GrabShape8Runs(x, y, w, h, (unsigned char *)surf->bits, mode, (unsigned short)surf->w);
            type = 0;
            break;
        case 16:
            r = d2_GrabShape16Runs(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 1;
            break;
        case 24:
            r = d2_GrabShape24Runs(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 2;
            break;
        default:
            break;
        }
    } else {
        switch (*((char *)surf + 0x14)) {
        case 16:
            r = d2_GrabShape16NoRuns(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 1;
            break;
        case 24:
            r = d2_GrabShape24NoRuns(x, y, w, h, surf->bits, mode, (unsigned short)surf->w);
            type = 2;
            break;
        default:
            break;
        }
    }
    if (r != 0) {
        r->type = type;
        r->mode = mode;
    }
    return r;
}
